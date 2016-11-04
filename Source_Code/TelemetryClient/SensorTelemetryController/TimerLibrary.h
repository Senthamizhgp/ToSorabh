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
*	@file		TimerLibrary.h
*
*	@brief
*		        This file contains the Timer API's which will be used by
*				the Sensor Telemetry Controller DLL.
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

#ifndef TIMER_LIB_H
#define TIMER_LIB_H

#include <stdio.h>

class SensorTelemetryController;

#ifdef _WIN32
#include <windows.h>

#elif __linux__
#include <sys/time.h>
#include <signal.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

	int start_ST_timer(int, void(SensorTelemetryController::*)(void));

	void stop_ST_timer(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //TIMER_LIB_H


