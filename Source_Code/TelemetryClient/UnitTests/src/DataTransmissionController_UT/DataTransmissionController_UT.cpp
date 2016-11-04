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

#include "DataTransmissionController.h"
#include "SensorTelemetryController.h"
#include "ConfigurationReader.h"
#include "PublishTableController.h"
#include "MessageHandler.h"
#include "IDEX.h"
#include "datatranslator.h"
#include "MessageQueue.h"

#if defined(REALSENSE)
#include "RealSensePeopleCounter.h"
#endif

#include "pir.h"

#if defined(__linux__)
#include "timer_lib.h"
#include "bluetooth.h"
#include "bluetoothhandler.h"
#include "wifi_ppl_counter.h"
#endif

using namespace CppUnit;
using namespace std;

enum DB_STATUS_CODE_UT
{
	DTC_UT_DB_OPEN_SUCCESS = 0,			///< Success
	DTC_UT_CONNECTION_FAILURE,				///< Failed to open the main connection
	INSERT_CONNECTION_FAILURE,		///< Failed to open the Insert query connection
	INSERT_QUERY_SUCCESS,			///< Insert Query Success
	INSERT_QUERY_FAILURE,			///< Insert Query Failure	
};

QSqlDatabase DTC_UT_DatabaseCtn;
QSqlDatabase DTC_UT_InsertCtn;

class DataTransmissionController_UT :
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(DataTransmissionController_UT);
	CPPUNIT_TEST(test_getConnection_tc_01);
	CPPUNIT_TEST(test_execSelectQuery_tc_02);
	CPPUNIT_TEST(test_execSelectQuery_tc_03);
	CPPUNIT_TEST(test_execSelectQuery_tc_04);
	CPPUNIT_TEST(test_execUpdateQuery_tc_05);
	CPPUNIT_TEST(test_execUpdateQuery_tc_06);
	CPPUNIT_TEST(test_execUpdateQuery_tc_07);
	CPPUNIT_TEST(test_StringSplitter_tc_08);
	CPPUNIT_TEST(test_StringSplitter_tc_09);
	CPPUNIT_TEST(test_is_number_tc_10);
	CPPUNIT_TEST(test_is_number_tc_11);
	CPPUNIT_TEST(test_is_number_tc_12);
	CPPUNIT_TEST(test_ProcessReceivedMessage_tc_13);
	CPPUNIT_TEST(test_ProcessReceivedMessage_tc_14);
	CPPUNIT_TEST(test_PushToMessageQueue_tc_15);
	CPPUNIT_TEST(test_PushToMessageQueue_tc_16);
	CPPUNIT_TEST(DatabasePurgingHandler_tc_17);
	CPPUNIT_TEST(DatabasePurgingHandler_tc_18);
	CPPUNIT_TEST(ReadPublishTable_tc_19);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test_getConnection_tc_01(void);
	void test_execSelectQuery_tc_02(void);
	void test_execSelectQuery_tc_03(void);
	void test_execSelectQuery_tc_04(void);
	void test_execUpdateQuery_tc_05(void);
	void test_execUpdateQuery_tc_06(void);
	void test_execUpdateQuery_tc_07(void);
	void test_StringSplitter_tc_08(void);
	void test_StringSplitter_tc_09(void);
	void test_is_number_tc_10(void);
	void test_is_number_tc_11(void);
	void test_is_number_tc_12(void);
	void test_ProcessReceivedMessage_tc_13(void);
	void test_ProcessReceivedMessage_tc_14(void);
	void test_PushToMessageQueue_tc_15(void);
	void test_PushToMessageQueue_tc_16(void);
	void DatabasePurgingHandler_tc_17(void);
	void DatabasePurgingHandler_tc_18(void);
	void ReadPublishTable_tc_19(void);

private:
	DataTransmissionController *m_DataTransmissionController;
};

void DataTransmissionController_UT::setUp()
{
	m_DataTransmissionController = new DataTransmissionController();
}

void DataTransmissionController_UT::tearDown()
{
	delete m_DataTransmissionController;
}

