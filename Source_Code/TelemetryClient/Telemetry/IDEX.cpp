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
*	@file		IDEX.cpp
*
*	@brief		IDEX module processes the vend information to update auditinfo table.
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

#include "IDEX.h"
#include "QtCore/qdatetime.h"
#include "ConfigurationReader.h"
#include "DBHandler.h"
#include<QtCore/qfile.h>				// Qfile header file
#include <qsqlrecord.h>
#include <qdebug.h>
QString spFile = "./CreateFetchEventAndSP.txt";

QSqlDatabase IDEX::m_IDEXDatabaseCtn = QSqlDatabase();
bool VDBDeleteFlag = false;
bool IDEX::IDEXConnectionStatus = false;

// Following are the queries to be utilized by the IDEX module
const char* initialAuditUpdQuery = "Insert into auditinfo (Timestamp,Coins_in_CashBox,Coins_to_Tubes,Cash,Card_sales,VM_Refilled,Coins_in_CashBox_after_Refill,"
				"Coins_to_Tubes_after_Refill,Cash_after_Refill,card_sales_after_Refill,NoofReads,LastUpdated)values(:InitTimestamp,:CoinsinCashBox,:CoinsToTubes,"
				":Cash,:Cardsales,:VMRefilled,:CoinsinCashBoxafterRefill,:CoinstoTubesafterRefill,:CashafterRefill,:cardsalesafterRefill,:NoofReads,:LastUpdated)";

const char* processedAuditUpdQuery = "Insert into auditinfo(Timestamp, Coins_in_CashBox, Coins_to_Tubes, Cash, Card_sales, VM_Refilled, Coins_in_CashBox_after_Refill, "
	"Coins_to_Tubes_after_Refill,Cash_after_Refill,card_sales_after_Refill,NoofReads,LastUpdated)values(:InitTimestamp,:CoinsinCashBox,:CoinsToTubes,"
	":Cash,:Cardsales,:VMRefilled,:CoinsinCashBoxafterRefill,:CoinstoTubesafterRefill,:CashafterRefill,:cardsalesafterRefill,:NoofReads,:LastUpdated)";

const char * createAuditInfoTableQuery = "CREATE TABLE `auditinfo` (\
												`Timestamp` BIGINT(20) NOT NULL COMMENT 'Timestamp',\
												`Coins_in_CashBox` FLOAT NULL DEFAULT NULL COMMENT 'Coins in CashBox since Initialization',\
												`Coins_to_Tubes` FLOAT NULL DEFAULT NULL COMMENT 'Coins to Tubes since Initialization',\
												`Cash` INT(10) NULL DEFAULT NULL COMMENT 'Cash since Initialization',\
												`card_sales` INT(10) NULL DEFAULT NULL COMMENT 'Card sales since Initialization',\
												`VM_Refilled` BIGINT(20) NULL DEFAULT NULL COMMENT 'VM Refilled',\
												`Coins_in_CashBox_after_Refill` FLOAT NULL DEFAULT NULL COMMENT 'Coins in CashBox after Refill',\
												`Coins_to_Tubes_after_Refill` FLOAT NULL DEFAULT NULL COMMENT 'Coins to Tubes after Refill',\
												`Cash_after_Refill` INT(10) NULL DEFAULT NULL COMMENT 'Cash after Refill',\
												`card_sales_after_Refill` INT(10) NULL DEFAULT NULL COMMENT 'Card sales after Refill',\
												`NoofReads` INT(10) NULL DEFAULT NULL COMMENT 'No of Reads',\
												`LastUpdated` BIGINT(20) NULL DEFAULT NULL COMMENT 'Last Updated Timestamp')";
												
const char * createSalesInfoTableQuery = "CREATE TABLE `salesinfo` (\
												`Product_ID` VARCHAR(50) NULL DEFAULT NULL COMMENT 'Product ID of the product',\
												`Paidvendsales` INT(11) NULL DEFAULT NULL COMMENT 'Paid vend sales of the product',\
												`PaidvendvalueInit` FLOAT NULL DEFAULT NULL COMMENT 'Paid vend value of the product since Initialization')";

const char * createVMInfoTableQuery			= "CREATE TABLE `vminfo` (\
													`Timestamp` BIGINT(20) NOT NULL,\
													`Comm_Id` VARCHAR(10) NULL DEFAULT NULL,\
													`Version` VARCHAR(6) NULL DEFAULT NULL,\
													`Serial_No` VARCHAR(20) NULL DEFAULT NULL,\
													`Asset_No` VARCHAR(20) NULL DEFAULT NULL,\
													`Model_No` VARCHAR(20) NULL DEFAULT NULL,\
													`Machine_Build_Std` INT(11) NULL DEFAULT NULL,\
													`Machine_Location` VARCHAR(50) NULL DEFAULT NULL,\
													`Currency_Decimal` INT(20) NULL DEFAULT NULL,\
													`Currency_Code` INT(4) UNSIGNED ZEROFILL NOT NULL,\
													`Alphabetic_Currency_Code` VARCHAR(50) NULL DEFAULT NULL)";

