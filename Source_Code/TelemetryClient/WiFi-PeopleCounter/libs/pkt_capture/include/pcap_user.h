
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
 *	@file		pcap_user.h
 *
 *	@brief		This file contains declaration of all APIs used to
 *	                capture packets using the pcap library by the
 *	                WiFi People Counter module.
 *
 */

#ifndef PCAP_USER_H
#define PCAP_USER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pcap.h>

int32_t wifi_pcap_setup_function(void);
void wifi_start_pcap_capture_function(void);
void wifi_stop_pcap_capture_function(void);
int32_t init_pkt_capture_module(const char* ifname, uint32_t interval);
void free_pkt_capture_module(void);

#endif
