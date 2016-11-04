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
*	@file		DataTransmissionController.cpp
*
*	@brief		
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

/* System headers */
#include <stdio.h>
#include <iostream>
#include <sstream>

/* Own headers */
#include "DataTransmissionController.h"
#include "SensorTelemetryController.h"
#include "ConfigurationReader.h"
#include "PublishTableController.h"
#include "MessageHandler.h"
#include "IDEX.h"
#include "datatranslator.h"
#include "MessageQueue.h"

#if defined(REALSENSE)
#include "RealSensePeopleCounter.h"
#endif

#include "pir.h"

#if defined(__linux__)
#include "timer_lib.h"
#include "bluetooth.h"
#include "bluetoothhandler.h"
#include "wifi_ppl_counter.h"
#endif

using namespace std;

// Replace the 'EntryFunction, Arg1, Arg2' with your corresponding module entryfunction and arguments (if any).
//			Example : Init, 50, 'ABC'
// The arguments count can vary and even can have no arguments. In that case just provide the entryfunction alone (without comma)
//			Example : Initialize

//#define PIRENTRYPOINT init_PIR  // Entry for PIR module

#if defined(__linux__)
#define WIFIENTRYPOINT start_peoplecounter// Entry for WiFi module
#define BLEENTRYPOINT EntryFunction, Arg1, Arg2  // Entry for BLE module
#endif

///< Global and Static initializations
DataTransmissionController* DataTransmissionController::instance = NULL;

QSqlDatabase DataTransmissionController::m_dtcDatabaseCtn = QSqlDatabase();
QSqlDatabase DataTransmissionController::m_dtcSelectCtn = QSqlDatabase();
QSqlDatabase DataTransmissionController::m_dtcUpdateCtn = QSqlDatabase();
QSqlDatabase DataTransmissionController::m_dtcMQTTUpdateCtn = QSqlDatabase();

static bool DTCConnectionStatus = false;

/**
-------------------------------------------------------------------------------
DataTransmissionController
-------------------------------------------------------------------------------
* Constructor for the DataTransmissionController
*
*	@param	None
*
*	@return	None
*
*/
DataTransmissionController::DataTransmissionController() :
	m_pirDevId_1(-1),
	m_pirDevId_2(-1),
	m_configReader(NULL)
{
	// Define all the class initialization	
	// Initialized only at the first invocation of ctor
	static bool initFlag = false;

	// True only for the first instance of DataAcquisition
	if (true != initFlag)
	{
		m_configReader = ConfigurationReader::getInstance();

		DTC_DB_STATUS_CODE connectionStatus = getConnection(m_configReader->m_DatabaseHostName, m_configReader->m_DatabaseName, m_configReader->m_DatabaseUserName,
			m_configReader->m_DatabasePassword, m_configReader->m_DatabasePort);

		if (DTC_DB_OPEN_SUCCESS != connectionStatus)
		{
			DTCConnectionStatus = false;
			// Failed to open the required database connections
			cout << "[DTC Constructor] Warning- Failure to open database connections with errorCode -"
				<< connectionStatus << endl;
		}

		// Set the Data TransmissionController connection status
		DTCConnectionStatus = true;

		// To ensure the getConnection is called only once
		initFlag = true;
	}
}

/**
-------------------------------------------------------------------------------
PushToMessageQueue
-------------------------------------------------------------------------------
*	Pushes the message to the queue to be processed by Telemetry Client
*
*	@param	jsonMessage		Message to be pushed
*
*	@return None
*
*/
void DataTransmissionController::PushToMessageQueue(std::string & jsonMessage)
{
	MessageList *messageListInstance = MessageList::getInstance();
	
	// Lock the mutex before filling the message queue
	messageListInstance->m_queueMutex.lock();

	// Fill the message queue with message
	messageListInstance->Add(1, jsonMessage);

	// Unlock the mutex after completing the filling
	messageListInstance->m_queueMutex.unlock();
}

/**
-------------------------------------------------------------------------------
DataTransmissionController
-------------------------------------------------------------------------------
* Destructor for the DataTransmissionController
*
*	@param	None
*
*	@return	None
*
*/
DataTransmissionController::~DataTransmissionController()                 
{
	// Define all the code cleanup and memory delete 
}

/**
-------------------------------------------------------------------------------
getInstance
-------------------------------------------------------------------------------
*	Obtain the singleton instance of the Datatransmission Controller
*
*	@param
*
*	@return
*
*/
DataTransmissionController* DataTransmissionController::getInstance()
{
	static DataTransmissionController dataTransmissionHdlr;
	return &dataTransmissionHdlr;
}

