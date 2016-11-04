#include <iostream>
#include <string>
#include <list>
#include <unistd.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>

#include "common_ut.h"
#include "bluetooth.h"
#include "dataobject.h"


using namespace CppUnit;
using namespace std;

//extern mockConfigurationReader *m_configInstance;

class TestBLE : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestBLE);
	//test to open database
	CPPUNIT_TEST(test_database_open_tc_01);

	//Test to add devices in the devicelist
	CPPUNIT_TEST(test_add_device_tc_01);
	CPPUNIT_TEST(test_add_device_tc_02);
	CPPUNIT_TEST(test_add_device_tc_03);
	CPPUNIT_TEST(test_add_device_tc_04);
	CPPUNIT_TEST(test_add_device_tc_05);
	CPPUNIT_TEST(test_add_device_tc_06);

	//Test to discover device again & update scantime
 	CPPUNIT_TEST(test_start_device_discovery_tc_01);
	CPPUNIT_TEST(test_start_device_discovery_tc_02);
	CPPUNIT_TEST(test_start_device_discovery_tc_03);
	CPPUNIT_TEST(test_start_device_discovery_tc_04);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:

	void test_database_open_tc_01(void);

	void test_add_device_tc_01(void);
	void test_add_device_tc_02(void);
	void test_add_device_tc_03(void);
	void test_add_device_tc_04(void);
	void test_add_device_tc_05(void);
	void test_add_device_tc_06(void);
	
	void test_start_device_discovery_tc_01(void);
	void test_start_device_discovery_tc_02(void);
	void test_start_device_discovery_tc_03(void);
	void test_start_device_discovery_tc_04(void);

	void test_scan_finished_tc_01(void);
	void test_scan_finished_tc_02(void);

private:
	
        Bluetooth BLScaner;
	mockQBluetoothDeviceInfo device;

	DataObject data;

};

void TestBLE::setUp(void)
{
	/* setup functions for test cases */
	//Ble = new Bluetooth(1);
}

void TestBLE::tearDown(void)
{

	/* tear down functions for test cases */	
	//delete Ble;
}

/**
* @brief Testcase to validate the database connection
*/

void TestBLE::test_database_open_tc_01(void)
{
	
	CPPUNIT_ASSERT_EQUAL(1, BLScaner.IsdbOpen);
}

/**
* @brief Testcase for add_device to validate single record inserted into the device list.
*/

void TestBLE::test_add_device_tc_01(void)
{
	
       //Set Up                            
	device.set_name("ble01");
	device.set_rssi(1234);
	QBluetoothAddress add("10:05:B0:0A:23:A1");
	device.set_address(add);
	      //Process
	BLScaner.addDevice(device);  //one device is newly found & as it is not present in the device list, it's should insert it in device list
	      //Assert
	CPPUNIT_ASSERT_EQUAL(1, BLScaner.devicelistsize);

}

/**
* @brief Testcase for add_device to validate two records inserted into the device list.
*/
 
void TestBLE::test_add_device_tc_02(void)
{
	device.set_name("ble01");
	device.set_rssi(1234);
	QBluetoothAddress add("10:05:B0:0A:23:A1");
	device.set_address(add);
	BLScaner.addDevice(device);   //one device is newly found & as it is not present in the device list, it's should insert it in device list

	device.set_name("ble02");
	device.set_rssi(5678);
	QBluetoothAddress add1("11:05:C0:0A:78:D1");
	device.set_address(add1);

	BLScaner.addDevice(device);  //one device is newly found & as it is not present in the device list, it's should insert it in device list

	CPPUNIT_ASSERT_EQUAL(2, BLScaner.devicelistsize);
}

/**
* @brief Testcase for add_device to validate duplicate records not inserted into the device list.
*/

void TestBLE::test_add_device_tc_03(void)
{
	device.set_name("ble01");
	device.set_rssi(5678);
	QBluetoothAddress add("11:05:C0:0A:78:D1");
	device.set_address(add);
	BLScaner.addDevice(device);   //one device is newly found & as it is not present in the device list, it's should insert it in device list

	BLScaner.addDevice(device);  //Same device is again found & as it is already present in the device list, It shouldn't be again added in device list

	CPPUNIT_ASSERT_EQUAL(1, BLScaner.devicelistsize);
}

/**
* @brief Testcase for add_device to validate duplicate multiple records not inserted into the device list.
*/

void TestBLE::test_add_device_tc_04(void)
{
	device.set_name("ble02");
	device.set_rssi(5678);
	QBluetoothAddress add1("11:05:C0:0A:78:D1");
	device.set_address(add1);

	BLScaner.addDevice(device);  //one device is newly found & as it is not present in the device list, it's should insert it in device list

	BLScaner.addDevice(device);  //Same device is again found & as it is already present in the device list, It shouldn't be again added in device list

	device.set_name("ble01");
	device.set_rssi(5678);
	QBluetoothAddress add("10:05:B0:0A:23:A1");
	device.set_address(add);

	BLScaner.addDevice(device);  //one device is newly found & as it is not present in the device list, it's should insert it in device list

	BLScaner.addDevice(device);  //Same device is again found & as it is already present in the device list, It shouldn't be again added in device list

	device.set_name("ble03");
	device.set_rssi(9000);
	QBluetoothAddress add2("21:B5:A0:C5:E3:A1");
	device.set_address(add2);

	BLScaner.addDevice(device);   //one device is newly found & as it is not present in the device list, it's should insert it in device list

	CPPUNIT_ASSERT_EQUAL(3, BLScaner.devicelistsize);
}

