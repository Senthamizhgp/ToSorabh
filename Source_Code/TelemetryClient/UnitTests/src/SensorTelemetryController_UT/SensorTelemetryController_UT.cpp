// SensorTelemetryController_UT.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include <thread>

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

#include "SensorTelemetryController.h"
#include "ConfigurationReader.h"
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqldriver.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlfield.h>
#include <QtSql/qsqlindex.h>
#include <QtSql/qsqlquery.h>
#include <QtSql/qsqlrecord.h>
#include <QtSql/qsqlresult.h>
#include <QtCore/qstring.h>
#include <qmap.h>
#include <qstring.h>
#include <qvariant.h>
#include <qdebug.h>

using namespace CppUnit;
using namespace std;

extern void SensorEventHandler(int32_t devID, int32_t eventType, int32_t eventData1,
	int32_t eventData2, void *eventData3, int32_t reserved);

enum DB_STATUS_CODE
{
	DB_OPEN_SUCCESS = 0,			///< Success
	CONNECTION_FAILURE,				///< Failed to open the main connection
	SELECT_CONNECTION_FAILURE,		///< Failed to open the Select query connection
	UPDATE_CONNECTION_FAILURE,		///< Failed to open the Update query connection
	SELECT_QUERY_SUCCESS,			///< Select Query Success
	SELECT_QUERY_FAILURE,			///< Select Query Failure
	UPDATE_QUERY_SUCCESS,			///< Update Query Success
	UPDATE_QUERY_FAILURE			///< Update Query Failure
};

QSqlDatabase DatabaseCtn;
QSqlDatabase UpdateCtn;
QSqlDatabase SelectCtn;
QMap<QString, QVariant> selection_map;	///> For processing database result values


class SensorTelemetryController_UT :
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(SensorTelemetryController_UT);
	CPPUNIT_TEST(test_getConnection_tc_01);
	CPPUNIT_TEST(test_execInsertQuery_tc_02);
	CPPUNIT_TEST(test_execInsertQuery_tc_03);
	CPPUNIT_TEST(test_execInsertQuery_tc_04);
	CPPUNIT_TEST(test_execInsertQuery_tc_05);
	CPPUNIT_TEST(test_TimerEvent_tc_06);
	CPPUNIT_TEST(test_SensorEventHandler_tc_07);
	CPPUNIT_TEST(test_SensorEventHandler_tc_08);
	CPPUNIT_TEST(test_SensorEventHandler_tc_09);
	CPPUNIT_TEST(test_initSensorTelemetry_tc_10);
	CPPUNIT_TEST(test_initSensorTelemetry_tc_11);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test_getConnection_tc_01(void);
	void test_execInsertQuery_tc_02(void);
	void test_execInsertQuery_tc_03(void);
	void test_execInsertQuery_tc_04(void);
	void test_execInsertQuery_tc_05(void);
	void test_TimerEvent_tc_06(void);
	void test_SensorEventHandler_tc_07(void);
	void test_SensorEventHandler_tc_08(void);
	void test_SensorEventHandler_tc_09(void);
	void test_initSensorTelemetry_tc_10(void);
	void test_initSensorTelemetry_tc_11(void);

public:
	void setUp(void);
	void tearDown(void);

private:
	SensorTelemetryController *m_SensorTelemetryController;
};

void SensorTelemetryController_UT::setUp()
{
	m_SensorTelemetryController = new SensorTelemetryController();
}

void SensorTelemetryController_UT::tearDown()
{
	delete m_SensorTelemetryController;
}

DB_STATUS_CODE getConnection(string HostName, string DatabaseName, string UserName, string Password, int Port)
{
	//connection configuration should done only one time for db
	if (!DatabaseCtn.isOpen()) {
		DatabaseCtn = QSqlDatabase::addDatabase("QMYSQL", "SensorTelemetryTestConnection");
		DatabaseCtn.setHostName(QString::fromStdString(HostName));
		DatabaseCtn.setDatabaseName(QString::fromStdString(DatabaseName));
		DatabaseCtn.setUserName(QString::fromStdString(UserName));
		DatabaseCtn.setPassword(QString::fromStdString(Password));
		DatabaseCtn.setPort(Port);
		if (!DatabaseCtn.open())
		{
			std::cout << "SensorTelemetry_UT :: Failed to open database:" << endl;
			return CONNECTION_FAILURE;
		}
		else
		{
			SelectCtn = QSqlDatabase::cloneDatabase(DatabaseCtn, "Select_Connection");
			UpdateCtn = QSqlDatabase::cloneDatabase(DatabaseCtn, "Update_Connection");

			if (!SelectCtn.open())
			{
				cout << "[STC_UT::getConnection]::Failed to open the select query connection" << endl;
				return SELECT_CONNECTION_FAILURE;
			}
			if (!UpdateCtn.open())
			{
				cout << "[STC_UT::getConnection]::Failed to open the update query connection" << endl;
				return UPDATE_CONNECTION_FAILURE;
			}
		}
	}
	return DB_OPEN_SUCCESS;
}

