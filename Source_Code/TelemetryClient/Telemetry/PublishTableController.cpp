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
*	@file		PublishTableController.cpp
*
*	@brief	
*				This file contains declaration of all the data structures and    
*				methods used for the Publish Table manipulations
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

/*System headers*/
#include <thread>

/*Own headers*/
#include "PublishTableController.h"
#include "ConfigurationReader.h"
#include "qdatetime.h"
#include "qjsonobject.h"
#include "Transaction.h"
#include "DexConverter.h"
#include "DBHandler.h"

using namespace std;

// Global and Static initializations
PublishTableController* PublishTableController::instance = NULL;
QSqlDatabase PublishTableController::m_ptcDatabaseCtn = QSqlDatabase();
QSqlDatabase PublishTableController::m_ptcSelectCtn = QSqlDatabase();
QSqlDatabase PublishTableController::m_ptcUpdateCtn = QSqlDatabase();
QSqlDatabase PublishTableController::m_ptcInsertCtn = QSqlDatabase();
QSqlDatabase PublishTableController::m_ptcDeleteCtn = QSqlDatabase();
extern bool VDBDeleteFlag;


/**
-------------------------------------------------------------------------------
PublishTableController
-------------------------------------------------------------------------------
*   @brief		Constructor initializing the member variables and required inits.
*
*	@param		none
*
*	@return		None
*
*/
PublishTableController::PublishTableController() :
	m_configReader(NULL),
	m_dbHandler(NULL)
{
	//Define all the initializations
	m_configReader = ConfigurationReader::getInstance();
	m_dbHandler = DBHandler::getInstance();

	PTC_DB_STATUS_CODE connectionStatus = getConnection(m_configReader->m_DatabaseHostName, m_configReader->m_DatabaseName, m_configReader->m_DatabaseUserName,
		m_configReader->m_DatabasePassword, m_configReader->m_DatabasePort);
	/*Reading configuration for json string appending*/

	if (PTC_DB_OPEN_SUCCESS != connectionStatus)
	{
		// Failed to open the required database connections
		cout << "[PTC Constructor] Warning- Failure to open database connections with errorCode -"
				<< connectionStatus << endl;
	}
	
	// Obtain the configured JSON header for all the modules from configuration XML
	configurationReaderJson();
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
*	@param4	Port				Port to be used for the database
*
*	@return	DB_STATUS_CODE		Status code for the database connection
*
*/
PTC_DB_STATUS_CODE PublishTableController::getConnection(string HostName, string DatabaseName, string UserName, string Password, int Port)
{
	//connection configuration should done only one time for db
	if (!m_ptcDatabaseCtn.isOpen()) {
		m_ptcDatabaseCtn = QSqlDatabase::addDatabase("QMYSQL", "PTC_CONNECTION");
		m_ptcDatabaseCtn.setHostName(QString::fromStdString(HostName));
		m_ptcDatabaseCtn.setDatabaseName(QString::fromStdString(DatabaseName));
		m_ptcDatabaseCtn.setUserName(QString::fromStdString(UserName));
		m_ptcDatabaseCtn.setPassword(QString::fromStdString(Password));
		m_ptcDatabaseCtn.setPort(Port);
		if (!m_ptcDatabaseCtn.open())
		{
			cout << "PublishTableController ::  Failed to open database:" << endl;
			return PTC_CONNECTION_FAILURE;
		}
		else
		{
			m_ptcSelectCtn = QSqlDatabase::cloneDatabase(m_ptcDatabaseCtn, "PTC_Select_Connection");
			m_ptcUpdateCtn = QSqlDatabase::cloneDatabase(m_ptcDatabaseCtn, "PTC_Update_Connection");
			m_ptcInsertCtn = QSqlDatabase::cloneDatabase(m_ptcDatabaseCtn, "PTC_Insert_Connection");
			m_ptcDeleteCtn = QSqlDatabase::cloneDatabase(m_ptcDatabaseCtn, "PTC_Delete_Connection");

			if (!m_ptcSelectCtn.open())
			{
				cout << "[PTC::Select Query]::Failed to open the database connection" << endl;
				return PTC_SELECT_CONNECTION_FAILURE;
			}
			if (!m_ptcUpdateCtn.open())
			{
				cout << "[PTC::Update Query]::Failed to open the database connection" << endl;
				return PTC_UPDATE_CONNECTION_FAILURE;
			}
			if (!m_ptcInsertCtn.open())
			{
				cout << "[PTC::Insert Query]::Failed to open the database connection" << endl;
				return PTC_INSERT_CONNECTION_FAILURE;
			}
			if (!m_ptcDeleteCtn.open())
			{
				cout << "[PTC::Delete Query]::Failed to open the database connection" << endl;
				return PTC_DELETE_CONNECTION_FAILURE;
			}
		}
	}
	return PTC_DB_OPEN_SUCCESS;
}

/**
-------------------------------------------------------------------------------
configurationReaderJson
-------------------------------------------------------------------------------
*   @brief		Reading the xml file for json header and footer updates.
*
*	@param		none
*
*	@return		None
*
*/

void PublishTableController::configurationReaderJson()
{	
	m_deviceidentifier = m_configReader->m_deviceIdentifier;
	m_string_pir_configure = m_configReader->m_PIRJson;
	m_string_pir_configure = "{" + m_string_pir_configure + "}";
	QString JsonData1 = m_string_pir_configure.c_str();
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	//cout << JsonData1.toStdString();
	m_outer_pir_configure = JsonResponse.object();
	if (m_outer_pir_configure.isEmpty())
	{
		cout << "PIR-xml data json parsingerror" << endl;
	}
	//cout << m_outer_pir_configure["deviceidentifier"].toString().toStdString();

	/*BLE string*/
	m_string_ble_configure = m_configReader->m_BLEJson;
	m_string_ble_configure = "{" + m_string_ble_configure + "}";
	JsonData1 = m_string_ble_configure.c_str();
	JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	//cout << JsonData1.toStdString();
	m_outer_ble_configure = JsonResponse.object();
	if (m_outer_ble_configure.isEmpty())
	{
		cout << "BLE-xml data json parsingerror" << endl;
	}

	/*WiFi string*/
	m_string_wifi_configure = m_configReader->m_WiFiJson;
	m_string_wifi_configure = "{" + m_string_wifi_configure + "}";
	JsonData1 = m_string_wifi_configure.c_str();
	JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	//cout << JsonData1.toStdString();
	m_outer_wifi_configure = JsonResponse.object();
	if (m_outer_wifi_configure.isEmpty())
	{
		cout << " WiFi -xml data json  parsingerror" << endl;
	}

	/*Real Sense string*/
	m_string_Realsense_configure = m_configReader->m_RealSenseJson;
	m_string_Realsense_configure = "{" + m_string_Realsense_configure + "}";
	JsonData1 = m_string_Realsense_configure.c_str();
	JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	//cout << JsonData1.toStdString();
	m_outer_Realsense_configure = JsonResponse.object();
	if (m_outer_Realsense_configure.isEmpty())
	{
		cout << " RealSense - xml data json parsingerror" << endl;
	}

	/*Sensor Telemetry string */
	m_string_sens_t_configure = m_configReader->m_sensorJson;
	m_string_sens_t_configure = "{" + m_string_sens_t_configure + "}";
	JsonData1 = m_string_sens_t_configure.c_str();
	JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	//cout << JsonData1.toStdString();
	m_outer_sens_t_configure = JsonResponse.object();
	if (m_outer_sens_t_configure.isEmpty())
	{
		cout << " Sensor_Telemetry - xml data json parsingerror" << endl;
	}

	/*dex string */
	m_string_dex_configure = m_configReader->m_DEXJson;
	m_string_dex_configure = "{" + m_string_dex_configure + "}";
	JsonData1 = m_string_dex_configure.c_str();
	JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	//cout << JsonData1.toStdString();
	m_outer_dex_configure = JsonResponse.object();
	if (m_outer_dex_configure.isEmpty())
	{
		cout << " dex string - xml data json parsingerror" << endl;
	}

	/*Trans string */
	m_string_Trans_configure = m_configReader->m_TransactionJson;
	m_string_Trans_configure = "{" + m_string_Trans_configure + "}";
	JsonData1 = m_string_Trans_configure.c_str();
	JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	//cout << JsonData1.toStdString();
	m_outer_Trans_configure = JsonResponse.object();
	if (m_outer_Trans_configure.isEmpty())
	{
		cout << " transaction string  - xml data json parsingerror" << endl;
	}

	/*stock string */
	m_string_stock_configure = m_configReader->m_StockJson;
	m_string_stock_configure = "{" + m_string_stock_configure + "}";
	JsonData1 = m_string_stock_configure.c_str();
	JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	//cout << JsonData1.toStdString();
	m_outer_stock_configure = JsonResponse.object();
	if (m_outer_stock_configure.isEmpty())
	{
		cout << " stock - xml data json parsingerror" << endl;
	}


}

/**
-------------------------------------------------------------------------------
~PublishTableController
-------------------------------------------------------------------------------
*   @brief		Destructor performing the cleanup like deleting allocated memory, dereferencing etc..
*
*	@param		none
*
*	@return		None
*
*/
PublishTableController::~PublishTableController()
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
*	@return		Instance of the PublishTableController
*
*/
PublishTableController* PublishTableController::getInstance()
{
	static PublishTableController publishTableInstance;
	return &publishTableInstance;
}

/**
-------------------------------------------------------------------------------
PublishTableInsertion
-------------------------------------------------------------------------------
*   @brief				This function inserts records into the database.
*
*	@param1 passjson	JSON message to be inserted into the database
*   @param2 sensor		Sensor type
*
*	@return				None
*/
void PublishTableController::PublishTableInsertion(string passjson, string sensor)
{
	const char* dataInsertionQuery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALUES (:Timestamp, :Type, :Value, :isSent)";
	map<string, string> args_dataInsertion;

	std::time_t present_time = std::time(NULL);
	QString present_time_string = QString::number(present_time);	

	args_dataInsertion[":Timestamp"] = present_time_string.toStdString();
	args_dataInsertion[":Type"] = sensor;
	args_dataInsertion[":Value"] = passjson;
	args_dataInsertion[":isSent"] = "0";

	if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_ptcInsertCtn, dataInsertionQuery, args_dataInsertion))
	{
		cout << "[PTC::PublishTableInsertion1] ERROR::Failure in Insertion Query" << endl;
	}
}