/**
-------------------------------------------------------------------------------
getConnection
-------------------------------------------------------------------------------
*   @brief						This function creates a connection to the database.
*
*	@param HostName			Hostname of the connection
*	@param DatabaseName		Name of the database
*   @param UserName			UserName(Credentials) for the database
*   @param Password			Password(Credentials) for the database
*	@param	Port			Port to be used for the database
*
*	@return	DTC_DB_STATUS_CODE	Status code for the database connection
*/
DTC_DB_STATUS_CODE DataTransmissionController::getConnection(string HostName, 
															string DatabaseName, 
															string UserName, 
															string Password, 
															int Port)
{
	//connection configuration should done only one time for db
	if (!m_dtcDatabaseCtn.isOpen()) 
	{
		m_dtcDatabaseCtn = QSqlDatabase::addDatabase("QMYSQL", "DTC_Connection");
		m_dtcDatabaseCtn.setHostName(QString::fromStdString(HostName));
		m_dtcDatabaseCtn.setDatabaseName(QString::fromStdString(DatabaseName));
		m_dtcDatabaseCtn.setUserName(QString::fromStdString(UserName));
		m_dtcDatabaseCtn.setPassword(QString::fromStdString(Password));
		m_dtcDatabaseCtn.setPort(Port);

		if (!m_dtcDatabaseCtn.open())
		{
			cout << "DataTransmissionController ::  Failed to open database:" << endl;

			return DTC_CONNECTION_FAILURE;
		}
		else
		{
			m_dtcSelectCtn = QSqlDatabase::cloneDatabase(m_dtcDatabaseCtn, "DTC_Select_Connection");
			m_dtcUpdateCtn = QSqlDatabase::cloneDatabase(m_dtcDatabaseCtn, "DTC_Update_Connection");
			m_dtcMQTTUpdateCtn = QSqlDatabase::cloneDatabase(m_dtcDatabaseCtn, "DTC_MQTTUpdate_Connection");
			if (!m_dtcSelectCtn.open())
			{
				cout << "[DTC::Select Query]::Failed to open the database connection" << endl;
				return DTC_SELECT_CONNECTION_FAILURE;
			}
			if (!m_dtcUpdateCtn.open())
			{
				cout << "[DTC::Update Query]::Failed to open the database connection" << endl;
				return DTC_UPDATE_CONNECTION_FAILURE;
			}
			if (!m_dtcMQTTUpdateCtn.open())
			{
				cout << "[DTC::MQTTUpdate Query]::Failed to open the database connection" << endl;
				return DTC_UPDATE_CONNECTION_FAILURE;
			}
		}
	}	

	return DTC_DB_OPEN_SUCCESS;
}

/**
-------------------------------------------------------------------------------
execSelectQuery
-------------------------------------------------------------------------------
* Perform a select query onto the SQL database
*
*	@param	strQuery	Query string to be performed
*	@param	arguments	Any additional arguments for the query
*	@param	records		Output to be filled with the query result 
*
*	@return	Refer this Enum @ref DTC_DB_STATUS_CODE		
*
*/
DTC_DB_STATUS_CODE DataTransmissionController::execSelectQuery(const string &strQuery, 
																map<string, string> &arguments, 
																QList<QMap<QString, QVariant>> &records)
{	
	QVariantMap args;
	QString key;
	QVariant value;
	QString sql = QString::fromStdString(strQuery);
	records.clear();
	for (map<string, string>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter)
	{
		key = QString::fromStdString((*iter).first);
		value = QString::fromStdString((*iter).second);
		args.insert(key, value);

	}

	if (m_dtcSelectCtn.isOpen())
	{
		QSqlQuery *query = new QSqlQuery(m_dtcSelectCtn);
		/*
		* FROM QT DOCUMENTATION - Portability note: Some databases choose to
		* delay preparing a query until it is executed the first time. In this case,
		* preparing a syntactically wrong query succeeds, but every consecutive
		* exec() will fail. Hence the return for prepare() is not verified.
		*/
		query->prepare(sql);
		foreach(QString key_index, args.keys()) query->bindValue(key_index, args.value(key_index));
		if (query->exec())
		{
			// Process the query results
			while (query->next())
			{
				int count = query->record().count();

				for (int index = 0; index < count; index++)
				{
					key = query->record().field(index).name();
					value = query->record().field(index).value();
					selection_map.insert(key, value);
				}

				records.push_back(selection_map);
				selection_map.clear();
			}
		}
		else
		{
			/*
			* FROM MYSQL DOCUMENTATION - The most common reason for the MySQL
			* server has gone away error  is that the server timed out and closed
			* the connection. In this case, you normally get one of the following
			* error codes (which one you get is operating system-dependent).
			*
			* CR_SERVER_GONE_ERROR (2006) 	- 	The client couldn't send a question
			* 								 	to the server.
			*
			* CR_SERVER_LOST (2013) 		- 	The client didn't get an error when
			*									writing to the server, but it didn't
			*									get a full answer (or any answer) to the question.
			*/
			if (query->lastError().number() == 2006 || query->lastError().number() == 2013)
			{
				/* close and open connection to execute query to handle
				* CR_SERVER_GONE_ERROR or CR_SERVER_LOST */
				m_dtcSelectCtn.close();
				if (m_dtcSelectCtn.open() && !query->exec())
				{
					std::cout << "[DTC::Select Query]::Failed to execute query with errorcode - "
						<< query->lastError().text().toStdString() << endl;
					delete query;
					return DTC_SELECT_QUERY_FAILURE;
				}
				else
				{
					/* Process the query results obtained after calling execute query
					*  once again after reconnection */
					while (query->next())
					{
						int count = query->record().count();

						for (int index = 0; index < count; index++)
						{
							key = query->record().field(index).name();
							value = query->record().field(index).value();
							selection_map.insert(key, value);
						}

						records.push_back(selection_map);
						selection_map.clear();
					}
				}
			}
		}

		delete query;
		return DTC_SELECT_QUERY_SUCCESS;
	}

	return DTC_SELECT_QUERY_FAILURE;
}

