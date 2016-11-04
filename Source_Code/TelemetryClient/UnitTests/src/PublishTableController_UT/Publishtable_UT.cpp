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

#include "common_ut.h"
#include "PublishTableController.h"
#include "qdatetime.h"

using namespace CppUnit;
using namespace std;

class Testpublishtable : 
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Testpublishtable);
	//	CPPUNIT_TEST(test_PublishControllerUpdatingBLE);
	CPPUNIT_TEST(test_PublishControllerUpdatingwifi);
	CPPUNIT_TEST(test_PublishControllerUpdatingpir);
	CPPUNIT_TEST(test_PublishControllerUpdatingSens_T);
//	CPPUNIT_TEST(test_PublishCtlrUpdatingRealsense);
	CPPUNIT_TEST(test_PublishControllerUpdatingTransaction);
	CPPUNIT_TEST(test_PublishControllerUpdatingstock);
	CPPUNIT_TEST(test_PublishControllerUpdatingVendingDB);
	CPPUNIT_TEST(test_PublishControllerUpdatingdex);
	CPPUNIT_TEST(test_UpdatePublishTable);
	CPPUNIT_TEST(test_executeQuery_fail);
	CPPUNIT_TEST(test_executeQuery_pass);
	CPPUNIT_TEST(test_PublishTableInsertion);
	CPPUNIT_TEST(test_executeselectquery);
	CPPUNIT_TEST(test_configurationreader);
	CPPUNIT_TEST(test_configurationreader_fail);
	
CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);
	
protected:
	void test_configurationreader();
	void test_PublishControllerUpdatingBLE();
	void test_PublishControllerUpdatingwifi();
	void test_PublishControllerUpdatingpir();
	void test_PublishControllerUpdatingSens_T();
	void test_PublishCtlrUpdatingRealsense();
	void test_PublishControllerUpdatingTransaction();
	void test_PublishControllerUpdatingstock();
	void test_PublishControllerUpdatingVendingDB();
	void test_PublishControllerUpdatingdex();
	void test_UpdatePublishTable();
	void test_configurationreader_fail();
	void test_executeQuery_fail();
	void test_executeQuery_pass();
	
	void test_PublishTableInsertion();
	void test_executeselectquery();