/**
-------------------------------------------------------------------------------
PublishTableInsertion
-------------------------------------------------------------------------------
*   @brief					This function inserts records into the database.
*
*	@param1 passjson		JSON message to be inserted into the database
*   @param2 sensor			Sensor type
*	@para3	send_receive	Arguments required for the insertion
*
*	@return					None
*/
void PublishTableController::PublishTableInsertion(string passjson, string sensor, pair<string, string> send_receive)
{
	const char* dataInsertionQuery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALUES (:Timestamp, :Type, :Value, :isSent)";
	map<string, string> args_dataInsertion;

	std::time_t presenttime = std::time(NULL);
	QString presenttime_string = QString::number(presenttime);

	args_dataInsertion[":Timestamp"] = presenttime_string.toStdString();
	args_dataInsertion[":Type"] = sensor;
	args_dataInsertion[":Value"] = passjson;
	args_dataInsertion[":isSent"] = "0";

	if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_ptcInsertCtn, dataInsertionQuery, args_dataInsertion))
	{
		cout << "[PTC::PublishTableInsertion2] ERROR::Failure in Insertion Query" << endl;
	}

	if (sensor == "WIFI")
	{
		const char* dataDeletionQueryWifi = "delete from WiFi_Counter where End_Time <= :endtime and macAddress = :macid";
		map<string, string> args_dataDeletionWifi;

		args_dataDeletionWifi[":endtime"] = send_receive.second;
		args_dataDeletionWifi[":macid"] = send_receive.first;
		if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_ptcDeleteCtn, dataDeletionQueryWifi, args_dataDeletionWifi))
		{
			cout << "[PTC::PublishTableInsertion2] ERROR::Failure in WiFi Delete Query" << endl;
		}
	}
	if (sensor == "BLE")
	{
		const char* dataDeletionQueryBLE = "delete from Bluetooth_Counter where End_Time <= :endtime and macAddress = :macid";
		map<string, string> args_dataDeletionBLE;

		args_dataDeletionBLE[":endtime"] = send_receive.second;
		args_dataDeletionBLE[":macid"] = send_receive.first;
		if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_ptcDeleteCtn, dataDeletionQueryBLE, args_dataDeletionBLE))
		{
			cout << "[PTC::PublishTableInsertion2] ERROR::Failure in BLE Delete Query" << endl;
		}
	}
}