/**
-------------------------------------------------------------------------------
execUpdateQuery
-------------------------------------------------------------------------------
* Perform a update query onto the SQL database
*
*	@param	connectionName	Connection name on which the update query to be performed
*	@param	strQuery		Query string to be performed
*	@param	arguments		Any additional arguments for the query
*
*	@return	Refer this Enum @ref DTC_DB_STATUS_CODE
*
*/
DTC_DB_STATUS_CODE DataTransmissionController::execUpdateQuery(QSqlDatabase connectionName, 
																const string &strQuery, 
																map<string, string> &arguments)
 {
	 QVariantMap args;
	 QString key;
	 QString value;	 
	 QString sql = QString::fromStdString(strQuery);
	 for (map<string, string>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter)
	 {
		 key = QString::fromStdString((*iter).first);
		 value = QString::fromStdString((*iter).second);
		 args.insert(key, value);
	 }
	 if(connectionName.isOpen())
	 {
		 QSqlQuery *query = new QSqlQuery(connectionName);
		 /*
		 * FROM QT DOCUMENTATION - Portability note: Some databases choose to
		 * delay preparing a query until it is executed the first time. In this case,
		 * preparing a syntactically wrong query succeeds, but every consecutive
		 * exec() will fail. Hence the return for prepare() is not verified.
		 */
		query->prepare(sql);
		foreach(QString key_index, args.keys()) query->bindValue(key_index, args.value(key_index));
		if (!query->exec())
		{
			/*
			* FROM MYSQL DOCUMENTATION - The most common reason for the MySQL
			* server has gone away error  is that the server timed out and closed
			* the connection. In this case, you normally get one of the following
			* error codes (which one you get is operating system-dependent).
			*
			* CR_SERVER_GONE_ERROR (2006) 	- 	The client couldn't send a question
			* 								 	to the server.
			*
			* CR_SERVER_LOST (2013) 		- 	The client didn't get an error when
			*									writing to the server, but it didn't
			*									get a full answer (or any answer) to the question.
			*/
			if (query->lastError().number() == 2006 || query->lastError().number() == 2013)
			{
				/* close and open connection to execute query to handle
				* CR_SERVER_GONE_ERROR or CR_SERVER_LOST */
				connectionName.close();
				if (connectionName.open() && !query->exec())
				{
					std::cout << "[DTC::Update Query]::Failed to execute query with errorcode - "
						<< query->lastError().text().toStdString() << endl;
					delete query;
					return DTC_UPDATE_QUERY_FAILURE;
				}
			}
		}
		delete query;	
		return DTC_UPDATE_QUERY_SUCCESS;
	 } 		 
	 return DTC_UPDATE_QUERY_FAILURE;
}

/**
-------------------------------------------------------------------------------
DataTransmissionCtlrHdlr
-------------------------------------------------------------------------------
*	Event handler when reporting frequency timer elapsed and it starts reading the publish table.
*	
*	@param		None
*
*	@return		None
*
*/
void DataTransmissionCtlrHdlr (void)
{
	DataTransmissionController *m_dataAcquistionController = DataTransmissionController::getInstance();

	// Process the publish table and obtain the data to be transmitted
	m_dataAcquistionController->ReadPublishTable();
}
	   
