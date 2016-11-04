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
*	@file		DBHandler.cpp
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

#include "DBHandler.h"

/**
-------------------------------------------------------------------------------
getInstance
-------------------------------------------------------------------------------
*   @brief		To obtain the singleton instance
*
*	@param		none
*
*	@return		Instance of the DBHandler
*
*/
DBHandler* DBHandler::getInstance()
{
	static DBHandler dbHandlerInstance;
	return &dbHandlerInstance;
}

/**
-------------------------------------------------------------------------------
execUpdateQuery
-------------------------------------------------------------------------------
*   @brief					This function updates records in the database.
*
*	@param1	connectionName	The name of the connection to be used for updation
*	@param2 strQuery		Query to the database
*   @param3 arguments		Arguments to be passed for the query
*
*	@return					DB_STATUS_CODE
*
*/
DB_STATUS_CODE DBHandler::execUpdateQuery(QSqlDatabase connectionName,const string &strQuery,map<string, string> &arguments)
{
	QVariantMap args;
	QString key;
	QString value;
	QString sql = QString::fromStdString(strQuery);
	for (map<string, string>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter)
	{
		key = QString::fromStdString((*iter).first);
		value = QString::fromStdString((*iter).second);
		args.insert(key, value);
	}
	if (connectionName.isOpen())
	{
		QSqlQuery *query = new QSqlQuery(connectionName);
		/*
		* FROM QT DOCUMENTATION - Portability note: Some databases choose to
		* delay preparing a query until it is executed the first time. In this case,
		* preparing a syntactically wrong query succeeds, but every consecutive
		* exec() will fail. Hence the return for prepare() is not verified.
		*/
		query->prepare(sql);
		foreach(QString key_index, args.keys()) query->bindValue(key_index, args.value(key_index));
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
				connectionName.close();
				if (connectionName.open() && !query->exec())
				{
					std::cout << "[Update Query]::Failed to execute query with errorcode - "
						<< query->lastError().text().toStdString() << endl;
					delete query;
					return UPDATE_QUERY_FAILURE;
				}
			}
		}
		delete query;
		return UPDATE_QUERY_SUCCESS;
	}
	return UPDATE_QUERY_FAILURE;
}


/**
-------------------------------------------------------------------------------
execSelectQuery
-------------------------------------------------------------------------------
*   @brief					This function selects records from the database.
*
*	@param1 strQuery		Query to the database
*   @param2 arguments		Arguments to be passed for the query
*	@param3	records			List to hold the selected data
*
*	@return					DB_STATUS_CODE
*
*/
DB_STATUS_CODE DBHandler::execSelectQuery(QSqlDatabase connectionName, const string &strQuery, map<string, string> &arguments, QList<QMap<QString, QVariant> > &records)
{
	QVariantMap args;
	QString key;
	QVariant value;
	QString sql = QString::fromStdString(strQuery);
	records.clear();
	for (map<string, string>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter)
	{
		key = QString::fromStdString((*iter).first);
		value = QString::fromStdString((*iter).second);
		args.insert(key, value);

	}
	if (connectionName.isOpen())
	{
		QSqlQuery *query = new QSqlQuery(connectionName);
		/*
		* FROM QT DOCUMENTATION - Portability note: Some databases choose to
		* delay preparing a query until it is executed the first time. In this case,
		* preparing a syntactically wrong query succeeds, but every consecutive
		* exec() will fail. Hence the return for prepare() is not verified.
		*/
		query->prepare(sql);
		foreach(QString key_index, args.keys()) query->bindValue(key_index, args.value(key_index));
		if (query->exec())
		{
			while (query->next())
			{
				int count = query->record().count();
				for (int index = 0; index < count; index++)
				{
					key = query->record().field(index).name();
					value = query->record().field(index).value();
					selectionMap.insert(key, value);
				}
				records.push_back(selectionMap);
				selectionMap.clear();
			}
		}
		else
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
				connectionName.close();
				if (connectionName.open() && !query->exec())
				{
					std::cout << "[Select Query]::Failed to execute query with errorcode - "
						<< query->lastError().text().toStdString() << endl;
					delete query;
					return SELECT_QUERY_FAILURE;
				}
				else
				{
					/* Process the query results obtained after calling execute query
					*  once again after reconnection */
					while (query->next())
					{
						int count = query->record().count();
						for (int index = 0; index < count; index++)
						{
							key = query->record().field(index).name();
							value = query->record().field(index).value();
							selectionMap.insert(key, value);
						}
						records.push_back(selectionMap);
						selectionMap.clear();
					}
				}
			}
		}

		delete query;
		return SELECT_QUERY_SUCCESS;
	}

	// Return failure incase of open fails
	cout << "[Select Query]:: Failed to open database connection" << connectionName.databaseName().toStdString() << endl;
	return SELECT_QUERY_FAILURE;
}
