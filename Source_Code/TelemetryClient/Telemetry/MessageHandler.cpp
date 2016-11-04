#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTAsync.h"

#if defined(_WIN32)
	#include <Windows.h>
#elif __linux__
	#include <unistd.h>
#endif

#include <thread>
#include <iostream>
#include <sstream>
#include "MessageHandler.h"
#include "MessageQueue.h"
#include "ConfigurationReader.h"

#ifdef _WIN32
	#define STRCPY(dest, src) strcpy_s(dest, src.size() + 1, src.c_str());	
#elif __linux__
	#define STRCPY(dest, src) strncpy(dest, src.c_str(), src.size() + 1);
#endif

using namespace std;

#define CLIENTID    "TelemetryClient"

// Global and const declarations
static int Msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
// Topic to which the client will publish to send messages
const char* PUBLISHTOPIC = (ConfigurationReader::getInstance()->m_MQTTSendTopic).c_str();
// Topic to which the client will subscribe for receiving messages
const char* SUBSCRIBETOPIC = (ConfigurationReader::getInstance()->m_MQTTReceiveTopic).c_str();
// End point server URI for the client to connect with
const char* ENDPOINTURI = (ConfigurationReader::getInstance()->m_MQTTEndPoint).c_str();
// QOS level to be used by the Telemetry client for the communication with MQTT broker
int QOSLEVEL = ConfigurationReader::getInstance()->m_mqttQoS;
// Waiting timeout period for the client to check its connection with server 
const int keepAliveInterval = 20;
//  Discard the state information during connect and disconnect 
const int cleanSession = 1;
// Time interval to allow the client to connect
const int connectionTimeout = 25;
// Flag to stop the Telemetry message processing
bool MessageHandler::stopProcessingFlag = false;

/**
-------------------------------------------------------------------------------
MessageHandler
-------------------------------------------------------------------------------
*   @brief		Constructor
*
*	@param		none
*
*	@return		none
*
*/
MessageHandler::MessageHandler() :
	m_clientInstance(NULL)	
{
	//Define all the initializations	
}

/**
-------------------------------------------------------------------------------
MessageHandler
-------------------------------------------------------------------------------
*   @brief		Destructor
*
*	@param		none
*
*	@return		none
*
*/
MessageHandler::~MessageHandler()
{
	/* Define all the code cleanup and memory delete  */
}

/**
-------------------------------------------------------------------------------
getInstance
-------------------------------------------------------------------------------
*   @brief		To obtain the singleton instance
*
*	@param		none
*
*	@return		Instance of the MessageHandler
*
*/
MessageHandler *MessageHandler::getInstance()
{
	static MessageHandler messageHdlr;
	return &messageHdlr;
}

