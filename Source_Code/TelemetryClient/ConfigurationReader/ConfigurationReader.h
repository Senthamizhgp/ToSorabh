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
*	@file		ConfigurationReader.h
*
*	@brief
*		Placeholder for the input configuration parameters.
*		It performs following actions:
*			- Parsing the input configuration file
*			- Holds the input configuration paramters
*			- Exports the configuration parameters for other libraries to utilize them
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

#ifndef CONFIGURATIONREADER_H
#define CONFIGURATIONREADER_H

#ifdef _WIN32
#define CONFIGURATIONREADER_API __declspec(dllexport) 
#elif __linux__
#define CONFIGURATIONREADER_API
#endif

#pragma once

#include <iostream> 
#include <map>

/* libxml headers */
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "libxml/xmlschemas.h"
#include "libxml/valid.h"

using namespace std;

/**
*  @brief ConfigurationReader
*	Parses the input configuration file and stores them. Exports the configuration parameters for other libraries to utilize them.
*/
class CONFIGURATIONREADER_API ConfigurationReader
{
	typedef std::map<std::string, std::map<std::string, std::string> > ConfigParameters;

public:
	/// Default constructor
	ConfigurationReader();

	/// Default destructor
	~ConfigurationReader();

	/// To obtain a singleton instance
	static ConfigurationReader* getInstance();

	/// For debugging purpose
	void printMap();

	//Structure to store the available sensor data
	struct SensorInfo
	{
		std::string sensorName;
		int sensorId;
		int deviceId;
		string sensorType;
		string sensorUnit;
		int sensorAcquisitionFrequency;
		int sensorFrequencyCount;
		int eventSensorFlag;
		SensorInfo():sensorName(""),sensorId(0),deviceId(0),sensorType(""),
			sensorUnit(""),sensorAcquisitionFrequency(0), sensorFrequencyCount(0), eventSensorFlag(0){}
	};

