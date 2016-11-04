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
*	@file		MessageHandler.h
*
*	@brief
*		Abstract class for the Send and Receive Handler of Telemetry messages
*
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Intel Smart Shelf
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/


#ifndef TRANSACTION_H
#define TRANSACTION_H

#include<iostream>
#include<string>

#include <QtCore/qstring.h>				// QString header file
#include <QtCore/qfile.h>				// Qfile header file
#include <QtCore/qdebug.h>				// qdebug header file
#include <QtCore/qjsondocument.h>		// qjsondocument header file
#include <QtCore/qjsonobject.h>			// qjsonobject header file
#include <QtCore/qjsonarray.h>			// qjsonarray header file

#include<fstream>						// file stream header file

#include"datatranslator.h"
#include"PublishTableController.h"

using namespace std;

enum TRANSACTION_SUMMARY_STATUS_CODE
{
	READ_VMINFO_SUCCESS = 0,			///< Success
	READ_VMINFO_FAILURE,					///< Failed 
	PROCESS_ALL_TRANSACTION_SUCCESS,
	PROCESS_ALL_TRANSACTION_FAILURE,
	TRANSACTION_SUMMARY_SUCCESS,
	TRANSACTION_SUMMARY_FAILURE,
	STOCK_SUMMARY_SUCCESS,
	STOCK_SUMMARY_FAILURE
};

class Transaction
{

	typedef map<std::string, QVariant> ProdInnerInfo1;
	typedef map<std::string, ProdInnerInfo1> ProductInfo1;

public:

	/// Default constructor
	Transaction();

	/// Default destructor
	~Transaction();

	/// Write DEX data to a file
	void WriteToFile();

	/// To generate Transaction report
	//std::string Transaction_Summary(string);
	TRANSACTION_SUMMARY_STATUS_CODE Transaction_Summary(string JsonData);
	
	/// To process Transaction
	TRANSACTION_SUMMARY_STATUS_CODE Process_All_Transaction(string JsonData);

	/// To read vminfo table
	TRANSACTION_SUMMARY_STATUS_CODE Read_Vminfo_Table(string JsonData);

	/// To generate Stock report
	//string Stock_Summary(string JsonData);
	TRANSACTION_SUMMARY_STATUS_CODE Stock_Summary(string JsonData);

	/// To Convert integer to string
	QString InttoStringConvertion(int);

	/// To Convert double to string
	QString FloattoStringConvertion(double Value);
	
private:
	
	QString Comm_ID;

	ProdInnerInfo1 ProdInnerMap1;
	ProdInnerInfo1::const_iterator ProdInnerMapIter1;
	ProductInfo1 ProductMap1;
	ProductInfo1::const_iterator ProductinfoIter1;

};

#endif
