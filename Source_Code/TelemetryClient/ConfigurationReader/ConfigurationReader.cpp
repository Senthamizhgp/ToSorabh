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
*	@file		ConfigurationReader.cpp
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

/* System headers */
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

/* Own headers */
#include "ConfigurationReader.h"

#ifdef _WIN32
#define VSPRINTF	vsprintf_s
#elif __linux__
#define VSPRINTF	vsprintf
#endif

/* Global and Static initializations */
const char* CONFIGURATION_XML_PATH = "./TelemetryConfiguration.xml";
const char* CONFIGURATION_XSD_PATH = "./TelemetryConfiguration.xsd";
unsigned short ConfigurationReader::ReportingFreq = 0;
ConfigurationReader::SensorInfo ConfigurationReader::sensorInfo[10];
int ConfigurationReader::sensorCount = 0;
string ConfigurationReader::DeviceIdentifier;
std::map<std::string, std::map<std::string, std::string>>m_configParams;
ConfigurationReader::ConfigParameters ConfigurationReader::m_configParameters = ConfigurationReader::parseXML();
static int m_parameterFlag = 0;
static string m_parameterName = "";
string moduleName;

/**
-------------------------------------------------------------------------------
ConfigurationReader
-------------------------------------------------------------------------------
*   @brief		Constructor initializing the member variables and required inits.
*
*	@param		none
*
*	@return		None
*
*/
ConfigurationReader::ConfigurationReader() :
	m_sensorEnable(0),
	m_sensorJson(""),
	m_sensorExternalModuleEnable(0),
	m_sensorPlugin1Enable(0),
	m_sensorPlugin2Enable(0),
	m_sensorPlugin3Enable(0),
	m_PIREnable(0),
	m_PIRJson(""),
	m_PIRExternalModuleEnable(0),
	m_PIRPlugin1Enable(0),
	m_PIRPlugin2Enable(0),
	m_PIRPlugin3Enable(0),
	m_WiFiEnable(0),
	m_WiFiAcquisitionFreq(0),
	m_WiFiDeviceID(""),
	m_WiFiRSSI(0),
	m_WiFiJson(""),
	m_WiFiExternalModuleEnable(0),
	m_WiFiPlugin1Enable(0),
	m_WiFiPlugin2Enable(0),
	m_WiFiPlugin3Enable(0),
	m_BLEEnable(0),
	m_BLEAcquisitionFreq(0),
	m_BLEJson(""),
	m_BLEExternalModuleEnable(0),
	m_BLEPlugin1Enable(0),
	m_BLEPlugin2Enable(0),
	m_BLEPlugin3Enable(0),
	m_RealSenseEnable(0),
	m_RealSenseDepthVal(0),
	m_RealSenseEnableWindow(false),
	m_RealSenseJson(""),
	m_RealSenseExternalModuleEnable(0),
	m_RealSensePlugin1Enable(0),
	m_RealSensePlugin2Enable(0),
	m_RealSensePlugin3Enable(0),
	m_TelemetryEnable(0),
	m_DatabaseHostName(""),
	m_DatabaseUserName(""),
	m_DatabasePassword(""),
	m_DatabaseName(""),
	m_DatabasePort(0),
	m_DatabaseAcquisitionFreq(0),
	m_DatabasePurgingFreq(0),
	m_AuditEnable(0),
	m_VendingDatabaseHostName(""),
	m_VendingDatabaseUserName(""),
	m_VendingDatabasePassword(""),
	m_VendingDatabaseName(""),
	m_VendingDatabasePort(0),
	m_DEXJson(""),
	m_TransactionJson(""),
	m_StockJson(""),
	m_AuditExternalModuleEnable(0),
	m_DEXPluginEnable(0),
	m_AuditPlugin2Enable(0),
	m_AuditPlugin3Enable(0),
	m_mqttTLS(0),
	m_mqttQoS(0),
	m_ReportingFreq(0),
	m_sensorCount(0),
	m_MQTTSendTopic(""),
	m_MQTTReceiveTopic(""),
	m_MQTTEndPoint(""),
	m_deviceIdentifier("")
{
	// Define all the class initialization	
	// Initialized only at the first invocation of ctor
	static bool mapInit = false;

	// True only for the first instance of ConfigurationReader
	if (true != mapInit)
	{
		mapInit = true;

		// Validate the input XML file against the schema defined
		// and Initialize all the input config parameters

		if (true == validateXML())
		{
			// Initialize all the input configuration parameters
			init();
		}
		else
		{
			cout << "Failure : Input configuration doesn't match with the schema" << endl;
			exit(EXIT_FAILURE);
		}
	}
}

