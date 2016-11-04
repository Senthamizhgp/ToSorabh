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
*	@file		DataTransmissionController.h
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

#ifndef DATATRANSMISSIONCONTROLLER_H
#define DATATRANSMISSIONCONTROLLER_H

/* System headers */
#include <map>
#include <mutex>
#include <thread>

/* Qt headers */
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlfield.h>
#include <QtSql/qsqlquery.h>
#include <QtCore/qstring.h>

/* Sunset Pass header */
#include "SPInterface.h"

/* Timer library header */
#include "TimerLibrary_DTC.h"

using namespace std;

class ConfigurationReader;
class SensorTelemetryController;
class PublishTableController;

/// Return code for the DataTransmission Controller
enum DTC_DB_STATUS_CODE
{
	DTC_DB_OPEN_SUCCESS = 0,		///< Success
	DTC_CONNECTION_FAILURE,			///< Failed to open the main connection
	DTC_SELECT_CONNECTION_FAILURE,	///< Failed to open the Select query connection
	DTC_UPDATE_CONNECTION_FAILURE,	///< Failed to open the Update query connection
	DTC_SELECT_QUERY_FAILURE,		///< Failed to select data from the database
	DTC_SELECT_QUERY_SUCCESS,		///< Select Query success
	DTC_UPDATE_QUERY_FAILURE,		///< Failed to update the database
	DTC_UPDATE_QUERY_SUCCESS		///< Success in updating the database
};


/**
-------------------------------------------------------------------------------
DataTransmissionController
-------------------------------------------------------------------------------
*	Process the data available in the publish table as per the reporting frequency (configured),
*	add the necessary headers and finally push into the Telemetry client queue to be sent to MQTT subscriber
*
*/
class DataTransmissionController                  
{	
public:    

	/// Destructor
	~DataTransmissionController();

	/// Obtain the Datatransmission controller instance
	static DataTransmissionController* getInstance();

	///Handles init for Sensor Telemetry Controller Thread
	void SensorTelThreadController();

	///Reads publish table
	void ReadPublishTable();

	///Handles the response received from the MQTT
	void ProcessReceivedMessage();

	///Purge publish table at regular intervals
	void DatabasePurgingHandler();

	/// Selecting data from the database
	DTC_DB_STATUS_CODE execSelectQuery(const string &strQuery, map<string, string> &arguments, QList<QMap<QString, QVariant>> &records);

	///Connecting to the database
	static DTC_DB_STATUS_CODE getConnection(string HostName, string DatabaseName, string UserName, string Password, int Port);

	/// Updates data into the database
	DTC_DB_STATUS_CODE  execUpdateQuery(QSqlDatabase connectionName, const string &strQuery, map<string, string> &arguments);

	///String Tokenizer
	vector<string> StringSplitter(string str, char delimiter);

	/// To check if the string contain only number
	bool is_number(const std::string& stringToCheck);

	/// Initialize the port for the Sunset Pass API
	SP_STATUS_CODE initPort();	

	/// Event handler when timer elapses to process the individual module data
	void PublishTableUpdationEvent();

	int32_t m_pirDevId_1;   ///> Device Id of the left PIR sensor
	int32_t m_pirDevId_2;   ///> Device Id of the right PIR sensor

	QMap<QString, QVariant> selection_map;	///> For processing database result values
	QList<QMap<QString, QVariant>> listmap;	///> For processing database result values
	ConfigurationReader *m_configReader;    ///> Instance of the Configuration reader
	
private:  
	
	/*Functions*/
	/// Constructor
	DataTransmissionController();

	/// Pushes the message to the Telemetry client queue
	void PushToMessageQueue(std::string &jsonMessage);
	
	/*Variables*/
	static DataTransmissionController *instance;	///> Obtain the Data Transmission Controller instance
	static QSqlDatabase m_dtcDatabaseCtn;			///> Database connection for the Data transmission controller
	static QSqlDatabase m_dtcSelectCtn;				///> Select connection for the Data transmission controller
	static QSqlDatabase m_dtcUpdateCtn;				///> Update connection for the Data transmission controller
	static QSqlDatabase m_dtcMQTTUpdateCtn;			///> MQTT connection for the Data transmission controller
};

#endif // DATATRANSMISSIONCONTROLLER_H