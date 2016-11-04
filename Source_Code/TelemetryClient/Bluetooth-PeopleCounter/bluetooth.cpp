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
 *	@file		bluetooth.cpp
 *
 *	@brief		This file contains definition of all APIs which will be exported
 *				by the Blutooth module.
 *
 *
 */
/*
 * System headers
 */
#include <time.h>
#include <vector>
#include <sstream>
#include <string>
#include <map>
/*
 * Own headers
 */
#include "bluetooth.h"
#include "dataobject.h"
#include "DBHandler.h"
#include <map>
#ifndef UNIT_TEST
#include "ConfigurationReader.h"
#include <time.h>
#include <vector>
#endif

#ifdef UNIT_TEST
#include <iostream>
#include "common_ut.h"

#define QBluetoothDeviceInfo			mockQBluetoothDeviceInfo
#define	ConfigurationReader				mockConfigurationReader1
#endif

using namespace std;

#ifdef _WIN32
#define SNPRINTF	_snprintf_s
#elif __linux__
#define SNPRINTF	snprintf
#endif

static int      argc = 1;
static char    *argv[] = { NULL, NULL };
QString         tab = "\t";
DBHandler *m_dbHandler;

/**
 * @brief
 * This has useful methods such as pring message.
 */
namespace util {

	void print(const char *msg, ...)
	{
#ifndef UNIT_TEST
		va_list va_args;
		va_start(va_args, msg);
		vprintf(msg, va_args);
		va_end(va_args);
		fflush(stdout);
#endif
	}

	template < typename T > std::string to_string(const T & n) {
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
	
	size_t stringLength(const char *source) {
		if (source == NULL) {
			return 0;
		}

		size_t length = 0;
		while (*source != '\0') {
			length++;
			source++;
		}
		return length;
	}

	static string getHash(const char *source) {
		size_t length = stringLength(source);
		size_t hash = 0;
		for (size_t i = 0; i < length; i++) {
			char c = source[i];
			int a = c - '0';
			hash = (hash * 10) + a;
		}

		char str[256] = "";
		SNPRINTF(str, sizeof(str), "%zu", hash);

		return std::string(str);
	}
}


/**
 * @brief
 * Constructor method to create Bluetooth Scanner Object.
 */
#ifndef UNIT_TEST
Bluetooth::Bluetooth():localDevice(new QBluetoothLocalDevice)
#endif
#ifdef UNIT_TEST
Bluetooth::Bluetooth()
#endif
{
#ifndef UNIT_TEST
	QCoreApplication app(argc, argv);
	discoveryAgent = new QBluetoothDeviceDiscoveryAgent(localDevice->address());
	connect(discoveryAgent,  SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)), this,
			SLOT(printError(QBluetoothDeviceDiscoveryAgent::Error)));
	connect(discoveryAgent, SIGNAL(finished()), this,  SLOT(scanFinished()));
	connect(this, SIGNAL(finished()), this, SLOT(reScan()));
	discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);
#endif

	//Instantiate ConfigurationReader
	ConfigurationReader *configReader = ConfigurationReader::getInstance();
	acquitionFrequency = configReader->m_BLEAcquisitionFreq;

#ifndef UNIT_TEST
	db = QSqlDatabase::addDatabase("QMYSQL","BLE_DB_CONNECTION_NAME");
#endif


#ifdef UNIT_TEST
	if (QSqlDatabase::contains("BLE_DB_CONNECTION_NAME"))
	{
		db = QSqlDatabase::database("QMYSQL");
	}
	else
	{
		db = QSqlDatabase::addDatabase("QMYSQL", "BLE_DB_CONNECTION_NAME");
	}
#endif
	db.setDatabaseName(QString::fromStdString(configReader->m_DatabaseName));
	db.setHostName(QString::fromStdString(configReader->m_DatabaseHostName));
	db.setUserName(QString::fromStdString(configReader->m_DatabaseUserName));
	db.setPassword(QString::fromStdString(configReader->m_DatabasePassword));
	db.setPort(configReader->m_DatabasePort);

	if (!db.open()) {
		util::print("[BLUETOOTH] ERROR: Failed to open \"%s\" database\n", 
				configReader->m_DatabaseName.c_str());
#ifdef UNIT_TEST
		IsdbOpen = db.isOpen();
#endif
	} else {
		util::print("[BLUETOOTH] INFO: opened database:\"%s\" succesfully \n", configReader->m_DatabaseName.c_str());
		
#ifdef UNIT_TEST
		IsdbOpen = db.isOpen();
#endif
	}
