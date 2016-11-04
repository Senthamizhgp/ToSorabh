
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
 *	@file		channel_selector.c
 *
 *	@brief		This file contains definition of all APIs of the
 *	                channel selector and mode manager module which will
 *	                be used by the WiFi People Counter module.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/rfkill.h>
#include <sys/ioctl.h>
#include "channel_selector_ut.h"
#include "channel_selector.h"

#define NULL_CHECK(val)  if (val == NULL) return -EINVAL; /**< Check for NULL @hideinitializer */
#define UNUSED(expr) do { (void)(expr); } while (0)
#define MAC_SIZE 6
#define CYCLE_COMPLETE 1

const char *wifi_modes[] = {
	"WIFI_MODE_AUTO",
	"WIFI_MODE_ADHOC",
	"WIFI_MODE_MANAGED",
	"WIFI_MODE_MASTER",
	"WIFI_MODE_REPEAT",
	"WIFI_MODE_SECOND",
	"WIFI_MODE_MONITOR"
};

static inline double wifi_freq2float(const struct iw_freq *freq)
{
	return ((double) freq->m) * pow(10, freq->e);
}

static inline void wifi_float2freq(double floatfreq, struct iw_freq *freq)
{
	freq->e = (short) floor(log10(floatfreq));
	if (freq->e > 8)
	{
		freq->m = ((long) (floor(floatfreq / pow(10, freq->e - 6)))) * 100;
		freq->e -= 8;
	}
	else
	{
		freq->m = (long) floatfreq;
		freq->e = 0;
	}
}

static int32_t wifi_open_socket(void)
{
	int32_t sockfd = -1;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "Cannot open socket \n");
		fprintf(stderr, "errno = %d \n", errno);
		fprintf(stderr, "Error description is : %s\n",strerror(errno));
		return(-1);
	}
	return sockfd;
}

/**
* @brief Get the WiFi adapter state (DOWN, UP, RUNNING)
*
* @param ifname Interface name e.g wlan0
* @param flags  Interface flags which indicate its state
*
* @return 0 for success
*/
int32_t wifi_get_state(const char *ifname, long *flags)
{
	int32_t ret = -1;
	int32_t sock = -1;
	struct ifreq ifr;

	NULL_CHECK(ifname);
	NULL_CHECK(flags);

	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	if((sock = wifi_open_socket()) != -1)
	{
		if ((ret = ioctl(sock, SIOCGIFFLAGS, &ifr)) < 0)
			fprintf(stderr, "%s\n" , strerror(errno));
		else
			*flags = ifr.ifr_flags;

		close(sock);
	}

	return ret;
}

/**
* @brief Set the WiFi adapter state (DOWN, UP, RUNNING)
*
* @param ifname Interface name e.g wlan0
* @param flags  Interface flags which its state needs to be set
*
* @return 0 for success
*/
int32_t wifi_set_state(const char *ifname, long flags)
{
	int32_t ret = -1;
	int32_t sock = -1;
	struct ifreq ifr;

	NULL_CHECK(ifname);

	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	ifr.ifr_flags = flags;

	if((sock = wifi_open_socket()) != -1)
	{
		if ((ret = ioctl(sock, SIOCSIFFLAGS, &ifr)) < 0)
			fprintf(stderr, "%s\n" , strerror(errno));
		close(sock);
	}

	return ret;
}

static int32_t wifi_parse_mode(uint32_t iw_mode, wifi_mode_t *wifi_mode)
{

	NULL_CHECK(wifi_mode);

	switch (iw_mode)
	{
		case WIFI_MODE_AUTO:
		case WIFI_MODE_ADHOC:
		case WIFI_MODE_MANAGED:
		case WIFI_MODE_MASTER:
		case WIFI_MODE_REPEAT:
		case WIFI_MODE_SECOND:
		case WIFI_MODE_MONITOR:
			*wifi_mode = iw_mode;
			return 0;
		default:
			printf("Unknown mode: %d.\n", iw_mode);
			return -1;
	}
}

/**
* @brief Get WiFi adapter's current operation mode
*
* @param ifname Interface name e.g wlan0
* @param mode   Mode in which the interface is set currently
*
* @return 0 for success
*/
int32_t wifi_get_mode(const char *ifname, wifi_mode_t *mode)
{
	struct iwreq wreq;
	int32_t ret = -1;
	int32_t sock = -1;

	NULL_CHECK(ifname);
	NULL_CHECK(mode);

	memset(&wreq, 0, sizeof(struct iwreq));
	strncpy(wreq.ifr_name, ifname, IFNAMSIZ);

	if((sock = wifi_open_socket()) != -1)
	{
		if ((ret = ioctl(sock, SIOCGIWMODE, &wreq)) >= 0)
			ret = wifi_parse_mode(wreq.u.mode, mode);
		else
			fprintf(stderr, "%s\n" , strerror(errno));

		close(sock);
	}
	return ret;
}