DB_STATUS_CODE_UT getConnection(string HostName, string DatabaseName, string UserName, string Password, int Port)
{
	//connection configuration should done only one time for db
	if (!DTC_UT_DatabaseCtn.isOpen()) {
		DTC_UT_DatabaseCtn = QSqlDatabase::addDatabase("QMYSQL", "DTCTestConnection");
		DTC_UT_DatabaseCtn.setHostName(QString::fromStdString(HostName));
		DTC_UT_DatabaseCtn.setDatabaseName(QString::fromStdString(DatabaseName));
		DTC_UT_DatabaseCtn.setUserName(QString::fromStdString(UserName));
		DTC_UT_DatabaseCtn.setPassword(QString::fromStdString(Password));
		DTC_UT_DatabaseCtn.setPort(Port);
		if (!DTC_UT_DatabaseCtn.open())
		{
			std::cout << "DTC_UT :: Failed to open database:" << endl;
			return DTC_UT_CONNECTION_FAILURE;
		}
		else
		{
			DTC_UT_InsertCtn = QSqlDatabase::cloneDatabase(DTC_UT_DatabaseCtn, "DTC_UT_Insert_Connection");			

			if (!DTC_UT_InsertCtn.open())
			{
				cout << "[DTC_UT::getConnection]::Failed to open the insert query connection" << endl;
				return INSERT_CONNECTION_FAILURE;
			}			
		}
	}
	return DTC_UT_DB_OPEN_SUCCESS;
}

DB_STATUS_CODE_UT  execInsertQuery(const string &strQuery, map<string, string> &arguments)
{
	QVariantMap args;
	QString key;
	QString value;
	QString sql = QString::fromStdString(strQuery);
	for (map<string, string>::iterator ii = arguments.begin(); ii != arguments.end(); ++ii)
	{
		key = QString::fromStdString((*ii).first);
		value = QString::fromStdString((*ii).second);
		args.insert(key, value);
	}
	if (DTC_UT_DatabaseCtn.isOpen())
	{
		QSqlQuery *query = new QSqlQuery(DTC_UT_DatabaseCtn);
		if (query->prepare(sql))
		{
			foreach(QString keyIndex, args.keys()) query->bindValue(keyIndex, args.value(keyIndex));
			if (!query->exec())
			{
				std::cout << "[DTC_UT::Insert Query]::Failed to execute query with errorcode - "
					<< query->lastError().text().toStdString() << endl;
				delete query;
				return INSERT_QUERY_FAILURE;
			}
		}
		else
		{
			std::cout << "[DTC_UT::Insert Query]::Failed to create prepared statement with errorcode - "
				<< query->lastError().text().toStdString() << endl;
			delete query;
			return INSERT_QUERY_FAILURE;
		}
		delete query;
		return INSERT_QUERY_SUCCESS;
	}
	return INSERT_QUERY_FAILURE;
}

void DataTransmissionController_UT::test_getConnection_tc_01()
{
	string HostName = "localhost";
	string DatabaseName = "sensor_db";
	string UserName = "root";
	string Password = "intel";
	int Port = 3306;
	int success = 1;

	if (DTC_UT_DB_OPEN_SUCCESS != m_DataTransmissionController->getConnection(HostName, DatabaseName, UserName, Password, Port))
	{
		success = 0;
	}

	CPPUNIT_ASSERT_EQUAL(1, success);
}

