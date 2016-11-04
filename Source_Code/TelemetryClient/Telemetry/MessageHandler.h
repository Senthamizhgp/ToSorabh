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
*	@file		MessageHandler.h
*
*	@brief
*				This file defines all the data structures and methods used for
*				Sending and Receiving telemetry messages through MQTT.
*
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Intel Smart Shelf
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "IMessageHandler.h"
#include "MQTTAsync.h"
#include <memory>

/**
*  @brief MessageHandler
*	Handles the transmission and reception of telemetry messages.
*/
class MessageHandler : public IMessageHandler
{
public:
	/// Constructor
	MessageHandler();

	/// Destructor
	~MessageHandler();

	/// Initializes an MQTT Client and creates a connection to the MQTT broker
	void Init();

	/// Send messages from the Queue
	void ProcessMessage();

	/// To obtain a singleton instance
	static MessageHandler *getInstance();	

	/// To perform the cleanup and shutdown the client in graceful manner
	void Cleanup();

	/// Set the flags, resources to stop processing further messages
	static void StopProcessing();

private:
	/// Functions used during MQTT Client Initialization 
	static void onConnect(void* context, MQTTAsync_successData* response);
	static void onConnectFailure(void* context, MQTTAsync_failureData* response);
	static void onDisconnect(void* context, MQTTAsync_successData* response);
	static void onSend(void* context, MQTTAsync_successData* response);
	static void connlost(void *context, char *cause);
	static void onSubscribe(void* context, MQTTAsync_successData* response);
	static void onSubscribeFailure(void* context, MQTTAsync_failureData* response);

	/*Variables*/
	MQTTAsync m_clientInstance;		///< To hold the Telemetry client instance
	static bool stopProcessingFlag;	///< Flag used to stop processing messages

};

#endif // MESSAGEHANDLER_H