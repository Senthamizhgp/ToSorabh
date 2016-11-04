
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
 *	@file		dbconnector.h
 *
 *	@brief		This file contains declaration of all APIs used to
 *	                connect to the MariaDB database and insert records
 *	                by the WiFi People Counter module.
 *
 */

#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include "wifilib.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t init_database_connector(void);
void free_database_connector(void);
int32_t add_record_database(wifi_user_record_data_t * record);

#ifdef __cplusplus
 }
#endif

#endif
