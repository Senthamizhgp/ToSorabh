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
*	@file		SensorTelemetryController.cpp
*
*	@brief
*		        This file contains declaration of all the data structures and
*				APIs which will be exported by the Sensor Telemetry Controller DLL.
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

/* System headers */
//#include <stdexcept>
#include <string>
#include <thread>

/* Own headers */
#include "SensorTelemetryController.h"
#include "ConfigurationReader.h"
#include "qdatetime.h"
#include <qdebug.h>

#if defined(_WIN32)
#include <Windows.h>
#endif

/* Global and Static initializations */
SensorTelemetryController* SensorTelemetryController::m_sensorTelemetryInstance = NULL;
QSqlDatabase SensorTelemetryController::m_stcDatabaseCtn= QSqlDatabase();
QSqlDatabase SensorTelemetryController::m_stcInsertCtn= QSqlDatabase();

/**
-------------------------------------------------------------------------------
SensorTelemetryController
-------------------------------------------------------------------------------
*   @brief		Constructor initializing the member variables and required inits.
*
*	@param		none
*
*	@return		None
*
*/
SensorTelemetryController::SensorTelemetryController() :
	m_configReader(NULL),
	m_dbHandler(NULL)
{
	m_configReader = ConfigurationReader::getInstance();
	m_dbHandler = DBHandler::getInstance();

	STC_DB_STATUS_CODE connectionStatus = getConnection(m_configReader->m_DatabaseHostName, m_configReader->m_DatabaseName, m_configReader->m_DatabaseUserName,
		m_configReader->m_DatabasePassword, m_configReader->m_DatabasePort);

	if (STC_DB_OPEN_SUCCESS != connectionStatus)
	{
		// Failed to open the required database connections
		cout << "[STC Constructor] Warning- Failure to open database connections with errorCode -"
			<< connectionStatus << endl;
	}
}

