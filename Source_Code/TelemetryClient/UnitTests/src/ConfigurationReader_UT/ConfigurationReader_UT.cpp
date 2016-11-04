// ConfigurationReader_UT.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>

#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/ui/text/TextTestRunner.h"
#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestResultCollector.h"
#include "cppunit/TestRunner.h"
#include "cppunit/BriefTestProgressListener.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/XmlOutputter.h"

#include "ConfigurationReader.h"

using namespace CppUnit;
using namespace std;

class ConfigurationReader_UT :
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ConfigurationReader_UT);
	CPPUNIT_TEST(test_init_tc_01);	
	CPPUNIT_TEST(test_printMap_tc_02);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:

	void test_init_tc_01(void);
	void test_printMap_tc_02(void);

private:
	ConfigurationReader *m_ConfigurationReader;
};

void ConfigurationReader_UT::setUp()
{
	m_ConfigurationReader = new ConfigurationReader();
}

void ConfigurationReader_UT::tearDown()
{
	delete m_ConfigurationReader;
}

void ConfigurationReader_UT::test_init_tc_01()
{
	
	int success = 1;
	unsigned short m_sensorGlobalAcquisitionFreq = 200;
	unsigned short m_sensorEnable = 1;
	string m_sensorJson = "\"protocol\":{\"id\":1,\"name\":\"SunsetPassAPI\",\"type\":20},\"sensoridentifier\":\"SunsetPassAPI_1\",\"type\":\"sensor\",\"valuetype\":\"0xFF82\"";
	unsigned short m_sensorExternalModuleEnable = 1;
	unsigned short m_sensorPlugin1Enable = 1;
	unsigned short m_sensorPlugin2Enable = 0;
	unsigned short m_sensorPlugin3Enable = 1;
	unsigned short m_PIREnable = 1;
	string m_PIRJson = "\"protocol\":{\"id\":2,\"name\":\"PIR\",\"type\":21},\"sensoridentifier\":\"PIR\",\"type\":\"sensor\",\"valuetype\":\"0xFF82\"";
	unsigned short m_PIRExternalModuleEnable = 1;
	unsigned short m_PIRPlugin1Enable = 0;
	unsigned short m_PIRPlugin2Enable = 1;
	unsigned short m_PIRPlugin3Enable = 0;
	unsigned short m_WiFiEnable = 1;
	unsigned short m_WiFiAcquisitionFreq = 5;
	string m_WiFiDeviceID = "wlp2s0";
	signed short m_WiFiRSSI = -148;
	string m_WiFiJson = "\"protocol\":{\"id\":3,\"name\":\"WiFi\",\"type\":22},\"sensoridentifier\":\"WiFi\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"";
	unsigned short m_WiFiExternalModuleEnable = 1;
	unsigned short m_WiFiPlugin1Enable = 1;
	unsigned short m_WiFiPlugin2Enable = 1;
	unsigned short m_WiFiPlugin3Enable = 1;
	unsigned short m_BLEEnable = 1;
	unsigned short m_BLEAcquisitionFreq = 5000;
	string m_BLEJson = "\"protocol\":{\"id\":4,\"name\":\"BLE\",\"type\":23},\"sensoridentifier\":\"BLE\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"";
	unsigned short m_BLEExternalModuleEnable = 1;
	unsigned short m_BLEPlugin1Enable = 1;
	unsigned short m_BLEPlugin2Enable = 1;
	unsigned short m_BLEPlugin3Enable = 1;
	unsigned short m_RealSenseEnable = 1;
	unsigned short m_RealSenseAcquisitionFreq = 200;
	string m_RealSenseJson = "\"protocol\":{\"id\":5,\"name\":\"RealSense\",\"type\":24},\"sensoridentifier\":\"RealSense\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"";	
	unsigned short m_RealSenseExternalModuleEnable = 0;
	unsigned short m_RealSensePlugin1Enable = 0;
	unsigned short m_RealSensePlugin2Enable = 0;
	unsigned short m_RealSensePlugin3Enable = 0;
	unsigned short m_TelemetryEnable = 1;
	string m_DatabaseHostName = "localhost";
	string m_DatabaseUserName = "root";
	string m_DatabasePassword = "intel";
	string m_DatabaseName = "sensor_db";
	unsigned short m_DatabasePort = 3306;
	unsigned short m_DatabaseAcquisitionFreq = 800;
	unsigned short m_DatabasePurgingFreq = 360;
	unsigned short m_AuditEnable = 1;
	string m_VendingDatabaseHostName = "localhost";
	string m_VendingDatabaseUserName = "root";
	string m_VendingDatabasePassword = "intel";
	string m_VendingDatabaseName = "vending_db";
	unsigned short  m_VendingDatabasePort = 3306;
	string m_DEXJson = "\"protocol\":{\"id\":6,\"name\":\"DEX\",\"type\":25},\"sensoridentifier\":\"DEX\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"";						
	string m_TransactionJson = "\"protocol\":{\"id\":7,\"name\":\"Transaction\",\"type\":26},\"sensoridentifier\":\"Transaction\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"";				
	string m_StockJson = "\"protocol\":{\"id\":8,\"name\":\"Stock\",\"type\":27},\"sensoridentifier\":\"Stock\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"";						
	unsigned short m_AuditExternalModuleEnable = 1;
	unsigned short m_DEXPluginEnable = 1;			
	unsigned short m_AuditPlugin2Enable = 0;		
	unsigned short m_AuditPlugin3Enable = 0;
	unsigned short m_mqttTLS = 1;
	unsigned short m_mqttQoS = 1;
	unsigned short m_ReportingFreq = 120;
	unsigned short m_sensorCount = 2;
	string m_MQTTSendTopic = "Telemetry Information";
	string m_MQTTReceiveTopic = "Telemetry Information Ack";
	string m_MQTTEndPoint = "tcp://localhost:1883";
	string m_deviceIdentifier = "abcde";
	
	struct SensorInfo
	{
		std::string sensorName = "";
		int sensorId = 0;
		int deviceId = 0;
		string sensorType;
		string sensorUnit;
		int sensorAcquisitionFrequency = 0;
		int sensorFrequencyCount = 0;
		int eventSensorFlag = 0;
	};

	static SensorInfo m_sensorInfo[10];
	
	m_sensorInfo[0].sensorName = "Temperature sensor 3";
	m_sensorInfo[0].sensorAcquisitionFrequency = 50;
	m_sensorInfo[1].sensorName = "TEM_SENSOR_EXCEEDED";
	success = success * (m_sensorGlobalAcquisitionFreq == m_ConfigurationReader->m_sensorGlobalAcquisitionFreq);
	success = success * (m_sensorEnable == m_ConfigurationReader->m_sensorEnable);	
	success = success * (m_sensorJson == m_ConfigurationReader->m_sensorJson);
	success = success * (m_sensorExternalModuleEnable == m_ConfigurationReader->m_sensorExternalModuleEnable);
	success = success * (m_sensorPlugin1Enable == m_ConfigurationReader->m_sensorPlugin1Enable);
	success = success * (m_sensorPlugin2Enable == m_ConfigurationReader->m_sensorPlugin2Enable);
	success = success * (m_sensorPlugin3Enable == m_ConfigurationReader->m_sensorPlugin3Enable);
	success = success * (m_PIREnable == m_ConfigurationReader->m_PIREnable);
	success = success * (m_PIRJson == m_ConfigurationReader->m_PIRJson);
	success = success * (m_PIRExternalModuleEnable == m_ConfigurationReader->m_PIRExternalModuleEnable);
	success = success * (m_PIRPlugin1Enable == m_ConfigurationReader->m_PIRPlugin1Enable);
	success = success * (m_PIRPlugin2Enable == m_ConfigurationReader->m_PIRPlugin2Enable);
	success = success * (m_PIRPlugin3Enable == m_ConfigurationReader->m_PIRPlugin3Enable);
	success = success * (m_WiFiEnable == m_ConfigurationReader->m_WiFiEnable);
	success = success * (m_WiFiAcquisitionFreq == m_ConfigurationReader->m_WiFiAcquisitionFreq);
	success = success * (m_WiFiDeviceID == m_ConfigurationReader->m_WiFiDeviceID);
	success = success * (m_WiFiRSSI == m_ConfigurationReader->m_WiFiRSSI);
	success = success * (m_WiFiJson == m_ConfigurationReader->m_WiFiJson);
	success = success * (m_WiFiExternalModuleEnable == m_ConfigurationReader->m_WiFiExternalModuleEnable);
	success = success * (m_WiFiPlugin1Enable == m_ConfigurationReader->m_WiFiPlugin1Enable);
	success = success * (m_WiFiPlugin2Enable == m_ConfigurationReader->m_WiFiPlugin2Enable);
	success = success * (m_WiFiPlugin3Enable == m_ConfigurationReader->m_WiFiPlugin3Enable);
	success = success * (m_BLEEnable == m_ConfigurationReader->m_BLEEnable);
	success = success * (m_BLEAcquisitionFreq == m_ConfigurationReader->m_BLEAcquisitionFreq);
	success = success * (m_BLEJson == m_ConfigurationReader->m_BLEJson);
	success = success * (m_BLEExternalModuleEnable == m_ConfigurationReader->m_BLEExternalModuleEnable);
	success = success * (m_BLEPlugin1Enable == m_ConfigurationReader->m_BLEPlugin1Enable);
	success = success * (m_BLEPlugin2Enable == m_ConfigurationReader->m_BLEPlugin2Enable);
	success = success * (m_BLEPlugin3Enable == m_ConfigurationReader->m_BLEPlugin3Enable);	
	success = success * (m_RealSenseEnable == m_ConfigurationReader->m_RealSenseEnable);
	success = success * (m_RealSenseAcquisitionFreq == m_ConfigurationReader->m_RealSenseAcquisitionFreq);
	success = success * (m_RealSenseJson == m_ConfigurationReader->m_RealSenseJson);
	success = success * (m_RealSenseExternalModuleEnable == m_ConfigurationReader->m_RealSenseExternalModuleEnable);
	success = success * (m_RealSensePlugin1Enable == m_ConfigurationReader->m_RealSensePlugin1Enable);
	success = success * (m_RealSensePlugin2Enable == m_ConfigurationReader->m_RealSensePlugin2Enable);
	success = success * (m_RealSensePlugin3Enable == m_ConfigurationReader->m_RealSensePlugin3Enable);
	success = success * (m_TelemetryEnable == m_ConfigurationReader->m_TelemetryEnable);
	success = success * (m_DatabaseHostName == m_ConfigurationReader->m_DatabaseHostName);
	success = success * (m_DatabaseUserName == m_ConfigurationReader->m_DatabaseUserName);
	success = success * (m_DatabasePassword == m_ConfigurationReader->m_DatabasePassword);
	success = success * (m_DatabaseName == m_ConfigurationReader->m_DatabaseName);
	success = success * (m_DatabasePort == m_ConfigurationReader->m_DatabasePort);
	success = success * (m_DatabaseAcquisitionFreq == m_ConfigurationReader->m_DatabaseAcquisitionFreq);
	success = success * (m_DatabasePurgingFreq == m_ConfigurationReader->m_DatabasePurgingFreq);
	success = success * (m_AuditEnable == m_ConfigurationReader->m_AuditEnable);
	success = success * (m_VendingDatabaseHostName == m_ConfigurationReader->m_VendingDatabaseHostName);
	success = success * (m_VendingDatabaseUserName == m_ConfigurationReader->m_VendingDatabaseUserName);
	success = success * (m_VendingDatabasePassword == m_ConfigurationReader->m_VendingDatabasePassword);
	success = success * (m_VendingDatabaseName == m_ConfigurationReader->m_VendingDatabaseName);
	success = success * (m_VendingDatabasePort == m_ConfigurationReader->m_VendingDatabasePort);
	success = success * (m_DEXJson == m_ConfigurationReader->m_DEXJson);
	success = success * (m_TransactionJson == m_ConfigurationReader->m_TransactionJson);
	success = success * (m_StockJson == m_ConfigurationReader->m_StockJson);
	success = success * (m_AuditExternalModuleEnable == m_ConfigurationReader->m_AuditExternalModuleEnable);
	success = success * (m_DEXPluginEnable == m_ConfigurationReader->m_DEXPluginEnable);
	success = success * (m_AuditPlugin2Enable == m_ConfigurationReader->m_AuditPlugin2Enable);
	success = success * (m_AuditPlugin3Enable == m_ConfigurationReader->m_AuditPlugin3Enable);
	success = success * (m_mqttTLS == m_ConfigurationReader->m_mqttTLS);
	success = success * (m_mqttQoS == m_ConfigurationReader->m_mqttQoS);
	success = success * (m_ReportingFreq == m_ConfigurationReader->m_ReportingFreq);
	success = success * (m_sensorCount == m_ConfigurationReader->m_sensorCount);
	success = success * (m_MQTTSendTopic == m_ConfigurationReader->m_MQTTSendTopic);
	success = success * (m_MQTTReceiveTopic == m_ConfigurationReader->m_MQTTReceiveTopic);
	success = success * (m_MQTTEndPoint == m_ConfigurationReader->m_MQTTEndPoint);
	success = success * (m_deviceIdentifier == m_ConfigurationReader->m_deviceIdentifier);	

	for (int i = 0; i < m_sensorCount; i++)
	{
		if (m_sensorInfo[i].eventSensorFlag == 0)
		{
			success = success * (m_sensorInfo[i].sensorName == m_ConfigurationReader->m_sensorInfo[i].sensorName);
			success = success * (m_sensorInfo[i].sensorAcquisitionFrequency == m_ConfigurationReader->m_sensorInfo[i].sensorAcquisitionFrequency);
		}
	}	
	CPPUNIT_ASSERT_EQUAL(1, success);
}

void ConfigurationReader_UT::test_printMap_tc_02()
{
	m_ConfigurationReader->printMap();

	CPPUNIT_ASSERT_EQUAL(true, true);
}

CPPUNIT_TEST_SUITE_REGISTRATION(ConfigurationReader_UT);

int main()
{
	// informs test-listener about testresults
	CPPUNIT_NS::TestResult testresult;

	// register listener for collecting the test-results
	CPPUNIT_NS::TestResultCollector collectedresults;
	testresult.addListener(&collectedresults);

	// register listener for per-test progress output
	CPPUNIT_NS::BriefTestProgressListener progress;
	testresult.addListener(&progress);

	// insert test-suite at test-runner by registry
	CPPUNIT_NS::TestRunner testrunner;
	testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	testrunner.run(testresult);

	// output results in compiler-format
	CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
	compileroutputter.write();

	// Output XML for Jenkins CPPunit plugin
	ofstream xmlFileOut("Test_Results_ConfigurationReader_UT.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	getchar();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}