DB_STATUS_CODE execSelectQuery(const string &strQuery,
	map<string, string> &arguments,
	QList<QMap<QString, QVariant>> &records)
{
	QVariantMap args;
	QString key;
	QVariant value;
	QString sql = QString::fromStdString(strQuery);
	records.clear();
	for (map<string, string>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter)
	{
		key = QString::fromStdString((*iter).first);
		value = QString::fromStdString((*iter).second);
		args.insert(key, value);

	}

	if (SelectCtn.isOpen())
	{
		QSqlQuery *query = new QSqlQuery(SelectCtn);
		if (query->prepare(sql))
		{
			foreach(QString key_index, args.keys()) query->bindValue(key_index, args.value(key_index));
			if (query->exec())
			{
				while (query->next())
				{
					int count = query->record().count();

					for (int index = 0; index < count; index++)
					{
						key = query->record().field(index).name();
						value = query->record().field(index).value();
						selection_map.insert(key, value);
					}

					records.push_back(selection_map);
					selection_map.clear();
				}
			}
			else
			{
				std::cout << "[STC_UT::Select Query]::Failed to execute query with errorcode - "
					<< query->lastError().text().toStdString() << endl;
				delete query;

				return SELECT_QUERY_FAILURE;
			}
		}
		else
		{
			std::cout << "[STC_UT::Select Query]::Failed to create prepare statement with errorcode - "
				<< query->lastError().text().toStdString() << endl;
			delete query;

			return SELECT_QUERY_FAILURE;
		}

		delete query;

		return SELECT_QUERY_SUCCESS;
	}

	return SELECT_QUERY_FAILURE;
}

DB_STATUS_CODE execUpdateQuery(QSqlDatabase connectionName,
	const string &strQuery,
	map<string, string> &arguments)
{
	QVariantMap args;
	QString key;
	QString value;
	QString sql = QString::fromStdString(strQuery);
	for (map<string, string>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter)
	{
		key = QString::fromStdString((*iter).first);
		value = QString::fromStdString((*iter).second);
		args.insert(key, value);
	}
	if (connectionName.isOpen())
	{
		QSqlQuery *query = new QSqlQuery(connectionName);
		if (query->prepare(sql))
		{
			foreach(QString key_index, args.keys()) query->bindValue(key_index, args.value(key_index));

			if (!query->exec())
			{
				std::cout << "[STC_UT::Update Query]::Failed to execute query with errorcode - "
					<< query->lastError().text().toStdString() << endl;
				delete query;
				return UPDATE_QUERY_FAILURE;
			}
		}
		else
		{
			std::cout << "[STC_UT::Update Query]::Failed to create prepared statement with errorcode - "
				<< query->lastError().text().toStdString() << endl;
			delete query;
			return UPDATE_QUERY_FAILURE;
		}
		delete query;
		return UPDATE_QUERY_SUCCESS;
	}
	return UPDATE_QUERY_FAILURE;
}


void SensorTelemetryController_UT::test_getConnection_tc_01()
{
	string HostName = "localhost";
	string DatabaseName = "sensor_db";
	string UserName = "root";
	string Password = "intel";
	int Port = 3306;
	int success = 1;

	if (STC_DB_OPEN_SUCCESS != m_SensorTelemetryController->getConnection(HostName, DatabaseName, UserName, Password, Port))
	{
		success = 0;
	}

	CPPUNIT_ASSERT_EQUAL(1, success);	
}