const char * createProductVendInfoTableQuery = "CREATE TABLE `product_vend_info` (\
													`InternalKey` INT(5) NULL DEFAULT NULL COMMENT 'Since TransactionID will not be unique, Internal key is used.',\
													`Transaction_ID` INT(5) NULL DEFAULT NULL COMMENT 'Transaction for this product',\
													`Timestamp` BIGINT(20) NOT NULL COMMENT 'Current Timestamp',\
													`Product_price` FLOAT NULL DEFAULT NULL COMMENT 'Product price',\
													`SalePrice` FLOAT NULL DEFAULT NULL COMMENT 'Sale price of the product',\
													`Tax` FLOAT NULL DEFAULT NULL COMMENT 'Tax',\
													`Product_ID` VARCHAR(50) NULL DEFAULT NULL,\
													`VendType` INT(2) NULL DEFAULT NULL COMMENT 'Vend type : 0->Paid vend, 1->Free vend, 2->Test vend',\
													`ProductSold` INT(10) NULL DEFAULT NULL COMMENT 'Quantity of the product sold',\
													`PaymentSource` INT(2) UNSIGNED NULL DEFAULT NULL COMMENT 'Payment type : 0->Coin, 1-> Cashless, 2-> Cash',\
													`CampaignId` INT(2) NULL DEFAULT NULL COMMENT 'Campaign Name',\
													`Discount_Ratio` FLOAT NULL DEFAULT NULL COMMENT 'Discount ',\
													`ShelfID` VARCHAR(2) NULL DEFAULT NULL COMMENT 'Shelf ID',\
													`ColumnID` VARCHAR(2) NULL DEFAULT NULL COMMENT 'Column ID',\
													`VendorID` VARCHAR(6) NULL DEFAULT NULL COMMENT 'Vendor ID for the product')";

const char * createStockInfoTableQuery		 = "CREATE TABLE `stockinfo` (\
													`Product_ID` VARCHAR(50) NULL DEFAULT NULL COMMENT 'Product ID of the product',\
													`Stock` INT(10) NULL DEFAULT NULL COMMENT 'Stock of the product',\
													`Timestamp` BIGINT(20) NULL DEFAULT NULL)";

const char * createEventInfoTableQuery		 = "CREATE TABLE `eventinfo` (\
													`Timestamp` BIGINT(20) NOT NULL COMMENT 'Current Timestamp',\
													`Event_Identity` CHAR(50) NULL DEFAULT NULL COMMENT 'Event identifier',\
													`Duration_of_Event` INT(10) NULL DEFAULT NULL COMMENT 'Duration of event',\
													`EventCurrentState` VARCHAR(50) NULL DEFAULT NULL)";

const char * createVMdeviceTableQuery		 = "CREATE TABLE `vmdevice` (\
													`VM_Asset_No` VARCHAR(50) NOT NULL,\
													`VM_Device_Type` VARCHAR(50) NOT NULL,\
													`SW_Version` INT(11) NOT NULL,\
													`Model_No` VARCHAR(50) NOT NULL,\
													`Serial_No` VARCHAR(50) NOT NULL,\
													`VM_Device_Asset_No` VARCHAR(50) NOT NULL)";

const char * createCashininfoTableQuery		 = "CREATE TABLE `cashin_info` (\
													`Timestamp` BIGINT(20) NULL DEFAULT NULL,\
													`Coins_in_CashBox` FLOAT NULL DEFAULT NULL,\
													`Coins_to_Tubes` FLOAT NULL DEFAULT NULL,\
													`Bills_in_CashBox` FLOAT NULL DEFAULT NULL,\
													`Cash_to_Recycler` FLOAT NULL DEFAULT NULL)";

const char * createConfigurationDataTableQuery = "CREATE TABLE `configurationdata` (\
													`Timestamp` BIGINT(20) NULL DEFAULT NULL COMMENT 'Current Timestamp',\
													`Block_Identifier` VARCHAR(50) NULL DEFAULT NULL COMMENT 'Block Identifier',\
													`Configuration_Data` VARCHAR(100) NULL DEFAULT NULL COMMENT 'Configuration Data',\
													`OptionalField2` VARCHAR(20) NULL DEFAULT NULL COMMENT 'Optional Field #2',\
													`OptionalField3` VARCHAR(20) NULL DEFAULT NULL COMMENT 'Optional Field #3',\
													`OptionalField4` VARCHAR(20) NULL DEFAULT NULL COMMENT 'Optional Field #4',\
													`OptionalField5` VARCHAR(20) NULL DEFAULT NULL COMMENT 'Optional Field #5',\
													`OptionalField6` VARCHAR(20) NULL DEFAULT NULL COMMENT 'Optional Field #6',\
													`OptionalField7` VARCHAR(20) NULL DEFAULT NULL COMMENT 'Optional Field #7',\
													`OptionalField8` VARCHAR(20) NULL DEFAULT NULL COMMENT 'Optional Field #8',\
													`OptionalField9` VARCHAR(20) NULL DEFAULT NULL COMMENT 'Optional Field #9',\
													`OptionalField10` VARCHAR(20) NULL DEFAULT NULL COMMENT 'Optional Field #10')";

const char * createCoinMechanismTableQuery	  = "CREATE TABLE `coinmechanism_info` (\
													`VM_DeviceId` VARCHAR(50) NULL DEFAULT NULL,\
													`Coin_Type_Number` INT(11) NULL DEFAULT NULL,\
													`Coin_value` INT(11) NULL DEFAULT NULL,\
													`Number_of_Coins_in_Tube` INT(11) NULL DEFAULT NULL,\
													`Coin_Insertion_during_Manual_Fill` INT(11) NULL DEFAULT NULL,\
													`Coins_Dispensed_during_Manual_Invent` INT(11) NULL DEFAULT NULL,\
													`IsCoinTubeFull` INT(11) NULL DEFAULT NULL)";

