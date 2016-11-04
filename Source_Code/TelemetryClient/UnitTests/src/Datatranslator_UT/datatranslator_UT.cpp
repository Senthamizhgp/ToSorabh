#include <iostream>
#include <string>
#include <list>

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

//#include "common_ut.h"
#include "datatranslator.h"

using namespace CppUnit;
using namespace std;

class Testdatatranslator : 
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Testdatatranslator);
	
	CPPUNIT_TEST(test_datatransalator_tc_01);
	CPPUNIT_TEST(test_datatransalator_tc_02);
	CPPUNIT_TEST(test_datatransalator_tc_03);
	CPPUNIT_TEST(test_datatransalator_tc_04);
	CPPUNIT_TEST(test_datatransalator_tc_05);
	CPPUNIT_TEST(test_datatransalator_tc_06);
	CPPUNIT_TEST(test_datatransalator_tc_07);
	CPPUNIT_TEST(test_datatransalator_tc_08);
	CPPUNIT_TEST(test_datatransalator_tc_09);
	CPPUNIT_TEST(test_datatransalator_tc_10);
	CPPUNIT_TEST(test_datatransalator_tc_11);
	CPPUNIT_TEST(test_datatransalator_tc_12);
	CPPUNIT_TEST(test_datatransalator_tc_13);
	CPPUNIT_TEST(test_datatransalator_tc_14);
	CPPUNIT_TEST(test_datatransalator_tc_15);
	CPPUNIT_TEST(test_datatransalator_tc_16);
	CPPUNIT_TEST(test_datatransalator_tc_17);
	CPPUNIT_TEST(test_datatransalator_tc_18);
	CPPUNIT_TEST(test_datatransalator_tc_19);
	CPPUNIT_TEST(test_datatransalator_tc_20);
	CPPUNIT_TEST(test_datatransalator_tc_21);

	
CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test_datatransalator_tc_01(void);
	void test_datatransalator_tc_02(void);
	void test_datatransalator_tc_03(void);
	void test_datatransalator_tc_04(void);
	void test_datatransalator_tc_05(void);
	void test_datatransalator_tc_06(void);
	void test_datatransalator_tc_07(void);
	void test_datatransalator_tc_08(void);
	void test_datatransalator_tc_09(void);
	void test_datatransalator_tc_10(void);
	void test_datatransalator_tc_11(void);
	void test_datatransalator_tc_12(void);
	void test_datatransalator_tc_13(void);
	void test_datatransalator_tc_14(void);
	void test_datatransalator_tc_15(void);
	void test_datatransalator_tc_16(void);
	void test_datatransalator_tc_17(void);
	void test_datatransalator_tc_18(void);
	void test_datatransalator_tc_19(void);
	void test_datatransalator_tc_20(void);
	void test_datatransalator_tc_21(void);

private:
	Datatranslator *mDatatransaction;

};