/**
-------------------------------------------------------------------------------
PublishControllerUpdatingBLE
-------------------------------------------------------------------------------
*   @brief		Read the Bluetooth data from the database table and update in the publish table
*
*	@param		None		 
*
*	@return		None
*/
void PublishTableController::PublishControllerUpdatingBLE()
{
	const char* dataRetrieveQuery = "Select * from Bluetooth_Counter";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	recs;
	std::pair<string, string> send_receive;						

	cout << "Reading BLE table...." << endl; 	
	
	if (SELECT_QUERY_SUCCESS == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
	{
		// Get the Datatranslator instance	
		Datatranslator *getinstance_BLE = Datatranslator::getInstance();
		getinstance_BLE->converting_sensor_data_JSON_Indvidual_wifi_BLE(recs, "BLE", send_receive);
		recs.clear();
	}	
	else
	{
		cout << "[PTC::PublishControllerUpdatingBLE] ERROR::Failure in Select Query" << endl;
	}
}

/**
-------------------------------------------------------------------------------
PublishControllerUpdatingWiFi
-------------------------------------------------------------------------------
*   @brief		Read the WiFi data from the database table and update in the publish table
*
*	@param		None
*
*	@return		None
*/
void PublishTableController::PublishControllerUpdatingWiFi()
{
	const char* dataRetrieveQuery = "Select * from WiFi_Counter";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	recs;
	std::pair<string, string> send_receive;

	cout << "Reading WiFi_Counter table...." << endl; 
	
	if (SELECT_QUERY_SUCCESS == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
	{		
		// Get the Datatranslator instance
		Datatranslator *getinstance_wifi = Datatranslator::getInstance();
		getinstance_wifi->converting_sensor_data_JSON_Indvidual_wifi_BLE(recs, "WIFI", send_receive);
		recs.clear();
	}
	else
	{
		cout << "[PTC::PublishControllerUpdatingWiFi] ERROR::Failure in Select Query" << endl;
	}
}

/**
-------------------------------------------------------------------------------
PublishControllerUpdatingPIR
-------------------------------------------------------------------------------
*   @brief		Read the PIR data from the database table and update in the publish table
*
*	@param		None
*
*	@return		None
*/
void PublishTableController::PublishControllerUpdatingPIR()
{
	const char* dataRetrieveQuery = "Select * from pir_table";
	const char *dataDeletionQuery = "DELETE FROM pir_table WHERE timestamp  <= :Timestamp";
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList <QMap<QString, QVariant>>	recs;
	string timestamp;

	cout << "Reading PIR table...." << endl; ///

	if (SELECT_QUERY_SUCCESS == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
	{
		// Get the Datatranslator instance
		Datatranslator *getinstance_PIR = Datatranslator::getInstance();

		timestamp = getinstance_PIR->converting_sensor_data_JSON_Indvidual(recs, "PIR");
		args_dataDeletion[":Timestamp"] = timestamp;
		if (recs.size() > 0)
		{
			/*Deleting record from the PIR Table*/
			if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_ptcDeleteCtn, dataDeletionQuery, args_dataDeletion))
			{
				cout << "[PTC::PublishControllerUpdatingPIR] ERROR::Failure in Deletion Query" << endl;
			}
			/*End of deletion*/
		}
		recs.clear();
	}
	else
	{
		cout << "[PTC::PublishControllerUpdatingPIR] ERROR::Failure in Select Query" << endl;
	}
}

/**
-------------------------------------------------------------------------------
PublishCtlrUpdatingSensorTelemetry
-------------------------------------------------------------------------------
*   @brief		Read the sensor telemetry data from the database table and update in the publish table
*
*	@param		None
*
*	@return		None
*/

void PublishTableController::PublishCtlrUpdatingSensorTelemetry()
{
	const char* dataRetrieveQuery = "Select * from sensortelemetry_table";
	const char *dataDeletionQuery = "DELETE FROM sensortelemetry_table WHERE timestamp <= :Timestamp";
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList <QMap<QString, QVariant>>	recs;
	string timestamp;

	cout << "Reading Sensor Telemetry table...." << endl; 

	if (SELECT_QUERY_SUCCESS == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
	{
		// Get the Datatranslator instance
		Datatranslator *getinstance_sens_T = Datatranslator::getInstance();

		timestamp = getinstance_sens_T->converting_sensor_data_JSON_Indvidual(recs, "Sens_T");
		if (recs.size() > 0)
		{
			/*Deleting record from the sens_T Table*/
			args_dataDeletion[":Timestamp"] = timestamp;
			if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_ptcDeleteCtn, dataDeletionQuery, args_dataDeletion))
			{
				cout << "[PTC::PublishCtrlUpdatingSensorTelemetry] ERROR::Failure in Delete Query" << endl;
			}
			/*End of deletion*/
		}
		recs.clear();
	}
	else
	{
		cout << "[PTC::PublishCtrlUpdatingSensorTelemetry] ERROR::Failure in Select Query" << endl;
	}
}