void SensorTelemetryController_UT::test_execInsertQuery_tc_02()
{
	int success = 1;
	char* query = "INSERT INTO sensortelemetry_table(timestamp, deviceId, sensorName, sensorType, value, measurementUnit) VALUES (:timestamp, :deviceId, :sensorName, :sensorType, :value, :measurementUnit)";
	char* dataRetrieveQuery = "select * from sensortelemetry_table";
	char* dataDeletionQuery = "truncate sensortelemetry_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	args_dataInsertion[":timestamp"] = "123456789";
	args_dataInsertion[":deviceId"] = "Dev-12050";
	args_dataInsertion[":sensorName"] = "Temperature Sensor 3";
	args_dataInsertion[":value"] = "12345";
	args_dataInsertion[":measurementunit"] = "C";

	if (DB_OPEN_SUCCESS == getConnection("localhost", "sensor_db", "root", "intel", 3306))
	{
		if (UPDATE_QUERY_SUCCESS == execUpdateQuery(UpdateCtn, dataDeletionQuery, args_dataDeletion))
		{
			success = success * (STC_INSERT_QUERY_SUCCESS == m_SensorTelemetryController->execInsertQuery(query, args_dataInsertion));

			execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
			if (recs.size() > 0)
			{
				for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
				{
					for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
					{
						if (!QString::compare(mapIter.key(), QString::fromStdString("timestamp"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("123456789"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("deviceId"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Dev-12050"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorName"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Temperature Sensor 3"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("value"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("12345"));
						}
						else if (!QString::compare(mapIter.key(), QString::fromStdString("measurementunit"), Qt::CaseSensitive))
						{
							success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("C"));
						}
					}
				}
			}

			CPPUNIT_ASSERT_EQUAL(1, success);
		}
		else
		{
			std::cout << "[STC_UT::tc_02]::Update query failed" << endl;
		}
	}
	else
	{
		std::cout << "[STC_UT::tc_02]::Failed to open the connection" << endl;
	}
}

void SensorTelemetryController_UT::test_execInsertQuery_tc_03()
{
	int success = 1;
	char* query = "INSERT INTO sensortelemetry_table(timestamp, deviceId, sensorName, sensorType, value, measurementUnit) VALUES (:timestamp, :deviceId, :sensorName, :sensorType, :value, :measurementUnit)";
	char* dataRetrieveQuery = "select * from sensortelemetry_table";
	char* dataDeletionQuery = "truncate sensortelemetry_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	args_dataInsertion[":timestamp"] = "11111111";
	args_dataInsertion[":deviceId"] = "Dev-12000";
	args_dataInsertion[":sensorName"] = "Light Sensor";
	args_dataInsertion[":value"] = "2222";
	args_dataInsertion[":measurementunit"] = "A";

	if (UPDATE_QUERY_SUCCESS == execUpdateQuery(UpdateCtn, dataDeletionQuery, args_dataDeletion))
	{
		success = success * (STC_INSERT_QUERY_SUCCESS == m_SensorTelemetryController->execInsertQuery(query, args_dataInsertion));
		
		execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
		if (recs.size() > 0)
		{
			for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
			{
				for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
				{
					if (!QString::compare(mapIter.key(), QString::fromStdString("timestamp"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("11111111"));						
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("deviceId"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Dev-12000"));						
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorName"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Light Sensor"));						
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("value"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("2222"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("measurementunit"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("A"));
					}
				}
			}
		}

		CPPUNIT_ASSERT_EQUAL(1, success);
	}
}

void SensorTelemetryController_UT::test_execInsertQuery_tc_04()
{
	int success = 1;
	char* query = "INSERT INTO sensortelemetry_table(timestamp, deviceId, sensorName, sensorType, value, measurementUnit) VALUES (:timestamp, :deviceId, :sensorName, :sensorType, :value, :measurementUnit)";
	char* dataRetrieveQuery = "select * from sensortelemetry_table";
	char* dataDeletionQuery = "truncate sensortelemetry_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	args_dataInsertion[":timestamp"] = "987654321";
	args_dataInsertion[":deviceId"] = "Dev-12000";
	args_dataInsertion[":sensorName"] = "Motion Sensor";
	args_dataInsertion[":value"] = "12345";
	args_dataInsertion[":measurementunit"] = "kmph";

	if (UPDATE_QUERY_SUCCESS == execUpdateQuery(UpdateCtn, dataDeletionQuery, args_dataDeletion))
	{
		success = success * (STC_INSERT_QUERY_SUCCESS == m_SensorTelemetryController->execInsertQuery(query, args_dataInsertion));

		execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
		if (recs.size() > 0)
		{
			for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
			{
				for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
				{
					if (!QString::compare(mapIter.key(), QString::fromStdString("timestamp"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("987654321"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("deviceId"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Dev-12000"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorName"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Motion Sensor"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("value"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("12345"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("measurementunit"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("kmph"));
					}
				}
			}
		}
		CPPUNIT_ASSERT_EQUAL(1, success);
	}
}

void SensorTelemetryController_UT::test_execInsertQuery_tc_05()
{
	int success = 1;
	char* query = "INSERT INTO sensortelemetry_table(timestamp, deviceId, sensorName, sensorType, value, measurementUnit) VALUES (:timestamp, :deviceId, :sensorName, :sensorType, :value, :measurementUnit)";
	char* dataRetrieveQuery = "select * from sensortelemetry_table";
	char* dataDeletionQuery = "truncate sensortelemetry_table";
	map<string, string> args_dataInsertion;
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	args_dataInsertion[":timestamp"] = "111222333";
	args_dataInsertion[":deviceId"] = "Dev-1121";
	args_dataInsertion[":sensorName"] = "Pressure Sensor";
	args_dataInsertion[":value"] = "98765";
	args_dataInsertion[":measurementunit"] = "torr";

	if (UPDATE_QUERY_SUCCESS == execUpdateQuery(UpdateCtn, dataDeletionQuery, args_dataDeletion))
	{
		success = success * (STC_INSERT_QUERY_SUCCESS == m_SensorTelemetryController->execInsertQuery(query, args_dataInsertion));

		execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);
		if (recs.size() > 0)
		{
			for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
			{
				for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
				{
					if (!QString::compare(mapIter.key(), QString::fromStdString("timestamp"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("111222333"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("deviceId"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Dev-1121"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorName"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Pressure Sensor"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("value"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("98765"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("measurementunit"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("torr"));
					}
				}
			}
		}
		CPPUNIT_ASSERT_EQUAL(1, success);
	}
}


void SensorTelemetryController_UT::test_TimerEvent_tc_06()
{
	int success = 1;
	ConfigurationReader *m_configReader = ConfigurationReader::getInstance();
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

	m_sensorInfo[0].sensorFrequencyCount = 0;

	for (int j = 1; j <= 6; j = j + 1)
	{
		m_SensorTelemetryController->TimerEvent();

		for (int i = 0; i < 1; i++)
		{
			m_sensorInfo[i].sensorFrequencyCount = m_sensorInfo[i].sensorFrequencyCount + 5;

			success = success * (m_sensorInfo[i].sensorFrequencyCount == m_configReader->m_sensorInfo[i].sensorFrequencyCount);			
		}

	}

	CPPUNIT_ASSERT_EQUAL(1, success);
}

void SensorTelemetryController_UT::test_SensorEventHandler_tc_07()
{
	ConfigurationReader *m_configReader = ConfigurationReader::getInstance();
	char* dataRetrieveQuery = "select * from sensortelemetry_table";
	char* dataDeletionQuery = "truncate sensortelemetry_table";
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	void* ptr;
	int success = 1;

	if (UPDATE_QUERY_SUCCESS == execUpdateQuery(UpdateCtn, dataDeletionQuery, args_dataDeletion))
	{
		m_configReader->m_sensorInfo[1].sensorId = 16060;
		m_configReader->m_sensorInfo[1].deviceId = 1606;
		m_configReader->m_sensorInfo[1].sensorType = "MAX31820";
		m_configReader->m_sensorInfo[1].sensorUnit = "C";

		SensorEventHandler(16060, 1, 100, 200, ptr, 0);
		execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
			{
				for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
				{
					if (!QString::compare(mapIter.key(), QString::fromStdString("deviceId"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1606"));	
					}					
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorName"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("TEM_SENSOR_EXCEEDED"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorType"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("MAX31820"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorUnit"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("C"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("value"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("200"));
					}
				}
			}
		}

		CPPUNIT_ASSERT_EQUAL(1, success);
	}
}

void SensorTelemetryController_UT::test_SensorEventHandler_tc_08()
{
	ConfigurationReader *m_configReader = ConfigurationReader::getInstance();
	char* dataRetrieveQuery = "select * from sensortelemetry_table";
	char* dataDeletionQuery = "truncate sensortelemetry_table";
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	void* ptr;
	int success = 1;

	if (UPDATE_QUERY_SUCCESS == execUpdateQuery(UpdateCtn, dataDeletionQuery, args_dataDeletion))
	{
		m_configReader->m_sensorInfo[0].sensorId = 19001;
		m_configReader->m_sensorInfo[0].deviceId = 1900;
		m_configReader->m_sensorInfo[0].eventSensorFlag = 1;
		m_configReader->m_sensorInfo[0].sensorType = "ABCDE";
		m_configReader->m_sensorInfo[0].sensorName = "Motion Sensor";
		m_configReader->m_sensorInfo[0].sensorUnit = "kmph";

		SensorEventHandler(19001, 1, 100, 300, ptr, 0);

		execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
			{
				for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
				{
					if (!QString::compare(mapIter.key(), QString::fromStdString("deviceId"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1900"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorType"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("ABCDE"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorName"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Motion Sensor"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorUnit"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("kmph"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("value"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("300"));
					}
				}
			}
		}
		CPPUNIT_ASSERT_EQUAL(1, success);
	}
}

void SensorTelemetryController_UT::test_SensorEventHandler_tc_09()
{
	ConfigurationReader *m_configReader = ConfigurationReader::getInstance();
	char* dataRetrieveQuery = "select * from sensortelemetry_table";
	char* dataDeletionQuery = "truncate sensortelemetry_table";
	map<string, string> args_dataRetrieve;
	map<string, string> args_dataDeletion;
	QList<QMap<QString, QVariant>>  recs;
	QList < QMap<QString, QVariant>>::iterator listIterate;
	QMap<QString, QVariant>::iterator mapIter;
	void* ptr;
	int success = 1;

	if (UPDATE_QUERY_SUCCESS == execUpdateQuery(UpdateCtn, dataDeletionQuery, args_dataDeletion))
	{
		m_configReader->m_sensorInfo[0].sensorId = 19050;
		m_configReader->m_sensorInfo[0].deviceId = 1905;
		m_configReader->m_sensorInfo[0].eventSensorFlag = 1;
		m_configReader->m_sensorInfo[0].sensorType = "MAX12345";
		m_configReader->m_sensorInfo[0].sensorName = "Voltage Sensor";
		m_configReader->m_sensorInfo[0].sensorUnit = "V";
		
		SensorEventHandler(19050, 1, 100, 400, ptr, 0);
		execSelectQuery(dataRetrieveQuery, args_dataRetrieve, recs);

		if (recs.size() > 0)
		{
			for (listIterate = recs.begin(); listIterate != recs.end(); listIterate++)
			{
				for (mapIter = listIterate->begin(); mapIter != listIterate->end(); mapIter++)
				{
					if (!QString::compare(mapIter.key(), QString::fromStdString("deviceId"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("1905"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorType"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("MAX12345"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorName"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("Voltage Sensor"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("sensorUnit"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("V"));
					}
					else if (!QString::compare(mapIter.key(), QString::fromStdString("value"), Qt::CaseSensitive))
					{
						success = success * !QString::compare(mapIter.value().toString(), QString::fromStdString("400"));
					}
				}
			}
		}
		CPPUNIT_ASSERT_EQUAL(1, success);
	}
}


void SensorTelemetryController_UT::test_initSensorTelemetry_tc_10()
{
	ConfigurationReader *m_configReader = ConfigurationReader::getInstance();
	int errorCount = -1;
	SP_STATUS_CODE status;
	int success = 1;

	m_configReader->m_sensorInfo[0].sensorId = 1900;
	m_configReader->m_sensorInfo[1].sensorId = 1600;

	status = m_SensorTelemetryController->initSensorTelemetry(errorCount);

	success = success * (status == SP_SUCCESS);

	CPPUNIT_ASSERT_EQUAL(1, success);
}

void SensorTelemetryController_UT::test_initSensorTelemetry_tc_11()
{
	ConfigurationReader *m_configReader = ConfigurationReader::getInstance();
	int errorCount = -1;
	SP_STATUS_CODE status;
	int success = 1;

	m_configReader->m_sensorInfo[0].sensorId = 1600;
	m_configReader->m_sensorInfo[1].sensorId = 1900;

	status = m_SensorTelemetryController->initSensorTelemetry(errorCount);

	success = success * (status != SP_SUCCESS);

	CPPUNIT_ASSERT_EQUAL(1, success);
}

CPPUNIT_TEST_SUITE_REGISTRATION(SensorTelemetryController_UT);

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
	ofstream xmlFileOut("Test_Results_SensorTelemetryController_UT.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	getchar();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}