const char * dropAuditInfoTableQuery = "drop table if exists auditinfo";
const char * dropVMInfoTableQuery = "drop table if exists vminfo";
const char * dropProductVendInfoTableQuery = "drop table if exists product_vend_info";
const char * dropStockInfoTableQuery = "drop table if exists stockinfo";
const char * dropEventInfoTableQuery = "drop table if exists eventinfo";
const char * dropCashininfoTableQuery = "drop table if exists cashin_info";
const char * dropVMdeviceTableQuery = "drop table if exists vmdevice";
const char * dropConfigurationDataTableQuery = "drop table if exists configurationdata";
const char * dropCoinMechanismTableQuery = "drop table if exists coinmechanism_info";
const char * dropSalesInfoTableQuery = "drop table if exists salesinfo";

/**
------------------------------------------------------------------------------ -
IDEX
------------------------------------------------------------------------------ -
*@brief		Constructor initializing the member variables and required inits.
*
*	@param		none
*
*	@return		None
*
*/
IDEX::IDEX() :
	m_configReader(NULL),
	m_dbHandler(NULL),
	InitTimestamp(0),
	RefillTimestamp(0),
	NoofReads(0),
	TCoin(0),
	CoinsToTubes(0),
	TCash(0),
	TCard(0),
	RCoin(0),
	RCoinsToTubes(0),
	RCash(0),
	RCard(0),
	TRecycler(0)
{
	// Obtain the Configuration reader instance
	m_configReader = ConfigurationReader::getInstance();
	m_dbHandler = DBHandler::getInstance();

	// To obtain the connection of the 'sensor_Db' database and have it open for further processing
	IDEX_DB_STATUS_CODE connectionStatus = getConnection(m_configReader->m_DatabaseHostName, m_configReader->m_DatabaseName, m_configReader->m_DatabaseUserName,
		m_configReader->m_DatabasePassword, m_configReader->m_DatabasePort);

	// Check if the connection is successfully opened or not
	if (DB_OPEN_SUCCESS != connectionStatus)
	{
		IDEXConnectionStatus = false;
		// Failed to open the required database connections
		cout << "[IDEX Constructor] Error - Failure to open database connections with errorCode -"
			<< connectionStatus << endl;
	}

	// Set the IDEX connection status
	IDEXConnectionStatus = true;
}

/**
-------------------------------------------------------------------------------
~IDEX
-------------------------------------------------------------------------------
*   @brief		Destructor performing the cleanup like deleting allocated memory, dereferencing etc..
*
*	@param		none
*
*	@return		None
*
*/
IDEX::~IDEX()
{
	// Define all the code cleanup and memory delete 
}

/**
-------------------------------------------------------------------------------
getInstance
-------------------------------------------------------------------------------
* To obtain instance
*
*	@pre		None
*
*	@param		None
*
*	@return		Instance of the IDEX
*
*/
IDEX* IDEX::getInstance()
{
	// Obtain the IDEX static instance
	static IDEX IDEXInstance;
	return &IDEXInstance;
}

/**
-------------------------------------------------------------------------------
IDEX_Init
-------------------------------------------------------------------------------
*   @brief		This function initiates other IDEX functions 
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::IDEX_Init()
{
	// Perform the creation and deletion of vending tables required in the sensor database	
	if ((TableCreation(createAuditInfoTableQuery, dropAuditInfoTableQuery, false) &&
		TableCreation(createSalesInfoTableQuery, dropSalesInfoTableQuery, false) &&
		TableCreation(createVMInfoTableQuery, dropVMInfoTableQuery, true) &&
		TableCreation(createProductVendInfoTableQuery, dropProductVendInfoTableQuery, true) &&
		TableCreation(createStockInfoTableQuery, dropStockInfoTableQuery, true) &&
		TableCreation(createVMdeviceTableQuery, dropVMdeviceTableQuery, true) &&
		TableCreation(createCashininfoTableQuery, dropCashininfoTableQuery, true) &&
		TableCreation(createConfigurationDataTableQuery, dropConfigurationDataTableQuery, true) &&
		TableCreation(createCoinMechanismTableQuery, dropCoinMechanismTableQuery, true) &&
		TableCreation(createEventInfoTableQuery, dropEventInfoTableQuery, true)) == false)
	{
		// Stop further processing of IDEX module incase of any failure in query handling
		return;
	}

	// Creation of the stored procedure in the database
	if (SPEventCreation() == false)
	{
		// Stop further processing of IDEX module incase of failure for stored procedure creation
		return;
	}
}

/**
-------------------------------------------------------------------------------
IDEX_Process
-------------------------------------------------------------------------------
*   @brief		This function initiates other IDEX functions
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::IDEX_Process()
{
	const char* dataRetrieveQuery = "Call sp_FetchVendDetails";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	recs;

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, dataRetrieveQuery, args_dataRetrieve, recs))
	{
		cout << "ERROR :: [IDEX::IDEX_Process::Execute Store procedure Query] Failed to execute query" << endl;
		return;
	}

	cout << "Store Procedure executed successfully!!!!" << endl;
	
	// Copy the data from vending database to the sensor database
	if (!VDBDeleteFlag)
	{
		if (true == CheckforNewTransactionEntries() && !VDBDeleteFlag)
		{
			IDEX_Read_VmInfoTable();                // Read the Vending information table
			IDEX_Read_CashinInfoTable();			// Read the Cashin Info table
			IDEX_Initial_Update_AuditInfoTable();   // Manipulate vending inputs and update audit info table
			IDEX_Read_EventTable();                 // Read the event table information
			IDEX_Update_AuditInfoTable();           // Update audit info table
			CollectProductSalesRecord();
			UpdateProductSalesRecord();
			cout << "[IDEX::IDEX_Init] INFO: Telemetry database updated with audit information" << endl;
			ClearMemory();
		}
	}
}

/**
-------------------------------------------------------------------------------
CheckforNewTransactionEntries
-------------------------------------------------------------------------------
*   @brief		This function to check if there are entries in product vend info
*
*	@param		none
*
*	@return		return status - true or false
*
*/
bool IDEX::CheckforNewTransactionEntries()
{
	const char *selectQuery = "Select * from product_vend_info";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	resultRecds;

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, selectQuery, args_dataRetrieve, resultRecds))
	{
		cout << "ERROR :: [IDEX::CheckforNewTransactionEntries]:: Failed to execute select query" << endl;
		return false;
	}

	// Check the results record
	if (resultRecds.size() == 0)
	{
		// Query results in empty value meaning no records in the prodcut_vend_info table
		return false;
	}

	// Query results show that it contains atleast one record in the product_vend_info table
	return true;
}