/**
-------------------------------------------------------------------------------
ReadPublishTable
-------------------------------------------------------------------------------
*	Starts reading the publish table, add necessary headers and pushes the message 
*	in to the Telemetry client queue
*
*	@param		None 
*
*	@return		None
*
*/
void DataTransmissionController::ReadPublishTable()
{
	map<string, string> args_dataRetrieve; // As of now no arguments, included for future purpose
	QList<QMap<QString, QVariant>>::iterator listofmapiter;
	QMap<QString, QVariant>::iterator mapiter;		
	QList <QMap<QString, QVariant>>	recs;
	ConfigurationReader *configReader = ConfigurationReader::getInstance();
	static unsigned short messageReportingCounter = 0;
	static unsigned short purgingCounter = 0;
	static unsigned short messageReportingFreq;
	static unsigned short purgingFreq;

	QString value_json="" ;
	QString sensordata = "";
	time_t now = time(NULL);
	string timenow = QString::number(now).toStdString();
	// TEMP : To be removed
	static bool isBusyFlag = false; 
	static int Cycle = 0;
	
	const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";
	// get the instance of Data translator and Publish controller
	Datatranslator *p_datatranslator = Datatranslator::getInstance();
	PublishTableController *p_publishTableController = PublishTableController::getInstance();

	// Set the reporting frequency
	messageReportingFreq = configReader->m_ReportingFreq / 60;

	// Set the purging frequency
	purgingFreq = configReader->m_DatabasePurgingFreq / 60;

	// Increment the send counter value
	messageReportingCounter++;

	// Increment the purging counter value
	purgingCounter++;

	// Check if busy flag is true pointing that the previous cycle processing is still in progress
	// Hence exit this cycle without doing any operation 
	if (isBusyFlag == true)	 
	{
		cout << " ** READ PT AGAIN ** " << time (NULL) << endl; // TEMP : To be removed
		return;
	}
	
	Cycle++; // TEMP : To be removed
	cout << " ** Processing Publish Table  - Cycle :   " << Cycle << " TIME :: " << time (NULL)  << "  ** " << endl;

	// Set the busy flag to true so that it does not process following operations in next cycle in case
	// prior cycle is not yet completed.
	isBusyFlag = true;
	
	// Process the received acknowledgement for the sent messages and update publish table's 'isSent' flag accordingly
	ProcessReceivedMessage();

	// Purge publish table once the specified frequency is arrived
	if (purgingCounter >= purgingFreq)
	{
		// Call the purging handler
		DatabasePurgingHandler();

		//Reset the purging counter
		purgingCounter = 0;
	}

	// Start reading individual module table entries and updating them in publish table after converting them to JSON data
	p_publishTableController->UpdatePublishTable();

	if (messageReportingCounter >= messageReportingFreq)
	{		// Process the JSON data updated in the publish table
		if (DTC_SELECT_QUERY_SUCCESS == execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs))
		{
			if (recs.size() > 0)
			{
				for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
				{
					string timestamplimiter;
					for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
					{
						QString key = mapiter.key();
						QString value = mapiter.value().typeName();

						if (key == "Message_ID")
						{
							string messageid = mapiter.value().toString().toStdString();
							string time_stampstring = timenow;						
							 timestamplimiter = time_stampstring + "." + messageid;
							//p_datatranslator->sufix_and_prefix(timestamplimiter);

						}
						else if (key == "Type")
						{
							sensordata = mapiter.value().toString();					
						}
						else if (key == "Value")
						{
							if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
							{
								value_json = mapiter.value().toString();					
							}
						}
					}
				
					// Convert the read data into the required JSON string 
					string jsonstring = p_datatranslator->converting_Final_JSON(value_json, sensordata, timestamplimiter);
			
					// Push it into the Message queue to be processed by the Telemetry client 
					PushToMessageQueue(jsonstring);
				
					value_json.clear();
					sensordata.clear();				
				}	
			}
		}
		else
		{
			cout << "[DTC::ReadPublishTable] ERROR::Failure in Select Query" << endl;
		}
	
		// Reset the counter for the next cycle
		messageReportingCounter = 0;
	}

	// Reset the busy flag so that it is available for processing in next cycle
	isBusyFlag = false;
	
	cout << "READ PT completed :: " << time(NULL) << endl; // TEMP : To be removed
}


