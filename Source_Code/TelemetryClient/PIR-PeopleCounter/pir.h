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
 *	@file		pir.h
 *
 *	@brief		This file contains declaration of all the data structures and
 *				APIs which will be exported by the PIR DLL.
 *
 *	\par  Software License Agreement:
 *
 *	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
 *	Project for use exclusively on PC. The software is owned by the Intel, and
 *	is protected under applicable copyright laws.
 *
 */

#ifndef _PIR_H_
#define _PIR_H_

#ifndef UNIT_TEST
#include "SPInterface.h"
#else
#include "common_ut.h"
#endif // !UNIT_TEST

/**
 * PIR Status codes
 */
enum PIR_STATUS_CODE
{
	PIR_SUCCESS = 0,		///< Success
	PIR_E_FAILURE,			///< Failed to perform the requested operation
	PIR_E_DB_FAILURE,		///< DB failure on using QtSql methods
	PIR_E_SP_FAILURE		///< Failure returned when calling Sunset Pass API
};

/**
-------------------------------------------------------------------------------
init_PIR
-------------------------------------------------------------------------------
 *	This function initialize PIR module.
 *
 *	@pre		Module calling this function must initialize the communication  
 *				channel before intializing PIR module.
 *
 *	@param	devId1	Device ID for right PIR sensor read from Telemetry Configuration XML
 *	@param	devId2	Device ID for left PIR sensor read from Telemetry Configuration XML
 *
 *	@return		On successful intialization this function returns 0. On failure
 *				this function returns -1.
 */
DLL_EXPORT PIR_STATUS_CODE init_PIR(int32_t devId1, int32_t devId2);

/**
-------------------------------------------------------------------------------
deinit_PIR
-------------------------------------------------------------------------------
*	This function de-initialize PIR module.
*
*	@pre		init_PIR must be called before calling this function.
*
*	@return		On successful intialization this function returns 0. On failure
*				this function returns -1.
*/
DLL_EXPORT PIR_STATUS_CODE deinit_PIR(void);

/**
* @defgroup		grpPIR PIR
* The PIR library will allow Telemetry application to interact with PIR sensor device. 
* The PIR sensor generates notification events in case of motion thru the Event notification interface.
*
* Following is typical calling sequence of the PIR Sensor APIs \n
* @ref init_PIR \n
* @ref deinit_PIR \n
*
*/
/**
* @addtogroup	grpPIR
* @{
*/

/**
* @}
*/

#endif // !_PIR_H_