/**
-------------------------------------------------------------------------------
~SensorTelemetryController
-------------------------------------------------------------------------------
*   @brief		Destructor performing the cleanup like deleting allocated memory, dereferencing etc..
*
*	@param		none
*
*	@return		None
*
*/
SensorTelemetryController::~SensorTelemetryController()
{
	// Define all the code cleanup and memory delete 	
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
*
*/
SensorTelemetryController* SensorTelemetryController::getInstance()
{
	static SensorTelemetryController sensorTelemetryInstance;
	return &sensorTelemetryInstance;	
}

/**
-------------------------------------------------------------------------------
getConnection
-------------------------------------------------------------------------------
*   @brief						This function creates a connection to the database.
*
*	@param1 HostName			Hostname of the connection
*	@param1 DatabaseName		Name of the database
*   @param2 UserName			UserName(Credentials) for the database
*   @param3 Password			Password(Credentials) for the database
*	@param4 Port				Port for the database
*
*	@return	STC_DB_STATUS_CODE	Status code for the database connection
*
*/
STC_DB_STATUS_CODE SensorTelemetryController::getConnection(string HostName, string DatabaseName, string UserName, string Password, int Port)
{
	//connection configuration should done only one time for db
	if (!m_stcDatabaseCtn.isOpen()) {
		m_stcDatabaseCtn = QSqlDatabase::addDatabase("QMYSQL", "SensorTelemetryConnection");
		m_stcDatabaseCtn.setHostName(QString::fromStdString(HostName));
		m_stcDatabaseCtn.setDatabaseName(QString::fromStdString(DatabaseName));
		m_stcDatabaseCtn.setUserName(QString::fromStdString(UserName));
		m_stcDatabaseCtn.setPassword(QString::fromStdString(Password));
		m_stcDatabaseCtn.setPort(Port);
		if (!m_stcDatabaseCtn.open())
		{
			std::cout << "SensorTelemetry :: Failed to open database:" << endl;
			return STC_CONNECTION_FAILURE;
		}
		else
		{
			m_stcInsertCtn = QSqlDatabase::cloneDatabase(m_stcDatabaseCtn, "STC_Insert_Connection");

			if (!m_stcInsertCtn.open())
			{
				cout << "[STC::Insert Query]::Failed to open the database connection" << endl;
				return STC_INSERT_CONNECTION_FAILURE;
			}
		}
	}	
	return STC_DB_OPEN_SUCCESS;
}


/**
-------------------------------------------------------------------------------
TimerEvent
-------------------------------------------------------------------------------
*   @brief		This function acts as the timer event handler.
*				This inturn calls the sunsetPass API at regular intervals
*				for different sensors based on their frequency defined in
*				TelemetryConfiguration.xml file and inserts the sensor data
*				into the database
*
*	@param		None
*
*	@return		None
*
*/
void SensorTelemetryController::TimerEvent()
{
	SensorTelemetryController *m_sensorTelemetryController = SensorTelemetryController::getInstance();
	SPDataBuff pStructDataBuff;
	SPRTC pstructRTC;

	map<string, string> args_dataInsertion;
	const char* query = "INSERT INTO sensortelemetry_table(timestamp, deviceId, sensorName, sensorType, value, measurementUnit) VALUES (:timestamp, :deviceId, :sensorName, :sensorType, :value, :measurementUnit)";

	for (int iter = 0; iter < m_configReader->m_sensorCount && m_configReader->m_sensorInfo[iter].eventSensorFlag == 0; iter++)
	{
		// Increasing the frequency count every 5s
		m_configReader->m_sensorInfo[iter].sensorFrequencyCount += 5;
	}

	for (int iter = 0; iter < m_configReader->m_sensorCount && m_configReader->m_sensorInfo[iter].eventSensorFlag == 0; iter++)
	{
		// Calling the spSensorGetValueA once the time elapsed for that sensor
		if (m_configReader->m_sensorInfo[iter].sensorAcquisitionFrequency == m_configReader->m_sensorInfo[iter].sensorFrequencyCount)
		{
			// Resetting the Timer
			m_configReader->m_sensorInfo[iter].sensorFrequencyCount = 0;

			SP_STATUS_CODE returnStatus = spSensorGetValueA(m_configReader->m_sensorInfo[iter].sensorId, true, pstructRTC, pStructDataBuff);

			if (returnStatus != SP_SUCCESS)
			{
				cout << " FAILURE :: Failure to obtain value for " << m_configReader->m_sensorInfo[iter].sensorName
					<< " Sensor with error code " << returnStatus << endl;
			}
			else
			{
				QDateTime currentDateTime = QDateTime::currentDateTime();
				uint timestamp = currentDateTime.toTime_t();

				// The coefficient should be read using the Sunset Pass API from the SPM_device_configuration.xml
				// It is required to calculate the actual sensor value				
				// double cofficient = 0.00390625; // As of now commented due to unavailabilty currently
				short combined = (pStructDataBuff.pcData[1] << 8 ) | (pStructDataBuff.pcData[0] & 0xff);
				float sensorValue = (float) combined /16;
				
				args_dataInsertion[":timestamp"] = to_string(timestamp);
				args_dataInsertion[":deviceId"] = to_string(m_configReader->m_sensorInfo[iter].deviceId);
				args_dataInsertion[":sensorName"] = m_configReader->m_sensorInfo[iter].sensorName;
				args_dataInsertion[":sensorType"] = m_configReader->m_sensorInfo[iter].sensorType;
				args_dataInsertion[":value"] = to_string(sensorValue);
				args_dataInsertion[":measurementUnit"] = m_configReader->m_sensorInfo[iter].sensorUnit;

				if (m_sensorTelemetryController->m_dbHandler != NULL)
				{
					if (UPDATE_QUERY_FAILURE == m_sensorTelemetryController->m_dbHandler->execUpdateQuery
						(m_sensorTelemetryController->m_stcInsertCtn, query, args_dataInsertion))
					{
						cout << "[STC::TimerEvent] ERROR::Failure in Insertion Query" << endl;
					}
				}
			}
		}
	}
}

/**
-------------------------------------------------------------------------------
SensorTelemetryDataHdlr
-------------------------------------------------------------------------------
*   @brief		This function performs the following operations
*				1) Reads the configuration parameters and creates a
*				   connection to the database.
*				2) Get the sensor data from SunsetPass API's at reg intervals
*				3) Updates the sensor data into the database
*
*	@param		None
*
*	@return		None
*
*/
void SensorTelemetryController::SensorTelemetryDataHdlr()
{
	const char * createTableQuery = "create table sensortelemetry_table(timestamp BIGINT, deviceId varchar(20), sensorName varchar(60), sensorType varchar(60), value float(5,2), measurementUnit varchar(10))";
	const char * dropTableQuery = "drop table if exists sensortelemetry_table";
	static bool initialize = false;
		
	map<string, string> args_dataInsertion;

	// Code for table creation
	if (!initialize)
	{
		if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_stcInsertCtn, dropTableQuery, args_dataInsertion))
		{
			cout << "ERROR :: [IDEX::CheckforNewTransactionEntries]:: Failed to execute select query" << endl;
			return;
		}

		if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_stcInsertCtn, createTableQuery, args_dataInsertion))
		{
			cout << "ERROR :: [IDEX::CheckforNewTransactionEntries]:: Failed to execute select query" << endl;
			return;
		}

		initialize = true;
	}

	// Kick start the timer
	start_ST_timer(5000, &SensorTelemetryController::TimerEvent);
}