/**
-------------------------------------------------------------------------------
Init
-------------------------------------------------------------------------------
*   @brief		To initialize the Telemetry client by creating a client instance and establishing connection to server
*
*	@param		None
*
*	@return		None
*
*/
void MessageHandler::Init()
{
	printf("[MessageHandler] INFO :: MessageHandler Init...\n");	

	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_SSLOptions sslopts = MQTTAsync_SSLOptions_initializer; // SSL connection options

	// Create a MQTT connection
	int retCode = MQTTAsync_create(&m_clientInstance, ENDPOINTURI, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if (retCode != MQTTASYNC_SUCCESS)
	{
		std::cout << "[MessageHandler] ERROR:: Failed to create MQTT client !!" << endl;		
		return;
	}

	// Set the MQTT callbacks mainly connectionlost and messagearrived
	retCode = MQTTAsync_setCallbacks(m_clientInstance, NULL, connlost, Msgarrvd, NULL);
	if (retCode != MQTTASYNC_SUCCESS)
	{
		std::cout << "[MessageHandler] ERROR:: Failed to set MQTT callbacks !!" << endl;
	}
	
	conn_opts.keepAliveInterval = keepAliveInterval;    // Waiting timeout period for the client to check its connection with server
	conn_opts.cleansession = cleanSession;				// Discard the state information during connect and disconnect
	conn_opts.onSuccess = onConnect;					// Callback function to be invoked on successful connection
	conn_opts.onFailure = onConnectFailure;				// Callback function to be invoked on failure to establish connection
	conn_opts.context = m_clientInstance;				// Pointer to the application specific context
	conn_opts.ssl = &sslopts;							// SSL options 
	conn_opts.ssl->enableServerCertAuth = 0;			// Disable verfication of server certificate
	conn_opts.connectTimeout = connectionTimeout;		// Time interval to allow the client to connect

	// SSL Client connection to non SSL MQTT server
	if ((retCode = MQTTAsync_connect(m_clientInstance, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("[MessageHandler] ERROR:: Failed to start connect, return code %d\n", retCode);
		return;
	}
	
	//std::thread::id mainthdID = std::this_thread::get_id();
	//std::cout << "Init thread ID :: " << mainthdID << std::endl;	

	// Wait until the connection is successfully established
	while (0 == MQTTAsync_isConnected(m_clientInstance))
	{
		//std::cout << "[MessageHandler] INFO :: Wait until connection is successfully established and subscription is done." << endl;
	}

#ifndef UNIT_TEST
	// Start processing the messages in the queue since the connection is established now
	ProcessMessage();
#endif

}

/**
-------------------------------------------------------------------------------
ProcessMessage
-------------------------------------------------------------------------------
*   @brief		To process the MQTT messages
*
*	@param		none
*
*	@return		none
*
*/
void MessageHandler::ProcessMessage()
{
	int retCode = -1;

	printf("[MessageHandler] INFO :: ProcessMessage - Successful connection\n");

	std::thread::id mainthdID = std::this_thread::get_id();
	std::cout << "ProcessMessage thread ID :: " << mainthdID << std::endl;

	// Obtain the messageList instance
	MessageList *messageListInstance = MessageList::getInstance();

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;

	opts.onSuccess = onSend;
	opts.context = m_clientInstance;

	pubmsg.qos = QOSLEVEL;
	pubmsg.retained = 0;

#ifndef UNIT_TEST
	// Run this loop until there is request to stop the Telemetry message processing
	while (stopProcessingFlag == false)
#endif
	{
		// must take all messages until Count is zero, before waiting again
		while (messageListInstance->Count(1) > 0)
		{
			// Lock the mutex before filling the message queue
			messageListInstance->m_queueMutex.lock();

			std::string message = messageListInstance->TakeFirst(1);
			//cout << "Message to publish ::  " << message << endl;

			// Unlock the mutex after completing the filling
			messageListInstance->m_queueMutex.unlock();

            // Allocate memory to hold the message to be sent
			char *messageBuf = (char *)calloc(message.size() + 1, sizeof(char));
			pubmsg.payload = messageBuf;

			if (pubmsg.payload != NULL)
			{
				// Fill the message payload with the message data to be sent
				STRCPY((char *)pubmsg.payload, message);

				// Size of the message payload
				pubmsg.payloadlen = static_cast<int>(message.length());

				if ((retCode = MQTTAsync_sendMessage(m_clientInstance, PUBLISHTOPIC, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
				{
					printf("[MessageHandler] ERROR:: Failed to start sendMessage, return code %d\n", retCode);						
				}

				//printf("\n  MSG TOKEN :: %d \n  MSG PUBLISHED :: %s\n\n\n", opts.token, pubmsg.payload);
				free(pubmsg.payload);
			}
			else
			{
				cout << "[MessageHandler] ERROR:: Failure to allocate memory " << endl;
			}
		}

#if defined _WIN32
		// Sleep to reduce the CPU cycles being eaten up by this infinite loop
		Sleep(1);
#else
		// Sleep to reduce the CPU cycles being eaten up by this infinite loop
		sleep(1);
#endif
	}	
}
/**
-------------------------------------------------------------------------------
onSend
-------------------------------------------------------------------------------
*   @brief		Callback on successful delivery of the payload to the MQTT broker
*
*	@param1	context		Client handle returned internally for this callback
*	@param2	response	Data returned on completion of a successful delivery of message
*						is held here
*
*	@return		None
*
*/
void MessageHandler::onSend(void* context, MQTTAsync_successData* response)
{
	(void)context;  // To pacify the static code analyser warning
	(void)response; // To pacify the static code analyser warning
	
	//printf(" [MessageHandler] INFO :: Message with token value %d delivery confirmed \n", 
		//response->token);
}

/**
-------------------------------------------------------------------------------
onConnect
-------------------------------------------------------------------------------
*   @brief		Callback on successful connection established
*
*	@param1	context		Client handle returned internally for this callback
*	@param2	response	Data returned on completion of a successfuly API call is held here
*
*	@return		None
*
*/
void MessageHandler::onConnect(void* context, MQTTAsync_successData* response)
{
	(void)response; // To pacify the compiler warnings
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;	
	int rc;

	printf("onConnect : Successful connection\n");
	//std::thread::id mainthdID = std::this_thread::get_id();	
	//std::cout << "onConnect thread ID :: " << mainthdID << std::endl;	
	
	printf("[MessageHandler] INFO :: Subscribing to topic %s\nfor client %s using QoS%d\n\n"
		"Press Q<Enter> to quit\n\n", SUBSCRIBETOPIC, CLIENTID, QOSLEVEL);
	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = client;

	if ((rc = MQTTAsync_subscribe(client, SUBSCRIBETOPIC, QOSLEVEL, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("[MessageHandler] ERROR:: Failed to start subscribe, return code %d\n", rc);		
	}	
}

/**
-------------------------------------------------------------------------------
onConnectFailure
-------------------------------------------------------------------------------
*   @brief		Callback on failure of client trying to connect to server
*
*	@param1	context		Client handle returned internally for this callback
*	@param2	response	Data returned on failure of API call is held here
*
*	@return		None
*
*/
void MessageHandler::onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	(void)context;  // To pacify the static code analyser warning
	//(void)response; // To pacify the static code analyser warning
	
	printf(" [MessageHandler] INFO :: Connect failed with rc %d \n", response ? response->code : 0);
	// Set the connection flag to false since the connection is broken
}

/**
-------------------------------------------------------------------------------
onDisconnect
-------------------------------------------------------------------------------
*   @brief		Callback invoked when the disconnection to server is triggered
*
*	@param1	context		Client handle returned internally for this callback
*	@param2	response	Data returned on completion of a successfuly disconnection
*						is held here
*
*	@return		None
*
*/
void MessageHandler::onDisconnect(void* context, MQTTAsync_successData* response)
{
	(void)context;  // To pacify the static code analyser warning
	(void)response; // To pacify the static code analyser warning
	
	printf("[MessageHandler] INFO :: Successful disconnection \n");
}


/**
-------------------------------------------------------------------------------
connlost
-------------------------------------------------------------------------------
*   @brief		Handle connection loss
*
*	@param1	context		Client handle returned internally for this callback
*	@param2	cause		Cause for the connection loss
*
*
*	@return		none
*
*/
void MessageHandler::connlost(void *context, char *cause)
{	
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_SSLOptions sslopts = MQTTAsync_SSLOptions_initializer; // SSL connection options
	int retCode;

	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);

	printf("Reconnecting\n");
	
	// Try to reconnect and establish the connection incase the client handle is still valid
	if (client != NULL)
	{
		conn_opts.keepAliveInterval = keepAliveInterval;    // Waiting timeout period for the client to check its connection with server
		conn_opts.cleansession = cleanSession;				// Discard the state information during connect and disconnect
		conn_opts.onSuccess = onConnect;					// Callback function to be invoked on successful connection
		conn_opts.onFailure = onConnectFailure;				// Callback function to be invoked on failure to establish connection
		conn_opts.context = client;							// Pointer to the application specific context
		conn_opts.ssl = &sslopts;							// SSL options 
		conn_opts.ssl->enableServerCertAuth = 0;			// Disable verfication of server certificate
		conn_opts.connectTimeout = connectionTimeout;		// Time interval to allow the client to connect

		if ((retCode = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
		{
			printf("[MessageHandler] ERROR:: Failed to start connect, return code %d\n", retCode);
		}
	}
	else
	{
		printf("[MessageHandler] ERROR:: Reconnection Failed.. Telemetry Client no more available \n");
	}
}


/**
-------------------------------------------------------------------------------
onSubscribe
-------------------------------------------------------------------------------
*   @brief		Callback on successful subscription of client on a particular topic
*
*	@param1	context		Client handle returned internally for this callback
*	@param2	response	Data returned on successful subscription is held here
*
*	@return		None
*
*/
void MessageHandler::onSubscribe(void* context, MQTTAsync_successData* response)
{
	(void)context;  // To pacify the static code analyser warning
	(void)response; // To pacify the static code analyser warning
	printf("[MessageHandler] INFO :: Subscribe succeeded for client handle \n");
}

/**
-------------------------------------------------------------------------------
onSubscribeFailure
-------------------------------------------------------------------------------
*   @brief		Callback on failed subscription of client on a particular topic
*
*	@param1	context		Client handle returned internally for this callback
*	@param2	response	Data returned on failure of API call is held here
*
*	@return		None
*
*/
void MessageHandler::onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	(void)context;  // To pacify the static code analyser warning
	(void)response; // To pacify the static code analyser warning
	
	printf("[MessageHandler] INFO :: Subscribe failed with rc %d \n", response ? response->code : 0);
}


/**
-------------------------------------------------------------------------------
Cleanup
-------------------------------------------------------------------------------
*   @brief		To perform the cleanup and shutdown the client in graceful manner
*
*	@param		none
*
*	@return		none
*
*/
void MessageHandler::Cleanup()
{	
	int retCode = -1;
	MQTTAsync_disconnectOptions disconnectOpts = MQTTAsync_disconnectOptions_initializer;

	if ((m_clientInstance != NULL) && (MQTTAsync_isConnected(m_clientInstance) == true))
	{
		if ((retCode = MQTTAsync_disconnect(m_clientInstance, &disconnectOpts)) != MQTTASYNC_SUCCESS)
		{
			printf("[MessageHandler] ERROR:: Failed to disconnect the client, return code %d\n", retCode);
		}

		// Destroy the client instance
		MQTTAsync_destroy(&m_clientInstance);
	}
}


/**
-------------------------------------------------------------------------------
Msgarrvd
-------------------------------------------------------------------------------
*   @brief		Callback on arrival of messages
*
*	@param1	context		Client handle returned internally for this callback
*	@param2 topicName	Topic for which the subscription is done
*	@param3	topicLen	Length of the topic
*	@param4	message		Message received
*
*	@return		None
*
*/
int Msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
	(void)topicName; // To pacify the static code analyser warning
	(void)topicLen;   // To pacify the static code analyser warning
	(void)context; // To pacify the static code analyser warning
	
	//printf("[MessageHandler] INFO :: Message Arrived : %d\n", (intptr_t)client);
	int i;
	std::string messgeArrived = "";

	char * payloadptr = (char*)message->payload;
	if (payloadptr != NULL)
	{
		for (i = 0; i<message->payloadlen; i++)
		{
			messgeArrived += *payloadptr++;
		}
		messgeArrived += '\n';

		//messgeArrived(payloadptr, message->payloadlen + 1);

		MessageList *messageListInstance = MessageList::getInstance();
		messageListInstance->m_queueMutex.lock(); //MUTEX LOCK
		messageListInstance->Add(2, messgeArrived);
		messageListInstance->m_queueMutex.unlock(); // MUTEX UNLOCK
		MQTTAsync_freeMessage(&message);
		MQTTAsync_free(topicName);
	}

	return 1;
}

/**
-------------------------------------------------------------------------------
StopProcessing
-------------------------------------------------------------------------------
*   @brief		Stop the Telemetry message processing by not sending any further MQTT messages
*				to end point
*
*	@param		None
*
*	@return		None
*
*/
void MessageHandler::StopProcessing()
{
	// Flag set to true so that further Telemetry message processing is stopped
	stopProcessingFlag = true;
}


