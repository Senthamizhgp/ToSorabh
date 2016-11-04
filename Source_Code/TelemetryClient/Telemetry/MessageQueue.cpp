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
*	@file		MessageQueue.cpp
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

#include "MessageQueue.h"

using namespace std;

/// Constructor
MessageList::MessageList()
{

}

/// Destructor
MessageList::~MessageList()
{
	Clear();
}

/**
-------------------------------------------------------------------------------
getInstance
-------------------------------------------------------------------------------
*   @brief		To obtain the singleton instance
*
*	@param		none
*
*	@return		Instance of the ConfigurationReader
*/
MessageList* MessageList::getInstance()
{
	static MessageList messageListInstance;
	return &messageListInstance;
}

/**
-------------------------------------------------------------------------------
Add
-------------------------------------------------------------------------------
*   @brief			To add an item into the specified list
*
*	@param1	options	To specify the list. ie, SendList or ReceiveList
*	@param2 message	Message to be inserted in the list
*
*	@return bool	Success/Failure of the operation.
*/
bool MessageList::Add(int options, std::string message)
{
	// Check we have a valid message to add
	if (message.size() == 0)
	{
		return false;
	}

	if (options == 1)
	{
		m_messageListSend.push_back(message);
	}
	else if (options == 2)
	{
		m_messageListReceive.push_back(message);
	}

	return true;
}

/**
-------------------------------------------------------------------------------
TakeFirst
-------------------------------------------------------------------------------
*   @brief			To pop the first item from the list.
*
*	@param	options	To specify the list. ie, SendList or ReceiveList
*
*	@return string	Message that is poped out from the list
*/
std::string MessageList::TakeFirst(int options)
{
	std::string first = "";
	if (options == 1)
	{
		first = m_messageListSend.front();
		m_messageListSend.pop_front();
	}
	else if (options == 2)
	{
		first = m_messageListReceive.front();
		m_messageListReceive.pop_front();
	}

	return first;
}

/**
-------------------------------------------------------------------------------
Count
-------------------------------------------------------------------------------
*   @brief			Gives the size of the list
*
*	@param1	options	To specify the list. ie, SendList or ReceiveList
*
*	@return int		Size of the list.
*/
unsigned long MessageList::Count(int options)
{
    unsigned long size = 0;
    m_queueMutex.lock();
    if (options == 1)
    {
        size = m_messageListSend.size();
    }
    else
    {
        size = m_messageListReceive.size();
    }
    m_queueMutex.unlock();

    return size;
}

/**
-------------------------------------------------------------------------------
Clear
-------------------------------------------------------------------------------
*   @brief		Deletes all the messages in the list
*
*	@param		None
*
*	@return 	None
*/
void MessageList::Clear(void)
{
	//m_messageList.remove_if(releaseMessage);
}
