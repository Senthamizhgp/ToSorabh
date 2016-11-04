// Transaction_UT.cpp : Defines the entry point for the console application.
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

#include "Transaction.h"

using namespace CppUnit;
using namespace std;

class Transaction_UT :
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Transaction_UT);
	CPPUNIT_TEST(test_Read_Vminfo_Table_tc_01);
	CPPUNIT_TEST(test_Read_Vminfo_Table_tc_02);
	CPPUNIT_TEST(test_Read_Vminfo_Table_tc_03);
	CPPUNIT_TEST(test_Read_Vminfo_Table_tc_04);
	CPPUNIT_TEST(test_Transaction_Summary_tc_01);
	CPPUNIT_TEST(test_Transaction_Summary_tc_02);
	CPPUNIT_TEST(test_Transaction_Summary_tc_03);
	CPPUNIT_TEST(test_Transaction_Summary_tc_04);
	CPPUNIT_TEST(test_Stock_Summary_tc_01);
	CPPUNIT_TEST(test_Stock_Summary_tc_02);
	CPPUNIT_TEST(test_Stock_Summary_tc_03);
	CPPUNIT_TEST(test_Stock_Summary_tc_04);
	CPPUNIT_TEST(test_Stock_Summary_tc_05);
	CPPUNIT_TEST(test_Process_All_Transaction_tc_01);
	CPPUNIT_TEST(test_Process_All_Transaction_tc_02);
	CPPUNIT_TEST(test_Process_All_Transaction_tc_03);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:

	void test_Read_Vminfo_Table_tc_01(void);
	void test_Read_Vminfo_Table_tc_02(void);
	void test_Read_Vminfo_Table_tc_03(void);
	void test_Read_Vminfo_Table_tc_04(void);

	void test_Transaction_Summary_tc_01(void);
	void test_Transaction_Summary_tc_02(void);
	void test_Transaction_Summary_tc_03(void);
	void test_Transaction_Summary_tc_04(void);

	void test_Stock_Summary_tc_01(void);
	void test_Stock_Summary_tc_02(void);
	void test_Stock_Summary_tc_03(void);
	void test_Stock_Summary_tc_04(void);
	void test_Stock_Summary_tc_05(void);

	void test_Process_All_Transaction_tc_01(void);
	void test_Process_All_Transaction_tc_02(void);
	void test_Process_All_Transaction_tc_03(void);
	
private:
	Transaction *mTransaction;
};

void Transaction_UT::setUp()
{
	mTransaction = new Transaction();
}

void Transaction_UT::tearDown()
{
	delete mTransaction;
}

/*********************/

void Transaction_UT::test_Read_Vminfo_Table_tc_01()
{
	string  TestComm_Id = "3142983500";
	string jsondata = "{\"vminfo\":[{\"Asset_No\":\"CRANE--VER 15822\",\"Cash\":0,\"Coins_in_CashBox\":0,\"Coins_to_Tubes\":2900,\"Comm_Id\":\"3142983500\",\"Serial_No\":\"295750\",\"Timestamp\":1368550052,\"Version\":\"V1/1\"}]}";
	mTransaction->Read_Vminfo_Table(jsondata);
	CPPUNIT_ASSERT_EQUAL(0, TestComm_Id.compare(mTransaction->Comm_ID.toStdString()));
}

void Transaction_UT::test_Read_Vminfo_Table_tc_02()
{
	string  TestComm_Id = "983500";
	string jsondata = "{\"vminfo\":[{\"Asset_No\":\"CRANE--VER 15822\",\"Cash\":0,\"Coins_in_CashBox\":0,\"Coins_to_Tubes\":2900,\"Comm_Id\":\"3142983500\",\"Serial_No\":\"295750\",\"Timestamp\":1368550052,\"Version\":\"V1/1\"}]}";
	mTransaction->Read_Vminfo_Table(jsondata);
	CPPUNIT_ASSERT_EQUAL(1, TestComm_Id.compare(mTransaction->Comm_ID.toStdString()));
}

