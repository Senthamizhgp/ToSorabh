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
*	@file		Datatranslator.cpp
*
*	@brief
*		 Converts the given data to JSON
*		It performs following actions:
*			- packaging of given array of data to json string
*   		- different methods are provided to convert for different sensors.
*
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Intel Smart Shelf
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/


#include "datatranslator.h"
#include "ConfigurationReader.h"



using namespace std;

/**
-------------------------------------------------------------------------------
mapDatatype()
-------------------------------------------------------------------------------
*   @brief	:  To equate the datatype for the enumerator
*
*	@param	: ********
*
*	@return	: Handle for object in the method.
*
*/
void Datatranslator::mapDatatype()
{
	eDatatype["QString"] = QString_e;
	eDatatype["int"] = int_e;
	eDatatype["uint"] = uint_e;
	eDatatype["QDateTime"] = QDateTime_e;
	eDatatype["qlonglong"] = qlonglong_e;
	eDatatype["double"] = double_e;
	eDatatype["float"] = float_e;
	eDatatype["qulonglong"] = qulonglong_e;
}

/**
-------------------------------------------------------------------------------
mapSensorName()
-------------------------------------------------------------------------------
*   @brief	:  To equate the datatype for the enumerator
*	@param	: ********
*
*	@return	: Handle for object in the method.
*
*/
void Datatranslator::mapSensorName()
{
	// PIR_e, dex_e,Bluetooth_e,wifi_e,trans_e,sense_t_e,realsense_e,stock_e
	eSensortype["PIR"] = PIR_e;
	eSensortype["Dex"] = dex_e;
	eSensortype["BLE"] = Bluetooth_e;
	eSensortype["WIFI"] = wifi_e;
	eSensortype["Trans"] = trans_e;
	eSensortype["Sens_T"] = sense_t_e;
	eSensortype["RealSens"] = realsense_e;
	eSensortype["stock"] = stock_e;
}

// Global and Static initializations

/**
-------------------------------------------------------------------------------
Get Instance method
-------------------------------------------------------------------------------
*   @brief	:  To get Instance of Datatranslator file
*
*	@param	: ********
*
*	@return	: Handle for object in the method.
*
*/

Datatranslator* Datatranslator::getInstance()
{
	static Datatranslator  dataTranslatorInstance;
	return &dataTranslatorInstance;	
}


/**
-------------------------------------------------------------------------------
constructure for Datatranslator
-------------------------------------------------------------------------------
*   @brief	:  Constructure for Data translator
*
*	@param	: ********
*
*	@return	: ********
*
*/

Datatranslator::Datatranslator()
{
	// Constructure method

	// Perform the registration of the sensor name against the enum type
	mapSensorName();

	// Perform the registration of the datatype against the enum type
	mapDatatype();
}


/*------------------------------------------------------------------------------ -
constructure for Datatranslator
------------------------------------------------------------------------------ -
*@brief	:  Constructure for Data translator
*
*	@param : ********
*
*	@return	: ********
*
**/


Datatranslator::~Datatranslator()
{
	// deconstruction method
}


/**
-------------------------------------------------------------------------------
Reading_DEX_data
-------------------------------------------------------------------------------
*   @brief		Handling the Dex data from the map to Json string
*
*	@param		*********
*
*	@return		return the JSON string of the Map
*
*/

string Datatranslator::Dexdata_handler(QMap<QString,QJsonArray> innerdata)
{
	/*converting PIR to JSON string */
	QJsonObject  jsonObjectFirst;
	string finalJson;
	QMap<QString, QJsonArray>::iterator local;

	for (local = innerdata.begin();local != innerdata.end(); local++)							///< Iterating sufix and prefix for updating time stamp and device id's
	{
		QJsonArray value = local.value();
		jsonObjectFirst.insert(local.key(), value);
	}
	finalJson = QJsonDocument(jsonObjectFirst).toJson(QJsonDocument::Compact).toStdString();
	
	return finalJson;
}

/**
-------------------------------------------------------------------------------
converting_PIR_JSON
-------------------------------------------------------------------------------
*   @brief		Converting data to JSON string
*
*	@param		passing the publish data to string.
*
*	@return		A complete json string to be sent to server.
*
*/