/**
-------------------------------------------------------------------------------
ClearMemory
-------------------------------------------------------------------------------
*   @brief		This function clears the audit table variables
*
*	@param		none
*
*	@return		return status (true or false)
*
*/
void IDEX::ClearMemory()
{
	// Clear all the audit info variables
	InitTimestamp = 0;
	RefillTimestamp = 0;
	NoofReads = 0;

	TCoin = 0;
	CoinsToTubes = 0;
	TCash = 0;
	TCard = 0;
	RCoin = 0;
	RCoinsToTubes = 0;
	RCash = 0;
	RCard = 0;
	TRecycler = 0;

	ProductMap.clear();
	ProductMap1.clear();
}

/**
-------------------------------------------------------------------------------
SPEventCreation
-------------------------------------------------------------------------------
*   @brief		This function creates the stored procedure to copy the audit contents from 
*				the third party vending database to the local sensor database
*
*	@param		none
*
*	@return		return status (true or false)
*
*/
bool IDEX::SPEventCreation()
{
	// Open the file containing the stored procedure to copy the audit contents from 
	// the third party vending database to the local sensor database
	QFile file(spFile);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		cout << "Could not open store procedure sql file for reading" << endl;
		return false;
	}

	// Read the contents of the stored procedure
	QString spData = file.readAll();
	file.close();

	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	recs;

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, spData.toStdString(), args_dataRetrieve, recs))
	{
		cout << "ERROR :: [IDEX::SPEventCreation]::Failed to execute delete query" << endl;
		return false;
	}

	return true;
}

/**
-------------------------------------------------------------------------------
ConnectionStatus
-------------------------------------------------------------------------------
*   @brief		Returns the connection status
*
*	@param		none
*
*	@return		Connection status (true or false)
*
*/

bool IDEX::ConnectionStatus()
{
	return IDEXConnectionStatus;
}

/**
-------------------------------------------------------------------------------
TableCreation
-------------------------------------------------------------------------------
*   @brief					This function creates the required vending database tables
*
*	@param1 creationQuery	Query to create the corresponding table
*	@param2 deletionQuery	Query to delete the corresponding table
*	@param2 isDropRequired	Flag to say whether drop is required or not
*
*	@return	query status
*
*/
bool IDEX::TableCreation(const string &creationQuery, const string &deletionQuery, bool isDropRequired)
{
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	recs;

	if ((isDropRequired == true) && 
		(SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, deletionQuery, args_dataRetrieve, recs)))
	{
		cout << "ERROR :: [IDEX::Table Creation]::Failed to execute delete query" << endl;
		return false;
	}

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, creationQuery, args_dataRetrieve, recs))
	{
		cout << "ERROR :: [IDEX::Table Creation]::Failed to execute creation query" << endl;
		return false;
	}

	// Return true when table creation is successful
	return true;
}

/**
-------------------------------------------------------------------------------
getConnection
-------------------------------------------------------------------------------
*   @brief					This function creates a connection to the database.
*
*	@param1 HostName		Hostname of the connection
*	@param2 DatabaseName	Name of the database
*   @param3 UserName		UserName(Credentials) for the database
*   @param4 Password		Password(Credentials) for the database
*   @param5 port			Port ID used by the database connection
*
*	@return	IDEX_DB_STATUS_CODE	Statuscode for the getconnection.
*
*/
IDEX_DB_STATUS_CODE IDEX::getConnection(string HostName, string DatabaseName, string UserName, string Password, int Port)
{
	//connection configuration should done only one time for db
	if (!m_IDEXDatabaseCtn.isOpen()) {
		m_IDEXDatabaseCtn = QSqlDatabase::addDatabase("QMYSQL", "IDEX_Connection");
		m_IDEXDatabaseCtn.setHostName(QString::fromStdString(HostName));
		m_IDEXDatabaseCtn.setDatabaseName(QString::fromStdString(DatabaseName));
		m_IDEXDatabaseCtn.setUserName(QString::fromStdString(UserName));
		m_IDEXDatabaseCtn.setPassword(QString::fromStdString(Password));
		m_IDEXDatabaseCtn.setPort(Port);
		if (!m_IDEXDatabaseCtn.open())
		{
			cout << "[IDEX] Error ::  Failed to open database:" << endl;
			return IDEX_CONNECTION_FAILURE;
		}
	}
	return DB_OPEN_SUCCESS;
}