	unsigned short m_sensorEnable;					///< Holds whether the sensor Telemetry is enabled or not status
	string m_sensorJson;							///< Holds Json header for the Sensor Telemetry module
	unsigned short m_sensorExternalModuleEnable;	///< Holds Enablement for External Modules
	unsigned short m_sensorPlugin1Enable;			///< Holds Enablement indication for a plugin
	unsigned short m_sensorPlugin2Enable;			///< Holds Enablement indication for a plugin
	unsigned short m_sensorPlugin3Enable;			///< Holds Enablement indication for a plugin
	unsigned short m_PIREnable;						///< Holds Enablement indication for PIR sensor	
	string m_PIRLeftSensor;							///< Holds the PIR left sensor name
	string m_PIRRightSensor;						///< Holds the PIR right sensor name
	string m_PIRJson;								///< Holds Json header for the PIR module
	unsigned short m_PIRExternalModuleEnable;		///< Holds Enablement for External Modules
	unsigned short m_PIRPlugin1Enable;				///< Holds Enablement indication for a plugin
	unsigned short m_PIRPlugin2Enable;				///< Holds Enablement indication for a plugin
	unsigned short m_PIRPlugin3Enable;				///< Holds Enablement indication for a plugin
	unsigned short m_WiFiEnable;					///< Holds Enablement indication for WiFi
	unsigned short m_WiFiAcquisitionFreq;			///< Holds scanning frequency for the WiFi channel
	string m_WiFiDeviceID;							///< Holds the WiFi device ID
	signed short m_WiFiRSSI;						///< Holds the RSSI filter for the WiFi scanning
	string m_WiFiJson;								///< Holds Json header for the WiFi module
	unsigned short m_WiFiExternalModuleEnable;		///< Holds Enablement for External Modules
	unsigned short m_WiFiPlugin1Enable;				///< Holds Enablement indication for a plugin
	unsigned short m_WiFiPlugin2Enable;				///< Holds Enablement indication for a plugin
	unsigned short m_WiFiPlugin3Enable;				///< Holds Enablement indication for a plugin
	unsigned short m_BLEEnable;						///< Holds Enablement indication for BLE
	unsigned short m_BLEAcquisitionFreq;			///< Holds acquisition frequency for the BLE
	string m_BLEJson;								///< Holds Json header for the BLE module
	unsigned short m_BLEExternalModuleEnable;		///< Holds Enablement for External Modules
	unsigned short m_BLEPlugin1Enable;				///< Holds Enablement indication for a plugin
	unsigned short m_BLEPlugin2Enable;				///< Holds Enablement indication for a plugin
	unsigned short m_BLEPlugin3Enable;				///< Holds Enablement indication for a plugin
	unsigned short m_RealSenseEnable;				///< Holds Enablement indication for Real Sense
	unsigned short m_RealSenseDepthVal;				///< Holds Depth value for Real Sense
	unsigned short m_RealSenseEnableWindow;	  		///< Holds enable window for Real Sense
	string m_RealSenseJson;							///< Holds Json header for the RealSense module
	unsigned short m_RealSenseExternalModuleEnable;	///< Holds Enablement for External Modules
	unsigned short m_RealSensePlugin1Enable;		///< Holds Enablement indication for a plugin
	unsigned short m_RealSensePlugin2Enable;		///< Holds Enablement indication for a plugin
	unsigned short m_RealSensePlugin3Enable;		///< Holds Enablement indication for a plugin
	unsigned short m_TelemetryEnable;				///< Holds Enablement indication for Telemetry
	string m_DatabaseHostName;						///< Holds hostname for the database
	string m_DatabaseUserName;						///< Holds username for the database
	string m_DatabasePassword;						///< Holds password for the database
	string m_DatabaseName;							///< Holds name of the database
	unsigned short m_DatabasePort;					///< Holds the port to be used for the database connection
	unsigned short m_DatabaseAcquisitionFreq;		///< Holds the acquisition frequency for the Database
	unsigned short m_DatabasePurgingFreq;			///< Holds the purging frequency for the Database
	unsigned short m_AuditEnable;					///< Holds Enablement indication for Audit
	string m_VendingDatabaseHostName;				///< Holds hostname for the vending database
	string m_VendingDatabaseUserName;				///< Holds username for the vending database
	string m_VendingDatabasePassword;				///< Holds password for the vending database
	string m_VendingDatabaseName;					///< Holds name of the vending database
	unsigned short  m_VendingDatabasePort;			///< Holds the port to be used for the vending database
	string m_DEXJson;								///< Holds Json header for the DEX
	string m_TransactionJson;						///< Holds Json header for the Transaction
	string m_StockJson;								///< Holds Json header for the Stock
	unsigned short m_AuditExternalModuleEnable;		///< Holds Enablement for External Modules
	unsigned short m_DEXPluginEnable;			    ///< Holds Enablement indication for a plugin
	unsigned short m_AuditPlugin2Enable;			///< Holds Enablement indication for a plugin
	unsigned short m_AuditPlugin3Enable;			///< Holds Enablement indication for a plugin
	unsigned short m_mqttTLS;						///< Holds option deciding whether TLS should be enabled or not
	unsigned short m_mqttQoS;						///< Holds the level of QoS to be supported
	unsigned short m_ReportingFreq;					///< Holds reporting frequency of telemetry data to MQTT broker
	unsigned short m_sensorCount;					///< Holds the number of sensors
	SensorInfo m_sensorInfo[10];					///< Holds the Sensors information	
	string m_MQTTSendTopic;							///< Holds the Topic Name to be sent through MQTT
	string m_MQTTReceiveTopic;						///< Holds the Topic Name to which the MQTT Client subscribed
	string m_MQTTEndPoint;							///< Holds the Endpoint to which the MQTT Client needs to connect
	string m_deviceIdentifier;						///< Holds the NUC ID of the device


	/// Variables used during Initialization
	static SensorInfo sensorInfo[10];
	static int sensorCount;
	static unsigned short ReportingFreq;
	static string DeviceIdentifier;

private:

	/*Functions*/

	/// To validate the input xml
	bool validateXML();

	/// Perform the initialization
	void init();

	/// To traverse the XML tree
	static void traverseXMLTree(xmlNode *a_node);

	/// To Parse the input configuration xml
	static ConfigParameters parseXML();

	/*Variables*/
	static ConfigParameters m_configParameters;		///< To hold the input configuration parameters (as a map)	
};

#endif // CONFIGURATIONREADER_H