/**
-------------------------------------------------------------------------------
SensorEventHandler
-------------------------------------------------------------------------------
*   @brief		This function acts as the event handler, which will be called
*				by the SunsetPass API's during any event occured in the
*				sensor devices
*
*	@param1		devID			Device ID of the sensor
*	@param2		eventType		Nature of the event
*	@param3		eventData1,2,3	Data collected during the event
*	@param4		reserved		Reserved for some other use
*
*	@return		None
*
*/
void SensorEventHandler(int32_t devID, int32_t eventType, int32_t eventData1,
	int32_t eventData2, void *eventData3, int32_t reserved)
{
	(void)eventType;  // To pacify the compiler warning
	(void)eventData1; // To pacify the compiler warning
	(void)eventData3; // To pacify the compiler warning
	(void)reserved;  // To pacify the compiler warning
	
	SensorTelemetryController *m_sensorTelemetryController = SensorTelemetryController::getInstance();
	map<string, string> args_dataInsertion;
	string sensorName = "";
	string sensorType = "";
	string sensorUnit = "";
	int deviceId = 0;

	const char* query = "INSERT INTO sensortelemetry_table(timestamp, deviceId, sensorName, sensorType, value, measurementUnit) VALUES (:timestamp, :deviceId, :sensorName, :sensorType, :value, :measurementUnit)";

	QDateTime currentDateTime = QDateTime::currentDateTime();
	uint timestamp = currentDateTime.toTime_t();

	for (int iter = 0; iter < m_sensorTelemetryController->m_configReader->m_sensorCount; iter++)
	{
		if (devID == m_sensorTelemetryController->m_configReader->m_sensorInfo[iter].sensorId &&
			m_sensorTelemetryController->m_configReader->m_sensorInfo[iter].eventSensorFlag == 1)
		{
			sensorName = m_sensorTelemetryController->m_configReader->m_sensorInfo[iter].sensorName;
			sensorType = m_sensorTelemetryController->m_configReader->m_sensorInfo[iter].sensorType;
			sensorUnit = m_sensorTelemetryController->m_configReader->m_sensorInfo[iter].sensorUnit;
			deviceId = m_sensorTelemetryController->m_configReader->m_sensorInfo[iter].deviceId;
			break;
		}
	}

	args_dataInsertion[":timestamp"] = to_string(timestamp);
	args_dataInsertion[":deviceId"] = to_string(deviceId);
	args_dataInsertion[":sensorName"] = sensorName;
	args_dataInsertion[":sensorType"] = sensorType;
	args_dataInsertion[":value"] = to_string(eventData2);
	args_dataInsertion[":measurementUnit"] = sensorUnit;

	if (m_sensorTelemetryController->m_dbHandler != NULL)
	{
		if (UPDATE_QUERY_FAILURE == m_sensorTelemetryController->m_dbHandler->execUpdateQuery
			(m_sensorTelemetryController->m_stcInsertCtn, query, args_dataInsertion))
		{
			cout << "[STC::SensorEventHandler] ERROR::Failure in Insertion Query" << endl;
		}
	}
}