/**
-------------------------------------------------------------------------------
IDEX_Read_VmInfoTable
-------------------------------------------------------------------------------
*   @brief		This function reads informations from vminfo table and update auditinfo table for first time initialization
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::IDEX_Read_VmInfoTable()
{	
	const char *selectQuery = "Select * from vminfo";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	resultRecds;

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, selectQuery, args_dataRetrieve, resultRecds))
	{
		cout << "ERROR :: [IDEX::IDEX_Read_VmInfoTable]:: Failed to execute select query" << endl;
		return;
	}

	// Check the results record
	if (resultRecds.size() != 0)
	{
		// Results contains the entries from the vminfo table as per the query
		QList < QMap<QString, QVariant>>::iterator listIterate;
		QMap<QString, QVariant>::iterator mapIter;

		for (listIterate = resultRecds.begin(); listIterate != resultRecds.end(); listIterate++)
		{			
			mapIter = listIterate->find("Timestamp");
			if (mapIter != listIterate->end())
			{
				QVariant temp = mapIter.value();
				InitTimestamp = temp.toInt();
				break;
			}
		}		
	}
}

/**
-------------------------------------------------------------------------------
IDEX_Read_CashinInfoTable
-------------------------------------------------------------------------------
*   @brief		This function reads informations from cashinInfo table and update auditinfo table for first time initialization
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::IDEX_Read_CashinInfoTable()
{
	const char *selectQuery = "Select * from cashin_info";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	resultRecds;

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, selectQuery, args_dataRetrieve, resultRecds))
	{
		cout << "ERROR :: [IDEX::IDEX_Read_CashinInfoTable]:: Failed to execute select query" << endl;
		return;
	}

	// Check the results record
	if (resultRecds.size() != 0)
	{
		// Results contains the entries from the cashin_info table as per the query
		QList < QMap<QString, QVariant>>::iterator listIterate;
		QMap<QString, QVariant>::iterator mapIter;

		for (listIterate = resultRecds.begin(); listIterate != resultRecds.end(); listIterate++)
		{
			for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
			{
				QString fieldName = mapIter.key();
				if (fieldName == "Coins_in_CashBox")
					TCoin = mapIter.value().toFloat();
				if (fieldName == "Coins_to_Tubes")
					CoinsToTubes = mapIter.value().toFloat();
				if (fieldName == "Bills_in_CashBox")
					TCash = mapIter.value().toFloat();
				if (fieldName == "Cash_to_Recycler")
					TRecycler = mapIter.value().toFloat();
			}
		}
	}
}

/**
-------------------------------------------------------------------------------
IDEX_Initial_Update_AuditInfoTable
-------------------------------------------------------------------------------
*   @brief		This function updates audit information table at initial stage
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::IDEX_Initial_Update_AuditInfoTable()
{	
	static bool isEmptyFlag = false;

	const char *selectQuery = "select * from auditinfo where Cash is not null";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	resultRecds;

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, selectQuery, args_dataRetrieve, resultRecds))
	{
		cout << "ERROR :: [IDEX::IDEX_Initial_Update_AuditInfoTable]:: Failed to execute select query" << endl;
		return;
	}

	// Check the results record
	if (resultRecds.size() != 0)
	{
		// Results contains the entries from the cashin_info table as per the query
		QList < QMap<QString, QVariant>>::iterator listIterate;
		QMap<QString, QVariant>::iterator mapIter;
		QMap<QString, QVariant> innerMap;

		// Obtain the last map holding the latest values
		innerMap = resultRecds.last();

		if (!innerMap.isEmpty())
		{
			for (mapIter = innerMap.begin(); mapIter != innerMap.end(); mapIter++)
			{
				// Retrieve existing values
				QString fieldName = mapIter.key();
				if (fieldName == "Coins_in_CashBox")
					TCoin = mapIter.value().toFloat();
				if (fieldName == "Coins_to_Tubes")
					CoinsToTubes = mapIter.value().toFloat();
				if (fieldName == "Cash")
					TCash = mapIter.value().toFloat();
				if (fieldName == "card_sales")
					TCard = mapIter.value().toFloat();
				if (fieldName == "Coins_in_CashBox_after_Refill")
					RCoin = mapIter.value().toFloat();
				if (fieldName == "Coins_to_Tubes_after_Refill")
					RCoinsToTubes = mapIter.value().toFloat();
				if (fieldName == "Cash_after_Refill")
					RCash = mapIter.value().toFloat();
				if (fieldName == "card_sales_after_Refill")
					RCard = mapIter.value().toFloat();
				if (fieldName == "NoofReads")
					NoofReads = mapIter.value().toInt();
			}
		}
				
	}
	else
	{
		isEmptyFlag = true;
	}

	if (isEmptyFlag)
	{
		// Bind the values required to be inserted into the audit table
		map<string, string> args_dataInsertion;
		args_dataInsertion[":InitTimestamp"] = to_string(InitTimestamp);		
		args_dataInsertion[":CoinsinCashBox"] = to_string(TCoin);
		args_dataInsertion[":CoinsToTubes"] = to_string(CoinsToTubes);
		args_dataInsertion[":Cash"] = to_string(TCash);
		args_dataInsertion[":Cardsales"] = to_string(TCard);
		args_dataInsertion[":VMRefilled"] = to_string(RefillTimestamp);
		args_dataInsertion[":CoinsinCashBoxafterRefill"] = to_string(RCoin);
		args_dataInsertion[":CoinstoTubesafterRefill"] = to_string(RCoinsToTubes);
		args_dataInsertion[":CashafterRefill"] = to_string(RCash);
		args_dataInsertion[":cardsalesafterRefill"] = to_string(RCard);
		args_dataInsertion[":NoofReads"] = to_string(NoofReads);

		// Perform the update query onto the audit table
		if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_IDEXDatabaseCtn, initialAuditUpdQuery, args_dataInsertion))
		{
			cout << "[IDEX::IDEX_Initial_Update_AuditInfoTable] ERROR::Failure in Insertion Query" << endl;
		}

		// Reset the empty flag for next iteration
		isEmptyFlag = false;
	}
}

/**
-------------------------------------------------------------------------------
IDEX_Read_EventTable
-------------------------------------------------------------------------------
*   @brief		This function reads informations from eventinfo to find the occurence of REFILL event
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::IDEX_Read_EventTable()
{
	const char *selectQuery = "Select * from eventinfo";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	resultRecds;

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, selectQuery, args_dataRetrieve, resultRecds))
	{
		cout << "ERROR :: [IDEX::IDEX_Read_EventTable]:: Failed to execute select query" << endl;
		return;
	}

	// Check the results record
	if (resultRecds.size() != 0)
	{
		// Results contains the entries from the eventinfo table as per the query
		QList < QMap<QString, QVariant>>::iterator listIterate;
		QMap<QString, QVariant>::iterator mapIter;

		for (listIterate = resultRecds.begin(); listIterate != resultRecds.end(); listIterate++)
		{
			for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
			{	
				if ((mapIter.key() == "Event_Identity") &&
					(mapIter.value().toString() == "REFILL"))
				{					
					mapIter = listIterate->find("Timestamp");
					if (mapIter != listIterate->end())
					{
						RefillTimestamp = mapIter.value().toInt();
						break;
					}					
				}
			}
		}
	}
}

/**
-------------------------------------------------------------------------------
IDEX_All_Transaction_Summary
-------------------------------------------------------------------------------
*   @brief		This function reads informations from product_vend_info for further processing
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::IDEX_All_Transaction_Summary()
{
	const char *selectQuery = "Select * from product_vend_info";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	resultRecds;

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, selectQuery, args_dataRetrieve, resultRecds))
	{
		cout << "ERROR :: [IDEX::IDEX_All_Transaction_Summary]:: Failed to execute select query" << endl;
		return;
	}

	// Check the results record
	if (resultRecds.size() != 0)
	{
		// Results contains the entries from the eventinfo table as per the query
		QList < QMap<QString, QVariant>>::iterator listIterate;
		QMap<QString, QVariant>::iterator mapIter;

		for (listIterate = resultRecds.begin(); listIterate != resultRecds.end(); listIterate++)
		{
			int productInternalKey = 0;
			for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
			{
				string fieldName = mapIter.key().toStdString();
				QVariant fieldValue = mapIter.value();
				if (fieldName == "Transaction_ID")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toInt()));
				if (fieldName == "Timestamp")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toInt()));
				if (fieldName == "Product_price")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toFloat()));
				if (fieldName == "SalePrice")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toFloat()));
				if (fieldName == "Tax")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toFloat()));
				if (fieldName == "Product_ID")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toString()));
				if (fieldName == "VendType")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toInt()));
				if (fieldName == "ProductSold")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toInt()));
				if (fieldName == "PaymentSource")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toInt()));
				if (fieldName == "CampaignId")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toInt()));
				if (fieldName == "Discount_Ratio")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toFloat()));
				if (fieldName == "ShelfID")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toString()));
				if (fieldName == "ColumnID")
					ProdInnerMap.insert(map<std::string, QVariant>::value_type(fieldName, fieldValue.toString()));
				if (fieldName == "InternalKey")
					productInternalKey = fieldValue.toInt();
			}

			ProductMap.insert(map<int, ProdInnerInfo>::value_type(productInternalKey, ProdInnerMap));
			ProdInnerMap.clear();
		}
	}
	
#if 0
	/******************    Debugging purpose ************************/
	for (ProductinfoIter = ProductMap.begin(); ProductinfoIter != ProductMap.end(); ProductinfoIter++)
	{
		ProdInnerMap = ProductinfoIter->second;
		std::cout << "---- " << ProductinfoIter->first << " ----" << std::endl;
		for (ProdInnerMapIter = ProdInnerMap.begin(); ProdInnerMapIter != ProdInnerMap.end(); ProdInnerMapIter++)
		{
			std::cout << ProdInnerMapIter->first;
			std::cout << " : " << ProdInnerMapIter->second.toString().toStdString() << endl;
		}
		cout << endl;
	}
