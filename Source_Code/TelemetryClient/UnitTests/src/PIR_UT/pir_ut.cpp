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
#include "pir.h"

using namespace CppUnit;
using namespace std;

// Common Unit Test 
extern mockConfigurationReader *m_configInstance;
extern bool mockspDevOpenFlag;
extern bool mockspDevSetPropertyBFlag;
extern bool mockspDevRegCallbackFlag;
extern bool mockspDevClosenFlag;

// PIR
extern int32_t sensor_right_devId;
extern int32_t sensor_left_devId;
extern int32_t sensor_right_value;
extern int32_t sensor_left_value;
extern int32_t movingLeft;
extern int32_t movingRight;

/* event callback for right PIR sensor and left PIR sensor */
extern void right_sensor_notification(int32_t devID, int32_t eventType, int32_t eventData1,
	int32_t eventData2, void *eventData3, int32_t reserved);
extern void left_sensor_notification(int32_t devID, int32_t eventType, int32_t eventData1,
	int32_t eventData2, void *eventData3, int32_t reserved);

static char *hostname = NULL;
static char *db_name = NULL;
static char *username = NULL;
static char *password = NULL;


class TestPIR : 
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestPIR);
	
	CPPUNIT_TEST(test_init_PIR_tc_01);
	CPPUNIT_TEST(test_init_PIR_tc_02);
	CPPUNIT_TEST(test_init_PIR_tc_03);
	CPPUNIT_TEST(test_init_PIR_tc_04);
	CPPUNIT_TEST(test_init_PIR_tc_05);
	CPPUNIT_TEST(test_init_PIR_tc_06);
	CPPUNIT_TEST(test_init_PIR_tc_07);
	CPPUNIT_TEST(test_init_PIR_tc_08);
	CPPUNIT_TEST(test_init_PIR_tc_09);
	CPPUNIT_TEST(test_init_PIR_tc_10);
	CPPUNIT_TEST(test_init_PIR_tc_11);

	CPPUNIT_TEST(test_right_sensor_notification_tc_01);
	CPPUNIT_TEST(test_right_sensor_notification_tc_02);
	CPPUNIT_TEST(test_right_sensor_notification_tc_03);
	CPPUNIT_TEST(test_right_sensor_notification_tc_04);

	CPPUNIT_TEST(test_left_sensor_notification_tc_01);
	CPPUNIT_TEST(test_left_sensor_notification_tc_02);
	CPPUNIT_TEST(test_left_sensor_notification_tc_03);
	CPPUNIT_TEST(test_left_sensor_notification_tc_04);

	CPPUNIT_TEST(test_deinit_PIR_tc_01);
	CPPUNIT_TEST(test_deinit_PIR_tc_02);
	CPPUNIT_TEST(test_deinit_PIR_tc_03);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test_init_PIR_tc_01(void);
	void test_init_PIR_tc_02(void);
	void test_init_PIR_tc_03(void);
	void test_init_PIR_tc_04(void);
	void test_init_PIR_tc_05(void);
	void test_init_PIR_tc_06(void);
	void test_init_PIR_tc_07(void);
	void test_init_PIR_tc_08(void);
	void test_init_PIR_tc_09(void);
	void test_init_PIR_tc_10(void);
	void test_init_PIR_tc_11(void);

	void test_right_sensor_notification_tc_01(void);
	void test_right_sensor_notification_tc_02(void);
	void test_right_sensor_notification_tc_03(void);
	void test_right_sensor_notification_tc_04(void);

	void test_left_sensor_notification_tc_01(void);
	void test_left_sensor_notification_tc_02(void);
	void test_left_sensor_notification_tc_03(void);
	void test_left_sensor_notification_tc_04(void);

	void test_deinit_PIR_tc_01(void);
	void test_deinit_PIR_tc_02(void);
	void test_deinit_PIR_tc_03(void);