/**
* @brief Set WiFi adapter operation mode
*
* @param ifname Interface name e.g wlan0
* @param mode   Mode in which the interface is to be set
*
* @return 0 for success
*/
int32_t wifi_set_mode(const char *ifname, wifi_mode_t mode)
{
	struct iwreq wreq;
	int32_t ret = -1;
	int32_t sock = -1;

	NULL_CHECK(ifname);

	memset(&wreq, 0, sizeof(struct iwreq));
	wreq.u.mode = mode;
	strncpy(wreq.ifr_name, ifname, IFNAMSIZ);

	if((sock = wifi_open_socket()) != -1)
	{
		if ((ret = ioctl(sock, SIOCSIWMODE, &wreq)) < 0)
			fprintf(stderr, "%s\n" , strerror(errno));
		close(sock);
	}

	return ret;
}

/**
* @brief Convert WiFi channel number to respective frequency
*
* @param ifname Interface name e.g wlan0
* @param chan   Channel Number
* @param freq   Frequency in GHz
*
* @return 0 for success
*/
int32_t wifi_channel_to_freq(const char *ifname, uint32_t chan, double *freq)
{
	struct iwreq wrq;
	char buf[sizeof(struct iw_range) * 2];
	int32_t ret = -1;
	int32_t sock = -1 ;

	NULL_CHECK(ifname);
	NULL_CHECK(freq);

	/* Prepare request. */
	memset(buf, 0, sizeof(buf));
	memset(&wrq, 0, sizeof(struct iwreq));
	wrq.u.data.pointer = buf;
	wrq.u.data.length = sizeof(buf);
	wrq.u.data.flags = 0;
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	if((sock = wifi_open_socket()) != -1)
	{
		/* Get range. */
		if ((ret = ioctl(sock, SIOCGIWRANGE, &wrq)) >= 0)
		{
			struct iw_range *range = (struct iw_range *) buf;
			int32_t k;

			for (k = 0; k < range->num_frequency; k++)
			{
				if (chan == range->freq[k].i)
				{
					*freq = wifi_freq2float(&(range->freq[k]));
					close(sock);
					return 0;
				}
			}

			/* Oops! Nothing found. */
			printf("No frequency matches for the given channel!\n");
			ret = -2;
		}
		else
			fprintf(stderr, "%s\n" , strerror(errno));

		close(sock);
	}
	return ret;
}