/**
-------------------------------------------------------------------------------
~ConfigurationReader
-------------------------------------------------------------------------------
*   @brief		Destructor performing the cleanup like deleting allocated memory, dereferencing etc..
*
*	@param		none
*
*	@return		None
*
*/
ConfigurationReader::~ConfigurationReader()
{
	// Define all the code cleanup and memory delete 
}

/**
-------------------------------------------------------------------------------
getInstance
-------------------------------------------------------------------------------
*   @brief		To obtain the singleton instance
*
*	@param		none
*
*	@return		Instance of the ConfigurationReader
*
*/
ConfigurationReader* ConfigurationReader::getInstance()
{
	static ConfigurationReader configInstance;
	return &configInstance;
}

/**
-------------------------------------------------------------------------------
ValidationErrorHandler
-------------------------------------------------------------------------------
*   @brief		This function is an event handler that handler that
*				handles the error thrown during XML validation.
*
*	@param		Error format and types
*
*	@return		None
*
*/
void ValidationErrorHandler(void *ctx, const char *format, ...)
{
	(void)ctx; // To pacify the static code analyser warning
	char errMsg[256];
	va_list args;
	va_start(args, format);
	VSPRINTF(errMsg, format, args);
	perror(errMsg);
	va_end(args);
	fprintf(stderr, "Validation Error: %s", errMsg);
}

/**
-------------------------------------------------------------------------------
validateXML
-------------------------------------------------------------------------------
*   @brief		To validate the input configuration xml against the schema defined
*
*	@param		none
*
*	@return		1 on successful validation and 0 on unsuccessful validation
*
*/
bool ConfigurationReader::validateXML()
{
	int result = 0;
	xmlSchemaParserCtxtPtr parserCtxt = NULL;
	xmlSchemaPtr schema = NULL;
	xmlSchemaValidCtxtPtr validCtxt = NULL;

	parserCtxt = xmlSchemaNewParserCtxt(CONFIGURATION_XSD_PATH);
	if (parserCtxt == NULL) {
		fprintf(stderr, "XSD file is not present.\n");
		return false;
	}

	schema = xmlSchemaParse(parserCtxt);
	if (schema == NULL) {
		fprintf(stderr, "Could not parse XSD schema.\n");
		goto leave;
	}

	validCtxt = xmlSchemaNewValidCtxt(schema);
	if (!validCtxt) {
		fprintf(stderr, "Could not create XSD schema validation context.\n");
		goto leave;
	}

	xmlSetStructuredErrorFunc(NULL, NULL);
	xmlSetGenericErrorFunc(NULL, ValidationErrorHandler);
	xmlThrDefSetStructuredErrorFunc(NULL, NULL);
	xmlThrDefSetGenericErrorFunc(NULL, ValidationErrorHandler);

	result = xmlSchemaValidateFile(validCtxt, CONFIGURATION_XML_PATH, 0);

leave:
	if (parserCtxt) {
		xmlSchemaFreeParserCtxt(parserCtxt);
	}

	if (schema) {
		xmlSchemaFree(schema);
	}

	if (validCtxt) {
		xmlSchemaFreeValidCtxt(validCtxt);
	}
	printf("\n");
	printf("Validation successful: %s \n", (result == 0) ? "YES" : "NO");

	if (!result)
		return 1;
	else
		return 0;
}