/**
-------------------------------------------------------------------------------
setUp
-------------------------------------------------------------------------------
* Setup function for test cases.
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::setUp(void)
{
	mDatatransaction = new Datatranslator();
}

/**
-------------------------------------------------------------------------------
tearDown
-------------------------------------------------------------------------------
* Tear down functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::tearDown(void)
{
	delete mDatatransaction;
}

/**
-------------------------------------------------------------------------------
test_datatransalator_tc_01
-------------------------------------------------------------------------------
* 
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_01(void)
{


	QMap<QString, QVariant>::iterator map_iter;
	//mDatatransaction->Reading_PIR_data();
	
	for (map_iter = mDatatransaction->m_sensor_map.begin(); map_iter != mDatatransaction->m_sensor_map.end(); map_iter++)
	{
		if (map_iter.key() == "sensoridentifier")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "PIR";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "PIR_sensor";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

	}
	
		
		for (map_iter = mDatatransaction->m_Protocol.begin(); map_iter != mDatatransaction->m_Protocol.end(); map_iter++)
		{

			if (map_iter.key() == "id")
			{
				int check = map_iter.value().toInt();
				int tocheck = 1;
				CPPUNIT_ASSERT_EQUAL(tocheck, check);
			}

			else if (map_iter.key() == "name")
			{
				string check = map_iter.value().toString().toStdString();
				string tocheck = "PIR";
				CPPUNIT_ASSERT_EQUAL(tocheck, check);
			}

			else if (map_iter.key() == "type")
			{
				int check = map_iter.value().toInt();
				int tocheck = 28;
				CPPUNIT_ASSERT_EQUAL(tocheck, check);
			}

			
		}

}


/**
-------------------------------------------------------------------------------
test_datatransalator_tc_02
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_02(void)
{


	QMap<QString, QVariant>::iterator map_iter;
	//mDatatransaction->Reading_Dex_data();

	for (map_iter = mDatatransaction->m_sensor_map.begin(); map_iter != mDatatransaction->m_sensor_map.end(); map_iter++)
	{
		if (map_iter.key() == "sensoridentifier")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Dex";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "DEX_item";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

	}


	for (map_iter = mDatatransaction->m_Protocol.begin(); map_iter != mDatatransaction->m_Protocol.end(); map_iter++)
	{

		if (map_iter.key() == "id")
		{
			int check = map_iter.value().toInt();
			int tocheck = 2;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "name")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Dex";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			int check = map_iter.value().toInt();
			int tocheck = 29;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}


	}

}


/**
-------------------------------------------------------------------------------
test_datatransalator_tc_03
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_03(void)
{


	QMap<QString, QVariant>::iterator map_iter;
	//mDatatransaction->Reading_WIFI_data();

	for (map_iter = mDatatransaction->m_sensor_map.begin(); map_iter != mDatatransaction->m_sensor_map.end(); map_iter++)
	{
		if (map_iter.key() == "sensoridentifier")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "WIFI";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Module";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

	}


	for (map_iter = mDatatransaction->m_Protocol.begin(); map_iter != mDatatransaction->m_Protocol.end(); map_iter++)
	{

		if (map_iter.key() == "id")
		{
			int check = map_iter.value().toInt();
			int tocheck = 5;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "name")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "WIFI";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			int check = map_iter.value().toInt();
			int tocheck = 1;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}


	}

}



/**
-------------------------------------------------------------------------------
test_datatransalator_tc_04
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_04(void)
{


	QMap<QString, QVariant>::iterator map_iter;
	//mDatatransaction->Reading_BLE_data();

	for (map_iter = mDatatransaction->m_sensor_map.begin(); map_iter != mDatatransaction->m_sensor_map.end(); map_iter++)
	{
		if (map_iter.key() == "sensoridentifier")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Bluetooth";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Module";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

	}


	for (map_iter = mDatatransaction->m_Protocol.begin(); map_iter != mDatatransaction->m_Protocol.end(); map_iter++)
	{

		if (map_iter.key() == "id")
		{
			int check = map_iter.value().toInt();
			int tocheck = 3;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "name")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Bluetooth";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			int check = map_iter.value().toInt();
			int tocheck = 1;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}


	}

}



/**
-------------------------------------------------------------------------------
test_datatransalator_tc_05
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_05(void)
{


	QMap<QString, QVariant>::iterator map_iter;
	//mDatatransaction->Reading_Trans_data();

	for (map_iter = mDatatransaction->m_sensor_map.begin(); map_iter != mDatatransaction->m_sensor_map.end(); map_iter++)
	{
		if (map_iter.key() == "sensoridentifier")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Transcation";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "transcation";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

	}


	for (map_iter = mDatatransaction->m_Protocol.begin(); map_iter != mDatatransaction->m_Protocol.end(); map_iter++)
	{

		if (map_iter.key() == "id")
		{
			int check = map_iter.value().toInt();
			int tocheck = 8;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "name")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Transcation";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			int check = map_iter.value().toInt();
			int tocheck = 2;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}


	}

}


/**
-------------------------------------------------------------------------------
test_datatransalator_tc_06
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_06(void)
{


	QMap<QString, QVariant>::iterator map_iter;
	//mDatatransaction->Reading_stock_data();

	for (map_iter = mDatatransaction->m_sensor_map.begin(); map_iter != mDatatransaction->m_sensor_map.end(); map_iter++)
	{
		if (map_iter.key() == "sensoridentifier")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "stockdata";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "stock";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

	}


	for (map_iter = mDatatransaction->m_Protocol.begin(); map_iter != mDatatransaction->m_Protocol.end(); map_iter++)
	{

		if (map_iter.key() == "id")
		{
			int check = map_iter.value().toInt();
			int tocheck = 9;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "name")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "stock";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			int check = map_iter.value().toInt();
			int tocheck = 1;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}
	}

}




/**
-------------------------------------------------------------------------------
test_datatransalator_tc_07
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_07(void)
{


	QMap<QString, QVariant>::iterator map_iter;
	//mDatatransaction->Reading_sens_T_data();

	for (map_iter = mDatatransaction->m_sensor_map.begin(); map_iter != mDatatransaction->m_sensor_map.end(); map_iter++)
	{
		if (map_iter.key() == "sensoridentifier")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "SUNSETPASSAPI_1";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "sensor";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

	}


	for (map_iter = mDatatransaction->m_Protocol.begin(); map_iter != mDatatransaction->m_Protocol.end(); map_iter++)
	{

		if (map_iter.key() == "id")
		{
			int check = map_iter.value().toInt();
			int tocheck = 1;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "name")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "SunsetPassAPI";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			int check = map_iter.value().toInt();
			int tocheck = 28;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}


	}

}

/**
-------------------------------------------------------------------------------
test_datatransalator_tc_08
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_08(void)
{


	QMap<QString, QVariant>::iterator map_iter;
	//mDatatransaction->Reading_Real_Sens_data();

	for (map_iter = mDatatransaction->m_sensor_map.begin(); map_iter != mDatatransaction->m_sensor_map.end(); map_iter++)
	{
		if (map_iter.key() == "sensoridentifier")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Real_Sense";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "camera";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

	}


	for (map_iter = mDatatransaction->m_Protocol.begin(); map_iter != mDatatransaction->m_Protocol.end(); map_iter++)
	{

		if (map_iter.key() == "id")
		{
			int check = map_iter.value().toInt();
			int tocheck = 3;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "name")
		{
			string check = map_iter.value().toString().toStdString();
			string tocheck = "Real_sense_cam";
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}

		else if (map_iter.key() == "type")
		{
			int check = map_iter.value().toInt();
			int tocheck = 60;
			CPPUNIT_ASSERT_EQUAL(tocheck, check);
		}


	}

}

/**
-------------------------------------------------------------------------------
test_datatransalator_tc_09
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_09(void)
{
	QList<QMap <QString, QVariant>> test;
	QJsonArray array;
	QMap <QString, QVariant> innermap;
	QMap<QString, QJsonArray> testarray;
	QJsonObject  jsonObjectFirst;

	jsonObjectFirst.insert("DeviceName", "sriram");
	jsonObjectFirst.insert("MAC", 1234);
	jsonObjectFirst.insert("RSSI", 4321);
	jsonObjectFirst.insert("Sensor_Name", "sensor");
	jsonObjectFirst.insert("Timestamp", 4323232);
	array << jsonObjectFirst;
	testarray.insert("test", array);
	string jsonfinal = mDatatransaction->Dexdata_handler(testarray);
	string jsonexpected = "{\"test\":[{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}]}";
	CPPUNIT_ASSERT_EQUAL(jsonexpected, jsonfinal);
}


/**
-------------------------------------------------------------------------------
test_datatransalator_tc_10
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_10(void)
{

	QMap <QString, QVariant> innermap;
	time_t now = time(0);
	string timenow = QString::number(now).toStdString();
	////mDatatransaction->sufix_and_prefix(timenow);
	QMap <QString, QVariant>::iterator map_iter;

	for (map_iter = mDatatransaction->m_sufix.begin(); map_iter != mDatatransaction->m_sufix.end(); map_iter++)
	{
		if (map_iter.key() == "timestamp")
		{
			string check2 = map_iter.value().toString().toStdString();
			CPPUNIT_ASSERT_EQUAL(timenow, check2);
		}

		else if (map_iter.key() == "deviceidentifier")
		{
			string check2 = map_iter.value().toString().toStdString();
			string identify = "00137a0017bf2";
			CPPUNIT_ASSERT_EQUAL(identify, check2);
		}
		else if (map_iter.key() == "valuetype")
		{
			string check2 = map_iter.value().toString().toStdString();
			string identify ="0xFF82";
			CPPUNIT_ASSERT_EQUAL(identify, check2);
		}
	}
}

/**
-------------------------------------------------------------------------------
test_datatransalator_tc_11
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_11(void)
{
	QMap <QString, QVariant> innermap;
	time_t now = time(0);
	string timenow = QString::number(now).toStdString();
	//mDatatransaction->sufix_and_prefix(timenow);
	QMap <QString, QVariant>::iterator map_iter;
	string jsonexpected = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}";
	string check1 =	mDatatransaction->converting_Final_JSON(jsonexpected.c_str(), "PIR", "1470317074");
	string identify = "{\"datetime\":\"1470317074\",\"deviceidentifier\":\"00137a0017bf2\",\"protocol\" : {\"id\":1,\"name\" :\"PIR\",\"type\" : 28},\"sensoridentifier\" :\"PIR\",\"type\" :\"PIR_sensor\",\"value\" : { \"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232},\"valuetype\" :\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(identify, check1);
}

/**
-------------------------------------------------------------------------------
test_datatransalator_tc_12
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_12(void)
{
	QMap <QString, QVariant> innermap;
	time_t now = time(0);
	string timenow = QString::number(now).toStdString();
	//mDatatransaction->sufix_and_prefix(timenow);
	QMap <QString, QVariant>::iterator map_iter;
	string jsonexpected = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}";
	string check1 = mDatatransaction->converting_Final_JSON(jsonexpected.c_str(), "BLE", "1470317074");
	string identify = "{\"datetime\":\"1470317074\",\"deviceidentifier\":\"00137a0017bf2\",\"protocol\" : {\"id\":3,\"name\" :\"Bluetooth\",\"type\" : 1},\"sensoridentifier\" :\"Bluetooth\",\"type\" :\"Module\",\"value\" : { \"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232},\"valuetype\" :\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(identify, check1);
}

/**
-------------------------------------------------------------------------------
test_datatransalator_tc_13
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_13(void)
{
	QMap <QString, QVariant> innermap;
	time_t now = time(0);
	string timenow = QString::number(now).toStdString();
	//mDatatransaction->sufix_and_prefix(timenow);
	QMap <QString, QVariant>::iterator map_iter;
	string jsonexpected = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}";
	string check1 = mDatatransaction->converting_Final_JSON(jsonexpected.c_str(), "Dex",  "1470317074");
	string identify = "{\"datetime\":\"1470317074\",\"deviceidentifier\":\"00137a0017bf2\",\"protocol\" : {\"id\":2,\"name\" :\"Dex\",\"type\" : 29},\"sensoridentifier\" :\"Dex\",\"type\" :\"DEX_item\",\"value\" : { \"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232},\"valuetype\" :\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(identify, check1);
}


/**
-------------------------------------------------------------------------------
test_datatransalator_tc_14
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_14(void)
{
	QMap <QString, QVariant> innermap;
	time_t now = time(0);
	string timenow = QString::number(now).toStdString();
	//mDatatransaction->sufix_and_prefix(timenow);
	QMap <QString, QVariant>::iterator map_iter;
	string jsonexpected = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}";
	string check1 = mDatatransaction->converting_Final_JSON(jsonexpected.c_str(), "WIFI", "1470317074");
	string identify = "{\"datetime\":\"1470317074\",\"deviceidentifier\":\"00137a0017bf2\",\"protocol\" : {\"id\":5,\"name\" :\"WIFI\",\"type\" : 1},\"sensoridentifier\" :\"WIFI\",\"type\" :\"Module\",\"value\" : { \"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232},\"valuetype\" :\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(identify, check1);
}




/**
-------------------------------------------------------------------------------
test_datatransalator_tc_15
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_15(void)
{
	QMap <QString, QVariant> innermap;
	time_t now = time(0);
	string timenow = QString::number(now).toStdString();
	//mDatatransaction->sufix_and_prefix(timenow);
	QMap <QString, QVariant>::iterator map_iter;
	string jsonexpected = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}";
	string check1 = mDatatransaction->converting_Final_JSON(jsonexpected.c_str(), "Trans", "1470317074");
	string identify = "{\"datetime\":\"1470317074\",\"deviceidentifier\":\"00137a0017bf2\",\"protocol\" : {\"id\":8,\"name\" :\"Transcation\",\"type\" : 2},\"sensoridentifier\" :\"Transcation\",\"type\" :\"transcation\",\"value\" : { \"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232},\"valuetype\" :\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(identify, check1);
}



/**
-------------------------------------------------------------------------------
test_datatransalator_tc_16
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_16(void)
{
	QMap <QString, QVariant> innermap;
	time_t now = time(0);
	string timenow = QString::number(now).toStdString();
	//mDatatransaction->sufix_and_prefix(timenow);
	QMap <QString, QVariant>::iterator map_iter;
	string jsonexpected = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}";
	string check1 = mDatatransaction->converting_Final_JSON(jsonexpected.c_str(), "stock", "1470317074");
	string identify = "{\"datetime\":\"1470317074\",\"deviceidentifier\":\"00137a0017bf2\",\"protocol\" : {\"id\":9,\"name\" :\"stock\",\"type\" : 1},\"sensoridentifier\" :\"stockdata\",\"type\" :\"stock\",\"value\" : { \"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232},\"valuetype\" :\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(identify, check1);
}



/**
-------------------------------------------------------------------------------
test_datatransalator_tc_17
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_17(void)
{
	QMap <QString, QVariant> innermap;
	time_t now = time(0);
	string timenow = QString::number(now).toStdString();
	//mDatatransaction->sufix_and_prefix(timenow);
	QMap <QString, QVariant>::iterator map_iter;
	string jsonexpected = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}";
	string check1 = mDatatransaction->converting_Final_JSON(jsonexpected.c_str(), "Sens_T", "1470317074");
	string identify = "{\"datetime\":\"1470317074\",\"deviceidentifier\":\"00137a0017bf2\",\"protocol\" : {\"id\":1,\"name\" :\"SunsetPassAPI\",\"type\" : 28},\"sensoridentifier\" :\"SUNSETPASSAPI_1\",\"type\" :\"sensor\",\"value\" : { \"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232},\"valuetype\" :\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(identify, check1);
}



/**
-------------------------------------------------------------------------------
test_datatransalator_tc_18
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void Testdatatranslator::test_datatransalator_tc_18(void)
{
	QMap <QString, QVariant> innermap;
	time_t now = time(0);
	string timenow = QString::number(now).toStdString();
	//mDatatransaction->sufix_and_prefix(timenow);
	QMap <QString, QVariant>::iterator map_iter;
	string jsonexpected = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}";
	string check1 = mDatatransaction->converting_Final_JSON(jsonexpected.c_str(), "RealSens", "1470317074");
	string identify = "{\"datetime\":\"1470317074\",\"deviceidentifier\":\"00137a0017bf2\",\"protocol\" : {\"id\":3,\"name\" :\"Real_sense_cam\",\"type\" : 60},\"sensoridentifier\" :\"Real_Sense\",\"type\" :\"camera\",\"value\" : { \"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232},\"valuetype\" :\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(identify, check1);
}

/**
-------------------------------------------------------------------------------
test_datatransalator_tc_19
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/

void Testdatatranslator::test_datatransalator_tc_19(void)
{
	QList<QMap <QString, QVariant>> test;
	QJsonArray array;
	QMap <QString, QVariant> innermap;
	QMap<QString, QJsonArray> testarray;
	QJsonObject  jsonObjectFirst;
	qlonglong time = 4323232;
	
	innermap.insert("DeviceName", "sriram");
	innermap.insert("MAC", 1234);
	innermap.insert("RSSI", 4321);
	innermap.insert("Sensor_Name", "sensor");
	innermap.insert("Timestamp", time);

	test.push_back(innermap);
	string time_stamp = mDatatransaction->converting_sensor_data_JSON_Indvidual(test, "PIR");
	string time_convert = std::to_string(time);
	CPPUNIT_ASSERT_EQUAL(time_convert, time_stamp);
	
}


/**
-------------------------------------------------------------------------------
test_datatransalator_tc_20
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/

void Testdatatranslator::test_datatransalator_tc_20(void)
{
	QList<QMap <QString, QVariant>> test;
	QJsonArray array;
	QMap <QString, QVariant> innermap;
	QMap<QString, QJsonArray> testarray;
	QJsonObject  jsonObjectFirst;
	qlonglong time = 4323232;

	innermap.insert("DeviceName", "sriram");
	innermap.insert("MAC", 1234);
	innermap.insert("RSSI", 4321);
	innermap.insert("Sensor_Name", "sensor");
	innermap.insert("Timestamp", time);

	test.push_back(innermap);
	std::pair<string, string> send_receive;
	mDatatransaction->converting_sensor_data_JSON_Indvidual_wifi_BLE(test, "PIR", send_receive);
	cout <<send_receive.first;
	cout << send_receive.second;
	int mac_id = 1234;

	string time_convert = std::to_string(time);
	string mac_id_convert = std::to_string(mac_id);

	//string time_convert = std::to_string(time);
	CPPUNIT_ASSERT_EQUAL(mac_id_convert, send_receive.first);
	CPPUNIT_ASSERT_EQUAL(time_convert, send_receive.second);

}


/**
-------------------------------------------------------------------------------
test_datatransalator_tc_21
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/

void Testdatatranslator::test_datatransalator_tc_21(void)
{
	QList<QMap <QString, QVariant>> test;
	QJsonArray array;
	QMap <QString, QVariant> innermap;
	QMap<QString, QJsonArray> testarray;
	QJsonObject  jsonObjectFirst;
	qlonglong time = 4323232;

	innermap.insert("DeviceName", "sriram");
	innermap.insert("MAC", 1234);
	innermap.insert("RSSI", 4321);
	innermap.insert("Sensor_Name", "sensor");
	innermap.insert("Timestamp", time);

	test.push_back(innermap);
	std::pair<string, string> send_receive;
	pair<QJsonArray, qlonglong> test1= mDatatransaction->converting_dexdata_array(test);
	QList<QMap <QString, QVariant>> listOfMap = test;
	
//	CPPUNIT_ASSERT_EQUAL(time, test1.second);

}


CPPUNIT_TEST_SUITE_REGISTRATION(Testdatatranslator);

int main(int argc, char* argv[])
{

#if 0
	if (argc < 4) 
	{
		printf("Usage: pir [DB Name] [User Name] [Password]");

		return -1;
	}
	else
	{
		db_name = argv[1];
		username = argv[2];
		password = argv[3];
#endif
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
		ofstream xmlFileOut("Test_Results_Datatranslator_UT.xml");
		XmlOutputter xmlOut(&collectedresults, xmlFileOut);
		xmlOut.write();

		// return 0 if tests were successful
		return collectedresults.wasSuccessful() ? 0 : 1;
	}