/**
-------------------------------------------------------------------------------
SensorTelThreadController
-------------------------------------------------------------------------------
*	Handles init for Sensor Telemetry Controller Thread 
*
*	@param	None
*
*	@return	None
*
*/
void DataTransmissionController::SensorTelThreadController()
{
	std::thread::id mainthdID = std::this_thread::get_id();
	std::cout << "SensorController thread ID :: " << mainthdID << std::endl;
	SensorTelemetryController *p_sensorTelemetryController = SensorTelemetryController::getInstance();
		
	// Initialize the Sensor Telemetry and check device status
	int errorCount = -1;
	SP_STATUS_CODE retVal = p_sensorTelemetryController->initSensorTelemetry(errorCount);
	if (retVal != SP_SUCCESS)
	{
		cout << "Failure to init for Sensor Telemetry device for " << errorCount << " devices" << endl;
	}

	// Start the handler to process the Sensor Telemetry data
	p_sensorTelemetryController->SensorTelemetryDataHdlr();
}

/**
-------------------------------------------------------------------------------
PublishTableUpdationTimerEvent
-------------------------------------------------------------------------------
*	Event handler when timer elapses to process the individual module data
*
*	@param	None
*
*	@return	None
*
*/
void PublishTableUpdationTimerEvent()
{
	PublishTableController *p_publishTableController = PublishTableController::getInstance();	
	
	// Update the publish table by reading the individual module tables after converting them to JSON data
	p_publishTableController->UpdatePublishTable();
}

/**
-------------------------------------------------------------------------------
PublishTableUpdationEvent
-------------------------------------------------------------------------------
*	Event handler when timer elapses to process the individual module data
*
*	@param	None
*
*	@return	None
*
*/
void DataTransmissionController::PublishTableUpdationEvent()
{
	PublishTableController *p_publishTableController = PublishTableController::getInstance();

	// Update the publish table by reading the individual module tables after converting them to JSON data
	p_publishTableController->UpdatePublishTable();
}


/**
-------------------------------------------------------------------------------
initPort
-------------------------------------------------------------------------------
*	Initialize the port for the Sunset Pass API
*
*	@param	None
*
*	@return	Refer to Enum @SP_STATUS_CODE
*
*/
SP_STATUS_CODE DataTransmissionController::initPort()
{
	SP_STATUS_CODE responseCode = SP_SUCCESS;

	//open system
	responseCode = spSystemOpen();
	if (SP_SUCCESS != responseCode) 
	{
		cout << "System initialization failed" << endl;
		responseCode = spSystemClose();
		//If unable to de init
		if (SP_SUCCESS != responseCode) 
		{
			cout << "SystemDeinit failed" << endl;
			return responseCode;
		}
		return responseCode;
	}

	//get device list
	SPDeviceList spDeviceList;
	responseCode = spGetDevicesDetail(&spDeviceList);
	if (SP_SUCCESS != responseCode) 
	{
		cout << "spGetDevicesDetail Failed" << endl;
		responseCode = spSystemClose();
		if (SP_SUCCESS != responseCode)
		{
			cout << "SystemDeinit Failure" << endl;
			return responseCode;
		}
		return responseCode;
	}

	// Obtain the configuration reader instance
	ConfigurationReader *m_configInstance = ConfigurationReader::getInstance();

	// Check if the PIR module or Sensor Telemetry module is configured as enabled	
	if ((m_configInstance->m_PIREnable == 1) || (m_configInstance->m_sensorEnable == 1))
	{		
		// Obtain the device id for the PIR and other Telemetry sensors
		for (int it = 0; it < spDeviceList.nDeviceListSize; it++)
		{
			cout << "  Device ID : " << (spDeviceList.structDeviceList[it].m_nDeviceID) << " Device description : " << spDeviceList.structDeviceList[it].m_sDeviceDescription << endl;
			if ((SP_DEV_SENSOR == (int)(spDeviceList.structDeviceList[it].m_eDeviceType) && (true == (int)(spDeviceList.structDeviceList[it].m_bDeviceIsPowerdOn))))
			{			
				if (m_configInstance->m_PIREnable == 1)
				{
					if (strcmp(spDeviceList.structDeviceList[it].m_sName, "GPI Sensor I") == 0)
					{
						m_pirDevId_1 = (spDeviceList.structDeviceList[it].m_nID);
					}
					else if (strcmp(spDeviceList.structDeviceList[it].m_sName, "GPI Sensor II") == 0)
					{
						m_pirDevId_2 = (spDeviceList.structDeviceList[it].m_nID);
					}
				}
				
				if (m_configInstance->m_sensorEnable == 1)
				{
					for (int iter = 0; iter < m_configInstance->m_sensorCount; iter++)
					{
						if (m_configInstance->m_sensorInfo[iter].sensorName == spDeviceList.structDeviceList[it].m_sName)
						{
							m_configInstance->m_sensorInfo[iter].sensorId = (int)(spDeviceList.structDeviceList[it].m_nID);
							m_configInstance->m_sensorInfo[iter].deviceId = (int)(spDeviceList.structDeviceList[it].m_nDeviceID);
							m_configInstance->m_sensorInfo[iter].sensorType = spDeviceList.structDeviceList[it].m_sDeviceDescription;
							m_configInstance->m_sensorInfo[iter].sensorUnit = "?C";
						}
					}
				}
			}
		}
	}

	return responseCode;
}