void TestBLE::test_add_device_tc_05(void)
{
	device.set_name("ble01");
	device.set_rssi(5678);
	QBluetoothAddress add("11:05:C0:0A:78:D1");
	device.set_address(add);
	BLScaner.addDevice(device);   //one device is newly found & as it is not present in the device list, it's should insert it in device list

	device.set_name("ble02");
	device.set_rssi(5698);
	QBluetoothAddress add1("11:05:C0:0A:78:D1");
	device.set_address(add1);
	BLScaner.addDevice(device);  //Same device is again found & as it is already present in the device list, It shouldn't be again added in device list

	CPPUNIT_ASSERT_EQUAL(1, BLScaner.devicelistsize);
}

void TestBLE::test_add_device_tc_06(void)
{
	device.set_name("ble02");
	device.set_rssi(5678);
	QBluetoothAddress add1("11:05:C0:0A:78:D1");
	device.set_address(add1);

	BLScaner.addDevice(device);  //one device is newly found & as it is not present in the device list, it's should insert it in device list

	BLScaner.addDevice(device);  //Same device is again found & as it is already present in the device list, It shouldn't be again added in device list

	device.set_name("ble01");
	device.set_rssi(5678);
	QBluetoothAddress add("10:05:B0:0A:23:A1");
	device.set_address(add);

	BLScaner.addDevice(device);  //one device is newly found & as it is not present in the device list, it's should insert it in device list

	BLScaner.addDevice(device);  //Same device is again found & as it is already present in the device list, It shouldn't be again added in device list

	device.set_name("ble03");
	device.set_rssi(9000);
	QBluetoothAddress add2("21:B5:A0:C5:E3:A1");
	device.set_address(add2);

	BLScaner.addDevice(device);   //one device is newly found & as it is not present in the device list, it's should insert it in device list

	device.set_name("ble04");
	device.set_rssi(9899);
	QBluetoothAddress add3("21:B5:A0:C5:E3:A1");
	device.set_address(add3);

	BLScaner.addDevice(device);

	CPPUNIT_ASSERT_EQUAL(3, BLScaner.devicelistsize);
}


/**
* @brief Testcase for start_device_discovery to validate the scan time of device.
*/


void TestBLE::test_start_device_discovery_tc_01(void)
{
	device.set_name("ble01");
	device.set_rssi(5678);
	QBluetoothAddress add("10:05:B0:0A:23:A1");
	device.set_address(add);

	BLScaner.startDeviceDiscovery();
	QString time = BLScaner.ScanTime;

	BLScaner.addDevice(device);

	CPPUNIT_ASSERT_EQUAL(0, (time).compare(BLScaner.ScanTime));
}

/**
* @brief Testcase for start_device_discovery to validate the scan time of device.
*/
void TestBLE::test_start_device_discovery_tc_02(void)
{
	device.set_name("ble02");
	device.set_rssi(5678);
	QBluetoothAddress add1("11:05:C0:0A:78:D1");
	device.set_address(add1);

	BLScaner.startDeviceDiscovery();
	QString time = BLScaner.ScanTime;

	BLScaner.addDevice(device);

	CPPUNIT_ASSERT_EQUAL(0, (time).compare(BLScaner.ScanTime));
}

/**
* @brief Testcase for start_device_discovery to validate the scan time of device with mutiple scanning cycles.
*/

void TestBLE::test_start_device_discovery_tc_03(void)
{
	device.set_name("ble01");
	device.set_rssi(5678);
	QBluetoothAddress add("10:05:B0:0A:23:A1");
	device.set_address(add);

	BLScaner.startDeviceDiscovery();
	QString time = BLScaner.ScanTime;
	BLScaner.addDevice(device);
	//printf("\n Before ---  %s\n", time);
	sleep(1);

	BLScaner.startDeviceDiscovery();
	BLScaner.addDevice(device);
	//printf("\n After ---  %d\n", (time).compare(BLScaner.ScanTime));
	CPPUNIT_ASSERT_EQUAL(-1, (time).compare(BLScaner.ScanTime));
}


void TestBLE::test_start_device_discovery_tc_04(void)
{
	device.set_name("ble02");
	device.set_rssi(5679);
	QBluetoothAddress add1("11:05:C0:0A:78:D1");
	device.set_address(add1);

	BLScaner.startDeviceDiscovery();
	QString time = BLScaner.ScanTime;
	BLScaner.addDevice(device);

	sleep(1);

	BLScaner.startDeviceDiscovery();
	time = BLScaner.ScanTime;
	BLScaner.addDevice(device);

	CPPUNIT_ASSERT_EQUAL(0, (time).compare(BLScaner.ScanTime));
	//BLScaner.~Bluetooth();
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestBLE);

int main(int argc, char* argv[])
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
	ofstream xmlFileOut("test_results_BLE.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	getchar();
	exit(0);

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}

