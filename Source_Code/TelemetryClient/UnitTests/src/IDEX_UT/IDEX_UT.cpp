// IDEX_UnitTest.cpp : Defines the entry point for the console application.
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

#define private public

#include "IDEX.h"

using namespace CppUnit;
using namespace std;

class IDEX_UnitTest : 
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(IDEX_UnitTest);
	CPPUNIT_TEST(test_getconnection_tc_01);
	CPPUNIT_TEST(test_IDEX_TableCreation);
	CPPUNIT_TEST(test_IDEX_Init);
	CPPUNIT_TEST(test_IDEX_Read_EventTable_tc_01);
	CPPUNIT_TEST(test_IDEX_All_Transaction_Summary);
	CPPUNIT_TEST(test_IDEX_Initial_Update_AuditInfoTable_tc_01);
	CPPUNIT_TEST(test_IDEX_Initial_Update_AuditInfoTable_tc_02);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_01);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_02);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_03);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_04);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_05);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_06);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_07);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_08);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_09);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_10);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_11);
	CPPUNIT_TEST(test_IDEX_Update_AuditInfoTable_tc_12);
	CPPUNIT_TEST(test_IDEX_Read_VmInfoTable_tc_01);
	CPPUNIT_TEST(test_IDEX_Read_VmInfoTable_tc_02);
	CPPUNIT_TEST(test_IDEX_Read_VmInfoTable_tc_03);
	CPPUNIT_TEST(test_IDEX_Read_VmInfoTable_tc_04);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test_getconnection_tc_01(void);
	void test_IDEX_TableCreation(void);
	void test_IDEX_Init(void);
	void test_IDEX_Read_EventTable_tc_01(void);
	void test_IDEX_All_Transaction_Summary(void);
	
	void test_IDEX_Initial_Update_AuditInfoTable_tc_01(void);
	void test_IDEX_Initial_Update_AuditInfoTable_tc_02(void);

	void test_IDEX_Update_AuditInfoTable_tc_01(void);
	void test_IDEX_Update_AuditInfoTable_tc_02(void);
	void test_IDEX_Update_AuditInfoTable_tc_03(void);

	void test_IDEX_Update_AuditInfoTable_tc_04(void);
	void test_IDEX_Update_AuditInfoTable_tc_05(void);
	void test_IDEX_Update_AuditInfoTable_tc_06(void);

	void test_IDEX_Update_AuditInfoTable_tc_07(void);
	void test_IDEX_Update_AuditInfoTable_tc_08(void);
	void test_IDEX_Update_AuditInfoTable_tc_09(void);

	void test_IDEX_Update_AuditInfoTable_tc_10(void);
	void test_IDEX_Update_AuditInfoTable_tc_11(void);
	void test_IDEX_Update_AuditInfoTable_tc_12(void);

	void test_IDEX_Read_VmInfoTable_tc_01(void);
	void test_IDEX_Read_VmInfoTable_tc_02(void);
	void test_IDEX_Read_VmInfoTable_tc_03(void);
	void test_IDEX_Read_VmInfoTable_tc_04(void);

private:
	IDEX *mIDEX;

};

void IDEX_UnitTest::setUp() 
{
//	mIDEX = new IDEX();
	mIDEX = IDEX::getInstance();
}

void IDEX_UnitTest::tearDown()
{
//	delete mIDEX;
}

void IDEX_UnitTest::test_getconnection_tc_01(void)
{
	int STATUS_CODE = mIDEX->getConnection("localhost", "test_db", "root", "cognizant", 3306);
	CPPUNIT_ASSERT_EQUAL(0, STATUS_CODE);
}

void IDEX_UnitTest::test_IDEX_TableCreation(void)
{
	const char *createStockInfoTableQuery = "CREATE TABLE stockinfo (Product_ID varchar(50) DEFAULT NULL,Stock int(10) DEFAULT NULL, Timestamp bigint(20) NOT NULL)";
	const char *dropStockInfoTableQuery = "drop table if exists stockinfo";;
	bool create = mIDEX->TableCreation(createStockInfoTableQuery, dropStockInfoTableQuery, true);
	CPPUNIT_ASSERT_EQUAL(true,create);
}

void IDEX_UnitTest::test_IDEX_Init(void)
{
//	mIDEX->IDEX_Init(); // Manual check
}