/**
-------------------------------------------------------------------------------
StringSplitter
-------------------------------------------------------------------------------
*	Split the received string to obtain the timestamp and message Id
*
*	@param	str			String to be split
*	@param	delimiter	Delimiter to be used to split it
*
*	@return	vector<string>	Array of the string
*
*/
vector<string> DataTransmissionController::StringSplitter(string str, char delimiter) 
{
	vector<string> retValue;
	stringstream stringStream(str); // Turn the string into a stream.
	string tok;

	while (getline(stringStream, tok, delimiter))
		retValue.push_back(tok);
	
	return retValue;
}

/**
-------------------------------------------------------------------------------
is_number
-------------------------------------------------------------------------------
*	To check if the string contain only number
*
*	@param	stringToCheck	String which should be validated whether it is only number or not
*
*	@return	bool			Status of the validation. true or false
*
*/
bool DataTransmissionController::is_number(const std::string& stringToCheck)
{
	std::string::const_iterator it = stringToCheck.begin();

	while (it != stringToCheck.end() && (isdigit(*it) || (*it == '.')))
		++it;
	if (!stringToCheck.empty() && it == stringToCheck.end())
	{
		// The passed string contains only number
		return true;
	}
	else
	{
		// The passed string contains atleast one character other than number
		return false;
	}
}


/**
-------------------------------------------------------------------------------
ProcessReceivedMessage
-------------------------------------------------------------------------------
*	Process the received acknowledgement message from the subscriber 
*
*	@param	None
*
*	@return	None
*
*/
void DataTransmissionController::ProcessReceivedMessage()
{
	DataTransmissionController *DTCInstance = DataTransmissionController::getInstance();
	MessageList *messageListInstance = MessageList::getInstance();
	const char *dataUpdationQuery = "update publish_table set isSent = 1 where Message_ID = :Message_ID";
	map<string, string> args_dataUpdation;
	int receivedFlag = 0;
	string timeStamp, messageID;
	int check = 0;	
	
	while (messageListInstance->Count(2) > 0)
	{
		// Lock the receive queue before processing the response message
		messageListInstance->m_queueMutex.lock();

		// Read the acknowledgment message for processing
		std::string message = messageListInstance->TakeFirst(2);

		// Unlock the receive queue since the read is complete
		messageListInstance->m_queueMutex.unlock();

		vector<string> tokens = DTCInstance->StringSplitter(message, '\"');

		for (string iter : tokens)
		{
			if (iter == "received")
			{
				receivedFlag = 1;
			}
			if (receivedFlag == 1 && iter == "]}")
			{
				receivedFlag = 0;
			}
			if (receivedFlag == 1 && DTCInstance->is_number(iter))
			{
				vector<string> token = DTCInstance->StringSplitter(iter, '.');
				check = 1;
				for (string it : token)
				{
					if (check == 1)
					{
						timeStamp = it;
					}
					else
					{
						messageID = it;
					}
					check++;
				}

				args_dataUpdation[":Message_ID"] = messageID;
				//std::cout << "Timestamp :: " << timeStamp << endl;
				//std::cout << "Message ID :: " << messageID << endl;
				if (DTC_UPDATE_QUERY_FAILURE == execUpdateQuery(m_dtcMQTTUpdateCtn, dataUpdationQuery, args_dataUpdation))
				{
					// In case of updation failure, the message will be sent again as per normal process and response for that will be handled in next cycle
					cout << "[DTC::ProcessReceivedMessage] ERROR::Failure in Update Query" << endl;
				}
			}
		}			
	}
}


/**
-------------------------------------------------------------------------------
DatabasePurgingHandler
-------------------------------------------------------------------------------
*	Purge publish table data at regular specified intervals
*
*	@param	None
*
*	@return	None
*
*/
void DataTransmissionController::DatabasePurgingHandler()
{
	DataTransmissionController *DTCInstance = DataTransmissionController::getInstance();
	const char *dataUpdationQuery = "DELETE FROM publish_table WHERE isSent = 1";
	map<string, string> args_dataUpdation;

	if (DTC_UPDATE_QUERY_FAILURE == DTCInstance->execUpdateQuery(m_dtcUpdateCtn, dataUpdationQuery, args_dataUpdation))
	{
		cout << "[DTC::DatabasePurgingHandler] ERROR::Failure in Update Query" << endl;
	}
}