string Datatranslator::converting_Final_JSON(QString innerdata, QString sensor_detail, string timedelimiter)
{
#if 0
	/*converting PIR to JSON string */
	QJsonObject  jsonObjectFirst, jsonObjectSecond,jsonObjectthird;
	string finalJson;
	QMap<QString, QVariant>::iterator mapIter;								    ///<QMap iterator
	QMap<QString, QMap <QString, QVariant>>::iterator protocoliter;			    ///<QMap of Qmap iterator
	
	PublishTableController * Instance_individual = PublishTableController::getInstance();
	QString deviceid;
	QString datareceived = innerdata;

	QJsonDocument doc = QJsonDocument::fromJson(datareceived.toUtf8());
	jsonObjectthird = doc.object();
	QString deviceidentifier = Instance_individual->m_deviceidentifier.c_str();
	switch (eSensortype[sensor_detail])
	{

	case PIR_e:
		jsonObjectFirst = Instance_individual->m_outer_pir_configure;
		deviceid = "98000";
		jsonObjectthird.insert("sensorName", "IR People Tracker");
		jsonObjectthird.insert("sensorType", "PIR");
		jsonObjectthird.insert("deviceid", deviceid);
		jsonObjectFirst.insert("deviceidentifier", deviceidentifier);
		jsonObjectFirst.insert("value", jsonObjectthird);
		jsonObjectFirst.insert("datetime", timedelimiter.c_str());
		break;

	case Bluetooth_e:
		jsonObjectFirst = Instance_individual->m_outer_ble_configure;
		jsonObjectthird.insert("sensorName", "BLE people Counter");
		jsonObjectthird.insert("sensorType", "BLEScanner");
		jsonObjectFirst.insert("deviceidentifier", deviceidentifier);
		jsonObjectFirst.insert("value", jsonObjectthird);
		jsonObjectFirst.insert("datetime", timedelimiter.c_str());
		break;

	case wifi_e:
		jsonObjectFirst = Instance_individual->m_outer_wifi_configure;
		jsonObjectthird.insert("sensorName", "WiFi People Counter");
		jsonObjectthird.insert("sensorType", "WiFIScanner");
		jsonObjectFirst.insert("deviceidentifier", deviceidentifier);
		jsonObjectFirst.insert("value", jsonObjectthird);
		jsonObjectFirst.insert("datetime", timedelimiter.c_str());
		break;

	case realsense_e:
		jsonObjectFirst = Instance_individual->m_outer_Realsense_configure;
		deviceid = "99000";
		jsonObjectthird.insert("sensorName", "RealSensePeopleTracker");
		jsonObjectthird.insert("sensorType", "RealSense");
		jsonObjectthird.insert("deviceid", deviceid);
		jsonObjectFirst.insert("deviceidentifier", deviceidentifier);
		jsonObjectFirst.insert("value", jsonObjectthird);
		jsonObjectFirst.insert("datetime", timedelimiter.c_str());
		break;

	case sense_t_e:
		jsonObjectFirst = Instance_individual->m_outer_sens_t_configure;
		jsonObjectFirst.insert("deviceidentifier", deviceidentifier);
		jsonObjectFirst.insert("value", jsonObjectthird);
		jsonObjectFirst.insert("datetime", timedelimiter.c_str());
		break;

		
	case dex_e:
		jsonObjectFirst = Instance_individual->m_outer_dex_configure;
		jsonObjectFirst.insert("deviceidentifier", deviceidentifier);
		jsonObjectFirst.insert("value", jsonObjectthird);
		jsonObjectFirst.insert("datetime", timedelimiter.c_str());
		break;

	case trans_e:
		jsonObjectFirst = Instance_individual->m_outer_Trans_configure;
		jsonObjectFirst.insert("deviceidentifier", deviceidentifier);
		jsonObjectFirst.insert("value", jsonObjectthird);
		jsonObjectFirst.insert("datetime", timedelimiter.c_str());
		break;

	case stock_e:
		jsonObjectFirst = Instance_individual->m_outer_stock_configure;
		jsonObjectFirst.insert("deviceidentifier", deviceidentifier);
		jsonObjectFirst.insert("value", jsonObjectthird);
		jsonObjectFirst.insert("datetime", timedelimiter.c_str());
		break;

	default:
		break;

	}
	
	for (mapIter = m_sufix.begin(); mapIter != m_sufix.end(); mapIter++)							///< Iterating sufix and prefix for updating time stamp and device id's
	{																							
		QString dataTypename = mapIter.value().typeName();
		if (dataTypename == "QString")
		{
			QString value = mapIter.value().toString();
			jsonObjectFirst.insert(mapIter.key(), value);
		}
		if (dataTypename == "int")
		{
			int value = mapIter.value().toInt();
			jsonObjectFirst.insert(mapIter.key(), value);
		}
	}
	finalJson = QJsonDocument(jsonObjectFirst).toJson(QJsonDocument::Compact).toStdString();					// creates a Json document / Json String.
	//cout << "finalstring: " <<finalJson << endl;
	return finalJson;
#endif
	return "TEST";
}


