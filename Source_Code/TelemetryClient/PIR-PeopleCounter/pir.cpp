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
 *	@file		pir.cpp
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

/* System headers */
#include <stdio.h>
#include <time.h>

/* Qt headers for QtSql Database */
#include "QtSql/qsql.h"
#include "QtSql/qsqldatabase.h"
#include "QtSql/qsqlquery.h"
#include "QtSql/qsqlerror.h"

/* Own headers */
#ifndef UNIT_TEST
#include "ConfigurationReader.h"
#endif // !UNIT_TEST

#include "DBHandler.h"
#include "pir.h"

#define MAX_BUF_LEN				255
#define PIR_DB_CONNECTION_NAME	"PIR_CONNECT"	///< Connection name used for DB
#define QT_MYSQL				"QMYSQL"		///< MYSQL connector name used with DB

#ifdef UNIT_TEST
/* Unit test macros */
#define static
#define	ConfigurationReader	mockConfigurationReader
#define spDevOpen			mockspDevOpen
#define spDevSetPropertyB	mockspDevSetPropertyB
#define spDevRegCallback	mockspDevRegCallback
#define spDevClose			mockspDevClose
#endif // UNIT_TEST

static int32_t sensor_right_devId = 0;	///< Device ID from Telemetry for right PIR sensor
static int32_t sensor_left_devId = 0;	///< Device ID from Telemetry for left PIR sensor

static int32_t sensor_right_value = 0;	///< Value read from right PIR sensor using Event Notification Interface
static int32_t sensor_left_value = 0;	///< Value read from left PIR sensor using Event Notification Interface

static int32_t movingLeft = 0;			///< People count moving left to right
static int32_t movingRight = 0;			///< People count moving right to left

static PIR_STATUS_CODE execute_query(char *query);

/* event callback for right PIR sensor and left PIR sensor */
static void right_sensor_notification(int32_t devID, int32_t eventType, 
										int32_t eventData1, int32_t eventData2, 
										void *eventData3, int32_t reserved);

static void left_sensor_notification(int32_t devID, int32_t eventType, 
										int32_t eventData1, int32_t eventData2, 
										void *eventData3, int32_t reserved);
	
/* Create an instance with a connection name for DB */
QSqlDatabase db = QSqlDatabase::addDatabase(QString::fromStdString(QT_MYSQL),
											QString::fromStdString(PIR_DB_CONNECTION_NAME));

DBHandler *db_handler;

/**
-------------------------------------------------------------------------------
print_msg
-------------------------------------------------------------------------------
* This function prints user message to console.
*
*	@param msg		User message to print
*
*	@return			None
*
*/
static void print_msg(const char *msg, ...)
{
#ifndef UNIT_TEST
	va_list va_args;

	va_start(va_args, msg);
	vprintf(msg, va_args);
	va_end(va_args);
	fflush(stdout);
#endif
}

/**
-------------------------------------------------------------------------------
execute_query
-------------------------------------------------------------------------------
 * This function execute the query from calling funcion.
 *
 *	@param query	Query to execute on DB
 *
 *	@return			Refer this Enum @ref PIR_STATUS_CODE
 *
 */
static PIR_STATUS_CODE execute_query(char *query)
{
	if (query != NULL) 
	{
		map<string, string> dummy;
		if(db_handler->execUpdateQuery(db, query, dummy) == UPDATE_QUERY_FAILURE)
			return PIR_E_DB_FAILURE;
	}
	else
	{
		return PIR_E_FAILURE;		/* NULL argument check */
	}
	return PIR_SUCCESS;
}

