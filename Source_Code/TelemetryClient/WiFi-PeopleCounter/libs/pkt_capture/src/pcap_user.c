
/******************************************************************************
*
* INTEL CONFIDENTIAL
* Copyright 2016 Intel Corporation All Rights Reserved.
*
* The source code contained or described herein and all documents related to the
* source code ("Material") are  owned by Intel Corporation or its suppliers or
* licensors. Title to the Material remains with Intel Corporation or its suppliers
* and licensors. The Material contains trade secrets and proprietary and
* confidential information of Intel or its suppliers and licensors. The Material
* is protected by worldwide  copyright and trade secret laws and treaty provisions.
* No part of the Material may be used, copied, reproduced, modified, published,
* uploaded, posted, transmitted, distributed, or disclosed in any way without
* Intel's prior express written permission.
*
* No license under any patent, copyright, trade secret or other intellectual
* property right is granted to or conferred upon you by disclosure or delivery of
* the Materials, either expressly, by implication,  inducement, estoppel or
* otherwise. Any license under such intellectual property rights must be express
* and approved by Intel in writing.
*
******************************************************************************/

/**
 *
 *	@file		pcap_user.c
 *
 *	@brief		This file contains definition of all APIs used to
 *	                capture packets using the pcap library by the
 *	                WiFi People Counter module.
 *
 */

#include "pcap_user.h"
#include "pktparser.h"
#include <time.h>

/**
 * A structure to represent packet capture module instance
 */
typedef struct pkt_capture
{
	const char *dev;		/**< capture device name */
	char errbuf[PCAP_ERRBUF_SIZE];	/**< error buffer */
	char filter_exp[128];		/**< filter expression */
	struct bpf_program fp;		/**< compiled filter program (expression) */
	uint32_t interval;		/**< number of packets to capture */
} pkt_capture_t;

static pkt_capture_t *pktcap;           /**< packet capture module instance */

static pcap_t *handle;		/**< packet capture handle */
timer_t oneshotTimer;		/**< Channel switcher timer */
#define CHANNEL_SWITCH_SIG	SIGRTMAX - 1

void channel_switching_handler(int signo, siginfo_t * info, void *context);

#define FILTER_EXP "wlan type mgt subtype probe-req" /**< Filter expression */

/**
* @brief Initialize the packet capture module
*
* @param ifname   Interface name e.g wlan0
* @param interval Scanning interval
*
* @return 0 for success
*/
int32_t init_pkt_capture_module(const char* ifname, uint32_t interval)
{

	int32_t status = 0;
	struct sigaction sigact;

	pktcap = (pkt_capture_t *) calloc (1, sizeof(pkt_capture_t));
	if(!pktcap)
		return -ENOMEM;

	strncpy((char*) pktcap->filter_exp, FILTER_EXP, sizeof(FILTER_EXP) + 1);
	pktcap->dev = ifname;
	pktcap->interval = interval;

	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = channel_switching_handler;

	if (sigaction(CHANNEL_SWITCH_SIG, &sigact, NULL) == -1)
	{
		perror("sigaction failed");
		status = -errno;
		if(!pktcap)
		{
			free(pktcap);
			pktcap = NULL;
		}
	}

	return status;
}

/**
 * @brief Close the pcap capture handle
 */
static void wifi_pcap_close_function(void)
{
	if(handle != NULL)
	{
		pcap_close(handle);
		handle = NULL;
	}
}

/**
* @brief Un-Initialize the packet capture module
*/
void free_pkt_capture_module(void)
{
	if(handle != NULL)
	{
		pcap_breakloop(handle);
		pcap_freecode(&pktcap->fp);
		wifi_pcap_close_function();
	}
	if(pktcap)
	{
		free(pktcap);
		pktcap = NULL;
	}
}

/**
 * @brief Setup pcap engine
 * Create a pcap handle, compile the pcap filter and set the filter
 *
 * @return 0 for success
 */
