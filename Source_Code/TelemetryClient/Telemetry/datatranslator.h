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
*	@file		datatranslatorr.h
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
*	The software supplied herewith by Intel's vendor to Intel for Intel Smart Shelf
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/


#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <list>
#include <typeinfo>
#include "qjsondocument.h"
#include <QtCore/QCoreApplication>
#include <qpair.h>
#include "qjsonobject.h"
#include "qjsonarray.h"
#include "qstring.h"
#include "qvariant.h"
#include "qmap.h"
#include "qdatetime.h"
#include <iostream>
#include <fstream>
#include <qfile.h>
#include <QFile>
#include <QTextStream> 
#include <QDebug>
#include "qglobal.h"
#include "PublishTableController.h"
using namespace std;


/**
*  @brief Data translator
*	Data translator is used to convert the data variables in to JSON string.
*/


class Datatranslator
{
public:

	/// Default constructor
	Datatranslator();  
	/// Default destructor
	~Datatranslator(); 
	/// To obtain a singleton instance
	static Datatranslator* getInstance();		     							    ///< Prototype of getInstance
	QJsonObject ObjectFromString(const QString& in)								    ///< Method to convert object from string
	{
		QJsonValue val(in);
		return val.toObject();
	}

	/* variables*/
	QMap<QString, QVariant> m_Protocol;										    ///<QMap for PIR protocol which holds the protocol details
	QMap<QString, QVariant> m_sensor_map;									    ///<QMap for PIR sensor 
	QMap<QString, QVariant> m_sufix;											    ///<QMap for sufix which hold the time stamp and device id
	QList < QMap<QString, QVariant>> m_listofmap;									///<Qlist Holds for list of map
	QMap<QString, QMap <QString, QVariant>> m_Protocol_outer;				    ///<QMap of Qmap for PIR sensor

	enum datatype_e { QString_e, int_e, uint_e, QDateTime_e, qlonglong_e, double_e, float_e, qulonglong_e };
	map<QString, datatype_e> eDatatype;
	enum sensor_name { PIR_e, dex_e,Bluetooth_e,wifi_e,trans_e,sense_t_e,realsense_e,stock_e};
	map<QString, sensor_name> eSensortype;
		
	void Reading_PIR_data();					            			///< method to read PIR related data other than sensor data
	void Reading_Dex_data();					            			///< method to read DEX related data other than sensor data
	void Reading_WIFI_data();					            			///< method to read WIFI related data other than sensor data
	void Reading_BLE_data();					            			///< method to read BLE related data other than sensor data
	void Reading_Trans_data();											///< method to read Transaction related data other than sensor data
	void Reading_stock_data();											///< method to read stock related data other than sensor data	
	void Reading_sens_T_data();
	void Reading_Real_Sens_data();
	void mapDatatype();											///< method to map for datatype with enumerator
	void mapSensorName();

	void sufix_and_prefix(string timedelimiter);		   				///< method to make the sufix and prefix which is comman for all the sensor
	std::string converting_Final_JSON(QString,QString,string);					///< method to convert PIR sensor data to Json string	
	string converting_sensor_data_JSON_Indvidual(QList<QMap <QString, QVariant>> test, string sensor);
	
	pair<QJsonArray, qlonglong> converting_dexdata_array(QList<QMap <QString, QVariant>> test);  ///< method to convert dex data to json array.
	bool ProcessUsingExternalPlugin(string dataToBeProcessed, string & processedString, string sensor);
	void converting_sensor_data_JSON_Indvidual_wifi_BLE(QList<QMap <QString, QVariant>>, string, pair<string, string> &check);
	string Dexdata_handler(QMap<QString, QJsonArray>);					/// < method to convert dexdata to json string
private:
	/*Variables*/
	static Datatranslator* instance;
	qlonglong  time_value;
	
};