/**
---------------------------------------------------------------------------------------------------
cleanup
---------------------------------------------------------------------------------------------------
*	@brief		Cleaup method for the Telemetry application for proper shutdown
*
*	@param		None
*
*	@return		None
*
*/
void cleanup()
{
	// Perform all the cleanup before gracefully shutting down the Telemetry application
	// Like perform individual module clean up activities, kill all the static singletons, disconnect MQTT etc
#if defined (__linux__)
		//Stop the WiFi people counter module
		//stop_peoplecounter();
#endif

}

bool executeQuery(QSqlDatabase connectionName, const string &strQuery)
{
	QVariantMap args;
	QString key;
	QString value;
	QString sql = QString::fromStdString(strQuery);

	if (connectionName.isOpen())
	{
		QSqlQuery *query = new QSqlQuery(connectionName);
		/*
		* FROM QT DOCUMENTATION - Portability note: Some databases choose to
		* delay preparing a query until it is executed the first time. In this case,
		* preparing a syntactically wrong query succeeds, but every consecutive
		* exec() will fail. Hence the return for prepare() is not verified.
		*/
		query->prepare(sql);
		if (!query->exec())
		{
			/*
			* FROM MYSQL DOCUMENTATION - The most common reason for the MySQL
			* server has gone away error  is that the server timed out and closed
			* the connection. In this case, you normally get one of the following
			* error codes (which one you get is operating system-dependent).
			*
			* CR_SERVER_GONE_ERROR (2006) 	- 	The client couldn't send a question
			* 								 	to the server.
			*
			* CR_SERVER_LOST (2013) 		- 	The client didn't get an error when
			*									writing to the server, but it didn't
			*									get a full answer (or any answer) to the question.
			*/
			if (query->lastError().number() == 2006 || query->lastError().number() == 2013)
			{
				/* close and open connection to execute query to handle
				* CR_SERVER_GONE_ERROR or CR_SERVER_LOST */
				connectionName.close();
				if (connectionName.open() && !query->exec())
				{
					std::cout << "[DTC]::Failed to execute query with errorcode - "
						<< query->lastError().text().toStdString() << endl;
					delete query;
					return false;
				}
			}
		}
		delete query;
		return true;
	}
	return false;
}