m_dbHandler = DBHandler::getInstance();
//	printf("\nisOpen() - %d\n", db.isOpen());

#ifndef UNIT_TEST
	init();
	QTimer::singleShot(acquitionFrequency, this, SLOT(startDeviceDiscovery()));
	QCoreApplication::exec();
#endif
}

/**
 * @brief Destructor for bluetooth module which delete discoveryAgent,clear dataList and close the DB
 */
Bluetooth::~Bluetooth(void)
{
	if (db.isOpen()) {
		db.close();
	}
        qDeleteAll(dataList);
	deviceList.clear();
	delete discoveryAgent;
	delete localDevice;
        listOfDevices.clear();
util::print("[BLUETOOTH] Warning: Bluetooth Counter is Stopped! \n ");
}

#ifndef UNIT_TEST
/**
 * @brief Search Bluetooth devices using reScan at regular interval.
 */
void Bluetooth::reScan()
{
	qDebug() << "Rescan triggered..." << endl;
	QTimer::singleShot(acquitionFrequency, this, SLOT(startDeviceDiscovery()));

}
#endif

/**
 * @brief Search Bluetooth devices using QBluetoothDeviceDiscoveryAgent.
 */
void Bluetooth::startDeviceDiscovery()
{

	/*
	 * scanTime is used to check the updated timestamp
	 * for each device from tracked device list.
	 * if updated timestamp is older than scanTime then
	 * the device is not detected in new scan cycle.
	 * It will be moved to dataList to store in Sensor DB
	 */
	long long time_now = static_cast<long long> (time(NULL));
	scanTime =  QString::fromStdString(util::to_string(time_now));
#ifdef UNIT_TEST
	ScanTime = scanTime;
#endif
#ifndef UNIT_TEST
	if (discoveryAgent->isActive()) {
		util::print("[BLUETOOTH] INFO: waiting for current scan to finish...\n");
	} else {
		util::print("[BLUETOOTH] INFO: Scanning for Bluetooth devices ...\n");
		discoveryAgent->start();
	}
#endif
}


#ifndef UNIT_TEST
/**
  -------------------------------------------------------------------------------
  Bluetooth Init Method
  -------------------------------------------------------------------------------
 * @brief This function initialize bluetooth module by creating table in DB if not exist
 *
 *	@return			Refer this Enum @ref STATUS_CODE
 *
 */
STATUS_CODE Bluetooth::init()
{

string sql="CREATE TABLE IF NOT EXISTS Bluetooth_Counter(deviceId varchar(50), macAddress varchar(50),rssi INT,Start_Time BIGINT,End_Time BIGINT, deviceName varchar(1024), PRIMARY KEY (macAddress,Start_Time))";
	std::map<std::string, std::string> args_dataUpdation;
 					
	if (!db.isOpen())
	{
		util::print("[BLUETOOTH] ERROR: Failed to open \"%s\" database\n");
		return DB_FAILURE;
	}
	else
	{
		m_dbHandler->execUpdateQuery(QSqlDatabase::database(QString::fromStdString(BLE_DB_CONNECTION_NAME), true), sql, args_dataUpdation);
		
	}
	return SUCCESS;
}
#endif

/**
 * @brief This is SLOT method will be called once deviceDiscovered
 * SIGNAL generated from QBluetoothDeviceDiscoveryAgent.
 * @param device a QBluetoothDeviceInfo Object.
 */