void DataTransmissionController_UT::test_execSelectQuery_tc_02(void)
{
	int success = 1;
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALUES (:Timestamp, :Type, :Value, :isSent)";
	char* dataRetrieveQuery = "select * from publish_table";
	char* dataDeletionQuery = "truncate publish_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	args_dataInsertion[":Timestamp"] = "987654321";
	args_dataInsertion[":Type"] = "PIR";
	args_dataInsertion[":Value"] = "123";
	args_dataInsertion[":isSent"] = "1";

	if (DTC_UT_DB_OPEN_SUCCESS == getConnection("localhost", "sensor_db", "root", "intel", 3306))
	{
		if (DTC_UPDATE_QUERY_SUCCESS == m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataDeletionQuery, args_dataDeletion))
		{
			success = success * (INSERT_QUERY_SUCCESS == execInsertQuery(dataInsertionquery, args_dataInsertion));

			m_DataTransmissionController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
			if (recs.size() > 0)
			{
				for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
				{
					for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
					{
						if (!QString::compare(mapIter.key(), QString::fromStdString("Timestamp"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("987654321"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("Type"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("PIR"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("Value"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("123"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("isSent"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1"));
						}
					}
				}
			}

			CPPUNIT_ASSERT_EQUAL(1, success);
		}
		else
		{
			std::cout << "[DTC_UT::tc_03]::Insert query failed" << endl;
		}
	}
	else
	{
		std::cout << "[DTC_UT::tc_03]::Failed to open the connection" << endl;
	}
}

void DataTransmissionController_UT::test_execSelectQuery_tc_03(void)
{
	int success = 1;
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALUES (:Timestamp, :Type, :Value, :isSent)";
	char* dataRetrieveQuery = "select * from publish_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	args_dataInsertion[":Timestamp"] = "123456789";
	args_dataInsertion[":Type"] = "BLE";
	args_dataInsertion[":Value"] = "100";
	args_dataInsertion[":isSent"] = "0";
	int index = 1;

	if (DTC_UT_DB_OPEN_SUCCESS == getConnection("localhost", "sensor_db", "root", "intel", 3306))
	{		
		success = success * (INSERT_QUERY_SUCCESS == execInsertQuery(dataInsertionquery, args_dataInsertion));

		m_DataTransmissionController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
		if (recs.size() > 0)
		{
			for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
			{
				for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
				{
					if (!QString::compare(mapIter.key(), QString::fromStdString("Timestamp"), Qt::CaseSensitive))
					{
						if (index == 1)
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("987654321"));
						}
						else
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("123456789"));
						}
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("Type"), Qt::CaseSensitive))
					{
						if (index == 1)
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("PIR"));
						}
						else
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("BLE"));
						}
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("Value"), Qt::CaseSensitive))
					{
						if (index == 1)
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("123"));
						}
						else
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("100"));
						}
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("isSent"), Qt::CaseSensitive))
					{
						if (index == 1)
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1"));
						}
						else
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("0"));
						}
					}
				}
				index++;
			}			
		}
		CPPUNIT_ASSERT_EQUAL(1, success);		
	}
	else
	{
		std::cout << "[DTC_UT::tc_04]::Failed to open the connection" << endl;
	}
}

void DataTransmissionController_UT::test_execSelectQuery_tc_04(void)
{
	int success = 1;
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALUES (:Timestamp, :Type, :Value, :isSent)";
	char* dataRetrieveQuery = "select * fro publish_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	QList<QMap<QString, QVariant>>  recs;

	if (DTC_UT_DB_OPEN_SUCCESS == getConnection("localhost", "sensor_db", "root", "intel", 3306))
	{
		if (DTC_SELECT_QUERY_FAILURE == m_DataTransmissionController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs))
		{
			success = 1;
		}
		else
		{
			success = 0;
		}		
		CPPUNIT_ASSERT_EQUAL(1, success);
	}
	else
	{
		std::cout << "[DTC_UT::tc_04]::Failed to open the connection" << endl;
	}
}


