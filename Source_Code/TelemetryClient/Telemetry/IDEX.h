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
*	@file		IDEX.h
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

#ifndef IDEX_H
#define IDEX_H

/* System headers */
#include <iostream>
#include <string>
#include <ctime>

/* Qt headers for QtSql Database */
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqldriver.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlfield.h>
#include <QtSql/qsqlindex.h>
#include <QtSql/qsqlquery.h>
#include <QtSql/qsqlrecord.h>
#include <QtSql/qsqlresult.h>
#include <QtCore/qstring.h>

using namespace std;

class ConfigurationReader;
class DBHandler;

enum IDEX_DB_STATUS_CODE
{
	DB_OPEN_SUCCESS = 0,			///< Success
	IDEX_CONNECTION_FAILURE			///< Failed to open the main connection	
};

class IDEX
{
	typedef map<std::string, QVariant> ProdInnerInfo;
	typedef map<int, ProdInnerInfo> ProductInfo;

	typedef map<std::string, QVariant> ProdInnerInfo1;
	typedef map<std::string, ProdInnerInfo1> ProductInfo1;

public:
	
	/// Default constructor
	IDEX();

	/// Default destructor
	~IDEX();

	/// To obtian instance
	static IDEX* getInstance();

	/// To get database connection
//	static QSqlDatabase getConnection(string DatabaseName, string UserName, string Password);
	static IDEX_DB_STATUS_CODE getConnection(string HostName, string DatabaseName, string UserName, string Password, int Port);

	/// To initiate IDEX function
	void IDEX_Init();

	/// To read information from vminfo table
	void IDEX_Read_VmInfoTable();

	/// To read information from cashinInfo table
	void IDEX_Read_CashinInfoTable();

	/// Initial update of Auditinfo table
	void IDEX_Initial_Update_AuditInfoTable();

	/// To read information from event table
	void IDEX_Read_EventTable();

	/// To process information from product_vend_info table
	void IDEX_All_Transaction_Summary();

	/// To update processed audit information in auditinfo table
	void IDEX_Update_AuditInfoTable();

	/// To create individual tables required for DEX information
	bool TableCreation(const string &creationQuery, const string &deletionQuery, bool isDropRequired);

	/// Return the connection status
	bool ConnectionStatus();

	/// IDEX process to initiate other IDEX functions
	void IDEX_Process();

	/// IDEX process to product sales
	void UpdateProductSalesRecord();

	/// IDEX process to collect product sales details
	void CollectProductSalesRecord();
	
	/// Performs the query for insertion and updation
	void InsertAndUpdateQuery(QString ProductId, int ProductSalesCnt, float PaidvendAmount, bool isExisting);

private:

	/// Create the stored procedure event in the sensor database
	bool SPEventCreation();

	/// This function to check if there are entries in product vend info
	bool CheckforNewTransactionEntries();

	/// Clears the audit info variables
	void ClearMemory();

	/*variables*/
	static QSqlDatabase m_IDEXDatabaseCtn;
	ConfigurationReader *m_configReader;
	DBHandler *m_dbHandler;

	int PaidVendCount;
	int FreeVendCount;
	int TestVendCount;
	float PaidVendValueInit;
	float FreeVendValueInit;
	float TestVendValueInit;

	int InitTimestamp;
	int RefillTimestamp;
	int NoofReads;
	float TCoin;
	float CoinsToTubes;
	float TCash;
	float TCard;
	float RCoin;
	float RCoinsToTubes;
	float RCash;
	float RCard;
	float TRecycler;

	static bool IDEXConnectionStatus;
		
	ProdInnerInfo ProdInnerMap;
	ProdInnerInfo::const_iterator ProdInnerMapIter;
	ProductInfo ProductMap;
	ProductInfo::const_iterator ProductinfoIter;

	ProdInnerInfo1 ProdInnerMap1;
	ProdInnerInfo1::const_iterator ProdInnerMapIter1;
	ProductInfo1 ProductMap1;
	ProductInfo1::const_iterator ProductinfoIter1;

};
#endif