/**
-------------------------------------------------------------------------------
PublishCtlrUpdatingRealSense
-------------------------------------------------------------------------------
*   @brief		Read the Real Sense data from the database table and update in the publish table
*
*	@param		None
*
*	@return		None
*/
void PublishTableController::PublishCtlrUpdatingRealsense()
{
	const char* dataRetrieveQuery = "Select * from people_counter";
	const char *dataDeletionQuery = "DELETE FROM people_counter WHERE timestamp <= :Timestamp";
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList <QMap<QString, QVariant>>	recs;
	string json;
	string timestamp;

	cout << "Reading individual Real sense table...." << endl; 
	
	if (SELECT_QUERY_SUCCESS == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
	{
		// Get the Datatranslator instance
		Datatranslator *getinstance_realsense = Datatranslator::getInstance();

		timestamp = getinstance_realsense->converting_sensor_data_JSON_Indvidual(recs, "RealSens");
		if (recs.size() > 0)
		{
			/*Deleting record from the Real Sense Table*/
			args_dataDeletion[":Timestamp"] = timestamp;
			if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_ptcDeleteCtn, dataDeletionQuery, args_dataDeletion))
			{
				cout << "[PTC::PublishCtrlUpdatingRealSense] ERROR::Failure in Delete Query" << endl;
			}
			/*End of deletion*/
		}
		recs.clear();
	}
	else
	{
		cout << "[PTC::PublishCtrlUpdatingRealSense] ERROR::Failure in Select Query" << endl;
	}
}