private:
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
void TestPIR::setUp(void)
{

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
void TestPIR::tearDown(void)
{

}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_01
-------------------------------------------------------------------------------
* 
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_01(void)
{
	m_configInstance = new mockConfigurationReader();

	m_configInstance->m_DatabaseHostName = "";
	m_configInstance->m_DatabaseName = db_name;
	m_configInstance->m_DatabaseUserName = username;
	m_configInstance->m_DatabasePassword = password;
	CPPUNIT_ASSERT_EQUAL(PIR_SUCCESS, init_PIR(0, 0));
}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_02
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_02(void)
{
	m_configInstance->m_DatabaseHostName = "localhost";
	m_configInstance->m_DatabaseName = "";
	m_configInstance->m_DatabaseUserName = username;
	m_configInstance->m_DatabasePassword = password;
	CPPUNIT_ASSERT_EQUAL(PIR_E_DB_FAILURE, init_PIR(0, 0));
}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_03
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_03(void)
{
	m_configInstance->m_DatabaseHostName = "localhost";
	m_configInstance->m_DatabaseName = db_name;
	m_configInstance->m_DatabaseUserName = "";
	m_configInstance->m_DatabasePassword = password;
	CPPUNIT_ASSERT_EQUAL(PIR_E_DB_FAILURE, init_PIR(0, 0));
}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_04
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_04(void)
{
	m_configInstance->m_DatabaseHostName = "localhost";
	m_configInstance->m_DatabaseName = db_name;
	m_configInstance->m_DatabaseUserName = username;
	m_configInstance->m_DatabasePassword = "";
	CPPUNIT_ASSERT_EQUAL(PIR_E_DB_FAILURE, init_PIR(0, 0));
}


/**
-------------------------------------------------------------------------------
test_init_PIR_tc_05
-------------------------------------------------------------------------------
* This function execute the query from calling funcion.
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_05(void)
{
	m_configInstance->m_DatabaseHostName = "localhost";
	m_configInstance->m_DatabaseName = db_name;
	m_configInstance->m_DatabaseUserName = username;
	m_configInstance->m_DatabasePassword = password;

	CPPUNIT_ASSERT_EQUAL(PIR_SUCCESS, init_PIR(0, 0));
}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_06
-------------------------------------------------------------------------------
* 
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_06(void)
{
	mockspDevOpenFlag = true;
	CPPUNIT_ASSERT_EQUAL(PIR_SUCCESS, init_PIR(0, 0));
}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_07
-------------------------------------------------------------------------------
* 
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_07(void)
{
	mockspDevOpenFlag = false;
	CPPUNIT_ASSERT_EQUAL(PIR_E_SP_FAILURE, init_PIR(0, 0));
}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_08
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_08(void)
{
	mockspDevOpenFlag = true;
	mockspDevSetPropertyBFlag = true;
	CPPUNIT_ASSERT_EQUAL(PIR_SUCCESS, init_PIR(0, 0));
}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_09
-------------------------------------------------------------------------------
* 
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_09(void)
{
	mockspDevSetPropertyBFlag = false;
	CPPUNIT_ASSERT_EQUAL(PIR_E_SP_FAILURE, init_PIR(0, 0));
}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_10
-------------------------------------------------------------------------------
* 
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_10(void)
{
	mockspDevSetPropertyBFlag = true;
	mockspDevRegCallbackFlag = true;
	CPPUNIT_ASSERT_EQUAL(PIR_SUCCESS, init_PIR(0, 0));
}

/**
-------------------------------------------------------------------------------
test_init_PIR_tc_11
-------------------------------------------------------------------------------
* 
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_init_PIR_tc_11(void)
{
	mockspDevRegCallbackFlag = false;
	CPPUNIT_ASSERT_EQUAL(PIR_E_SP_FAILURE, init_PIR(0, 0));

	delete m_configInstance;
}

/**
-------------------------------------------------------------------------------
test_right_sensor_notification_tc_01
-------------------------------------------------------------------------------
* 
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_right_sensor_notification_tc_01(void)
{
	movingLeft = 0;
	sensor_left_value = 1;
	right_sensor_notification(0, 0, 0, 1, NULL, 0);
	CPPUNIT_ASSERT_EQUAL(1, movingLeft);
}

/**
-------------------------------------------------------------------------------
test_right_sensor_notification_tc_02
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_right_sensor_notification_tc_02(void)
{
	movingLeft = 1;
	sensor_left_value = 1;
	right_sensor_notification(0, 0, 0, 1, NULL, 0);
	CPPUNIT_ASSERT_EQUAL(2, movingLeft);
}

/**
-------------------------------------------------------------------------------
test_right_sensor_notification_tc_03
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_right_sensor_notification_tc_03(void)
{
	movingLeft = 1;
	sensor_left_value = 0;
	right_sensor_notification(0, 0, 0, 1, NULL, 0);
	CPPUNIT_ASSERT_EQUAL(1, movingLeft);
}

/**
-------------------------------------------------------------------------------
test_right_sensor_notification_tc_04
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_right_sensor_notification_tc_04(void)
{
	movingLeft = 20;
	sensor_left_value = 0;
	right_sensor_notification(0, 0, 0, 1, NULL, 0);
	CPPUNIT_ASSERT_EQUAL(20, movingLeft);
}

/**
-------------------------------------------------------------------------------
test_left_sensor_notification_tc_01
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_left_sensor_notification_tc_01(void)
{
	/* --- test case #1 - */
	movingRight = 11;
	sensor_right_value = 1;
	left_sensor_notification(0, 0, 0, 1, NULL, 0);
	CPPUNIT_ASSERT_EQUAL(12, movingRight);
}