private:
	//Datatranslator *mDatatransaction;
	PublishTableController *mPublishTableController;
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
void Testpublishtable::setUp(void)
{
	mPublishTableController = new PublishTableController();
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
void Testpublishtable::tearDown(void)
{
	delete mPublishTableController;
}

/**
-------------------------------------------------------------------------------
test_configurationreader
-------------------------------------------------------------------------------
* test configurationreader functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/
void Testpublishtable::test_configurationreader()
{
	mPublishTableController->configurationReaderJson();
	string check = mPublishTableController->m_string_pir_configure;
	string compare = "{\"protocol\":{\"id\":2,\"name\":\"PIR\",\"type\":21},\"sensoridentifier\" :\"PIR\",\"type\":\"sensor\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_sens_t_configure;
	compare = "{\"protocol\":{\"id\":1,\"name\":\"SunsetPassAPI\",\"type\":20},\"sensoridentifier\":\"SunsetPassAPI_1\",\"type\":\"sensor\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_wifi_configure;
	compare = "{\"protocol\":{\"id\":3,\"name\":\"WiFi\",\"type\":22},\"sensoridentifier\":\"WiFi\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_ble_configure;
	compare = "{\"protocol\":{\"id\":4,\"name\":\"BLE\",\"type\":23},\"sensoridentifier\":\"BLE\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_dex_configure;
	compare = "{\"protocol\":{\"id\":6,\"name\":\"DEX\",\"type\":25},\"sensoridentifier\":\"DEX\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_Realsense_configure;
	compare = "{\"protocol\":{\"id\":5,\"name\":\"RealSense\",\"type\":24},\"sensoridentifier\":\"RealSense\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_stock_configure;
	compare = "{\"protocol\":{\"id\":8,\"name\":\"Stock\",\"type\":27},\"sensoridentifier\":\"Stock\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_Trans_configure;
	compare = "{\"protocol\":{\"id\":7,\"name\":\"Transaction\",\"type\":26},\"sensoridentifier\":\"Transaction\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
}

/**
-------------------------------------------------------------------------------
test_configurationreader
-------------------------------------------------------------------------------
* test configurationreader functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/
void Testpublishtable::test_configurationreader_fail()
{
	mPublishTableController->configurationReaderJson();
	string check = mPublishTableController->m_string_pir_configure;
	string compare = "{\"protocol\":{\"id\":2,\"name\":\"PIR\",\"type\":21},\"sensoridentifier\" :\"PIR\",\"type\":\"sensor\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_sens_t_configure;
	compare = "{\"protocol\":{\"id\":1,\"name\":\"SunsetPassAPI\",\"type\":20},\"sensoridentifier\":\"SunsetPassAPI_1\",\"type\":\"sensor\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_wifi_configure;
	compare = "{\"protocol\":{\"id\":3,\"name\":\"WiFi\",\"type\":22},\"sensoridentifier\":\"WiFi\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_ble_configure;
	compare = "{\"protocol\":{\"id\":4,\"name\":\"BLE\",\"type\":23},\"sensoridentifier\":\"BLE\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_dex_configure;
	compare = "{\"protocol\":{\"id\":6,\"name\":\"DEX\",\"type\":25},\"sensoridentifier\":\"DEX\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_Realsense_configure;
	compare = "{\"protocol\":{\"id\":5,\"name\":\"RealSense\",\"type\":24},\"sensoridentifier\":\"RealSense\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_stock_configure;
	compare = "{\"protocol\":{\"id\":8,\"name\":\"Stock\",\"type\":27},\"sensoridentifier\":\"Stock\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
	check = mPublishTableController->m_string_Trans_configure;
	compare = "{\"protocol\":{\"id\":7,\"name\":\"Transaction\",\"type\":26},\"sensoridentifier\":\"Transaction\",\"type\":\"Module\",\"valuetype\":\"0xFF82\"}";
	CPPUNIT_ASSERT_EQUAL(compare, check);
}
/**
-------------------------------------------------------------------------------
test_PublishControllerUpdatingBLE
-------------------------------------------------------------------------------
* Publish controller updating BLE functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/

void Testpublishtable::test_PublishControllerUpdatingBLE()
{
	mPublishTableController->PublishControllerUpdatingBLE();
	
	bool check = mPublishTableController->m_ptcDatabaseCtn.isOpen();
	if (PTC_DB_STATUS_CODE check1 = PTC_SELECT_QUERY_SUCCESS)
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
							if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
							{
								if (sensordata == "BLE")
									{
									value_json = mapiter.value().toString();
									cout << value_json.toStdString();
									}
							}
					}
				}
			}
		}
		string compare = "{\"DeviceName\":\"d1\",\"MAC\":\"m1\",\"RSSI\":1,\"Sensor_MAC\":\"s1\",\"Sensor_Name\":\"s1\",\"Timestamp\":1471352036}";
		CPPUNIT_ASSERT_EQUAL(compare,value_json.toStdString());
	}
}

/**
-------------------------------------------------------------------------------
test_PublishControllerUpdatingwifi
-------------------------------------------------------------------------------
* Publish controller updating wifi functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/
void Testpublishtable::test_PublishControllerUpdatingwifi()
{
	mPublishTableController->PublishControllerUpdatingWiFi();
	bool check = mPublishTableController->m_ptcDatabaseCtn.isOpen();
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "WIFI")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string compare = "{\"Channel_No\":1,\"End_time\":1471352038,\"MAC_ID\":\"d1\",\"Noise\":1,\"NucId\":1,\"RSSI\":1,\"Start_time\":1471352036}";
		CPPUNIT_ASSERT_EQUAL(compare, value_json.toStdString());
		
	}
}

/**
-------------------------------------------------------------------------------
test_PublishControllerUpdatingpir
-------------------------------------------------------------------------------
* Publish controller updating pir functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/

void Testpublishtable::test_PublishControllerUpdatingpir()
{
	mPublishTableController->PublishControllerUpdatingPIR();
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "PIR")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string compare = "{\"Timestamp\":1471352036,\"count_left_to_right\":1,\"count_right_to_left\":2}";
		CPPUNIT_ASSERT_EQUAL(compare, value_json.toStdString());
	}
	
}
/**
-------------------------------------------------------------------------------
test_PublishControllerUpdatingSens_T
-------------------------------------------------------------------------------
*	Publish controller updating Sens_T functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/

void Testpublishtable::test_PublishControllerUpdatingSens_T()
{
	mPublishTableController->PublishCtlrUpdatingSensorTelemetry();
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "Sens_T")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string compare = "{\"SensorIdentifier\":\"2\",\"SensorValue\":1,\"Timestamp\":1471352036}";
		CPPUNIT_ASSERT_EQUAL(compare, value_json.toStdString());
	}
	
}

/**
-------------------------------------------------------------------------------
test_PublishControllerUpdatingRealsense
-------------------------------------------------------------------------------
* Publish controller updating Realsense functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/

void Testpublishtable::test_PublishCtlrUpdatingRealsense()
{
	mPublishTableController->PublishCtlrUpdatingRealsense();
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "Sens_T")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string compare = "{\"SensorIdentifier\":\"2\",\"SensorValue\":1,\"Timestamp\":1471352036}";
		CPPUNIT_ASSERT_EQUAL(compare, value_json.toStdString());
	}
}

/**
-------------------------------------------------------------------------------
test_PublishControllerUpdatingRealsense
-------------------------------------------------------------------------------
*	Publish controller updating Realsense functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/

void Testpublishtable::test_PublishControllerUpdatingTransaction()
{
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";

	mPublishTableController->PublishControllerUpdatingTransaction(jsondata);
	bool check = mPublishTableController->m_ptcDatabaseCtn.isOpen();
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "Trans")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string expectedstring = "{\"CampaignId\":1,\"Column_ID\":\"1\",\"DeviceID\":\"\",\"Product_ID\":730706,\"Product_Price\":100,\"Shelf_ID\":\"A\",\"TimeStamp\":1368550052,\"VendorID\":\"2039\",\"productDiscountPrice\":0.5,\"productPaymentMethod\":1}";
		CPPUNIT_ASSERT_EQUAL(expectedstring, value_json.toStdString());
	}
	
}

/**
-------------------------------------------------------------------------------
test_PublishControllerUpdatingstock
-------------------------------------------------------------------------------
* Publish controller updating Realsense functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/

void Testpublishtable::test_PublishControllerUpdatingstock()
{
	string jsondata ="{\"stockinfo\":[{\"Stock\":20,\"Product_ID\":730706},{\"Stock\":10,\"Product_ID\":730707},{\"Stock\":20,\"Product_ID\":730708}]}";

	mPublishTableController->PublishControllerUpdatingstock(jsondata);
	bool check = mPublishTableController->m_ptcDatabaseCtn.isOpen();
	PTC_DB_STATUS_CODE check1 = PTC_SELECT_QUERY_SUCCESS;
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "stock")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string expectedstring = "{\"ProductCurrentLevel\":\"20\"}";
		CPPUNIT_ASSERT_EQUAL(expectedstring, value_json.toStdString());
	}

}

/**
-------------------------------------------------------------------------------
test_PublishControllerUpdatingVendingDB
-------------------------------------------------------------------------------
* Publish controller updating Realsense functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/

void Testpublishtable::test_PublishControllerUpdatingVendingDB()
{
	pair<string, qlonglong> pair_value = mPublishTableController->PublishControllerUpdatingVendingDB();
	//bool check = mPublishTableController->m_ptcDatabaseCtn.isOpen();
	qlonglong check1 = pair_value.second;
		qlonglong check = 1399032266;
		CPPUNIT_ASSERT_EQUAL(check,check1);
}

/**
-------------------------------------------------------------------------------
test_PublishControllerUpdatingdex
-------------------------------------------------------------------------------
* Publish controller updating Realsense functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/

void Testpublishtable::test_PublishControllerUpdatingdex()
{
	string JsonData = "{\"test\":[{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}]}";
	mPublishTableController->PublishControllerUpdatingdex(JsonData);
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "Dex")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string expectedstring = "{\"Dex\":\"VA1*0*0*0*0\nCA3*0*0*0*0*0*0*0\nCA4*0\nCA15*0\nDA2*0*0*0*0\nG85*42F8\nSE*6*0001\nDXE*1*1\"}";
		CPPUNIT_ASSERT_EQUAL(expectedstring, value_json.toStdString());
	}


}



/**
-------------------------------------------------------------------------------
test_UpdatePublishTable
-------------------------------------------------------------------------------
* Publish controller updating Publishtable functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/
void Testpublishtable::test_UpdatePublishTable()
{
	//string JsonData = "{\"test\":[{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}]}";
	
	mPublishTableController->UpdatePublishTable();
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "Dex")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string expectedstring = "{\"Dex\":\"VA1*0*0*0*0\nCA3*0*0*0*0*0*0*0\nCA4*0\nCA15*0\nDA2*0*0*0*0\nG85*42F8\nSE*6*0001\nDXE*1*1\"}";
		CPPUNIT_ASSERT_EQUAL(expectedstring, value_json.toStdString());
	}
	
	
}

void Testpublishtable::test_executeQuery_pass()
{
	PublishTableController * Instance = PublishTableController::getInstance();
	const char* dataInsertionQuery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALUES (:Timestamp, :Type, :Value, :isSent)";
	map<string, string> args_dataInsertion;
	//QString present_time_string = QString::number(std::time(NULL));
	bool check = 0;
	args_dataInsertion[":Timestamp"] = "1234";// present_time_string.toStdString();
	args_dataInsertion[":Type"] = "PIR";
	args_dataInsertion[":Value"] = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}]";
	args_dataInsertion[":isSent"] = "0";
	mPublishTableController->PublishControllerUpdatingPIR();
	PTC_DB_STATUS_CODE retStatus = mPublishTableController->executeQuery(mPublishTableController->m_ptcDatabaseCtn, dataInsertionQuery, args_dataInsertion);
	cout << "RET STAT " << retStatus;
	if (PTC_SELECT_QUERY_SUCCESS == retStatus)
	{
		check = 1;
	}
	CPPUNIT_ASSERT_EQUAL(bool(1), check);
}


void Testpublishtable::test_executeQuery_fail()
{
	
//	PublishTableController * Instance = PublishTableController::getInstance();
	const char* dataInsertionQuery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALUES (:Timestamp, :Type, :Value, :isSent)";
	map<string, string> args_dataInsertion;
	//QString present_time_string = QString::number(std::time(NULL));
	bool check =0 ;
	args_dataInsertion[":Timestamp"] = "1234";      // present_time_string.toStdString();
	args_dataInsertion[":Type"] = "query";
	args_dataInsertion[":Value"] = "{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}]";
	args_dataInsertion[":isSent"] = "0";	
	if (PTC_DB_QUERY_FAILURE == mPublishTableController->executeQuery(QSqlDatabase::addDatabase("QMYSQL", "PTC_CONNECTION1"), dataInsertionQuery, args_dataInsertion))
	{
		check = 1;
	}
		CPPUNIT_ASSERT_EQUAL(bool(1), check);
		
}







/**
-------------------------------------------------------------------------------
test_UpdatePublishTable
-------------------------------------------------------------------------------
* Publish controller updating Publishtable functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/
void Testpublishtable::test_PublishTableInsertion()
{
	string passjson = "{\"test\":[{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}]}";
	mPublishTableController->PublishTableInsertion(passjson,"test");
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "test")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string expectedstring = "{\"test\":[{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}]}";
		CPPUNIT_ASSERT_EQUAL(expectedstring, value_json.toStdString());
	}
	
}
/**
-------------------------------------------------------------------------------
test_executeselectquery
-------------------------------------------------------------------------------
* Publish controller updating Publishtable functions for test cases.
*
*	@param		none
*
*	@return		none
*
*/
void Testpublishtable::test_executeselectquery()
{
	string passjson = "{\"test\":[{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}]}";
	mPublishTableController->PublishTableInsertion(passjson, "test");
	{
		map<string, string> args_dataRetrieve;
		QList<QMap<QString, QVariant>>::iterator listofmapiter;
		QMap<QString, QVariant>::iterator mapiter;
		qlonglong timestamp_long;
		QString value_json = "";
		QString sensordata;
		string compare1;

		const char* dataRetrieveQuery = "select TimeStamp, Type, Value,Message_ID,isSent from publish_table where isSent = 0 ";

		PublishTableController *p_publishTableController = PublishTableController::getInstance();

		QList <QMap<QString, QVariant>>	recs;
		map<string, string> args_dataUpdation;

		p_publishTableController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listofmapiter = recs.begin(); listofmapiter != recs.end(); listofmapiter++)
			{
				for (mapiter = listofmapiter->begin(); mapiter != listofmapiter->end(); mapiter++)
				{
					QString key = mapiter.key();

					if (key == "Type")
					{
						sensordata = mapiter.value().toString();
					}
					if (key == "Value")
					{
						if (mapiter.value().toString().at(0) == '{' || mapiter.value().toString().at(0) == '[')
						{
							if (sensordata == "test")
							{
								value_json = mapiter.value().toString();
								cout << value_json.toStdString();
							}
						}
					}
				}
			}
		}
		string expectedstring = "{\"test\":[{\"DeviceName\":\"sriram\",\"MAC\":1234,\"RSSI\":4321,\"Sensor_Name\":\"sensor\",\"Timestamp\":4323232}]}";
		CPPUNIT_ASSERT_EQUAL(expectedstring, value_json.toStdString());
	}


}


CPPUNIT_TEST_SUITE_REGISTRATION(Testpublishtable);

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
		ofstream xmlFileOut("Test_Results_PTC_UT.xml");
		XmlOutputter xmlOut(&collectedresults, xmlFileOut);
		xmlOut.write();

		// return 0 if tests were successful
		return collectedresults.wasSuccessful() ? 0 : 1;
	}