/**
-------------------------------------------------------------------------------
PublishControllerUpdatingTransaction
-------------------------------------------------------------------------------
*   @brief				Read the Transaction table data from the database and update in the publish table
*
*	@param	JsonData	Arguments to be passed for the query
*
*	@return				None
*/
void  PublishTableController::PublishControllerUpdatingTransaction(string JsonData)
{
	cout << "Reading individual Transaction table...." << endl; 
	Transaction transaction;

	// Obtain the transaction summary
	if (transaction.Transaction_Summary(JsonData) != TRANSACTION_SUMMARY_SUCCESS)
	{
		cout << "[PTC::PublishControllerUpdatingTransaction] ERROR::Failure to generate Transaction summary" << endl;
	}
}

/**
-------------------------------------------------------------------------------
PublishControllerUpdatingstock
-------------------------------------------------------------------------------
*   @brief				Read the stock table data from the database and update in the publish table
*
*	@param	JsonData	Arguments to be passed for the query
*
*	@return				None
*/
void  PublishTableController::PublishControllerUpdatingstock(string JsonData)
{
	cout << "Processing stock table updation...." << endl;
	Transaction transaction;

	// Obtain the stock summary
	if (transaction.Stock_Summary(JsonData) != STOCK_SUMMARY_SUCCESS)
	{
		cout << "[PTC::PublishControllerUpdatingstock] ERROR::Failure to generate stock summary" << endl;
	}
}