void Bluetooth::addDevice(QBluetoothDeviceInfo  device)
{
	/*
	 * if discovered device already exist in deviceList then
	 * update the RSSI value for it in deviceList else
	 * add newly found device into deviceList.
	 */

	if (device.rssi() != 0) {
		time_t secs = time(NULL);   // get time now
		struct tm *curr_time_stamp = NULL;
		curr_time_stamp = localtime(&secs);
		char str[50] = {};
		snprintf(str, 50, "%d%d%d%s", (curr_time_stamp->tm_year + 1900), (curr_time_stamp->tm_mon + 1), (curr_time_stamp->tm_mday),device.address().toString().toStdString().c_str());
		string sHashedMac = util::getHash(str);

		//qDebug() << "Mac " << device.address().toString().toStdString().c_str() << " " << str << " generated hash " << " " << sHashedMac.c_str() << endl; 

		if (deviceList.find(sHashedMac.c_str()) != deviceList.end()) {
			DataObject     *temp = deviceList[sHashedMac.c_str()];
			temp->setTimestamp(scanTime);
			temp->setRssi(device.rssi());
			deviceList[sHashedMac.c_str()] = temp;
			util::print("[BLUETOOTH] INFO: Found InMem Existing Device: %s in this Cycle \n",device.name().toStdString().c_str() );

		} else {						
			DataObject *temps =  new DataObject(device.name(), QString(sHashedMac.c_str()), scanTime, device.rssi());
			deviceList.insert((std::make_pair(sHashedMac.c_str(), temps)));
			util::print("[BLUETOOTH] INFO: Found InMem new device:%s  in this Cycle \n",device.name().toStdString().c_str()  );

		}

	}
#ifdef UNIT_TEST
	devicelistsize = deviceList.size();
#endif
#ifndef UNIT_TEST
	/*
	 * This logic is written to fix  the QT Bluetooth defect (https://bugreports.qt.io/browse/QTBUG-51311 )
	 */
	QDBusConnection bus = QDBusConnection::systemBus();
	QDBusInterface  iface("org.bluez", "/org/bluez/hci0",
			"org.bluez.Adapter1", bus);
	QString  address = "/org/bluez/hci0/dev_" +
		device.address().toString().split(":").join("_");
	QDBusObjectPath path = QDBusObjectPath(address);
	QList < QVariant > argumentList;
	argumentList << qVariantFromValue(path);
	iface.asyncCallWithArgumentList(QLatin1String("RemoveDevice"),argumentList);
#endif
}

/**
 * @brief
 * This is SLOT method will be called if any error
 * SIGNAL generated from QBluetoothDeviceDiscoveryAgent.
 * @param error a enum QBluetoothDeviceDiscoveryAgent::Error.
 */
void Bluetooth::printError(QBluetoothDeviceDiscoveryAgent::Error error)
{
	util::print("[BLUETOOTH] ERROR: Found  Error in Scanning Cycle  \n" );
#ifndef UNIT_TEST
	qDebug()<<error;
	emit  finished();
#endif
	
}

/**
 * @brief
 * This is SLOT method will be called once scan cycle
 * is completed in QBluetoothDeviceDiscoveryAgent.
 */
