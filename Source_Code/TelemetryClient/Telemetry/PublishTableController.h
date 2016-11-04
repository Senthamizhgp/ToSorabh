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
*	@file		PublishTableController.h
*
*	@brief
*		        This file contains declaration of all the data structures and
*				methods used for the Publish Table manipulations
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

#ifndef PUBLISH_TABLE_CONT_H
#define PUBLISH_TABLE_CONT_H

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
#include "qjsonobject.h"

using namespace std;

class ConfigurationReader;
class DBHandler;

enum PTC_DB_STATUS_CODE
{
	PTC_DB_OPEN_SUCCESS = 0,		///< Success in opening all the PTC connections	
	PTC_CONNECTION_FAILURE,			///< Failed to open the PTC main connection	
	PTC_SELECT_CONNECTION_FAILURE,	///< Failed to open the PTC Select query connection
	PTC_UPDATE_CONNECTION_FAILURE,	///< Failed to open the PTC Update query connection
	PTC_INSERT_CONNECTION_FAILURE,	///< Failed to open the PTC Insert query connection
	PTC_DELETE_CONNECTION_FAILURE	///< Failed to open the PTC Delete query connection
};

/*
Publish Table Controller class :
*/
class PublishTableController
{

public:

	/*Variables*/
	ConfigurationReader *m_configReader;
	DBHandler *m_dbHandler;
	static QSqlDatabase m_ptcDatabaseCtn;
	static QSqlDatabase m_ptcSelectCtn;
	static QSqlDatabase m_ptcInsertCtn;
	static QSqlDatabase m_ptcUpdateCtn;
	static QSqlDatabase m_ptcDeleteCtn;
	string m_string_pir_configure;
	string m_string_dex_configure;
	string m_string_wifi_configure;
	string m_string_ble_configure;
	string m_string_Trans_configure;
	string m_string_stock_configure;
	string m_string_sens_t_configure;
	string m_string_Realsense_configure;
	string m_deviceidentifier;
	QJsonObject m_outer_pir_configure;
	QJsonObject m_outer_dex_configure;
	QJsonObject m_outer_wifi_configure;
	QJsonObject m_outer_ble_configure;
	QJsonObject	m_outer_stock_configure;
	QJsonObject m_outer_sens_t_configure;
	QJsonObject m_outer_Realsense_configure;
	QJsonObject m_outer_Trans_configure;

	/*Functions*/
	PublishTableController();     // constructor	
	~PublishTableController();    // destructor

	//Getting the current instance
	static PublishTableController* getInstance();

	//Updates the publish table with the data from sensor tables
	void UpdatePublishTable();

	//Getting the connection to the database
	static PTC_DB_STATUS_CODE getConnection(string HostName, string DatabaseName, string UserName, string Password, int Port);

	//Publish Controller handling PIR table Manipulations
	void PublishControllerUpdatingPIR();

	//Publish Controller handling WiFi table Manipulations
	void PublishControllerUpdatingWiFi();

	//Publish Controller handling BLE table Manipulations
	void PublishControllerUpdatingBLE();

	//Publish Controller handling Sensor telemetry Table Manipulations
	void PublishCtlrUpdatingSensorTelemetry();

	//Publish Controller handling Real Sense table Manipulations
	void PublishCtlrUpdatingRealsense();

	//Publish Controller handling Transaction table Manipulations
	void PublishControllerUpdatingTransaction(string JsonData);

	//Publish Controller handling Stock table Manipulations
	void PublishControllerUpdatingstock(string JsonData);

	//Publish Controller handling Vending database Manipulations
	pair<string, qlonglong> PublishControllerUpdatingVendingDB();

	//Publish Controller handling DEX Manipulations
	void PublishControllerUpdatingdex(string JsonData);

	// Delete records from the Vending database
	void VendingDBdeletion(qlonglong timestamp);

	// Inserting data from different modules into the database
	void PublishTableInsertion(string passjson, string sensor);
	
	void configurationReaderJson();
	
	void PublishTableInsertion(string passjson, string sensor, pair<string, string> send_receive);
	
	
private:
	/*Variables*/
	static PublishTableController* instance;		
};

#endif // PUBLISH_TABLE_CONT_H