/**
-------------------------------------------------------------------------------
PublishControllerUpdatingVendingDB
-------------------------------------------------------------------------------
*   @brief		This function reads all the individual tables from the vending database 
*
*	@param		None
*
*	@return		pair<string, qlonglong> 	
*/
pair<string, qlonglong> PublishTableController::PublishControllerUpdatingVendingDB()
{
	const char* dataRetrieveQuery = "Select * from eventinfo";
	map<string, string> args_dataRetrieve;
	QMap<QString, QJsonArray> json_pair;
	QList <QMap<QString, QVariant>>	recs;

	cout << "Reading individual vending DB tables...." << endl;
	
	if (SELECT_QUERY_SUCCESS == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
	{		
		// Get the Datatranslator instance
		Datatranslator *dataTranslatorInstance = Datatranslator::getInstance();
		std::pair<QJsonArray, qlonglong> pair_value = dataTranslatorInstance->converting_dexdata_array(recs);
		json_pair.insert("eventinfo", pair_value.first);							// inserting the event info array into the json key value pair
		

		dataRetrieveQuery = "Select * from auditinfo";								// Read the Audit info from the database
		if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
		{
			cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in auditinfo Selection Query" << endl;
		}
		pair_value = dataTranslatorInstance->converting_dexdata_array(recs);
		json_pair.insert("auditinfo", pair_value.first);							// inserting the auditInfo array into the json key value pair
		

		dataRetrieveQuery = "Select * from stockinfo";								// Read the stock info from the database
		if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
		{
			cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in stockinfo Selection Query" << endl;
		}
		pair_value = dataTranslatorInstance->converting_dexdata_array(recs);	   // convert the map to Json array
		json_pair.insert("stockinfo", pair_value.first);						   // inserting the stockinfo array into the json key value pair
		

		dataRetrieveQuery = "Select * from vminfo";			                       // Read the Vminfo from the database
		if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
		{
			cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in vminfo Selection Query" << endl;
		}
		pair_value = dataTranslatorInstance->converting_dexdata_array(recs);	  // convert the map to Json array
		json_pair.insert("vminfo", pair_value.first);			                  // inserting the vminfo array into the json key value pair
	
		dataRetrieveQuery = "Select * from vmdevice";			                  // Read the vmdevice from the database
		if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
		{
			cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in vmdevice Selection Query" << endl;
		}
		pair_value = dataTranslatorInstance->converting_dexdata_array(recs);	  // convert the map to Json array
		json_pair.insert("vmdevice", pair_value.first);			                  // inserting the vmdevice array into the json key value pair

		dataRetrieveQuery = "Select * from cashin_info";			             // Read the cashin_info from the database
		if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
		{
			cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in cashin_info Selection Query" << endl;
		}
		pair_value = dataTranslatorInstance->converting_dexdata_array(recs);	  // convert the map to Json array
		json_pair.insert("cashin_info", pair_value.first);			              // inserting the cashin_info array into the json key value pair

		dataRetrieveQuery = "Select * from configurationdata";			          // Read the configurationdata from the database
		if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
		{
			cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in configurationdata Selection Query" << endl;
		}
		pair_value = dataTranslatorInstance->converting_dexdata_array(recs);	  // convert the map to Json array
		json_pair.insert("configurationdata", pair_value.first);			      // inserting the configurationdata array into the json key value pair

		dataRetrieveQuery = "Select * from salesinfo";							  // Read the salesinfo from the database
		if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
		{
			cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in salesinfo Selection Query" << endl;
		}
		pair_value = dataTranslatorInstance->converting_dexdata_array(recs);	  // convert the map to Json array
		json_pair.insert("salesinfo", pair_value.first);						  // inserting the salesinfo array into the json key value pair

		dataRetrieveQuery = "Select * from coinmechanism_info";			          // Read the coinmechanism_info from the database
		if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
		{
			cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in coinmechanism_info Selection Query" << endl;
		}
		pair_value = dataTranslatorInstance->converting_dexdata_array(recs);	  // convert the map to Json array
		json_pair.insert("coinmechanism_info", pair_value.first);			      // inserting the configurationdata array into the json key value pair

		dataRetrieveQuery = "Select * from product_vend_info";					  // Read the vend info from the database
		if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_ptcSelectCtn, dataRetrieveQuery, args_dataRetrieve, recs))
		{
			cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in product_vend_info Selection Query" << endl;
		}
		pair_value = dataTranslatorInstance->converting_dexdata_array(recs);      // convert the map to Json array
		json_pair.insert("product_vend_info", pair_value.first);                  // inserting the product_vend_info array into the json key value pair
	
		string jsonValue = dataTranslatorInstance->Dexdata_handler(json_pair);	  // convert the Json array to json string

		return pair<string, qlonglong>(jsonValue, pair_value.second);
	}
	else
	{
		cout << "[PTC::PublishControllerUpdatingVendingDB] ERROR::Failure in Select Query" << endl;
		// Return empty string incase of failure
		return pair<string, qlonglong>("", 0);
	}	
}

/**
-------------------------------------------------------------------------------
VendingDBdeletion
-------------------------------------------------------------------------------
*   @brief				Deletes the data read from the Vending DB
*
*	@param	timestamp	Argument to the query
*
*	@return				None
*/