void Bluetooth::scanFinished()
{
#ifndef UNIT_TEST
	// Get list of devices from current scan
	listOfDevices = discoveryAgent->discoveredDevices();
#endif
	// Get list of devices from previous scans
	string sql = "select * from Bluetooth_Counter";
	map<string, string> args_dataRetrieve; // As of now no arguments, included for future purpose

	map<string, string> args;
	QList <QMap<QString, QVariant> >	recs;	

	// Get list of Bluetooth records from the DB
	if (SELECT_QUERY_SUCCESS != m_dbHandler->execSelectQuery(QSqlDatabase::database(QString::fromStdString(BLE_DB_CONNECTION_NAME), true),sql, args, recs))
	{
		cout << "[Bluetooth::scanFinished] ERROR::Failure in Select Query" << endl;
		return; // TODO: What to do in this case?
	}

	qDebug() << "Found " << recs.count() << " existing. " << endl;
	// end of func
#ifndef UNIT_TEST
	// Add/update devices that were found in the scan
	for (int i = 0; i < listOfDevices.size(); i++) {
		addDevice(listOfDevices.at(i));
	}
#endif
	listOfDevices.clear();

	long long t = static_cast<long long> (time(NULL));
	time_t secs = time(NULL);   // get time now
	struct tm *curr_time_stamp = NULL;
	curr_time_stamp = localtime(&secs);
	
	// Traverse DB list over scanned deviceList since deviceList will be smaller
	// Update the DB with new device end times...
	QList < QMap<QString, QVariant> >::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
	{		
		QMap<QString, QVariant> map = (*listIterate);
		QString dbMacAddress = map.value("macAddress").toString();
		
			qDebug() << "DB searching for " << dbMacAddress << endl;

			if (deviceList.find(dbMacAddress) != deviceList.end())
			{				
				qDebug() << "Found existing device in DB... preparing to update " << endl;	
		
				sql = "update Bluetooth_Counter set End_Time=:ts where macAddress=:mac";
							std::map<std::string, std::string> args_dataUpdation;
						args_dataUpdation[":mac"] = dbMacAddress.toStdString();
 						args_dataUpdation[":ts"] = util::to_string(t);

				if (!m_dbHandler->execUpdateQuery(QSqlDatabase::database(QString::fromStdString(BLE_DB_CONNECTION_NAME), true), sql, args_dataUpdation))
				{
					// Dont care if we fail since we will send again anyway

					cout << "[Bluetooth::scanFinished] ERROR::Failure in Update query" << endl;	
			
				}
				else {

					qDebug() << dbMacAddress << " record was updated." << endl;

				}

				// erase from list so we don't duplicate in DB later
				deviceList.erase(dbMacAddress);

				qDebug() << "ERASED " << dbMacAddress << " " << deviceList.size() << endl;

			}
			else {

				qDebug() << "WARNING: Didn't find hashed mac " << dbMacAddress << endl;

			}
		//}
	}

	/* Insert new records found from scan.
	 * TODO: Remove below comment
	 * Check the updated timestamp for each device from tracked device list.
	 * if updated timestamp is older than scanTime then
	 * the device is not detected in new scan cycle.
	 * It will be moved to dataList to store in Sensor DB
	 */
	for (std::map < QString, DataObject * >::iterator iter = deviceList.begin(); iter != deviceList.end(); ++iter) {
		DataObject     *v = iter->second;
		//Not needed. Keep a live tally instead
		//if (v->timestamp() != scanTime) {
			dataList.push_back(new DataObject(v->name(), v->mac(), v->startTime(), v->rssi()));
			//util::print("[BLUETOOTH] INFO: Insert device into Sensor DB:  %s  \n",v->name().toStdString().c_str() );
			std::map < QString, DataObject * >::iterator toErase = iter;
			deviceList.erase(toErase);
		//}
	}

	sql =
		"INSERT INTO Bluetooth_Counter( macAddress, rssi, deviceId, Start_Time,End_Time, deviceName) VALUES (:MAC_ADDR,:RSSI, :SENSOR_ID, :Start_Time,:End_Time, :Device_Name )";

	/*
	 * below section is for Data insertion logic
	 */	

	/* get current date time to hash it with mac */


	// column name is as key and value to be inserted
	if (dataList.size() > 0) {

		qDebug() << "Inserting " << dataList.size() << " new devices in DB" << endl;

		for (unsigned int i = 0; i < dataList.size(); i++) {
		
						std::map<std::string, std::string> args_dataUpdation;

						args_dataUpdation[":RSSI"] =  util::to_string(dataList[i]->rssi());
 						args_dataUpdation[":MAC_ADDR"] = dataList[i]->mac().toStdString();
						args_dataUpdation[":SENSOR_ID"] = (localDevice->address().toString()).toStdString();
 						args_dataUpdation[":Start_Time"] =dataList[i]->startTime().toStdString();
				         args_dataUpdation[":End_Time"] = util::to_string(t);
 						args_dataUpdation[":Device_Name"] = dataList[i]->name().toStdString();
			

			qDebug() << "Inserting new record: " << dataList[i]->mac().toStdString().c_str() << endl;
		m_dbHandler->execUpdateQuery(QSqlDatabase::database(QString::fromStdString(BLE_DB_CONNECTION_NAME), true), sql, args_dataUpdation);
			//execInsertionQuery(sql, arg, 6);			

		}
		dataList.clear();
	}
#ifndef UNIT_TEST
	sql.clear();
	discoveryAgent->stop();
	util::print("[BLUETOOTH] INFO: Scanning finished.\n");
	emit finished();
#endif

}



