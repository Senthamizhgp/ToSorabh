
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
 *	@file		pktparser.c
 *
 *	@brief		This file contains definition of all APIs used to
 *	                parsed the captured packets using the pcap library
 *	                by the WiFi People Counter module.It also uses the
 *	                radiotap parser libray to extract the data from the
 *	                radiotap headeparser libray to extract the data from the
 *	                radiotap header.
 *
 */

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <endian.h>
#include <errno.h>
#include <string.h>
#include "radiotap_iter.h"
#include "wifilib.h"
#include "pktparser.h"

#define MAC_LEN 6

/**
 * A structure to represent the buffer in which the frame data is extracted
 */
struct pkt_struct_info{
	uint32_t max_rate;
	uint8_t channel;   /**< bit 7 is 1 when channel is determined from radiotap */
	int32_t signal;    /**< RSSI */
	char protocol;     /**< frame control version */
	char type;         /**< frame control type */
	char subtype;      /**< frame control subtype */
	char type_subtype;
	uint8_t sa[MAC_LEN]; /**< source address */
	time_t seen; /**< packet timestamp */
	uint8_t no; /**< user for number of different frames */
};

/*
 * IEEE802.11 version 0
 *
 * +---------------------------------------------------------------------------------------+
 * |     frame_type 8 bits     |                       flags ( 8 bits )                    |
 * +----------+------+---------+----+------+------+-------+-----+------+-----------+-------+
 * | Protocol | Type | Subtype | To | From | More | Retry | Pwr | More | Protected | Order |
 * | version  |      |         | Ds | Ds   | Flag |       | Mgt | Data | Frame     |       |
 * +----------+------+---------+----+------+------+-------+-----+------+-----------+-------+
 * |  2 bits  | 2 b  | 2 bits  | 1b |  1b  |  1b  | 1 bit |  1b |  1b  |   1 bit   | 1 bit |
 * +----------+------+---------+----+------+------+-------+-----+------+-----------+-------+
 *
 * */

/**
 * A structure to represent the 80211 frame
 */
struct ieee80211_frame{
	uint8_t frame_type;  /**< including protocol version,type and subtype */
	uint8_t flags; /**< flags */
	uint16_t duration;
	uint8_t addr1[MAC_LEN];
	uint8_t addr2[MAC_LEN];
	uint8_t addr3[MAC_LEN];
	uint16_t seq_cntrl; /**< fragment + sequence */
	uint8_t addr4[MAC_LEN];
	uint16_t qos_cntrl; /**< Just where subtype is QoS */
};

/**
 * A structure to represent packet parser module instance
 */
typedef struct pkt_parser
{
	wifi_ppl_counter_t *wifi_ppl_counter_inst; /**< pointer to library instance*/
} pkt_parser_t;

static pkt_parser_t *pktparser;

/**
 * @brief Initialize the packet parser module
 *
 * @param wifi_ppl_counter_inst pointer to library instance
 *
 * @return 0 for success
 */
int32_t init_pkt_parser_module(void *wifi_ppl_counter_inst)
{
	pktparser = (pkt_parser_t *) calloc (1, sizeof(pkt_parser_t));
	if(!pktparser)
		return -ENOMEM;
	pktparser->wifi_ppl_counter_inst = wifi_ppl_counter_inst;
	return 0;
}

/**
 * @brief Un-Initialize the packet parser module
 */
void free_pkt_parser_module(void)
{
	if(pktparser)
	{
		free(pktparser);
		pktparser = NULL;
	}
}

/**
 * @brief Extract data from the 802.11 frame into the pkt_info structure
 *
 * @param frame_80211 802.11 frame
 * @param pkt_info    Buffer in which data needs to be extracted.
 */
static void fill_pkt_struct_info(struct ieee80211_frame *frame_80211, struct pkt_struct_info *pkt_info )
{
	if(frame_80211)
	{
		pkt_info->protocol = frame_80211->frame_type & 0x03;
		pkt_info->type = (frame_80211->frame_type & 0x0C) >> 2;
		pkt_info->subtype =(frame_80211->frame_type & 0xF0) >> 4;
		pkt_info->type_subtype = (pkt_info->type<<4) | pkt_info->subtype;
		memcpy(pkt_info->sa, frame_80211->addr2, 6);
	}
}

/**
 * @brief Initialize the packet processing structure
 *
 * @param pkt_info structure to be initialized
 */
static void init_pkt_struct_info(struct pkt_struct_info *pkt_info)
{
	pkt_info->max_rate = 0;
	pkt_info->channel = 0;
	pkt_info->signal = -1;
	pkt_info->protocol = -1;
	pkt_info->type = -1;
	pkt_info->subtype = -1;
	pkt_info->type_subtype = -1;
	memset(pkt_info->sa, 0xff, sizeof(pkt_info->sa));
	pkt_info->seen = 0;
	pkt_info->no = 0;
}