/**
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
* << Func Description >>
*
*	@param
*
*	@return
*
*/
int main(int argc, const char **argv)
{
	// Input arguemnts
	// To pacify the compiler warning
	cout << "Arg count : " << argc << endl;
	cout << "Executing application : " << argv[0] << endl;

	thread SensorTelemetryControllerThread;
	thread MessageHandlerThread;
	thread IDEXthread;
	thread WiFiHandlerThread;
	thread BLEHandlerThread;
#if defined(REALSENSE)
	thread RealsenseHandlerThread;
#endif

	// Obtain the instance of the Configuration reader
	ConfigurationReader *configReader = ConfigurationReader::getInstance();

	// SQL Connection to create a Telemetry database
	QSqlDatabase DatabaseCtn = QSqlDatabase();

	// Check if the default 'performance_Schema'database is already open else open the database
	if (!DatabaseCtn.isOpen())
	{
		DatabaseCtn = QSqlDatabase::addDatabase("QMYSQL", "SENSOR_DB");
		DatabaseCtn.setHostName(QString::fromStdString(configReader->m_DatabaseHostName));
		DatabaseCtn.setDatabaseName(QString::fromStdString("performance_schema"));
		DatabaseCtn.setUserName(QString::fromStdString(configReader->m_DatabaseUserName));
		DatabaseCtn.setPassword(QString::fromStdString(configReader->m_DatabasePassword));
		DatabaseCtn.setPort(configReader->m_DatabasePort);
		if (!DatabaseCtn.open())
		{
			// Failure to open the mysql default 'performance_schema' database.
			cout << "Failed to open mysql's default database errorcode ::" 
					<< DatabaseCtn.lastError().text().toStdString() << endl;
			return 0;
		}
	}
	
	// Creates the Telemetry database 'sensor_db' acting as a database for all Telemetry specific modules
	const char* createQuery = "CREATE DATABASE IF NOT EXISTS sensor_db";
	if (executeQuery(DatabaseCtn, createQuery) == false)
	{
		// Failure to create the sensor_db database
		cout << "Failed to create 'sensor_db' database" << endl;		
		return 0;
	}

	// Obtain the instance of the data transmission controller
	DataTransmissionController *m_dataTranmissionController;	
	m_dataTranmissionController = DataTransmissionController::getInstance();

	if (DTCConnectionStatus == false)
	{
		// Since the Data transmission Controller's atleast one of the connection failed
		return 0;
	}

#if defined (__linux__)
	timer_t DTCTimerId;

	// Timer for the Data transmission controller collecting and reporting data
	start_periodic_timer(&DTCTimerId, 60000, &DataTransmissionCtlrHdlr);

#else
	// Timer for the Data transmission controller collecting and reporting data
	start_timer(60000, &DataTransmissionController::ReadPublishTable, 1);

#endif

	//configReader->printMap();

#if defined (__linux__)
	// Check if the WiFi is configured as enabled
	if (configReader->m_WiFiEnable == 1)
	{
		//Creating a thread for Wi-Fi module
		WiFiHandlerThread = std::thread(WIFIENTRYPOINT);
	}

	// Check if the BLE is configured as enabled
	if (configReader->m_BLEEnable == 1)
	{
		// Create a thread for the Bluetooth (BLE)
		BLEHandlerThread = std::thread(&BluetoothHandler::init, new BluetoothHandler());
	}	
#endif

	// Create a thread for the Telemetry client
	MessageHandler *m_messageHandlerinstance = MessageHandler::getInstance();	
	MessageHandlerThread = std::thread(&MessageHandler::Init, m_messageHandlerinstance);

	// Obtain the IDEX instance
	IDEX *IDEXinstance = IDEX::getInstance();

	// Spawn the IDEX thread only if the IDEX connection is successully established
	if (IDEXinstance->ConnectionStatus() == true)
	{
		// IDEX's database connection is successful
		IDEXthread = std::thread(&IDEX::IDEX_Init, IDEXinstance);
	}

	// TEMP : To be removed START
	// Create a thread for processing the incoming messages to Telemetry Client
	//ReceivedMessageThread = std::thread(&DataTransmissionController::ProcessReceivedMessage, m_dataTranmissionController);

	// Initialize the port for the access to Sunsetpass API (only if any of the modules requiring Sunset pass is enabled)
	if ((m_dataTranmissionController->m_configReader->m_PIREnable == 1) ||
		(m_dataTranmissionController->m_configReader->m_sensorEnable == 1))
	{
		// Initialize the port for the access to Sunsset pass API
		if (SP_SUCCESS == m_dataTranmissionController->initPort())
		{
			// Check if the PIR module is configured as enabled
			if (m_dataTranmissionController->m_configReader->m_PIREnable == 1)
			{
				// Initialize the PIR module so as to start its own processing
				PIR_STATUS_CODE initPIRStatus = init_PIR(m_dataTranmissionController->m_pirDevId_1, m_dataTranmissionController->m_pirDevId_2);
				if (initPIRStatus != PIR_SUCCESS)
				{
					cout << " FAILURE :: Failure to initialize the PIR module with error code - " << initPIRStatus << endl;
				}
			}

			// Check if the Sensor Telemetry Controller is configured as enabled
			if (m_dataTranmissionController->m_configReader->m_sensorEnable == 1)
			{
				//Creating a thread for SensorTelemetry Controller			
				SensorTelemetryControllerThread = std::thread(&DataTransmissionController::SensorTelThreadController,
					m_dataTranmissionController);

			}		
		}
		else
		{
			cout << " WARNING :: Failure to initialize the port for SunsetPass API communication!!" << endl;
		}
	}

	
	
	// Check if the Sensor Telemetry Controller is configured as enabled
#if defined(REALSENSE)
	if (configReader->m_RealSenseEnable == 1)
	{
		//Creating a thread for Realsense Controller
		RealsenseHandlerThread = std::thread(&RealSense::init_RS_PeopleCounter, new RealSense());
	
	}	
#endif
	// Join all the threads created 
	MessageHandlerThread.join();
	//ReceivedMessageThread.join(); // TEMP : To be removed

	// Join the IDEX thread only incase it is created 
	if (IDEXinstance->ConnectionStatus() == true)
	{
		IDEXthread.join();
	}

	if (m_dataTranmissionController->m_configReader->m_sensorEnable == 1)
	{
		SensorTelemetryControllerThread.join();
	}

#if defined(__linux__)
	if (configReader->m_WiFiEnable == 1)
	{
		WiFiHandlerThread.join();
	}	
	if (configReader->m_BLEEnable == 1)
	{
		BLEHandlerThread.join();
	}
#endif

	// Perform the cleanup before shutting down Telemetry application
	cleanup();
	return 0;
}