#endif
}

/**
-------------------------------------------------------------------------------
IDEX_Update_AuditInfoTable
-------------------------------------------------------------------------------
*   @brief		This function processes the vend information to update auditinfo table
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::IDEX_Update_AuditInfoTable()
{	
	int PVTimestamp = 0;
	float PPrice = 0;
	int Psold = 0;
	uint currentTS = 0;
	QString ProductID;
	
	// Obtains the required information from the product information table
	IDEX_All_Transaction_Summary();
	NoofReads++;
	for (ProductinfoIter = ProductMap.begin(); ProductinfoIter != ProductMap.end(); ProductinfoIter++)
	{
		ProdInnerMapIter = ProductinfoIter->second.find("ProductSold");
		Psold = ProdInnerMapIter->second.toInt();
		ProdInnerMapIter = ProductinfoIter->second.find("SalePrice");
		PPrice = ProdInnerMapIter->second.toFloat();
		ProdInnerMapIter = ProductinfoIter->second.find("PaymentSource");
		switch (ProdInnerMapIter->second.toInt())
		{
		case 0:			// coin
		//	TCoin += (PPrice* static_cast<float> (Psold)); 
			CoinsToTubes += (PPrice* static_cast<float> (Psold)); // coin sales will be sent to tubes only
			break;
		case 1:			// cashless i.e card
			TCard += (PPrice*  static_cast<float>(Psold));
			break;
		case 2:			// cash
			TCash += (PPrice* static_cast<float>(Psold));
			break;
		default:
			break;
		}

		ProdInnerMapIter = ProductinfoIter->second.find("Timestamp");
		PVTimestamp = ProdInnerMapIter->second.toInt();
		// Check if the product vending has happened after the Refill event
		if (PVTimestamp >= RefillTimestamp)
		{
			ProdInnerMapIter = ProductinfoIter->second.find("PaymentSource");
			switch (ProdInnerMapIter->second.toInt())
			{
			case 0:		//Refill coin		
			//	RCoin += (PPrice* static_cast<float> (Psold));
				RCoinsToTubes += (PPrice* static_cast<float> (Psold)); // coin sales will be sent to tubes only
				break;
			case 1:		//Refill card
				RCard += (PPrice* static_cast<float> (Psold));
				break;
			case 2:		//Refill cash
				RCash += (PPrice* static_cast<float> (Psold));
				break;
			default:
				break;
			}
		}
	}

	// Bind the values required to be inserted into the audit table
	map<string, string> args_dataInsertion;
	QDateTime currentDateTime = QDateTime::currentDateTime();
	currentTS = currentDateTime.toTime_t();

	args_dataInsertion[":InitTimestamp"] = to_string(currentTS);
	args_dataInsertion[":CoinsinCashBox"] = to_string(TCoin);
	args_dataInsertion[":CoinsToTubes"] = to_string(CoinsToTubes);
	args_dataInsertion[":Cash"] = to_string(TCash);
	args_dataInsertion[":Cardsales"] = to_string(TCard);
	args_dataInsertion[":VMRefilled"] = to_string(RefillTimestamp);
	args_dataInsertion[":CoinsinCashBoxafterRefill"] = to_string(RCoin);
	args_dataInsertion[":CoinstoTubesafterRefill"] = to_string(RCoinsToTubes);
	args_dataInsertion[":CashafterRefill"] = to_string(RCash);
	args_dataInsertion[":cardsalesafterRefill"] = to_string(RCard);
	args_dataInsertion[":NoofReads"] = to_string(NoofReads);

	// Perform the update query onto the audit table
	if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_IDEXDatabaseCtn, processedAuditUpdQuery, args_dataInsertion))
	{
		cout << "[IDEX::IDEX_Update_AuditInfoTable] ERROR::Failure in Insertion Query" << endl;
	}

	VDBDeleteFlag = true;
}

/**
-------------------------------------------------------------------------------
CollectProductSalesRecord
-------------------------------------------------------------------------------
*   @brief		This function collects the vend information for product sales
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::CollectProductSalesRecord()
{

	const char *selectQuery = "Select * from product_vend_info";
	map<string, string> args_dataRetrieve;
	QList <QMap<QString, QVariant>>	resultRecds;

	if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, selectQuery, args_dataRetrieve, resultRecds))
	{
		cout << "ERROR :: [IDEX::CollectProductSalesRecord]:: Failed to execute select query" << endl;
		return;
	}

	// Check the results record
	if (resultRecds.size() != 0)
	{
		// Results contains the entries from the eventinfo table as per the query
		QList < QMap<QString, QVariant>>::iterator listIterate;
		QMap<QString, QVariant>::iterator mapIter;

		for (listIterate = resultRecds.begin(); listIterate != resultRecds.end(); listIterate++)
		{
			mapIter = listIterate->begin();
			QMap<QString, QVariant> &innerMap = (*listIterate);

			mapIter = listIterate->find("Product_ID");
			if (mapIter != listIterate->end())
			{
				ProductinfoIter1 = ProductMap1.find(mapIter.value().toString().toStdString());
				if (ProductinfoIter1 != ProductMap1.end())
				{					
					if (0 == innerMap["VendType"].toInt())
					{
						ProdInnerMapIter1 = ProductinfoIter1->second.find("Paidvendsales");
						QVariant currentSales = ProdInnerMapIter1->second.toInt() + innerMap["ProductSold"].toInt();
						ProductMap1[innerMap["Product_ID"].toString().toStdString()]["Paidvendsales"] = currentSales;

						ProdInnerMapIter1 = ProductinfoIter1->second.find("PaidvendAmount");
						QVariant currentSalesVal = static_cast<float> (ProdInnerMapIter1->second.toInt()) + 
							(innerMap["SalePrice"].toFloat() * static_cast<float> (innerMap["ProductSold"].toInt()));
						ProductMap1[innerMap["Product_ID"].toString().toStdString()]["PaidvendAmount"] = currentSalesVal;
					}
				}
				else
				{
					if (0 == innerMap["VendType"].toInt())
					{
						ProdInnerMap1.insert(map<std::string, QVariant>::value_type("Paidvendsales", innerMap["ProductSold"].toInt()));
						ProdInnerMap1.insert(map<std::string, QVariant>::value_type("PaidvendAmount", 
							(static_cast<float> (innerMap["ProductSold"].toInt()) * innerMap["SalePrice"].toFloat())));
					}

					ProdInnerMap1.insert(map<std::string, QVariant>::value_type("SalePrice", innerMap["SalePrice"].toFloat()));
					ProdInnerMap1.insert(map<std::string, QVariant>::value_type("VendType", innerMap["VendType"].toInt()));
					ProdInnerMap1.insert(map<std::string, QVariant>::value_type("ProductSold", innerMap["ProductSold"].toInt()));
					ProdInnerMap1.insert(map<std::string, QVariant>::value_type("PaymentSource", innerMap["PaymentSource"].toInt()));
					ProductMap1.insert(map<std::string, ProdInnerInfo1>::value_type(innerMap["Product_ID"].toString().toStdString(),
						ProdInnerMap1));
					ProdInnerMap1.clear();
				}
			}
		}	
	}

	return;
}

/**
-------------------------------------------------------------------------------
UpdateProductSalesRecord
-------------------------------------------------------------------------------
*   @brief		This function updates the vend information for product sales in salesInfo table
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::UpdateProductSalesRecord()
{
	QString ProductID;
	float PaidvendAmount = 0;
	int Paidvendsale = 0;

	/*for (ProductinfoIter1 = ProductMap1.begin(); ProductinfoIter1 != ProductMap1.end(); ProductinfoIter1++)
	{
		ProdInnerMap1 = ProductinfoIter1->second;
		std::cout << "#### " << ProductinfoIter1->first << " ----" << std::endl;
		for (ProdInnerMapIter1 = ProdInnerMap1.begin(); ProdInnerMapIter1 != ProdInnerMap1.end(); ProdInnerMapIter1++)
		{
			std::cout << ProdInnerMapIter1->first;
			std::cout << " : " << ProdInnerMapIter1->second.toInt() << endl;
		}
		cout << endl;
	}*/

	if (!ProductMap1.empty())
	{
		for (ProductinfoIter1 = ProductMap1.begin(); ProductinfoIter1 != ProductMap1.end(); ProductinfoIter1++)
		{
			ProductID = QString::fromStdString(ProductinfoIter1->first);
			ProdInnerMapIter1 = ProductinfoIter1->second.find("PaidvendAmount");
			PaidvendAmount = ProdInnerMapIter1->second.toFloat();

			ProdInnerMapIter1 = ProductinfoIter1->second.find("Paidvendsales");

			if (ProdInnerMapIter1 != ProdInnerMap1.end())
			{
				Paidvendsale = ProdInnerMapIter1->second.toInt();
				bool isExisting = false;

				const char *selectQuery = "SELECT Count(1) as TempCnt from `salesinfo` WHERE Product_ID = :ProductID";
				QList <QMap<QString, QVariant>>	resultRecds;

				// Bind the values required to be inserted into the audit table
				map<string, string> args_dataInsertion;
				args_dataInsertion[":ProductID"] = ProductID.toStdString();

				if (SELECT_QUERY_FAILURE == m_dbHandler->execSelectQuery(m_IDEXDatabaseCtn, selectQuery, args_dataInsertion, resultRecds))
				{
					cout << "ERROR :: [IDEX::IDEX_All_Transaction_Summary]:: Failed to execute select query" << endl;
					return;
				}

				// Check the results record
				if (resultRecds.size() != 0)
				{
					// Results contains the entries from the eventinfo table as per the query
					QList < QMap<QString, QVariant>>::iterator listIterate;
					QMap<QString, QVariant>::iterator mapIter;

					for (listIterate = resultRecds.begin(); listIterate != resultRecds.end(); listIterate++)
					{						
						for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
						{
							if (mapIter.value().toInt() > 0)
							{
								isExisting = true;
								break;
							}
						}
					}
				}

				// Insert and update salesinfo table
				InsertAndUpdateQuery(ProductID, Paidvendsale, PaidvendAmount, isExisting);
			}
		}
	}
}