void PublishTableController::VendingDBdeletion(qlonglong timestamp)
{
	const char *dataDeletionQuery = "DELETE FROM eventinfo WHERE Timestamp <= :Timestamp";
	map<string, string> args_dataDeletion;
	QString timeStamp = QString::number(timestamp);

	args_dataDeletion[":Timestamp"] = timeStamp.toStdString();
	
	// Delete eventinfo table	
	if (UPDATE_QUERY_SUCCESS != m_dbHandler->execUpdateQuery(m_ptcDeleteCtn, dataDeletionQuery, args_dataDeletion))
	{
		cout << "[PTC::VendingDBdeletion] ERROR::Failure in Deleting event info table" << endl;
	}
	
	// Delete product_vend_info table
	dataDeletionQuery = "DELETE FROM product_vend_info WHERE Timestamp <= :Timestamp";
	if (UPDATE_QUERY_SUCCESS != m_dbHandler->execUpdateQuery(m_ptcDeleteCtn, dataDeletionQuery, args_dataDeletion))
	{
		cout << "[PTC::VendingDBdeletion] ERROR::Failure in Deleting product_vend_info table" << endl;
	}

	// Delete configurationdata table
	dataDeletionQuery = "DELETE FROM configurationdata WHERE Timestamp <= :Timestamp";
	if (UPDATE_QUERY_SUCCESS != m_dbHandler->execUpdateQuery(m_ptcDeleteCtn, dataDeletionQuery, args_dataDeletion))
	{
		cout << "[PTC::VendingDBdeletion] ERROR::Failure in Deleting configurationdata table" << endl;
	}

#if 0
	// Delete stockinfo table
	dataDeletionQuery = "DELETE FROM stockinfo WHERE Timestamp <= :Timestamp";
	if (PTC_DB_QUERY_SUCCESS != executeQuery(m_ptcDeleteCtn, dataDeletionQuery, args_dataDeletion))
	{
		cout << "[PTC::VendingDBdeletion] ERROR::Failure in Deleting stockinfo table" << endl;
	}

	// Delete auditinfo table
	dataDeletionQuery = "DELETE FROM auditinfo WHERE Timestamp <= :Timestamp";
	if (PTC_DB_QUERY_SUCCESS != executeQuery(m_ptcDeleteCtn, dataDeletionQuery, args_dataDeletion))
	{
		cout << "[PTC::VendingDBdeletion] ERROR::Failure in Deleting auditinfo table" << endl;
	}

	// Delete vminfo table
	dataDeletionQuery = "DELETE FROM vminfo WHERE Timestamp <= :Timestamp";
	if (PTC_DB_QUERY_SUCCESS != executeQuery(m_ptcDeleteCtn, dataDeletionQuery, args_dataDeletion))
	{
		cout << "[PTC::VendingDBdeletion] ERROR::Failure in Deleting vminfo table" << endl;
	}

#endif
}


/**
-------------------------------------------------------------------------------
PublishControllerUpdatingdex
-------------------------------------------------------------------------------
*   @brief				Updates the Vending DB
*
*	@param	JsonData	Argument to the query
*
*	@return				None
*/
void  PublishTableController::PublishControllerUpdatingdex(string JsonData)
{
	DexConverter *DexConverter = DexConverter::getInstance();
	QJsonObject object;
	QString jsonString;
	cout << "DEX File generation...." << endl; 	

	jsonString = DexConverter->DexFileGenerater(JsonData);  //dexbuffer

	if (!jsonString.isEmpty())
	{
		object.insert("Dex", jsonString);
		string json = QJsonDocument(object).toJson(QJsonDocument::Compact).toStdString();
		PublishTableInsertion(json, "Dex");
	}
	else
	{
		cout << "[PTC::PublishControllerUpdatingdex] ERROR:: No DEX data available" << endl;
	}
}