/**
-------------------------------------------------------------------------------
right_sensor_notification
-------------------------------------------------------------------------------
* Event function for right PIR sensor. When this event function triggerd by HLAPI and if 
* the left PIR sensor is triggered before, it shows that a person moved from Left to Right.
*
*	@param	devID			Device ID which triggers this event
*	@param	eventType		Event status from device
*	@param	eventData1		GPI value change with timestamp for generic GPI based sensor
*	@param	eventData2		Changed value of GPI pin from Sunset Pass module
*	@param	eventData3		RTC
*	@param	reserved		NA
*
*	@return					None
*
*/
static void right_sensor_notification(int32_t devID, int32_t eventType, 
										int32_t eventData1, int32_t eventData2, 
										void *eventData3, int32_t reserved)
{
	char buf[MAX_BUF_LEN];

	sensor_right_value = eventData2;

	if (sensor_right_value && sensor_left_value) 
	{
		movingLeft++;
		print_msg("[PIR] NOTIFY: L----->R triggered [devId - %d]\t%d\t%d\n", 
					devID, movingLeft, movingRight);
#ifdef _WIN32
		_snprintf_s(buf, MAX_BUF_LEN, MAX_BUF_LEN, 
					"INSERT into pir_table (timestamp, movingLeft, movingRight) VALUES(%lld, %d, %d)",
					(long long)time(NULL), movingLeft, movingRight);
#elif __linux__
		snprintf(buf, MAX_BUF_LEN, 
				"INSERT into pir_table (timestamp, movingLeft, movingRight) VALUES(%lld, %d, %d)",
				(long long)time(NULL), movingLeft, movingRight);
#endif
		execute_query(buf);
		// TODO - handle failure on execute_query
	}
}

/**
-------------------------------------------------------------------------------
left_sensor_notification
-------------------------------------------------------------------------------
 * Event function for left PIR sensor. When this event function triggerd by HLAPI and if 
 * the right PIR sensor is triggered before, it shows that a person moved from Right to Left.
 *
 *	@param	devID			Device ID which triggers this event	
 *	@param	eventType		Event status from device
 *	@param	eventData1		GPI value change with timestamp for generic GPI based sensor
 *	@param	eventData2		Changed value of GPI pin from Sunset Pass module
 *	@param	eventData3		RTC
 *	@param	reserved		NA
 *
 *	@return					None
 *
 */
static void left_sensor_notification(int32_t devID, int32_t eventType, int32_t eventData1, 
									int32_t eventData2, void *eventData3, int32_t reserved)
{
	char buf[MAX_BUF_LEN];

	sensor_left_value = eventData2;

	if (sensor_right_value && sensor_left_value) 
	{
		movingRight++;
		print_msg("[PIR] NOTIFY: R----->L triggered [devId - %d]\t%d\t%d\n", 
					devID, movingLeft, movingRight);
#ifdef _WIN32
		_snprintf_s(buf, MAX_BUF_LEN, MAX_BUF_LEN, 
					"INSERT into pir_table (timestamp, movingLeft, movingRight) VALUES(%lld, %d, %d)",
					(long long)time(NULL), movingLeft, movingRight);
#elif __linux__
		snprintf(buf, MAX_BUF_LEN, 
				"INSERT into pir_table (timestamp, movingLeft, movingRight) VALUES(%lld, %d, %d)",
				(long long)time(NULL), movingLeft, movingRight);
#endif
		execute_query(buf);
		// TODO - handle failure on execute_query
	}
}

/**
-------------------------------------------------------------------------------
init_PIR
-------------------------------------------------------------------------------
 * This function initialize PIR module by opening the device, enable the device, 
 * and register call back function for the device.
 *
 *	@pre	The calling function must open the communication channel before 
 *			calling this DLL function.
 *
 *	@param	devId1	Device ID for right PIR sensor read from Telemetry Configuration XML
 *	@param	devId2	Device ID for left PIR sensor read from Telemetry Configuration XML
 *
 *	@return			Refer this Enum @ref PIR_STATUS_CODE
 *
 */