/**
-------------------------------------------------------------------------------
Init
-------------------------------------------------------------------------------
*   @brief		To initialize the configuration parameters with the input values read from input configuration xml
*
*	@param		none
*
*	@return		None
*
*/
void ConfigurationReader::init()
{
	// Initialize all the configuration parameters	
	m_sensorEnable = std::stoi(m_configParameters["SensorTelemetry"]["enable"]);
	m_sensorJson = m_configParameters["SensorTelemetry"]["json_header"];
	m_sensorExternalModuleEnable = std::stoi(m_configParameters["SensorTelemetry"]["External Processing Unit"]);
	m_sensorPlugin1Enable = std::stoi(m_configParameters["SensorTelemetry"]["External Plugin 1"]);
	m_sensorPlugin2Enable = std::stoi(m_configParameters["SensorTelemetry"]["External Plugin 2"]);
	m_sensorPlugin3Enable = std::stoi(m_configParameters["SensorTelemetry"]["External Plugin 3"]);
	m_PIREnable = std::stoi(m_configParameters["PIR"]["enable"]);
	m_PIRLeftSensor = m_configParams["PIR"]["left_sensor_name"];
	m_PIRRightSensor = m_configParams["PIR"]["right_sensor_name"];
	m_PIRJson = m_configParameters["PIR"]["json_header"];
	m_PIRExternalModuleEnable = std::stoi(m_configParameters["PIR"]["External Processing Unit"]);
	m_PIRPlugin1Enable = std::stoi(m_configParameters["PIR"]["External Plugin 1"]);
	m_PIRPlugin2Enable = std::stoi(m_configParameters["PIR"]["External Plugin 2"]);
	m_PIRPlugin3Enable = std::stoi(m_configParameters["PIR"]["External Plugin 3"]);
	m_WiFiEnable = std::stoi(m_configParameters["WiFi"]["enable"]);
	m_WiFiAcquisitionFreq = std::stoi(m_configParameters["WiFi"]["acquisitionfreq"]);
	m_WiFiDeviceID = m_configParameters["WiFi"]["device"];
	m_WiFiRSSI = std::stoi(m_configParameters["WiFi"]["rssi"]);
	m_WiFiJson = m_configParameters["WiFi"]["json_header"];
	m_WiFiExternalModuleEnable = std::stoi(m_configParameters["WiFi"]["External Processing Unit"]);
	m_WiFiPlugin1Enable = std::stoi(m_configParameters["WiFi"]["External Plugin 1"]);
	m_WiFiPlugin2Enable = std::stoi(m_configParameters["WiFi"]["External Plugin 2"]);
	m_WiFiPlugin3Enable = std::stoi(m_configParameters["WiFi"]["External Plugin 3"]);
	m_BLEEnable = std::stoi(m_configParameters["BT"]["enable"]);
	m_BLEAcquisitionFreq = std::stoi(m_configParameters["BT"]["acquisitionfreq"]);
	m_BLEJson = m_configParameters["BT"]["json_header"];
	m_BLEExternalModuleEnable = std::stoi(m_configParameters["BT"]["External Processing Unit"]);
	m_BLEPlugin1Enable = std::stoi(m_configParameters["BT"]["External Plugin 1"]);
	m_BLEPlugin2Enable = std::stoi(m_configParameters["BT"]["External Plugin 2"]);
	m_BLEPlugin3Enable = std::stoi(m_configParameters["BT"]["External Plugin 3"]);
	m_RealSenseEnable = std::stoi(m_configParameters["RealSense"]["enable"]);
	m_RealSenseJson = m_configParameters["RealSense"]["json_header"];
	m_RealSenseDepthVal = std::stoi(m_configParameters["RealSense"]["depthValue"]);
	m_RealSenseEnableWindow = std::stoi(m_configParameters["RealSense"]["enableWindow"]);
	m_RealSenseExternalModuleEnable = std::stoi(m_configParameters["RealSense"]["External Processing Unit"]);
	m_RealSensePlugin1Enable = std::stoi(m_configParameters["RealSense"]["External Plugin 1"]);
	m_RealSensePlugin2Enable = std::stoi(m_configParameters["RealSense"]["External Plugin 2"]);
	m_RealSensePlugin3Enable = std::stoi(m_configParameters["RealSense"]["External Plugin 3"]);
	m_TelemetryEnable = std::stoi(m_configParameters["Telemetry"]["enable"]);
	m_DatabaseHostName = m_configParameters["Telemetry"]["hostname"];
	m_DatabaseUserName = m_configParameters["Telemetry"]["username"];
	m_DatabasePassword = m_configParameters["Telemetry"]["password"];
	m_DatabaseName = m_configParameters["Telemetry"]["databasename"];
	m_DatabasePort = std::stoi(m_configParameters["Telemetry"]["port"]);
	m_DatabaseAcquisitionFreq = std::stoi(m_configParameters["Database"]["globalacquisitionfreq"]);
	m_DatabasePurgingFreq = std::stoi(m_configParameters["Database"]["purgingfreq"]);
	m_AuditEnable = std::stoi(m_configParameters["Audit"]["enable"]);
	m_VendingDatabaseHostName = m_configParameters["Audit"]["hostname"];
	m_VendingDatabaseUserName = m_configParameters["Audit"]["username"];
	m_VendingDatabasePassword = m_configParameters["Audit"]["password"];
	m_VendingDatabaseName = m_configParameters["Audit"]["databasename"];
	m_VendingDatabasePort = std::stoi(m_configParameters["Audit"]["port"]);
	m_DEXJson = m_configParameters["Audit"]["DEX"];
	m_TransactionJson = m_configParameters["Audit"]["Transaction"];
	m_StockJson = m_configParameters["Audit"]["Stock"];
	m_AuditExternalModuleEnable = std::stoi(m_configParameters["Audit"]["External Processing Unit"]);
	m_DEXPluginEnable = std::stoi(m_configParameters["Audit"]["DEX Converter"]);
	m_AuditPlugin2Enable = std::stoi(m_configParameters["Audit"]["External Plugin 2"]);
	m_AuditPlugin3Enable = std::stoi(m_configParameters["Audit"]["External Plugin 3"]);
	m_mqttTLS = std::stoi(m_configParameters["MQTT"]["TLS"]);
	m_mqttQoS = std::stoi(m_configParameters["MQTT"]["QoS"]);
	m_MQTTSendTopic = m_configParameters["MQTT"]["SendTopic"];
	m_MQTTReceiveTopic = m_configParameters["MQTT"]["ReceiveTopic"];
	m_MQTTEndPoint = m_configParameters["MQTT"]["endPoint"];
	m_ReportingFreq = ReportingFreq;
	m_deviceIdentifier = DeviceIdentifier;

	for (int it = 0; it < sensorCount; it++)
	{
		m_sensorInfo[it] = sensorInfo[it];
	}
	m_sensorCount = sensorCount;
}