int32_t wifi_pcap_setup_function()
{
	int32_t status = 0;

	/*
	 * Create pcap handle
	 */

	if((handle = pcap_create(pktcap->dev, pktcap->errbuf)) == NULL)
	{
		fprintf(stderr, "pcap_create() failed: %s\n", pktcap->errbuf);
		status = -EFAULT;
		return status;
	}

	/*
	 * Turn off promisc mode
	 */

	if(pcap_set_promisc(handle, 0) != 0)
	{
		fprintf(stderr, "Failed to disable promisc mode.\n");
		status = -EFAULT;
		wifi_pcap_close_function();
		return status;
	}

	/*
	 * Set SNAP len
	 */

	if(pcap_set_snaplen(handle, BUFSIZ) != 0)
	{
		fprintf(stderr, "Failed to set snaplen.\n");
		status = -EFAULT;
		wifi_pcap_close_function();
		return status;
	}

	/*
	 * Set capture timeout
	 */

	if(pcap_set_timeout(handle, 1000) != 0)
	{
		fprintf(stderr, "Failed to set timeout.\n");
		status = -EFAULT;
		wifi_pcap_close_function();
		return status;
	}

	if(pcap_activate(handle) != 0)
	{
		fprintf(stderr, "pcap_activate() failed\n");
		status = -EFAULT;
		wifi_pcap_close_function();
		return status;
	}

	/*
	 * Compile a filter to sniff 802.11 probe requests
	 * wlan type mgt subtype probe-req
	 */
	if(pcap_compile(handle, &(pktcap->fp), pktcap->filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
	{
		fprintf(stderr, "pcap_compile() failed: %s\n", pcap_geterr(handle));
		status = -EFAULT;
		wifi_pcap_close_function();
		return status;
	}

	/*
	 * Set the compiled filter
	 */
	if(pcap_setfilter(handle, &(pktcap->fp)) == -1)
	{
		fprintf(stderr, "pcap_setfilter() failed: %s\n", pcap_geterr(handle));
		status = -EFAULT;
		pcap_freecode(&pktcap->fp);
		wifi_pcap_close_function();
		return status;
	}
	return status;
}

/**
* @brief Wrapper for the packet processing function from the parsing module
*
* @param user   pointer to the caller supplied buffer.
* @param header pointer to the packet header.
* @param packet pointer to the packet buffer excluding header.
*/
static void wifi_pcap_process_pkt(u_char *user, const struct pcap_pkthdr* header, const u_char* packet)
{
	UNUSED(user);
	pkt_processing(header, packet);
	return;
}

/**
* @brief Setup a timer for WiFi Channel switching
*
* @param timerid Pointer to the timer variable
* @param signo   Signal to be raised when the timer expires
* @param msec    Time in milliseconds
*
* @return 0 for success
*/
int32_t set_channel_switcher_timer(timer_t *timerid, int32_t signo, uint32_t msec)
{
	struct sigevent sigev;
	struct itimerspec itval;
	int32_t status = 0;

	// Create the POSIX timer to generate signo

	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = signo;
	sigev.sigev_value.sival_ptr = timerid;

	if (timer_create(CLOCK_REALTIME, &sigev, timerid) == 0)
	{
		itval.it_value.tv_sec = msec / 1000;
		itval.it_value.tv_nsec = (long)(msec % 1000) * (1000000L);
		itval.it_interval.tv_sec = 0;
		itval.it_interval.tv_nsec = 0;

		if (timer_settime(*timerid, 0, &itval, NULL) != 0)
		{
			status = -errno;
			perror("time_settime error!");
			timer_delete(*timerid);
			return status;
		}
	}
	else
	{
		perror("timer_create error!");
		status = -errno;
	}

	return status;
}

/**
* @brief Signal handler to trigger channging WiFi channel switching
*
* @param signo signal number which trigger the channel switcher
* @param info  signal info parameter
* @param context context pointer
*/

void channel_switching_handler(int signo, siginfo_t * info, void *context)
{
	UNUSED(context);

	if (signo == CHANNEL_SWITCH_SIG && (oneshotTimer == *((timer_t*)(info->si_value.sival_ptr))))
	{
		printf("Switching WiFi Channel\n");
		pcap_breakloop(handle);
		pcap_freecode(&pktcap->fp);
		wifi_pcap_close_function();
	}
}

/**
 * @brief Capture the filtered WiFi packets via pcap
 */
void wifi_start_pcap_capture_function(void)
{
	int32_t status = 0;

	/**
	 * Setup a timer to strike every scanning interval
	 *  to trigger WiFi channel switching.
	 */

	status = set_channel_switcher_timer(&oneshotTimer, CHANNEL_SWITCH_SIG, pktcap->interval * 1000);
	if(status != 0)
		return;

	/**
	 * Start the pcap loop to capture and process the WiFi packets
	 * loop is broken in the channel_switching_handler before
	 * before switching channel
	 */
	while((status = pcap_loop(handle, -1, wifi_pcap_process_pkt, NULL)) > 0 );

	timer_delete(oneshotTimer);
}
