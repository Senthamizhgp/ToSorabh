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
*	@file		DexConverter.h
*
*	@brief
*		To converts sales, stock and event information to DEX file
*		It performs following actions:
*			- Check for DEX processing unit is enabled
*			- If enabled converts sales, stock and event information to DEX file format
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Intel Smart Shelf
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

#ifndef DEXCONVERTER_H
#define DEXCONVERTER_H

#ifdef _WIN32
#define DEXCONVERTER_API __declspec(dllexport)
#else
#define DEXCONVERTER_API
#endif

#include<iostream>
#include<string>
#include<iomanip>
#include<sstream>
/* libxml headers */
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "libxml/xmlschemas.h"
#include "libxml/valid.h"

/*QT headers */
#include <QtCore/qstring.h>				// QString header file
#include <QtCore/qfile.h>				// Qfile header file
#include <QtCore/qdebug.h>				// qdebug header file
#include <QtCore/qjsondocument.h>		// qjsondocument header file
#include <QtCore/qjsonobject.h>			// qjsonobject header file
#include <QtCore/qjsonarray.h>			// qjsonarray header file
#include <QtCore/qdatetime.h>

#include<fstream>						// file stream header file

using namespace std;

/**
*  @brief DexConverter
*	If DEX processing unit is enabled, it converts sales, stock and event information to DEX file
*/

class DexConverter
{

	typedef map<std::string, QVariant> ProdInnerInfo1;
	typedef map<std::string, ProdInnerInfo1> ProductInfo1;

public:

	/// Default constructor
	DEXCONVERTER_API DexConverter();	

	/// Default destructor
	DEXCONVERTER_API ~DexConverter();	

	/// To obtain a singleton instance
	static DEXCONVERTER_API DexConverter* getInstance();

	/// Write DEX data to a file
	void WriteToFile();

	/// Manipulate the CRC value
	unsigned short crc16(unsigned short crc, unsigned char data);
	
	/// Obtain the CRC for the DEX data
	void calculateCRC(char *data);

	/// All Transaction Summary
	void All_Transaction_Summary(string jsondata);

	/// To clear variables
	void Clear_Memory();

	/// To Convert integer to string
	QString IntToStringConvertion(int);

	/// To Parse JSON string and to convert JSON string to DEX file
	QString DEXCONVERTER_API DexFileGenerater(string jsondata);
	
	/// To parse DEX config parameters
	void DEXConfigParametersXML();

	/// To traverse the DEX XML tree
	void traverseDEX_XMLTree(xmlNode *a_node);
	
	/// To print DEX XML config parameters (For Debugging purpose) 
	void printMap();

	/// To read vminfo json object
	void ReadvminfoObject(QJsonObject);

	/// To read eventinfo json object
	void ReadeventinfoObject(QJsonObject);

	/// To read productvendinfo json object
	void ReadproductvendinfoObject(QJsonObject);

	/// To read the vmDeviceInfo json object
	void ReadVmDeviceInfo(QJsonObject);

	/// To read the auditinfo json object
	void ReadAuditInfo(QJsonObject);

	/// To read the cashinInfo json object
	void ReadCashInInfo(QJsonObject);

	/// To read the ConfigurationData json object
	void ReadConfigurationData(QJsonObject);

	/// To read the CoinMechanismInfo json object
	void ReadCoinMechanismInfo(QJsonObject);

	/// To calculate Record Integrity Check
	void RecordIntegrityCheck();

	/// To calculate no of included sets
	void NoofIncludedSets();

	/// DEX trailer
	void DexTrailer();

	/// Blockidentifier Cash Out
	void DexCashOutflow();

	/// Blockidentifier for Number of Reads
	void NoofReads();

	/// Blockidentifier for value in Tubes
	void Dexvalueintubes();

	/// Blockidentifier cashless sale
	void DexCashlesssalesinfo();

	/// Blockidentifier Product info and sales
	void DexProductinfosales(QJsonObject jsonObject);

	/// Blockidentifier vend sales
	void DexVendsales(QJsonObject);

	/// Blockidentifier Token sales
	void DexTokensales();

	/// Blockidentifier Cash In
	void DexCashInflow();

	int ReadSalesInfo(QJsonObject, QString);

	double ReadSalesInfoPaidAmtInit(QJsonObject, QString);

	void BlockIdentifierLS();
	void BlockIdentifierLE();
	void PriceList();
	void RecentSale();
	void getUTCTime(time_t timeToBeConverted, QString &date, QString &time);

private:

	unsigned short crc;
	string CRCqstringtostr ;  ///> To hold the CRC for the DEX data
	QString delimiter;
	QString dexBuffer;
	QString CRCBuffer;
	QString Tofile;
	int RecentSaleTimeStamp;
	int LastSaleProductSelectionCnt;
	float CoinsToTubes;
	float CoinsInCashBox;
	float BillsInCashBox;
	
	int CountLine;
	int PaidVendCount;
	int FreeVendCount;
	int TestVendCount;
	int PaidVendCountRefill;
	int FreeVendCountRefill;
	int TestVendCountRefill;
	int TCardSalesCountInit;
	int TCashSalesCountInit;
	int TCoinSalesCountInit;
	int TCoinSalesCountRefill;
	int TCardSalesCountRefill;
	int TCashSalesCountRefill;
	int RefillTimeUTC;
	
	int ReadCount;
	int AuditTableCoinsInCashBoxInit;
	int AuditTableCoinsToTubesInit;
	int AuditTableCashSalesInit;
	int AuditTableCardSalesInit;

	float PaidVendValueRefill;
	float FreeVendValueRefill;
	float TestVendValueRefill;
	float PaidVendValueInit;
	float FreeVendValueInit;
	float TestVendValueInit;
	float TotalBillsValueInit;
	float TotalCoinsValueInit;
	float TotalCashlessValueInit;
	float TotalCoinsValueRefill;
	float TotalCashlessValueRefill;
	float TotalBillsValueRefill;

	float TotalTokenValueInit;
	float TTokenSalesCountInit;
	float TTokenSalesCountRefill;
	float TotalTokenValueRefill;

	static DexConverter *instance;

	ProdInnerInfo1 ProdInnerMap1;
	ProdInnerInfo1::const_iterator ProdInnerMapIter1;
	ProductInfo1 ProductMap1;
	ProductInfo1::const_iterator ProductinfoIter1;

};

#endif