/**
-------------------------------------------------------------------------------
TraverseXMLTree
-------------------------------------------------------------------------------
*   @brief		To traverse the XML tree
*
*	@param		The XML node
*
*	@return		None
*
*/
void ConfigurationReader::traverseXMLTree(xmlNode * a_node)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE && !xmlStrcmp(cur_node->name, (const xmlChar *)"config_parameter"))
		{
			m_parameterFlag = 1;

			m_parameterName = (char*)xmlGetProp(cur_node, (const xmlChar*)"type");

			if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"enable"))
			{
				m_configParams[m_parameterName]["enable"] = "1";
			}
			else if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"disable"))
			{
				m_configParams[m_parameterName]["enable"] = "0";
			}

			if (m_parameterName == "PIR")
			{
				// Obtain the left and right sensor name for the PIR sensor
				m_configParams[m_parameterName]["left_sensor_name"] 
					= (char*)xmlGetProp(cur_node, (const xmlChar*)"left_sensor_name");
				m_configParams[m_parameterName]["right_sensor_name"] 
					= (char*)xmlGetProp(cur_node, (const xmlChar*)"right_sensor_name");
			}
		}
		else if (cur_node->type == XML_ELEMENT_NODE && !xmlStrcmp(cur_node->name, (const xmlChar *)"external_module"))
		{
			moduleName = (char*)xmlGetProp(cur_node, (const xmlChar*)"type");

			if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"enable"))
			{
				m_configParams[m_parameterName][moduleName] = "1";
			}
			else if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"disable"))
			{
				m_configParams[m_parameterName][moduleName] = "0";
			}
		}
		else if (cur_node->type == XML_ELEMENT_NODE && !xmlStrcmp(cur_node->name, (const xmlChar *)"plugin") && m_parameterFlag == 1)
		{
			moduleName = (char*)xmlGetProp(cur_node, (const xmlChar*)"plugin_name");

			if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"enable"))
			{
				m_configParams[m_parameterName][moduleName] = "1";
			}
			else if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"disable"))
			{
				m_configParams[m_parameterName][moduleName] = "0";
			}
		}
		else if (cur_node->type == XML_ELEMENT_NODE && m_parameterFlag == 1)
		{
			if (!xmlStrcmp(cur_node->name, (const xmlChar*)"sub_type"))
			{
				m_parameterName = (char*)xmlGetProp(cur_node, (const xmlChar *)"type");

				if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"enable"))
				{
					m_configParams[m_parameterName]["enable"] = "1";
				}
				else if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"disable"))
				{
					m_configParams[m_parameterName]["enable"] = "0";
				}
			}
			else if (m_parameterName == "SensorTelemetry" && !xmlStrcmp(cur_node->name, (const xmlChar *)"sensoracquisitionfreq"))
			{
				int countAttr = 0;
				for (xmlAttrPtr attr = cur_node->properties; attr != NULL; attr = attr->next)
				{
					countAttr++;
				}
				sensorInfo[sensorCount].sensorName = (char*)xmlGetProp(cur_node, (const xmlChar *)"sensor_name");
				if (countAttr == 2)
				{
					sensorInfo[sensorCount].sensorAcquisitionFrequency = std::stoi((char*)xmlGetProp(cur_node, (const xmlChar *)"acquisitionfreq"));
				}
				else if (countAttr == 1)
				{
					sensorInfo[sensorCount].sensorAcquisitionFrequency = std::stoi(m_configParams["SensorTelemetry"]["globalacquisitionfreq"]);
				}
				sensorInfo[sensorCount].eventSensorFlag = 0;
				sensorCount++;
			}
			else if (m_parameterName == "SensorTelemetry" && !xmlStrcmp(cur_node->name, (const xmlChar *)"event"))
			{
				sensorInfo[sensorCount].sensorName = (char*)xmlGetProp(cur_node, (const xmlChar*)"sensor_name");
				sensorInfo[sensorCount].eventSensorFlag = 1;
				sensorCount++;
			}
			else
			{
				if (m_parameterName == "Audit" && !xmlStrcmp(cur_node->name, (const xmlChar *)"json_header"))
				{
					m_configParams[m_parameterName][(char*)xmlGetProp(cur_node, (const xmlChar*)"audit_report_type")] =
						(char*)xmlNodeGetContent(cur_node);
				}
				else
				{
					m_configParams[m_parameterName][(char*)cur_node->name] = (char*)xmlNodeGetContent(cur_node);
				}
			}
		}
		else if (cur_node->type == XML_ELEMENT_NODE && !xmlStrcmp(cur_node->name, (const xmlChar*)"reportingfreq"))
		{
			ReportingFreq = std::stoi((char*)xmlNodeGetContent(cur_node));
		}
		else if (cur_node->type == XML_ELEMENT_NODE && !xmlStrcmp(cur_node->name, (const xmlChar*)"deviceidentifier"))
		{
			DeviceIdentifier = (char*)xmlNodeGetContent(cur_node);
		}
		//Traversing the child node
		ConfigurationReader::traverseXMLTree(cur_node->children);
		if (cur_node->type == XML_ELEMENT_NODE && !xmlStrcmp(cur_node->name, (const xmlChar *)"config_parameter"))
		{
			m_parameterFlag = 0;
		}
	}
}