/**
 * @brief Convert WiFi frequency to channel representation.
 *
 * @param freq WiFi frequency
 *
 * @return WiFi channel on success, 0 on failure
 */
static uint8_t ieee80211_freq2chan(uint32_t freq)
{
	/* see 802.11 17.3.8.3.2 and Annex J */
	if (freq == 2484)
		return 14;
	else if (freq < 2484)
		return (freq - 2407) / 5;
	else if (freq >= 4910 && freq <= 4980)
		return (freq - 4000) / 5;
	else if (freq <= 45000) /* DMG band lower limit */
		return (freq - 5000) / 5;
	else if (freq >= 58320 && freq <= 64800)
		return (freq - 56160) / 2160;
	else
		return 0;
}

/**
 * @brief Get the 802.11 radiotap header from the packet
 *
 * @param rd	 Data structure where in the extracted data from radiotap header is populated.
 * @param packet 802.11 packet
 * @param len	 Length of the packet
 *
 * @return radiotap heaader len for success, 0 for failure
 */
static uint16_t get_radiotap(struct pkt_struct_info *rd, const u_char *packet, uint32_t len)
{
	struct ieee80211_radiotap_header *radiotap;
	struct ieee80211_radiotap_iterator iterator;

	radiotap=(struct ieee80211_radiotap_header *)packet;

	/**< Validate the radiotap version */
	if (radiotap->it_version > PKTHDR_RADIOTAP_VERSION){
		printf("Unsuported version of radiotap\n");
		return 0;
	}

	/**< Validate the radiotap header len */
	if (radiotap->it_len < 8 || radiotap->it_len > len){
		printf("Invalid packet length\n");
		return 0;
	}

	/**< Initialize the radiotap parser iterator */
	ieee80211_radiotap_iterator_init(&iterator, radiotap, radiotap->it_len, NULL);

	while (ieee80211_radiotap_iterator_next(&iterator) >= 0){

		/**< Only extract RSSI and Channel number from radiotap header*/
		switch (iterator.this_arg_index){
			case IEEE80211_RADIOTAP_CHANNEL:
				rd->channel = (uint8_t) ieee80211_freq2chan((uint32_t)((iterator.this_arg[1])*256+(iterator.this_arg[0])));
				break;
			case IEEE80211_RADIOTAP_DBM_ANTSIGNAL:
				rd->signal = (uint8_t)*iterator.this_arg - 256;
				break;
		}
	}
	return radiotap->it_len;
}

/**
 * @brief The main packet processing function that extracts the
 * packet data from radiotap header and 802.11 frame and
 * populates the WiFi user data record structure.
 *
 * @param header The captured packet header
 * @param packet Wi-Fi management packet excluding header
 */
void pkt_processing(const struct pcap_pkthdr *header, const u_char *packet)
{
	struct pkt_struct_info pkt_info;
	struct ieee80211_frame *frame_80211;
	uint16_t radiotap_len;

	if(!header || !packet)
		return;

	if (header->len < 8){
		printf("Invalid header len\n");
		return;
	}

	/**<  Initialize the buffer */
	init_pkt_struct_info(&pkt_info);

	pkt_info.seen = header->ts.tv_sec;

	/**< Get the radiotap header */
	radiotap_len = get_radiotap(&pkt_info, packet, header->len);
	if (radiotap_len == 0){
		printf("Invalid radiotap header\n");
		return;
	}

	frame_80211 = (struct ieee80211_frame *)(packet + radiotap_len);

	/**< Fill the buffer with the data extracted from the 802.11 frame */
	fill_pkt_struct_info(frame_80211, &pkt_info);

	/**< Apply the RSSI filter */
	if(pkt_info.signal >= pktparser->wifi_ppl_counter_inst->config_udata->signal)
	{
		pktparser->wifi_ppl_counter_inst->wifi_udata->channel = pkt_info.channel;
		pktparser->wifi_ppl_counter_inst->wifi_udata->signal = pkt_info.signal;
		snprintf((char*)pktparser->wifi_ppl_counter_inst->wifi_udata->mac, 20, "%02x:%02x:%02x:%02x:%02x:%02x", pkt_info.sa[0], pkt_info.sa[1], pkt_info.sa[2],pkt_info.sa[3],pkt_info.sa[4],pkt_info.sa[5]);
		pktparser->wifi_ppl_counter_inst->wifi_udata->t_start = pkt_info.seen;
		pktparser->wifi_ppl_counter_inst->wifi_udata->t_end = pkt_info.seen;
		if(pktparser->wifi_ppl_counter_inst->hashmap_udata->add_hashmap)
			pktparser->wifi_ppl_counter_inst->hashmap_udata->add_hashmap(pktparser->wifi_ppl_counter_inst->wifi_udata);
	}
}