/**
-------------------------------------------------------------------------------
UpdatePublishTable
-------------------------------------------------------------------------------
*   @brief		Updated all the Individual sensor and vending database
*
*	@param		None
*
*	@return		None
*/
void PublishTableController::UpdatePublishTable()
{
	const char * createTableQuery = "create table publish_table(Timestamp BIGINT, Type varchar(10), Value varchar(8000), Message_ID int NOT NULL AUTO_INCREMENT PRIMARY KEY, isSent int)";
	const char * dropTableQuery = "drop table if exists publish_table";
	static bool isFirst = false;
	static bool isBusyFlag = false; // TEMP : To be removed

	cout << " ## Publish Table Updation START ##" << endl;
	
	if (!isFirst)
	{
		/*Code for creating a table*/
		if (m_ptcUpdateCtn.isOpen())
		{
			QSqlQuery *query = new QSqlQuery(m_ptcUpdateCtn);
			/*
			* FROM QT DOCUMENTATION - Portability note: Some databases choose to
			* delay preparing a query until it is executed the first time. In this case,
			* preparing a syntactically wrong query succeeds, but every consecutive
			* exec() will fail. Hence the return for prepare() is not verified.
			*/
			QString TableCreationQuery = QString::fromStdString(dropTableQuery);
			query->prepare(TableCreationQuery);
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
					m_ptcUpdateCtn.close();
					if (m_ptcUpdateCtn.open() && !query->exec())
					{
						std::cout << "[PTC::UpdatePublishTable]::Failed to execute query with errorcode - "
							<< query->lastError().text().toStdString() << endl;
					}
				}				
			}

			TableCreationQuery = QString::fromStdString(createTableQuery);
			/*
			* FROM QT DOCUMENTATION - Portability note: Some databases choose to
			* delay preparing a query until it is executed the first time. In this case,
			* preparing a syntactically wrong query succeeds, but every consecutive
			* exec() will fail. Hence the return for prepare() is not verified.
			*/
			query->prepare(TableCreationQuery);
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
					m_ptcUpdateCtn.close();
					if (m_ptcUpdateCtn.open() && !query->exec())
					{
						std::cout << "[PTC::UpdatePublishTable]::Failed to execute query with errorcode - "
							<< query->lastError().text().toStdString() << endl;
					}
				}
			}

			// Delete the SQL query created
			delete query;
		}
		/*End of table creation*/
		isFirst = true;
	}
	
	// Check for busy flag and incase it is set meaning the previous cycle is in progress
	// Hence do not perform any processsing and just return.
	if (!isBusyFlag)	
	{
		// Set the flag so that following operations are not performed incase of next cycle prior to completion 
		// of current cycle
		isBusyFlag = true;	
		
		// Process the PIR individual table and update the Publish table with PIR values
		if (m_configReader->m_PIREnable == 1)
		{	
			PublishControllerUpdatingPIR();
		}

		// Process the SensorTelemetry individual table and update the Publish table with SensorTelemetry values
		if (m_configReader->m_sensorEnable == 1)
		{
			PublishCtlrUpdatingSensorTelemetry();
		}

#if defined (__linux__)
		// Process the BLE individual table and update the Publish table with BLE values
		if (m_configReader->m_BLEEnable == 1)
		{		
			PublishControllerUpdatingBLE();
		}

		// Process the WIFI individual table and update the Publish table with WIFI values
		if (m_configReader->m_WiFiEnable == 1)
		{		
			PublishControllerUpdatingWiFi();
		}

#elif defined(REALSENSE)
		// Process the RealSense individual table and update the Publish table with Realsense values
		if (m_configReader->m_RealSenseEnable == 1)
		{		
			PublishCtlrUpdatingRealsense();
		}
#endif

		pair<string, qlonglong> JsonData = PublishControllerUpdatingVendingDB();
		string json_vendinginfo = JsonData.first;

		// Check in case the IDEX thread completed it audit information processing 
		if (VDBDeleteFlag)
		{
			if (!json_vendinginfo.empty())
			{
				if (m_configReader->m_AuditExternalModuleEnable)
				{
					// DEX converter processing JSON data
					if (m_configReader->m_DEXPluginEnable)
						PublishControllerUpdatingdex(json_vendinginfo);

					// In case of sequential processing required, the processed DEX data from DEX converter
					// can be provided as input for below external modules for further processing

					// Second External process unit operate on JSON data
					if (m_configReader->m_AuditPlugin2Enable)
						cout << "Processing second External processing unit !!!" << endl;

					// Third External process unit operate on JSON data
					if (m_configReader->m_AuditPlugin3Enable)
						cout << "Processing Third External processing unit !!!" << endl;
				}
				else
				{
					cout << "Processed Transaction and Summary !!!" << endl;
					PublishControllerUpdatingTransaction(json_vendinginfo);	// to update Transcation data in the publish table
					PublishControllerUpdatingstock(json_vendinginfo);		// to update Stock data in the publish table
				}
			}
			else
			{
				cout << "[DTC::UpdatePublishTable] :: No valid sales and stock information available" << endl;
			}

			// Delete the vending related entries in the sensor db
			VendingDBdeletion(JsonData.second);

			// Set the flag back to false, so that IDEX running in other thread
			// can continue its processing of audit information
			VDBDeleteFlag = false;
		}

		// Reset back so that in next cycle this loop is processed
		isBusyFlag = false;
	}
	else // TEMP : To be removed
	{
		cout << "UpdatePublish table Next cycle prior to previous completion " << endl;
	}
	
	cout << " ## Publish Table Updation END ##" << endl;
}