void IDEX_UnitTest::test_IDEX_Initial_Update_AuditInfoTable_tc_01(void)
{
	mIDEX->IDEX_Read_VmInfoTable();
	mIDEX->IDEX_Initial_Update_AuditInfoTable(); // Manual check in first row of auditinfo table.makesure auditinfo table is empty
}

void IDEX_UnitTest::test_IDEX_Initial_Update_AuditInfoTable_tc_02(void)
{
	mIDEX->CoinsToTubes = 0;
	float TestCoinsToTubes = 2900;
	mIDEX->IDEX_Read_VmInfoTable();
	mIDEX->IDEX_Initial_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TestCoinsToTubes, mIDEX->CoinsToTubes);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_01(void)
{
	mIDEX->TCoin = 0;
	float TTCoin = 450;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TTCoin, mIDEX->TCoin);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_02(void)
{
	mIDEX->TCard = 0;
	float TTCard = 1400;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TTCard, mIDEX->TCard);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_03(void)
{
	mIDEX->TCash = 0;
	float TTCash = 162;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TTCash, mIDEX->TCash);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_04(void)
{
	mIDEX->RCoin = 0;
	float TRCoin = 450;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TRCoin, mIDEX->RCoin);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_05(void)
{
	mIDEX->RCard = 0;
	float TRCard = 700;
	mIDEX->RefillTimestamp = 1398436926;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TRCard, mIDEX->RCard);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_06(void)
{
	mIDEX->RCash = 0;
	float TRCash = 162;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TRCash, mIDEX->RCash);
}

/*********/

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_07(void)
{
	float TTCoin = 550;
	mIDEX->TCoin = 100;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TTCoin, mIDEX->TCoin);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_08(void)
{
	float TTCard = 1500;
	mIDEX->TCard = 100;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TTCard, mIDEX->TCard);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_09(void)
{
	float TTCash = 262;
	mIDEX->TCash = 100;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TTCash, mIDEX->TCash);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_10(void)
{
	float TRCoin = 650;
	mIDEX->RCoin = 200;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TRCoin, mIDEX->RCoin);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_11(void)
{
	float TRCard = 800;
	mIDEX->RCard = 100;
	mIDEX->RefillTimestamp = 1398436926;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TRCard, mIDEX->RCard);
}

void IDEX_UnitTest::test_IDEX_Update_AuditInfoTable_tc_12(void)
{
	float TRCash = 362;
	mIDEX->RCash = 200;
	mIDEX->IDEX_Update_AuditInfoTable();
	CPPUNIT_ASSERT_EQUAL(TRCash, mIDEX->RCash);
}
/*********/
void IDEX_UnitTest::test_IDEX_All_Transaction_Summary(void)
{
	mIDEX->IDEX_All_Transaction_Summary();
}

void IDEX_UnitTest::test_IDEX_Read_EventTable_tc_01(void)
{
	mIDEX->IDEX_Read_EventTable();
	CPPUNIT_ASSERT_EQUAL(1398436926, mIDEX->RefillTimestamp);
}

void IDEX_UnitTest::test_IDEX_Read_VmInfoTable_tc_01(void)
{
	float TestCoinsToTubes = 2900;
	mIDEX->IDEX_Read_VmInfoTable();
	CPPUNIT_ASSERT_EQUAL(TestCoinsToTubes, mIDEX->CoinsToTubes);
}

void IDEX_UnitTest::test_IDEX_Read_VmInfoTable_tc_02(void)
{
	float TestTCash = 0;
	mIDEX->IDEX_Read_VmInfoTable();
	CPPUNIT_ASSERT_EQUAL(TestTCash, mIDEX->TCash);
}

void IDEX_UnitTest::test_IDEX_Read_VmInfoTable_tc_03(void)
{
	float TestTCoin = 0;
	mIDEX->IDEX_Read_VmInfoTable();
	CPPUNIT_ASSERT_EQUAL(TestTCoin, mIDEX->TCoin);
}

void IDEX_UnitTest::test_IDEX_Read_VmInfoTable_tc_04(void)
{
	int TestInitTimestamp = 1368550052;
	mIDEX->IDEX_Read_VmInfoTable();
	CPPUNIT_ASSERT_EQUAL(TestInitTimestamp, mIDEX->InitTimestamp);
}

/*********/

CPPUNIT_TEST_SUITE_REGISTRATION(IDEX_UnitTest);

/*********/
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
	ofstream xmlFileOut("Test_Results_IDEX_UT.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	getchar();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}