/**
-------------------------------------------------------------------------------
InsertAndUpdateQuery
-------------------------------------------------------------------------------
*   @brief		This function performs the query for insertion and updation
*
*	@param		none
*
*	@return		none
*
*/
void IDEX::InsertAndUpdateQuery(QString ProductID, int Paidvendsale, float PaidvendAmount, bool isExisting)
{
	const char *insertQuery = "INSERT INTO `salesinfo` (`Product_ID`, `Paidvendsales`, `PaidvendvalueInit`) VALUES(:ProductID,:Paidvendsales,:PaidvendvalueInit)";
	const char *updateQuery = "UPDATE `salesinfo` SET Paidvendsales = Paidvendsales + :CurrentPaidvendsale, PaidvendvalueInit = PaidvendvalueInit + :CurrentProductprice WHERE Product_ID =:ProductID";

	if (!isExisting)
	{
		// Bind the values required to be inserted into the audit table
		map<string, string> args_dataInsertion;

		args_dataInsertion[":ProductID"] = ProductID.toStdString();
		args_dataInsertion[":Paidvendsales"] = to_string(Paidvendsale);
		args_dataInsertion[":PaidvendvalueInit"] = to_string(PaidvendAmount);

		// Perform the update query onto the salesInfo table
		if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_IDEXDatabaseCtn, insertQuery, args_dataInsertion))
		{
			cout << "[IDEX::InsertAndUpdateQuery] ERROR::Failure in Update Query" << endl;
		}
	}
	else
	{
		// Bind the values required to be inserted into the audit table
		map<string, string> args_dataInsertion;

		args_dataInsertion[":ProductID"] = ProductID.toStdString();
		args_dataInsertion[":CurrentPaidvendsale"] = to_string(Paidvendsale);
		args_dataInsertion[":CurrentProductprice"] = to_string(PaidvendAmount);

		// Perform the update query onto the salesInfo table
		if (UPDATE_QUERY_FAILURE == m_dbHandler->execUpdateQuery(m_IDEXDatabaseCtn, updateQuery, args_dataInsertion))
		{
			cout << "[IDEX::InsertAndUpdateQuery] ERROR::Failure in Update Query" << endl;
		}
	}
}

/* eof */
