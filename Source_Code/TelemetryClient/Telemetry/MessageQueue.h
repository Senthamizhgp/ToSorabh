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
*	@file		MessageQueue.h
*
*	@brief
*				This file defines the methods for Queue operations that will
*				be used for transmission & reception of messages through
*				MQTT
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <mutex>
#include <list>

/// Container class for RtdMessage objects
class MessageList
{

public:

	// See .cpp for description
	MessageList(); 

	// See .cpp for description
	~MessageList();

	// See .cpp for description
	bool Add(int options, std::string);

	// See .cpp for description
	std::string TakeFirst(int options);

	// See .cpp for description
    unsigned long Count(int options);

	// See .cpp for description
	void Clear(void);

	/// To obtain a singleton instance
	static MessageList* getInstance();

	std::mutex m_queueMutex;

private:
	std::list<std::string> m_messageListSend;         ///<  List to hold the payload to send	
	std::list<std::string> m_messageListReceive;	  ///<  List to hold the received message	
};
#endif // MESSAGEQUEUE_H