void DataTransmissionController_UT::test_execUpdateQuery_tc_05(void)
{
	int success = 1;
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALUES (:Timestamp, :Type, :Value, :isSent)";
	char* dataRetrieveQuery = "select * from publish_table";
	char* dataDeletionQuery = "truncate publish_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	args_dataInsertion[":Timestamp"] = "987654321";
	args_dataInsertion[":Type"] = "PIR";
	args_dataInsertion[":Value"] = "123";
	args_dataInsertion[":isSent"] = "1";

	if (DTC_UT_DB_OPEN_SUCCESS == getConnection("localhost", "sensor_db", "root", "intel", 3306))
	{
		if (DTC_UPDATE_QUERY_SUCCESS == m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataDeletionQuery, args_dataDeletion))
		{
			success = success * (DTC_UPDATE_QUERY_SUCCESS == m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataInsertionquery, args_dataInsertion));

			m_DataTransmissionController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
			if (recs.size() > 0)
			{
				for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
				{
					for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
					{
						if (!QString::compare(mapIter.key(), QString::fromStdString("Timestamp"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("987654321"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("Type"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("PIR"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("Value"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("123"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("isSent"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1"));
						}
					}
				}
			}

			CPPUNIT_ASSERT_EQUAL(1, success);
		}
		else
		{
			std::cout << "[DTC_UT::tc_03]::Insert query failed" << endl;
		}
	}
	else
	{
		std::cout << "[DTC_UT::tc_03]::Failed to open the connection" << endl;
	}
}

void DataTransmissionController_UT::test_execUpdateQuery_tc_06(void)
{
	int success = 1;
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALUES (:Timestamp, :Type, :Value, :isSent)";
	char* dataRetrieveQuery = "select * from publish_table";
	char* dataDeletionQuery = "truncate publish_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	args_dataInsertion[":Timestamp"] = "";
	args_dataInsertion[":Type"] = "PIR";
	args_dataInsertion[":Value"] = "123";
	args_dataInsertion[":isSent"] = "1";

	if (DTC_UT_DB_OPEN_SUCCESS == getConnection("localhost", "sensor_db", "root", "intel", 3306))
	{
		if (DTC_UPDATE_QUERY_SUCCESS == m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataDeletionQuery, args_dataDeletion))
		{
			success = success * (DTC_UPDATE_QUERY_SUCCESS == m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataInsertionquery, args_dataInsertion));

			m_DataTransmissionController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
			if (recs.size() > 0)
			{
				success = 0;
			}
			else
			{
				success = 1;
			}
			CPPUNIT_ASSERT_EQUAL(1, success);
		}
		else
		{
			std::cout << "[DTC_UT::tc_03]::Insert query failed" << endl;
		}
	}
	else
	{
		std::cout << "[DTC_UT::tc_03]::Failed to open the connection" << endl;
	}
}

void DataTransmissionController_UT::test_execUpdateQuery_tc_07(void)
{
	int success = 1;
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, isSent) VALU (:Timestamp, :Type, :Value, :isSent)";
	char* dataRetrieveQuery = "select * from publish_table";
	char* dataDeletionQuery = "truncate publish_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	args_dataInsertion[":Timestamp"] = "123456";
	args_dataInsertion[":Type"] = "PIR";
	args_dataInsertion[":Value"] = "123";
	args_dataInsertion[":isSent"] = "1";

	if (DTC_UT_DB_OPEN_SUCCESS == getConnection("localhost", "sensor_db", "root", "intel", 3306))
	{
		if (DTC_UPDATE_QUERY_SUCCESS == m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataDeletionQuery, args_dataDeletion))
		{
			success = success * (DTC_UPDATE_QUERY_FAILURE == m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataInsertionquery, args_dataInsertion));

			CPPUNIT_ASSERT_EQUAL(1, success);
		}
		else
		{
			std::cout << "[DTC_UT::tc_03]::Insert query failed" << endl;
		}
	}
	else
	{
		std::cout << "[DTC_UT::tc_03]::Failed to open the connection" << endl;
	}
}

void DataTransmissionController_UT::test_StringSplitter_tc_08(void)
{
	string sampleMessage = "Timestamp:MessageId:Value";
	int success = 1;

	vector<string> tokens = m_DataTransmissionController->StringSplitter(sampleMessage, ':');

	for (string iter : tokens)
	{
		if (iter == "Timestamp" || iter == "MessageId" || iter == "Value")
		{
			success = success * 1;
		}
		else
		{
			success = success * 0;
		}
	}
	CPPUNIT_ASSERT_EQUAL(1, success);
}

void DataTransmissionController_UT::test_StringSplitter_tc_09(void)
{
	string sampleMessage = "\"Timestamp\":\"12345\",\"Value\":\"123\"";
	int success = 1;
	int count = 0;

	vector<string> tokens = m_DataTransmissionController->StringSplitter(sampleMessage, '\"');

	for (string iter : tokens)
	{
		if (iter == "Timestamp" || iter == "12345" || iter == "Value" || iter == "123" || iter == ":" ||
			iter == "," || iter == "")
		{
			success = success * 1;
		}
		else
		{
			success = success * 0;
		}
	}
	CPPUNIT_ASSERT_EQUAL(1, success);
}


void DataTransmissionController_UT::test_is_number_tc_10(void)
{
	const string message = "123456.789";

	if (true == m_DataTransmissionController->is_number(message))
	{
		CPPUNIT_ASSERT_EQUAL(1, 1);
	}
	else
	{
		CPPUNIT_ASSERT_EQUAL(1, 0);
	}	
}

void DataTransmissionController_UT::test_is_number_tc_11(void)
{
	const string message = "12A345";

	if (false == m_DataTransmissionController->is_number(message))
	{
		CPPUNIT_ASSERT_EQUAL(1, 1);
	}
	else
	{
		CPPUNIT_ASSERT_EQUAL(1, 0);
	}
}

void DataTransmissionController_UT::test_is_number_tc_12(void)
{
	const string message = "1234#5";

	if (false == m_DataTransmissionController->is_number(message))
	{
		CPPUNIT_ASSERT_EQUAL(1, 1);
	}
	else
	{
		CPPUNIT_ASSERT_EQUAL(1, 0);
	}
}

void DataTransmissionController_UT::test_ProcessReceivedMessage_tc_13(void)
{
	string messgeArrived = "{\"ack\":[{\"protocolid\":1, \"received\": [\"98765432.1\"]}]}";
	char* dataRetrieveQuery = "select * from publish_table";
	MessageList *messageListInstance = MessageList::getInstance();
	messageListInstance->Add(2, messgeArrived);	
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, Message_ID, isSent) VALUES (:Timestamp, :Type, :Value, :Message_ID, :isSent)";
	char* dataDeletionQuery = "truncate publish_table";
	map<string, string> args_dataDeletion;
	args_dataInsertion[":Timestamp"] = "98765432";
	args_dataInsertion[":Type"] = "PIR";
	args_dataInsertion[":Value"] = "123";
	args_dataInsertion[":isSent"] = "0";
	args_dataInsertion[":Message_ID"] = "1";
	int success = 1;

	m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataDeletionQuery, args_dataDeletion);

	if (INSERT_QUERY_SUCCESS == execInsertQuery(dataInsertionquery, args_dataInsertion))
	{
		m_DataTransmissionController->ProcessReceivedMessage();

		m_DataTransmissionController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
			{
				for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
				{
					if (!QString::compare(mapIter.key(), QString::fromStdString("Timestamp"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("98765432"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("Type"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("PIR"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("Value"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("123"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("isSent"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("Message_ID"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1"));
					}
				}
			}
		}
		CPPUNIT_ASSERT_EQUAL(1, success);
	}
}


void DataTransmissionController_UT::test_ProcessReceivedMessage_tc_14(void)
{
	string messgeArrived = "{\"ack\":[{\"protocolid\":1, \"received\": [\"1234567.89\"]}]}";
	char* dataRetrieveQuery = "select * from publish_table";
	MessageList *messageListInstance = MessageList::getInstance();
	messageListInstance->Add(2, messgeArrived);
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, Message_ID, isSent) VALUES (:Timestamp, :Type, :Value, :Message_ID, :isSent)";
	char* dataDeletionQuery = "truncate publish_table";
	map<string, string> args_dataDeletion;
	args_dataInsertion[":Timestamp"] = "1234567";
	args_dataInsertion[":Type"] = "BLE";
	args_dataInsertion[":Value"] = "100";
	args_dataInsertion[":isSent"] = "0";
	args_dataInsertion[":Message_ID"] = "89";
	int success = 1;

	m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataDeletionQuery, args_dataDeletion);

	if (INSERT_QUERY_SUCCESS == execInsertQuery(dataInsertionquery, args_dataInsertion))
	{
		m_DataTransmissionController->ProcessReceivedMessage();

		m_DataTransmissionController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
			{
				for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
				{
					if (!QString::compare(mapIter.key(), QString::fromStdString("Timestamp"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1234567"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("Type"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("BLE"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("Value"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("100"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("isSent"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("Message_ID"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("89"));
					}
				}
			}
		}
		CPPUNIT_ASSERT_EQUAL(1, success);
	}
}

void DataTransmissionController_UT::test_PushToMessageQueue_tc_15(void)
{
	string message = "Message Arrived";
	MessageList *messageListInstance = MessageList::getInstance();

	m_DataTransmissionController->PushToMessageQueue(message);

	std::string messageQueued = messageListInstance->TakeFirst(1);

	if (message == messageQueued)
	{
		CPPUNIT_ASSERT_EQUAL(1, 1);
	}
	else
	{
		CPPUNIT_ASSERT_EQUAL(1, 0);
	}

}

void DataTransmissionController_UT::test_PushToMessageQueue_tc_16(void)
{
	string message = "{\"ack\":[{\"protocolid\":1, \"received\": [\"1234567.89\"]}]}";
	MessageList *messageListInstance = MessageList::getInstance();

	m_DataTransmissionController->PushToMessageQueue(message);

	std::string messageQueued = messageListInstance->TakeFirst(1);

	if (message == messageQueued)
	{
		CPPUNIT_ASSERT_EQUAL(1, 1);
	}
	else
	{
		CPPUNIT_ASSERT_EQUAL(1, 0);
	}
}

void DataTransmissionController_UT::DatabasePurgingHandler_tc_17(void)
{
	char* dataRetrieveQuery = "select * from publish_table";
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, Message_ID, isSent) VALUES (:Timestamp, :Type, :Value, :Message_ID, :isSent)";
	char* dataDeletionQuery = "truncate publish_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	
	args_dataInsertion[":Timestamp"] = "1234567";
	args_dataInsertion[":Type"] = "BLE";
	args_dataInsertion[":Value"] = "100";
	args_dataInsertion[":isSent"] = "1";
	args_dataInsertion[":Message_ID"] = "89";
	int success = 1;

	m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataDeletionQuery, args_dataDeletion);

	if (INSERT_QUERY_SUCCESS == execInsertQuery(dataInsertionquery, args_dataInsertion))
	{

		m_DataTransmissionController->DatabasePurgingHandler();

		m_DataTransmissionController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
		if (recs.size() > 0)
		{
			CPPUNIT_ASSERT_EQUAL(1, 0);
		}
		else
		{
			CPPUNIT_ASSERT_EQUAL(1, 1);
		}
	}
}

void DataTransmissionController_UT::DatabasePurgingHandler_tc_18(void)
{
	char* dataRetrieveQuery = "select * from publish_table";
	char* dataInsertionquery = "INSERT INTO publish_table(Timestamp, Type, Value, Message_ID, isSent) VALUES (:Timestamp, :Type, :Value, :Message_ID, :isSent)";
	char* dataDeletionQuery = "truncate publish_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;

	args_dataInsertion[":Timestamp"] = "1234567";
	args_dataInsertion[":Type"] = "BLE";
	args_dataInsertion[":Value"] = "100";
	args_dataInsertion[":isSent"] = "0";
	args_dataInsertion[":Message_ID"] = "89";
	int success = 1;

	m_DataTransmissionController->execUpdateQuery(DTC_UT_InsertCtn, dataDeletionQuery, args_dataDeletion);

	if (INSERT_QUERY_SUCCESS == execInsertQuery(dataInsertionquery, args_dataInsertion))
	{

		m_DataTransmissionController->DatabasePurgingHandler();

		m_DataTransmissionController->execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
		if (recs.size() > 0)
		{
			CPPUNIT_ASSERT_EQUAL(1, 1);
		}
		else
		{
			CPPUNIT_ASSERT_EQUAL(1, 0);
		}
	}
}

void DataTransmissionController_UT::ReadPublishTable_tc_19(void)
{
	m_DataTransmissionController->ReadPublishTable();

	CPPUNIT_ASSERT_EQUAL(1, 1);
}

CPPUNIT_TEST_SUITE_REGISTRATION(DataTransmissionController_UT);

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
	ofstream xmlFileOut("Test_Results_DTC_UT.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	getchar();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}
