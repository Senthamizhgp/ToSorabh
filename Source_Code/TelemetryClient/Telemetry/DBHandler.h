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
*	@file		DBHandler.h
*
*	@brief
*		        This file contains declaration of all the data structures and
*				methods used for the Database manipulations
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

#ifndef DBHANDLER_H
#define DBHANDLER_H

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

using namespace std;

/// Return code for the Database operations
enum DB_STATUS_CODE
{
	SELECT_QUERY_SUCCESS,			///< Select Query success
	SELECT_QUERY_FAILURE,			///< Failed to select data from the database
	UPDATE_QUERY_SUCCESS,			///< Success in updating the database
	UPDATE_QUERY_FAILURE			///< Failed to update the database
};


class DBHandler
{
public:

	/*Variables*/	
	QMap<QString, QVariant> selectionMap;

	//Getting the current instance
	static DBHandler* getInstance();

	// Updating the database
	DB_STATUS_CODE  execUpdateQuery(QSqlDatabase connectionName, const string &strQuery, map<string, string> &arguments);

	// Selecting data from the database
	DB_STATUS_CODE execSelectQuery(QSqlDatabase connectionName, const string &strQuery, map<string, string> &arguments, QList<QMap<QString, QVariant> > &records);	
};

#endif //DBHANDLER_H