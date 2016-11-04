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
*	@file		SensorTelemetryController.h
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

#ifndef SENS_TEL_CONT_H
#define SENS_TEL_CONT_H

#ifdef _WIN32
#define SENSORTELEMETRY_API __declspec(dllexport) 
#elif __linux__
#define SENSORTELEMETRY_API
#endif

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqldriver.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlfield.h>
#include <QtSql/qsqlindex.h>
#include <QtSql/qsqlquery.h>
#include <QtSql/qsqlrecord.h>
#include <QtSql/qsqlresult.h>
#include <QtCore/qstring.h>
#include "SPInterface.h"
#include "TimerLibrary.h"
#include "DBHandler.h"

using namespace std;

class ConfigurationReader;

enum STC_DB_STATUS_CODE
{
	STC_DB_OPEN_SUCCESS = 0,			///< Success
	STC_CONNECTION_FAILURE,			///< Failed to open the main connection
	STC_INSERT_CONNECTION_FAILURE,	///< Failed to open the Insert query connection
	STC_INSERT_QUERY_SUCCESS,		///< Insert query success
	STC_INSERT_QUERY_FAILURE		///< Failed to insert data into the database
};

/**
*  @brief SensorTelemetryController
*	Get the sensor information from SunsetPass API's and inserts it into the Sensor table.
*/
class SensorTelemetryController
{
public:
	ConfigurationReader *m_configReader;
	DBHandler *m_dbHandler;
	static QSqlDatabase m_stcDatabaseCtn;
	static QSqlDatabase m_stcInsertCtn;

	SENSORTELEMETRY_API SensorTelemetryController();     // constructor	
	SENSORTELEMETRY_API ~SensorTelemetryController();    // destructor

	/// Writes the sensor info and timestamp into the database
	SENSORTELEMETRY_API void SensorTelemetryDataHdlr();

	///Getting the current instance
	static SENSORTELEMETRY_API  SensorTelemetryController* getInstance();

	///Connecting to the database
	static STC_DB_STATUS_CODE getConnection(string Hostname, string DatabaseName, string UserName, string Password, int Port);

	/// Inserting data into the database
	//STC_DB_STATUS_CODE execInsertQuery(const string &strQuery, map<string, string> &arguments);

	/// Initializing the sensor	
	SENSORTELEMETRY_API SP_STATUS_CODE initSensorTelemetry(int & errCount);

	/// Cleanup for the Sensor Telemetry
	static SENSORTELEMETRY_API void stop_ST_Processing(void);

	/// Event handler for timer call back
	void TimerEvent();	

	/// De-initialize the sensor Telemetry
	static SENSORTELEMETRY_API void deinit_SensorTelemetry(void);

private:

	/*Variables*/
	static SensorTelemetryController * m_sensorTelemetryInstance;
};

#endif // SENS_TEL_CONT_H