/**
-------------------------------------------------------------------------------
converting_sensor_data_JSON_Indvidual(QList<QMap <QString, QVariant>> test)

-------------------------------------------------------------------------------
*   @brief		Converting data to JSON string
*
*	@param		passing the publish data to string.
*
*	@return		A complete json string to be sent to server.
*
*/

string Datatranslator::converting_sensor_data_JSON_Indvidual(QList<QMap <QString, QVariant>> listOfMap, string sensor)
{
#if 0 
	QJsonObject  jsonObjectFirst;
	string sensorJSONData;
	QJsonArray array;
	qlonglong time_value_long;
	string time_value_check;
	QString dataTypename;

	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;

	for (listIterate = listOfMap.begin(); listIterate != listOfMap.end(); listIterate++)
	{
		for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)							 // To push sensor identifier and type in to object.
		{
			 dataTypename = mapIter.value().typeName();
			 switch (eDatatype[dataTypename])
			 {

			 case QString_e:
			{
				QString value = mapIter.value().toString();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			 break;

			 case int_e:
			{
				int value = mapIter.value().toInt();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			 break;
			 case uint_e:
			{
				int value = mapIter.value().toInt();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			 break;

			 case QDateTime_e:
			{
				QString value = mapIter.value().toString();
				jsonObjectFirst.insert(mapIter.key(), value);
				
			}
			 break;
			 case qlonglong_e:
			{
				qlonglong value = mapIter.value().toLongLong();
				jsonObjectFirst.insert(mapIter.key(), value);
				 if (mapIter.key() == "Start_Time" || mapIter.key() == "date_time" || mapIter.key() == "timestamp")
				{
					time_value_long = mapIter.value().toLongLong();
				//	time_value = time_value1.toString().toStdString();
					QString time_value2 = QString::number(time_value_long);
					time_value_check = time_value2.toStdString();
				}
						
			}
			 break;

			 case double_e:
			{
				double value = mapIter.value().toDouble();
				jsonObjectFirst.insert(mapIter.key(), value);


			}
			 break;
			 case float_e:
			{
				float value = mapIter.value().toFloat();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			 break;
			 default:
			 break;
			 }

		}
		array << jsonObjectFirst;
		sensorJSONData = QJsonDocument(jsonObjectFirst).toJson(QJsonDocument::Compact).toStdString();

		// Pre process the Telemetry data with the external plugins if enabled any before pushing 
		// publish table
		string processedSensorJSONData;
		ProcessUsingExternalPlugin(sensorJSONData, processedSensorJSONData, sensor);

		PublishTableController * Instance_individual = PublishTableController::getInstance();
		Instance_individual->PublishTableInsertion(processedSensorJSONData, sensor);
	}

	return time_value_check;
#endif 
	return "SAMPLE";
}

/**
-------------------------------------------------------------------------------
converting_sensor_data_JSON_Indvidual_wifi_BLE

-------------------------------------------------------------------------------
*   @brief		Converting WIFI and BLE data to JSON string
*
*	@param		passing the publish data to string.
*
*	@return		A complete json string to be sent to server.
*
*/
void  Datatranslator::converting_sensor_data_JSON_Indvidual_wifi_BLE(QList<QMap <QString, QVariant>> listOfMap, string sensor, pair<string, string> &send_receive)
 {
#if 0
	QJsonObject  jsonObjectFirst;
	string sensorJSONData;
	QJsonArray array;
	qlonglong time_value_long;
	qulonglong timeValueULong;

	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;

	for (listIterate = listOfMap.begin(); listIterate != listOfMap.end(); listIterate++)
	{
		for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)							 // To push sensor identifier and type in to object.
		{
			QString dataTypename = mapIter.value().typeName();
			//cout << dataTypename.toStdString();
			
			switch (eDatatype[dataTypename])
			{

			case QString_e:
			{
				QString value = mapIter.value().toString();
				jsonObjectFirst.insert(mapIter.key(), value);
				if (mapIter.key() == "macAddress")
				{
					send_receive.first = mapIter.value().toString().toStdString();
				}				
			}
			break;
			case int_e:
			{
				int value = mapIter.value().toInt();
				jsonObjectFirst.insert(mapIter.key(), value);
				
				if (mapIter.key() == "macAddress")
				{
					send_receive.first = mapIter.value().toString().toStdString();
				}
			}
			break;

			case uint_e:
			{
				int value = mapIter.value().toInt();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			break;


			case QDateTime_e:
			{
				QString value = mapIter.value().toString();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			break;

			case qlonglong_e:
			{
				qlonglong value = mapIter.value().toLongLong();
				jsonObjectFirst.insert(mapIter.key(), value);
				if (mapIter.key() == "End_Time" || mapIter.key() == "date_time" || mapIter.key() == "timestamp")
				{
					time_value_long = mapIter.value().toLongLong();
					QString time_value2 = QString::number(time_value_long);
					send_receive.second = time_value2.toStdString();
				}
				if (mapIter.key() == "macAddress")
				{
					send_receive.first = mapIter.value().toString().toStdString();
				}			

			}
			break;

			case qulonglong_e:
			{
				qlonglong value = mapIter.value().toLongLong();
				jsonObjectFirst.insert(mapIter.key(), value);
				if (mapIter.key() == "End_Time" || mapIter.key() == "date_time" || mapIter.key() == "timestamp")
				{
					timeValueULong = mapIter.value().toULongLong();
					QString timeValue = QString::number(timeValueULong);
					send_receive.second = timeValue.toStdString();
				}
				if (mapIter.key() == "macAddress")
				{
					send_receive.first = mapIter.value().toString().toStdString();
				}			

			}
			break;

			case double_e:
			{
				double value = mapIter.value().toDouble();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			break;

			case float_e:
			{
				float value = mapIter.value().toFloat();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			break;

			}
		}
		array << jsonObjectFirst;
		sensorJSONData = QJsonDocument(jsonObjectFirst).toJson(QJsonDocument::Compact).toStdString();

		// Pre process the Telemetry data with the external plugins if enabled any before pushing 
		// publish table
		string processedSensorJSONData;
		ProcessUsingExternalPlugin(sensorJSONData, processedSensorJSONData, sensor);

		PublishTableController * Instance_individual_wifi_BLE = PublishTableController::getInstance();
		Instance_individual_wifi_BLE->PublishTableInsertion(processedSensorJSONData, sensor, send_receive);

	}
	//return send_receive;
#endif
}

/**
-------------------------------------------------------------------------------
converting_dexdata_array

-------------------------------------------------------------------------------
*   @brief		Converting Dex data to object array
*
*	@param
*
*	@return	   json object array
*
*/
pair<QJsonArray, qlonglong> Datatranslator::converting_dexdata_array(QList<QMap <QString, QVariant>> listOfMap)
{

	QJsonObject  jsonObjectFirst;
	QJsonArray array;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;

	for (listIterate = listOfMap.begin(); listIterate != listOfMap.end(); listIterate++)
	{
		for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)							 // To push sensor identifier and type in to object.
		{
			QString dataTypename = mapIter.value().typeName();
			switch (eDatatype[dataTypename])
			{

			case QString_e:
			{
				QString value = mapIter.value().toString();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			break;

			case int_e:
			{
				int value = mapIter.value().toInt();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			break;

			case uint_e:
			{
				int value = mapIter.value().toUInt();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			break;
			case QDateTime_e:
			{
				QString value = mapIter.value().toString();
				jsonObjectFirst.insert(mapIter.key(), value);

			}
			break;

			case qlonglong_e:
			{
				qlonglong value = mapIter.value().toLongLong();
				jsonObjectFirst.insert(mapIter.key(), value);
				if (mapIter.key() == "Start_Time" || mapIter.key() == "date_time" || mapIter.key() == "Timestamp")
				{
					time_value = mapIter.value().toLongLong();
				}
			}
			break;

			case qulonglong_e:
			{
				// Do nothing.. To pacify the compiler warning
			}
			break;

			case double_e:
			{
				double value = mapIter.value().toDouble();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			break;

			case float_e:
			{
				float value = mapIter.value().toFloat();
				jsonObjectFirst.insert(mapIter.key(), value);
			}
			break;
			}
		}
		array << jsonObjectFirst;
	}
		
	return pair<QJsonArray, qlonglong>(array, time_value);
}


/**
-------------------------------------------------------------------------------
ProcessUsingExternalPlugin

-------------------------------------------------------------------------------
*   @brief		Process the Telemetry data against the external plugins enabled before
*				filling it in the publish table
*
*	@param1 dataToBeProcessed	Data to be processed by external plugin
*   @param2 processedString		Data processed after being processed by external plugin
*	@param3 sensor				Type of the data to be processed PIR, BLE etc...
*
*	@return	   true in case of success else false
*
*/
bool Datatranslator::ProcessUsingExternalPlugin(string dataToBeProcessed, string &processedString, string sensor)
{
	if (dataToBeProcessed.empty())
	{
		cout << " Data to be processed is empty" << endl;
		return false;
	}

	// Make a copy of the data to be processed by external plugin
	processedString = dataToBeProcessed;	
	
	// Obtain the instance of the configuration reader
	ConfigurationReader *configReader = ConfigurationReader::getInstance();

	// Process the data based on the external plugins enabled
	switch (eSensortype[QString::fromStdString(sensor)])
	{
		case PIR_e:
		{
			if (configReader->m_PIRExternalModuleEnable)
			{
				// The JSON data is processed based on the external plugins enabled for further processing			
				if (configReader->m_PIRPlugin1Enable)
					cout << "Processing PIR's first External processing unit !!!" << endl;

				// Second External process unit operate on JSON data
				if (configReader->m_PIRPlugin2Enable)
					cout << "Processing PIR's second External processing unit !!!" << endl;

				// Third External process unit operate on JSON data
				if (configReader->m_PIRPlugin3Enable)
					cout << "Processing PIR's Third External processing unit !!!" << endl;
			}
		}
		break;

		case Bluetooth_e:
		{
			if (configReader->m_BLEExternalModuleEnable)
			{
				// The JSON data is processed based on the external plugins enabled for further processing			
				if (configReader->m_BLEPlugin1Enable)
					cout << "Processing BLE's first External processing unit !!!" << endl;

				// Second External process unit operate on JSON data
				if (configReader->m_BLEPlugin2Enable)
					cout << "Processing BLE's second External processing unit !!!" << endl;

				// Third External process unit operate on JSON data
				if (configReader->m_BLEPlugin3Enable)
					cout << "Processing BLE's Third External processing unit !!!" << endl;
			}
		}
		break;

		case wifi_e:
		{
			if (configReader->m_WiFiExternalModuleEnable)
			{
				// The JSON data is processed based on the external plugins enabled for further processing			
				if (configReader->m_WiFiPlugin1Enable)
					cout << "Processing Wifi's first External processing unit !!!" << endl;

				// Second External process unit operate on JSON data
				if (configReader->m_WiFiPlugin2Enable)
					cout << "Processing Wifi's second External processing unit !!!" << endl;

				// Third External process unit operate on JSON data
				if (configReader->m_WiFiPlugin3Enable)
					cout << "Processing Wifi's Third External processing unit !!!" << endl;
			}
		}
		break;

		case realsense_e:
		{
			if (configReader->m_RealSenseExternalModuleEnable)
			{
				// The JSON data is processed based on the external plugins enabled for further processing			
				if (configReader->m_RealSensePlugin1Enable)
					cout << "Processing Realsense's first External processing unit !!!" << endl;

				// Second External process unit operate on JSON data
				if (configReader->m_RealSensePlugin2Enable)
					cout << "Processing Realsense's second External processing unit !!!" << endl;

				// Third External process unit operate on JSON data
				if (configReader->m_RealSensePlugin3Enable)
					cout << "Processing Realsense's Third External processing unit !!!" << endl;
			}
		}
		break;

		case sense_t_e:
		{
			if (configReader->m_sensorExternalModuleEnable)
			{
				// The JSON data is processed based on the external plugins enabled for further processing			
				if (configReader->m_sensorPlugin1Enable)
					cout << "Processing SensorTelemetry's first External processing unit !!!" << endl;

				// Second External process unit operate on JSON data
				if (configReader->m_sensorPlugin2Enable)
					cout << "Processing SensorTelemetry's second External processing unit !!!" << endl;

				// Third External process unit operate on JSON data
				if (configReader->m_sensorPlugin3Enable)
					cout << "Processing SensorTelemetry's Third External processing unit !!!" << endl;
			}
		}
		break;

		case dex_e:
		{
			if (configReader->m_AuditExternalModuleEnable)
			{
				// The JSON data is processed based on the external plugins enabled for further processing			
				if (configReader->m_DEXPluginEnable)
					cout << "Processing Dex's first External processing unit !!!" << endl;

				// Second External process unit operate on JSON data
				if (configReader->m_AuditPlugin2Enable)
					cout << "Processing Dex's second External processing unit !!!" << endl;

				// Third External process unit operate on JSON data
				if (configReader->m_AuditPlugin3Enable)
					cout << "Processing Dex's Third External processing unit !!!" << endl;
			}
		}
		break;

		case trans_e:
		{
			// do nothing
		}
		break;

		case stock_e:
		{
			// do nothing
		}
		break;

		default:
		{
			cout << "Unknown Telemetry data type !!!" << endl;
		}
		break;

	}

	return true;
}