/**
-------------------------------------------------------------------------------
initSensorTelemetry
-------------------------------------------------------------------------------
*   @brief		This function performs the following operations
*				1) Get the device ID of the sensors.
*				2) Open and enable the sensor devices.
*				3) Register call back for the devices.
*
*	@param		errCount - Output value holding count for devices whose callback registration failed
*
*	@return		0 if success and SP_STATUS_CODE if failure
*
*/
SP_STATUS_CODE SensorTelemetryController::initSensorTelemetry(int &errCount)
{
	int errorCount = 0;
	SP_STATUS_CODE responseCode = SP_SUCCESS;	


	//opening the devices one by one
	for (int it = 0; it < m_configReader->m_sensorCount; it++)
	{
		responseCode = spDevOpen(m_configReader->m_sensorInfo[it].sensorId);
		if (SP_SUCCESS != responseCode)
		{
			std::cout << m_configReader->m_sensorInfo[it].sensorName << " spDevOpen failure with response code : " << responseCode << endl;
			continue;
		}
		else
		{
			cout << m_configReader->m_sensorInfo[it].sensorName << " spDevOpen success" << endl;
		}

		//set device enabled true
		responseCode = spDevSetPropertyB(m_configReader->m_sensorInfo[it].sensorId, SP_DEV_PROP_DEVICE_ENABLED, true);
		if (SP_SUCCESS != responseCode)
		{
			std::cout << m_configReader->m_sensorInfo[it].sensorName << " spDevSetPropertyB--SP_DEV_PROP_DEVICE_ENABLED failure with response code : " << responseCode << endl;
			responseCode = spDevClose(m_configReader->m_sensorInfo[it].sensorId);
			if (SP_SUCCESS != responseCode)
			{
				std::cout << m_configReader->m_sensorInfo[it].sensorName << " spDevClose failure with response code : " << responseCode << endl;
				continue;
			}
			continue;
		}
		else
		{
			cout << m_configReader->m_sensorInfo[it].sensorName << " spDevSetPropertyB--SP_DEV_PROP_DEVICE_ENABLED success" << endl;
		}
	}

	for (int it = 0; it < m_configReader->m_sensorCount && m_configReader->m_sensorInfo[it].eventSensorFlag == 1; it++)
	{
		//register callback
		responseCode = spDevRegCallback(m_configReader->m_sensorInfo[it].sensorId, SensorEventHandler);
		if (SP_SUCCESS != responseCode) 
		{
			errorCount++;
			std::cout << m_configReader->m_sensorInfo[it].sensorName << " spDevRegCallback failure with response code : " << responseCode << endl;
			responseCode = spDevClose(m_configReader->m_sensorInfo[it].sensorId);
			if (SP_SUCCESS != responseCode) 
			{
				std::cout << m_configReader->m_sensorInfo[it].sensorName << " spDevClose failure with response code : " << responseCode << endl;
				continue;
			}
			continue;
		}
		else 
		{
			cout << m_configReader->m_sensorInfo[it].sensorName << " spDevRegCallback success" << endl;
		}
	}

	errCount = errorCount;
	return responseCode;
}

/**
-------------------------------------------------------------------------------
stop_ST_Processing
-------------------------------------------------------------------------------
*   @brief		Stop the Sensor Telemetry processing by stopping timer which 
*				polls the sensors for value on periodic interval
*
*	@param		None
*
*	@return		None
*
*/
/// Cleanup for the Sensor Telemetry
void SensorTelemetryController::stop_ST_Processing(void)
{
	// Stop the timer which gets the sensor value on regular interval 
	stop_ST_timer();

	// De-intialize the sensor Telemetry
	deinit_SensorTelemetry();
}

/**
-------------------------------------------------------------------------------
deinit_SensorTelemetry
-------------------------------------------------------------------------------
* This function close devices opened in init routine
*
*	@param		None.
*
*	@return		None
*
*/
void SensorTelemetryController::deinit_SensorTelemetry(void)
{
	// Obtain the configReader singleton instance
	ConfigurationReader *m_configReader = ConfigurationReader::getInstance();

	// Iterate all the sensor Telemetry sensors and close each of the device
	for (int it = 0; it < m_configReader->m_sensorCount; it++)
	{
		SP_STATUS_CODE responseCode = spDevClose(m_configReader->m_sensorInfo[it].sensorId);
		if (SP_SUCCESS != responseCode)
		{
			std::cout << m_configReader->m_sensorInfo[it].sensorName << " spDevClose failure with response code : " << responseCode << endl;
			continue;
		}
	}
}