PIR_STATUS_CODE init_PIR(int32_t devId1, int32_t devId2)
{
	char buf[MAX_BUF_LEN];

	/* Initialize configuration reader to read PIR config parameters and db settings */
	ConfigurationReader *configReader = ConfigurationReader::getInstance();

	/* Connect to db and create table, if doesn't exists */
	db.setDatabaseName(QString::fromStdString(configReader->m_DatabaseName));
	db.setHostName(QString::fromStdString(configReader->m_DatabaseHostName));
	db.setUserName(QString::fromStdString(configReader->m_DatabaseUserName));
	db.setPassword(QString::fromStdString(configReader->m_DatabasePassword));
	db.setPort(configReader->m_DatabasePort);

	if (!db.open()) 
	{
		print_msg("[PIR] ERROR: Failed to open \"%s\" database\n", 
					configReader->m_DatabaseName.c_str());
	
		return PIR_E_DB_FAILURE;	/* Err - failed to open db */
	} 
	else 
	{
#ifdef _WIN32
		_snprintf_s(buf, MAX_BUF_LEN, MAX_BUF_LEN, 
					"CREATE TABLE IF NOT EXISTS pir_table(timestamp BIGINT, movingLeft INT, movingRight INT)");
#elif __linux__
		snprintf(buf, MAX_BUF_LEN, 
					"CREATE TABLE IF NOT EXISTS pir_table(timestamp BIGINT, movingLeft INT, movingRight INT)");
#endif
		db_handler = DBHandler::getInstance();

		if (PIR_E_DB_FAILURE == execute_query(buf))
		{
			print_msg("[PIR] ERROR: Failed to create table\n");

			return PIR_E_DB_FAILURE;
		}
	}

	/* Open PIR devices and register callback */
	sensor_right_devId = devId1;
	sensor_left_devId = devId2;
	print_msg("[PIR] INFO: Right Senor ID - %d Left Sensor ID - %d\n", 
			sensor_right_devId, sensor_left_devId);
	if ((SP_SUCCESS != spDevOpen(sensor_right_devId)) ||
		(SP_SUCCESS != spDevOpen(sensor_left_devId)))
	{
		print_msg("[PIR] ERROR: Unable to open device\n");
		db.close();

		return PIR_E_SP_FAILURE;	/* Err - failed to open device */
	}

	if ((SP_SUCCESS != spDevSetPropertyB(sensor_right_devId, SP_DEV_PROP_DEVICE_ENABLED, true)) ||
		(SP_SUCCESS != spDevSetPropertyB(sensor_left_devId, SP_DEV_PROP_DEVICE_ENABLED, true)))
	{
		print_msg("[PIR] ERROR: Unable to set property for device\n");
		deinit_PIR();

		return PIR_E_SP_FAILURE;	/* Err - failed to enable device*/
	}

	if ((SP_SUCCESS != spDevRegCallback(sensor_right_devId, right_sensor_notification)) ||
		(SP_SUCCESS != spDevRegCallback(sensor_left_devId, left_sensor_notification)))
	{
		print_msg("[PIR] ERROR: Unable to register callback for device\n");
		deinit_PIR();

		return PIR_E_SP_FAILURE;	/* Err - failed to register callback for device */
	}

	print_msg("[PIR] INFO: Successfully initialized PIR. Returning \"PIR_SUCCESS\" to calling function\n"); 

	return PIR_SUCCESS;
}

/**
-------------------------------------------------------------------------------
deinit_PIR
-------------------------------------------------------------------------------
* This function close devices opened in init routine and close db.
*
*	@pre		init_PIR must be called before calling this function.
*
*	@return		Refer this Enum @ref PIR_STATUS_CODE
*
*/
PIR_STATUS_CODE deinit_PIR(void)
{
	if (db.isOpen())
		db.close();

	if ((SP_SUCCESS != spDevClose(sensor_right_devId)) || 
		(SP_SUCCESS != spDevClose(sensor_left_devId)))
	{
		print_msg("[PIR] ERROR: Failed to close device\n");

		return PIR_E_SP_FAILURE;	/* Err - failed to close device */
	}

	return PIR_SUCCESS;
}

/* eof */
