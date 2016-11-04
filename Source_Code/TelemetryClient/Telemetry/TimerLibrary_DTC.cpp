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
*	@file		TimerLibrary_DTC.cpp
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

/*Own Headers*/
#include "TimerLibrary_DTC.h"
#include "DataTransmissionController.h"

void(DataTransmissionController::*PTC_message_send_handler_pntr)(void);
void(DataTransmissionController::*PTC_update_handler_pntr)(void);
void(DataTransmissionController::*Database_purging_handler_pntr)(void);

/* timer functions */
#ifdef _WIN32

HANDLE win_timer;

VOID CALLBACK PTC_message_send_handler(PVOID, BOOLEAN);
VOID CALLBACK PTC_update_handler(PVOID, BOOLEAN);
VOID CALLBACK Database_purging_handler(PVOID, BOOLEAN);

/*Function to start the Timer*/
int start_timer(int mSec, void(DataTransmissionController::*timer_func_handler)(void), int options)
{
	if (options == 1)
	{
		PTC_message_send_handler_pntr = timer_func_handler;

		if (CreateTimerQueueTimer(&win_timer, NULL, (WAITORTIMERCALLBACK)PTC_message_send_handler, NULL, mSec, mSec, WT_EXECUTELONGFUNCTION) == 0)
		{
			printf("\nCreateTimerQueueTimer() error\n");
			return(1);
		}
	}
	else if (options == 2)
	{
		PTC_update_handler_pntr = timer_func_handler;

		if (CreateTimerQueueTimer(&win_timer, NULL, (WAITORTIMERCALLBACK)PTC_update_handler, NULL, mSec, mSec, WT_EXECUTELONGFUNCTION) == 0)
		{
			printf("\nCreateTimerQueueTimer() error\n");
			return(1);
		}
	}
	else if (options == 3)
	{
		Database_purging_handler_pntr = timer_func_handler;

		if (CreateTimerQueueTimer(&win_timer, NULL, (WAITORTIMERCALLBACK)Database_purging_handler, NULL, mSec, mSec, WT_EXECUTELONGFUNCTION) == 0)
		{
			printf("\nCreateTimerQueueTimer3() error\n");
			return(1);
		}
	}
	return(0);
}

/*Function to stop the Timer*/
void stop_timer(void)
{
	DeleteTimerQueueTimer(NULL, win_timer, NULL);	
	win_timer = NULL;
}


/*Function to handle the call back occured during a timed event*/
VOID CALLBACK PTC_message_send_handler(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	DataTransmissionController *m_dataTransmissionController = DataTransmissionController::getInstance();
	(m_dataTransmissionController->*(PTC_message_send_handler_pntr))();
}

VOID CALLBACK PTC_update_handler(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	DataTransmissionController *m_dataTransmissionController = DataTransmissionController::getInstance();
	(m_dataTransmissionController->*(PTC_update_handler_pntr))();
}

VOID CALLBACK Database_purging_handler(PVOID, BOOLEAN)
{
	DataTransmissionController *m_dataTransmissionController = DataTransmissionController::getInstance();
	(m_dataTransmissionController->*(Database_purging_handler_pntr))();
}
#endif













