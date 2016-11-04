
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
 *	@file		dbconnector.cpp
 *
 *	@brief		This file contains definition of all APIs used to
 *	                connect to the MariaDB database and insert records
 *	                by the WiFi People Counter module.
 *
 */

#include <errno.h>

#include "QtSql/qsql.h"
#include "QtSql/qsqlquery.h"
#include "QtSql/qsqlerror.h"
#include "QtCore/qdebug.h"
#include "ConfigurationReader.h"
#include "DBHandler.h"
#include "dbconnector.h"

#define MAX_STR	512

QSqlDatabase wifi_db = QSqlDatabase::addDatabase(QString::fromStdString("QMYSQL") , QString::fromStdString("wifi_connection"));

DBHandler *db_handler;

static int32_t run_query(char *my_query)
{
	printf("DB status=%d\n", wifi_db.isOpen());

	if(my_query)
	{
		map<string, string> dummy;
		if(db_handler->execUpdateQuery(wifi_db, my_query, dummy) == UPDATE_QUERY_FAILURE)
			return -1;
	}
	else
		return -EINVAL;
	return 0;
}

/**
 * @brief Create the connection to a existing database
 *
 * @return 0 if success
 */
int32_t init_database_connector(void)
{
	char query1[MAX_STR];

	/* Initialize configuration reader to read WiFi config parameters and db settings */
	ConfigurationReader *configReader = ConfigurationReader::getInstance();

	/* Connect to db and create table, if doesn't exists */
	wifi_db.setDatabaseName(QString::fromStdString(configReader->m_DatabaseName));
	wifi_db.setHostName(QString::fromStdString(configReader->m_DatabaseHostName));
	wifi_db.setUserName(QString::fromStdString(configReader->m_DatabaseUserName));
	wifi_db.setPassword(QString::fromStdString(configReader->m_DatabasePassword));
	wifi_db.setPort(configReader->m_DatabasePort);

	memset(query1, 0, sizeof(query1));

	if( !wifi_db.open() )
	{
		qDebug() << wifi_db.lastError();
		qDebug( "WiFi:Failed to open DB.\n" );
		return -1;
	}

	db_handler = DBHandler::getInstance();

	//qDebug( "Connected!" );

	snprintf(query1 , MAX_STR, "CREATE TABLE IF NOT EXISTS WiFi_Counter(macAddress varchar(50), deviceId varchar(50), channel INT, rssi INT, Start_Time BIGINT UNSIGNED, End_Time BIGINT UNSIGNED, PRIMARY KEY (macAddress,End_Time))");

	return(run_query(query1));
}

/**
 * @brief Close the connection to a existing database
 */
void free_database_connector(void)
{
	if( wifi_db.isOpen() )
		wifi_db.close();
}

/**
* @brief Fowler/Noll/Vo (FNV) hash function, variant 1a
*
* @param cp charp to be hashed
*
* @return hashed value
*/
static size_t db_fnv1a_hash(const uint8_t* cp)
{
    size_t hash = 0x811c9dc5;
    while (*cp) {
        hash ^= *cp++;
        hash *= 0x01000193;
    }
    return hash;
}

/**
 * @brief Get current date of the system
 *
 * @param nuc_mac output buffer with mac address e.g aa:bb:cc:dd:ee:ff
 *
 * @return 0 for success
 */

int get_date(uint8_t* date, uint32_t len)
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	if(!date)
		return -EINVAL;

	snprintf((char*)date, len, "%d-%d-%d", (now->tm_year + 1900), (now->tm_mon + 1), now->tm_mday);
	return 0;
}

/**
 * @brief Insert data into wifi people counter table of the database
 *
 * @param record data to be added
 *
 * @return 0 for success
 */
int32_t add_record_database(wifi_user_record_data_t * record)
{
	char query2[MAX_STR];

	uint8_t mac_date[MAX_STR];

	memset(query2, '\0', sizeof(query2));
	memset(mac_date, '\0', sizeof(mac_date));

	get_date(mac_date, MAX_STR);

	strcat((char*)mac_date, (const char*) record->mac);

	snprintf(query2, MAX_STR, "INSERT INTO WiFi_Counter VALUES(%lu,\'%s\',%d, %d, %ld, %ld)",
			db_fnv1a_hash(mac_date), record->ap_mac, record->channel, record->signal, (long)record->t_start, (long)record->t_end);

	//printf("Query for %s (%ld) :%s\n",record->mac, (long)record->t_end, query2);

	return run_query(query2);
}