void Transaction_UT::test_Read_Vminfo_Table_tc_03()
{
	string  TestComm_Id = "983500";
	string jsondata = "{\"vminfo\":{\"Asset_No\":\"CRANE--VER 15822\",\"Cash\":0,\"Coins_in_CashBox\":0,\"Coins_to_Tubes\":2900,\"Comm_Id\":\"3142983500\",\"Serial_No\":\"295750\",\"Timestamp\":1368550052,\"Version\":\"V1/1\"}}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Read_Vminfo_Table(jsondata);
	CPPUNIT_ASSERT_EQUAL(1, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Read_Vminfo_Table_tc_04()
{
	string  TestComm_Id = "983500";
	string jsondata = "{\"vminfo\":[]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Read_Vminfo_Table(jsondata);
	CPPUNIT_ASSERT_EQUAL(1, TRANSACTION_SUMMARY_STATUS_CODE);
}

/*********************/

void Transaction_UT::test_Transaction_Summary_tc_01()
{
	string jsondata = "{\"vminfo\":[]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(5, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Transaction_Summary_tc_02()
{
	string jsondata = "{\"vminfo\":[{\"Comm_Id\":\"3142983500\"}],\"product_vend_info\":[]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(5, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Transaction_Summary_tc_03()
{
	string jsondata = "{\"vminfo\":[{\"Comm_Id\":\"3142983500\"}],\"product_vend_info\":{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(5, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Transaction_Summary_tc_04()
{
	string jsondata = "{\"vminfo\":[{\"Comm_Id\":\"3142983500\"}],\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(4, TRANSACTION_SUMMARY_STATUS_CODE);
}

/*********************/

void Transaction_UT::test_Stock_Summary_tc_01() 
{
	string jsondata = "{\"vminfo\":[]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Stock_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(7, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Stock_Summary_tc_02()
{
	string jsondata = "{\"vminfo\":[{\"Comm_Id\":\"3142983500\"}],\"product_vend_info\":[]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Stock_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(7, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Stock_Summary_tc_03()
{
	string jsondata = "{\"vminfo\":[{\"Comm_Id\":\"3142983500\"}],\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}],\"stockinfo\":[]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Stock_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(7, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Stock_Summary_tc_04()
{
	string jsondata = "{\"vminfo\":[{\"Comm_Id\":\"3142983500\"}],\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}],\"stockinfo\":{\"Stock\":20,\"Product_ID\":730706}}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Stock_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(7, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Stock_Summary_tc_05()
{
	string jsondata = "{\"vminfo\":[{\"Comm_Id\":\"3142983500\"}],\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}],\"stockinfo\":[{\"Stock\":20,\"Product_ID\":730706}]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Stock_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(6, TRANSACTION_SUMMARY_STATUS_CODE);
}

/*********************/

void Transaction_UT::test_Process_All_Transaction_tc_01()
{
	string jsondata = "{\"product_vend_info\":{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Process_All_Transaction(jsondata);
	CPPUNIT_ASSERT_EQUAL(3, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Process_All_Transaction_tc_02()
{
	string jsondata = "{\"product_vend_info\":[]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Process_All_Transaction(jsondata);
	CPPUNIT_ASSERT_EQUAL(3, TRANSACTION_SUMMARY_STATUS_CODE);
}

void Transaction_UT::test_Process_All_Transaction_tc_03()
{
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	int TRANSACTION_SUMMARY_STATUS_CODE = mTransaction->Process_All_Transaction(jsondata);
	CPPUNIT_ASSERT_EQUAL(2, TRANSACTION_SUMMARY_STATUS_CODE);
}

/*********************/

CPPUNIT_TEST_SUITE_REGISTRATION(Transaction_UT);

/*********************/

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
	ofstream xmlFileOut("Test_Results_Transaction_UT.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	getchar();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}