/**
-------------------------------------------------------------------------------
ParseXML
-------------------------------------------------------------------------------
*   @brief		To parse the input configuration xml
*
*	@param		none
*
*	@return		Return the input configuration parameters
*
*/
ConfigurationReader::ConfigParameters ConfigurationReader::parseXML()
{
	xmlDocPtr doc = NULL;
	xmlNodePtr node_ptr = NULL;

	//Read the xml file to be parsed
	doc = xmlReadFile(CONFIGURATION_XML_PATH, NULL, XML_PARSE_NOBLANKS);
	if (doc == NULL) {
		fprintf(stderr, "Could not read file...\n");
	}
	node_ptr = xmlDocGetRootElement(doc);
	ConfigurationReader::traverseXMLTree(node_ptr);
	//Close the xml file and clean parser
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return m_configParams;
}

/**
-------------------------------------------------------------------------------
PrintMap
-------------------------------------------------------------------------------
*   @brief		To print the internal maps used (For debugging purpose only)
*
*	@param		none
*
*	@return		None
*
*/
void ConfigurationReader::printMap()
{
	typedef std::map<std::string, std::string> inner_map;
	typedef std::map<std::string, inner_map> outer_map;

	for (outer_map::iterator i = m_configParameters.begin(), iend = m_configParameters.end(); i != iend; ++i)
	{
		inner_map &innerMap = i->second;
		std::cout << "---- " << i->first << " ----" << std::endl;
		for (inner_map::iterator j = innerMap.begin(), jend = innerMap.end(); j != jend; ++j)
		{
			std::cout << j->first << " :: " << j->second << std::endl;
		}
		std::cout << std::endl;
	}
}

/* eof */
