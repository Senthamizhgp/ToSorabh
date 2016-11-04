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
 *	@file		common.c
 *
 *	@brief		This file contains definition of all APIs which will be exported 
 *				by the PIR module.
 *
 *	\par  Software License Agreement:
 *
 *	The software supplied herewith by Intel's vendor to Intel for Sunset Pass 
 *	Project for use exclusively on PC. The software is owned by the Intel, and 
 *	is protected under applicable copyright laws.
 *
 */
 
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef __WIN32

#elif __linux__
#include <signal.h>
#endif

#include "timer_lib.h"

/* timer functions */
#ifdef _WIN32

#elif __linux__
int start_periodic_timer(timer_t *id, unsigned int time_msec, void(*callback)())
{
	struct sigevent timer_event;
	struct itimerspec timer;
	struct sigaction sig_action;
	static int timer_cnt = 0;
	int sigNo = SIGRTMIN + timer_cnt;

	if (sigNo > SIGRTMAX)
	{
		printf("Max timer used\n");
		return -1;
	}

	/* Set up signal handler. */
	sig_action.sa_flags = SA_SIGINFO;
	sig_action.sa_sigaction = callback;
	sigemptyset(&sig_action.sa_mask);
	if (sigaction(sigNo, &sig_action, NULL) == -1)
	{
		perror("Failed to install signal: ");
		return -1;
	}

	/* Set and enable alarm */
	timer_event.sigev_notify = SIGEV_SIGNAL;
	timer_event.sigev_signo = sigNo;
	timer_event.sigev_value.sival_ptr = id;
	if (timer_create(CLOCK_REALTIME, &timer_event, id) == -1)
	{
		perror("Failure creating timer: ");
		return -1;
	}

	timer.it_value.tv_sec = time_msec / 1000;
	timer.it_interval.tv_sec = time_msec / 1000;
	timer.it_value.tv_nsec = (time_msec % 1000) * 1000000;
	timer.it_interval.tv_nsec = (time_msec % 1000) * 1000000;

	if (timer_settime(*id, 0, &timer, NULL) == -1)
	{
		perror("Failure to settime: ");
		return -1;
	}

	printf("Timer %d set with signal # %d\n", *id, sigNo);

	timer_cnt++;

	return 0;

}
#endif

/* eof */
