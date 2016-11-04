
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
 *	@file		channel_selector.h
 *
 *	@brief		This file contains the declaration of all APIs exported
 *	                by the channel selector and mode manager module that
 *	                will be used by the WiFi People Counter module.
 *
 *
 */

#ifndef CHANNEL_SELECTOR_H
#define CHANNEL_SELECTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t channel_switcher(const char *ifname);
int32_t wifi_get_nuc_mac_addr(const char *ifname, uint8_t* nuc_mac);
int32_t wifi_set_monitor_mode(const char *ifname);
int32_t wifi_rfkill_unblock(void);

#ifdef __cplusplus
}
#endif

#endif