/**
* @brief Set WiFi operational frequency
*
* @param ifname Interface name e.g wlan0
* @param freq   Frequency to be set
* @param flag   Freqeuncy change scheduling (Auto / Fixed)
*
* @return 0 for success
*/
int32_t wifi_set_freq(const char *ifname, double freq, wifi_freq_flag_t flag)
{
	struct iwreq wrq;
	int32_t ret = -1;
	int32_t sock = -1;

	NULL_CHECK(ifname);

	/* Set freq. */

	memset(&wrq, 0, sizeof(struct iwreq));
	wifi_float2freq(freq, &(wrq.u.freq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/* Set flag. */
	switch (flag)
	{
		case WIFI_FREQ_AUTO:
			wrq.u.freq.flags = IW_FREQ_AUTO;
			break;
		case WIFI_FREQ_FIXED:
			wrq.u.freq.flags = IW_FREQ_FIXED;
			break;
		default:
			return -EINVAL;
	}

	if((sock = wifi_open_socket()) != -1)
	{
		ret = ioctl(sock, SIOCSIWFREQ, &wrq);
		if (ret < 0)
			fprintf(stderr, "%s\n" , strerror(errno));

		close(sock);
	}
	return ret;
}

/**
 * Get Device MAC address.
 *
 * param[in]  interface name.
 * param[out] MAC address of the NUC.
 *
 * returns    Zero on success.
 *            Non-zero on failure.
 */
/**
* @brief Get WiFi Device MAC address
*
* @param ifname Interface name e.g wlan0
* @param mac_address MAC address
*
* @return 0 for success
*/
int32_t wifi_get_dev_mac_addr(const char *ifname, uint8_t* mac_address)
{

	int32_t ret = -1;
	int32_t sock = -1;
	struct ifreq ifr;

	NULL_CHECK(ifname);
	NULL_CHECK(mac_address);

	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	memset(mac_address, 0, MAC_SIZE);

	if((sock = wifi_open_socket()) != -1)
	{

		if ((ret = ioctl(sock, SIOCGIFHWADDR, &ifr)) < 0)
			fprintf(stderr, "%s\n" , strerror(errno));
		else
			memcpy(mac_address, ifr.ifr_hwaddr.sa_data, MAC_SIZE);

		close(sock);
	}

	return ret;
}

/**
* @brief Set the WiFi adapter operating channel
*
* @param ifname  Interface name e.g wlan0
* @param channel Channel number
*
* @return 0 for success
*/
int32_t wifi_set_channel(const char *ifname, uint32_t channel)
{
	double freq;
	int32_t status;
	long flags = 0;

	NULL_CHECK(ifname);

	status = wifi_get_state(ifname, &flags);
	if(status < 0)
	{
		fprintf(stderr, "wifi get state failed\n");
		return status;
	}

	if((flags & (IFF_UP | IFF_RUNNING)) == 0)
	{
		status = wifi_set_state(ifname, (flags | (IFF_UP | IFF_RUNNING)));
		if(status < 0)
		{
			fprintf(stderr, "wifi set state failed\n");
			return status;
		}
	}

	status = wifi_channel_to_freq(ifname, channel, &freq);
	if(status < 0)
	{
		fprintf(stderr, "wifi_chan2freqfailed\n");
		return status;
	}

	status = wifi_set_freq(ifname, freq, 0);
	if(status < 0)
	{
		fprintf(stderr, "wifi_set_freq failed");
		return status;
	}
	return status;
}

static uint32_t channel_array[] = { 1, 2 }; /* List of channels used to capture packets */
//static uint32_t channel_array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 }; /* List of channels used to capture packets */
static uint32_t channel_arr_len = sizeof(channel_array) / sizeof(channel_array[0]);

/**
 * @brief Switches the WiFi channel after a specific configured
 * scanning interval
 *
 * @param ifname interface name e.g wlan0
 *
 * @return 0 for success, 1 for end of scanning cycle
 * other value indicate failure
 */
int32_t channel_switcher(const char *ifname)
{
	int32_t status;
	static uint32_t channel_iterator;

	NULL_CHECK(ifname);
	if(channel_iterator == channel_arr_len)
	{
		channel_iterator = 0;
		return CYCLE_COMPLETE;
	}
	if((status = wifi_set_channel(ifname, channel_array[channel_iterator])) < 0)
		return status;
	channel_iterator++;
	return status;
}

/**
 * @brief Get NUC WiFi adapter MAC address
 *
 * @param ifname interface name e.g wlan0
 * @param nuc_mac output buffer with mac address e.g aa:bb:cc:dd:ee:ff
 *
 * @return 0 for success
 */
int32_t wifi_get_nuc_mac_addr(const char *ifname, uint8_t* nuc_mac)
{
	int32_t status;
	uint8_t mac[MAC_SIZE];

	NULL_CHECK(ifname);
	NULL_CHECK(nuc_mac);

	status = wifi_get_dev_mac_addr(ifname, mac);
	if(status < 0)
	{
		fprintf(stderr, "wifi get_mac_addr failed\n");
		return status;
	}
	else
		snprintf((char*)nuc_mac, 20, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return status;
}

/**
 * @brief Set WiFi adapter to monitor mode
 *
 * @param ifname interface name e.g wlan0
 *
 * @return 0 for success
 */
int32_t wifi_set_monitor_mode(const char *ifname)
{
	int32_t status;

	NULL_CHECK(ifname);

	status = wifi_set_mode(ifname, WIFI_MODE_MONITOR);
	if(status < 0)
		fprintf(stderr, "wifi set mode failed\n");

	return status;
}


/**
* @brief Convert WiFi frequency to its respective channel
*
* @param ifname Interface name e.g wlan0
* @param freq   Frequency in GHz
* @param chan   Channel mapped to the frequency
*
* @return 0 for success
*/
int32_t wifi_freq_to_chan(const char *ifname, double freq, int32_t *chan)
{
	struct iwreq wrq;
	char buf[sizeof(struct iw_range) * 2];
	int32_t ret = -1;
	int32_t sock = -1;

	NULL_CHECK(ifname);
	NULL_CHECK(chan);

	/* Prepare request. */
	memset(buf, 0, sizeof(buf));
	memset(&wrq, 0, sizeof(struct iwreq));
	wrq.u.data.pointer = buf;
	wrq.u.data.length = sizeof(buf);
	wrq.u.data.flags = 0;
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	if((sock = wifi_open_socket()) != -1)
	{
		/* Get range. */
		if ((ret = ioctl(sock, SIOCGIWRANGE, &wrq)) >= 0)
		{
			struct iw_range *range = (struct iw_range *) buf;
			int32_t k;

			/* Compare the frequencies as double to ignore differences in encoding.
			 * Slower, but safer... */
			for (k = 0; k < range->num_frequency; k++)
				if (freq == wifi_freq2float(&(range->freq[k])))
				{
					*chan = range->freq[k].i;
					close(sock);
					return 0;
				}

			/* Oops! Nothing found. */
			printf("No channel matches for the given frequency!\n");
			ret = -2;
		}
		else
			fprintf(stderr, "%s\n" , strerror(errno));

		close(sock);
	}
	return ret;
}

/**
* @brief Get WiFi adapter operational frequency
*
* @param ifname  Interface name e.g wlan0
* @param freq    Frequency in GHz
* @param flag    Freqeuncy change scheduling (Auto / Fixed)
*
* @return 0 for success
*/
int32_t wifi_get_freq(const char *ifname, double *freq, wifi_freq_flag_t *flag)
{
	struct iwreq wrq;
	int32_t ret = -1;
	int32_t sock = -1;

	NULL_CHECK(ifname);
	NULL_CHECK(freq);
	NULL_CHECK(flag);

	memset(&wrq, 0, sizeof(struct iwreq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	if((sock = wifi_open_socket()) != -1)
	{
		if ((ret = ioctl(sock, SIOCGIWFREQ, &wrq)) >= 0)
		{
			/* Set flag. */
			if (IW_FREQ_AUTO == (wrq.u.freq.flags & IW_FREQ_AUTO))
				*flag = WIFI_FREQ_AUTO;
			else if (IW_FREQ_FIXED == (wrq.u.freq.flags & IW_FREQ_FIXED))
				*flag = WIFI_FREQ_FIXED;
			else
			{
				printf("Unknown flag: %d.\n", wrq.u.freq.flags);
				close(sock);
				return -1;
			}

			/* Set freq. */
			*freq = wifi_freq2float(&(wrq.u.freq));
		}
		close(sock);
	}
	return ret;
}

/**
* @brief Get the WiFi adapter operating channel
*
* @param ifname  Interface name e.g wlan0
* @param channel Channel number
*
* @return 0 for success
*/
int32_t wifi_get_channel(const char *ifname, int32_t* channel)
{
	double freq;
	wifi_freq_flag_t flag;
	int32_t status;

	NULL_CHECK(ifname);
	NULL_CHECK(channel);

	status = wifi_get_freq(ifname, &freq, &flag);
	if(status < 0)
	{
		fprintf(stderr, "wifi_get_freq failed");
		return status;
	}

	status = wifi_freq_to_chan(ifname, freq, channel);
	if(status < 0)
	{
		fprintf(stderr, "wifi_freq2chan failed");
		return status;
	}
	printf("%d\n", *channel);
	return status;
}

/**
* @brief Kill WiFi RF radio to mimic the hard button via software
*
* @return 0 for success
*/
int32_t wifi_rfkill_unblock(void)
{
	int rfkill_fd;
	struct rfkill_event event = {0};

	if((rfkill_fd = open("/dev/rfkill", O_RDWR | O_NONBLOCK)) < 0)
	{
		perror("open");
		return -errno;
	}

	//fcntl(rfkill_fd, F_SETFL, O_NONBLOCK);

	while(read(rfkill_fd, &event, sizeof(event)) > 0)
	{
		if(event.type == RFKILL_TYPE_WLAN)
		{
			if(event.soft || event.hard)
			{
				memset(&event, 0, sizeof(event));
				event.type = RFKILL_TYPE_WLAN; /* Targetting WiFi switches */
				event.op = RFKILL_OP_CHANGE_ALL; /* Change all switches */
				event.soft = 0; /* Set to unblock */
				event.hard = 0; /* Set to unblock */

				if(write(rfkill_fd, &event, sizeof(event)) < 0)
				{
					perror("write");
					close(rfkill_fd);
					return -errno;
				}
			}

		}
	}
	close(rfkill_fd);
	return 0;
}
