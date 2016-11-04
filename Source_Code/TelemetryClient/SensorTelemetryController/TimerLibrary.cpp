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
*	@file		TimerLibrary.cpp
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
#include "TimerLibrary.h"
#include "SensorTelemetryController.h"

void(SensorTelemetryController::*timer_func_handler_pntr)(void);

#ifdef __linux__

void timer_handler(int);

struct itimerval timervalue;

struct sigaction currentHandler, previousHandler;

void timer_sig_handler(int);

/*Function to start the Timer*/
int start_ST_timer(int mSec, void(SensorTelemetryController::*timer_func_handler)(void))
{
	timer_func_handler_pntr = timer_func_handler;

	timervalue.it_interval.tv_sec = mSec / 1000;
	timervalue.it_interval.tv_usec = (mSec % 1000) * 1000;
	timervalue.it_value.tv_sec = mSec / 1000;
	timervalue.it_value.tv_usec = (mSec % 1000) * 1000;
	if (setitimer(ITIMER_REAL, &timervalue, NULL))
	{
		printf("\nsetitimer() error\n");
		return(1);
	}

    currentHandler.sa_handler = &timer_sig_handler;
    currentHandler.sa_flags = SA_NOMASK;
    if (sigaction(SIGALRM, &currentHandler, &previousHandler))
	{
		printf("\nsigaction() error\n");
		return(1);
	}

	return(0);
}

/*Function to handle the call back occured during a timed event*/
void timer_sig_handler(int arg)
{
	(void)arg; // To pacify the compiler warning
	SensorTelemetryController *m_sensorTelemetryController = SensorTelemetryController::getInstance();

	(m_sensorTelemetryController->*(timer_func_handler_pntr))();
}

/*Function to stop the Timer*/
void stop_ST_timer(void)
{
	timervalue.it_interval.tv_sec = 0;
	timervalue.it_interval.tv_usec = 0;
	timervalue.it_value.tv_sec = 0;
	timervalue.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &timervalue, NULL);

    sigaction(SIGALRM, &previousHandler, NULL);
}

#elif _WIN32

HANDLE win_timer;

VOID CALLBACK timer_sig_handler(PVOID, BOOLEAN);

/*Function to start the Timer*/
int start_ST_timer(int mSec, void(SensorTelemetryController::*timer_func_handler)(void))
{
	timer_func_handler_pntr = timer_func_handler;

	if (CreateTimerQueueTimer(&win_timer, NULL, (WAITORTIMERCALLBACK)timer_sig_handler, NULL, mSec, mSec, WT_EXECUTEINTIMERTHREAD) == 0)
	{
		printf("\nCreateTimerQueueTimer() error\n");
		return(1);
	}

	return(0);
}

/*Function to handle the call back occured during a timed event*/
VOID CALLBACK timer_sig_handler(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	SensorTelemetryController *m_sensorTelemetryController = SensorTelemetryController::getInstance();
	(m_sensorTelemetryController->*(timer_func_handler_pntr))();	
}

/*Function to stop the Timer*/
void stop_ST_timer(void)
{
	DeleteTimerQueueTimer(NULL, win_timer, NULL);
	win_timer = NULL;
}
#endif