/**
-------------------------------------------------------------------------------
test_left_sensor_notification_tc_02
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_left_sensor_notification_tc_02(void)
{
	movingRight = 19;
	sensor_right_value = 1;
	left_sensor_notification(0, 0, 0, 1, NULL, 0);
	CPPUNIT_ASSERT_EQUAL(20, movingRight);
}

/**
-------------------------------------------------------------------------------
test_left_sensor_notification_tc_03
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_left_sensor_notification_tc_03(void)
{
	movingRight = 100;
	sensor_right_value = 0;
	left_sensor_notification(0, 0, 0, 1, NULL, 0);
	CPPUNIT_ASSERT_EQUAL(100, movingRight);
}

/**
-------------------------------------------------------------------------------
test_left_sensor_notification_tc_04
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_left_sensor_notification_tc_04(void)
{
	movingRight = 1;
	sensor_right_value = 0;
	left_sensor_notification(0, 0, 0, 1, NULL, 0);
	CPPUNIT_ASSERT_EQUAL(1, movingRight);
}

/**
-------------------------------------------------------------------------------
test_deinit_PIR_tc_01
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_deinit_PIR_tc_01(void)
{
	CPPUNIT_ASSERT_EQUAL(PIR_SUCCESS, deinit_PIR());
}

/**
-------------------------------------------------------------------------------
test_deinit_PIR_tc_02
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_deinit_PIR_tc_02(void)
{
	mockspDevClosenFlag = true;
	CPPUNIT_ASSERT_EQUAL(PIR_SUCCESS, deinit_PIR());
}

/**
-------------------------------------------------------------------------------
test_deinit_PIR_tc_03
-------------------------------------------------------------------------------
*
*
*	@param		none
*
*	@return		none
*
*/
void TestPIR::test_deinit_PIR_tc_03(void)
{
	mockspDevClosenFlag = false;
	CPPUNIT_ASSERT_EQUAL(PIR_E_SP_FAILURE, deinit_PIR());
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestPIR);

int main(int argc, char* argv[])
{
	if (argc < 4) 
	{
		printf("Usage: pir [DB Name] [User Name] [Password]\n");

		return -1;
	}
	else
	{
		db_name = argv[1];
		username = argv[2];
		password = argv[3];

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
		ofstream xmlFileOut("test_results_PIR.xml");
		XmlOutputter xmlOut(&collectedresults, xmlFileOut);
		xmlOut.write();

		// return 0 if tests were successful
		return collectedresults.wasSuccessful() ? 0 : 1;
	}
}

