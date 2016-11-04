// DEX_Converter_UT.cpp : Defines the entry point for the console application.
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



#include "DexConverter.h"

using namespace CppUnit;
using namespace std;

class DEX_Converter_UT:
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(DEX_Converter_UT);
	CPPUNIT_TEST(test_DexFileGenerater_tc_01);
	CPPUNIT_TEST(test_ReadvminfoObject_tc_02);
	CPPUNIT_TEST(test_ReadeventinfoObject_tc_03);
	CPPUNIT_TEST(test_ReadproductvendinfoObject_tc_04);
	CPPUNIT_TEST(test_DexPaidvendsales_tc_05);
	CPPUNIT_TEST(test_DexCashInflow_tc_06);
	CPPUNIT_TEST(test_DexCashOutflow_tc_07);
	CPPUNIT_TEST(test_Dexvalueintubes_tc_08);
	CPPUNIT_TEST(test_DexCashlesssalesinfo_tc_09);
	CPPUNIT_TEST(test_DexProductinfosales_tc_10);
	CPPUNIT_TEST(test_RecordIntegrityCheck_tc_11);
	CPPUNIT_TEST(test_NoofIncludedSets_tc_12);
	CPPUNIT_TEST(test_DexTrailer_tc_13);
	CPPUNIT_TEST(test_Clear_Memory_tc_14);
	CPPUNIT_TEST(test_ReadvminfoObject_tc_15);
	CPPUNIT_TEST(test_ReadeventinfoObject_tc_16);
	CPPUNIT_TEST(test_All_Transaction_Summary_tc_01);
	CPPUNIT_TEST(test_All_Transaction_Summary_tc_02);
	CPPUNIT_TEST(test_All_Transaction_Summary_tc_03);
	CPPUNIT_TEST(test_All_Transaction_Summary_tc_04);
	CPPUNIT_TEST(test_All_Transaction_Summary_tc_05);
	CPPUNIT_TEST(test_All_Transaction_Summary_tc_06);
	CPPUNIT_TEST(test_All_Transaction_Summary_tc_07);
	CPPUNIT_TEST(test_All_Transaction_Summary_tc_08);
	CPPUNIT_TEST(test_All_Transaction_Summary_tc_09);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:
	
	void test_DexFileGenerater_tc_01(void);
	void test_ReadvminfoObject_tc_02(void);
	void test_ReadeventinfoObject_tc_03(void);
	void test_ReadproductvendinfoObject_tc_04(void);
	void test_DexPaidvendsales_tc_05(void);
	void test_DexCashInflow_tc_06(void);
	void test_DexCashOutflow_tc_07(void);
	void test_Dexvalueintubes_tc_08(void);
	void test_DexCashlesssalesinfo_tc_09(void);
	void test_DexProductinfosales_tc_10(void);
	void test_RecordIntegrityCheck_tc_11(void);
	void test_NoofIncludedSets_tc_12(void);
	void test_DexTrailer_tc_13(void);
	void test_Clear_Memory_tc_14(void);
	void test_ReadvminfoObject_tc_15(void);
	void test_ReadeventinfoObject_tc_16(void);

	void test_All_Transaction_Summary_tc_01(void);
	void test_All_Transaction_Summary_tc_02(void);
	void test_All_Transaction_Summary_tc_03(void);
	void test_All_Transaction_Summary_tc_04(void);
	void test_All_Transaction_Summary_tc_05(void);
	void test_All_Transaction_Summary_tc_06(void);
	void test_All_Transaction_Summary_tc_07(void);
	void test_All_Transaction_Summary_tc_08(void);
	void test_All_Transaction_Summary_tc_09(void);

private:
	DexConverter *mDexConverter;
};

void DEX_Converter_UT::setUp()
{
	mDexConverter = new DexConverter();
}

void DEX_Converter_UT::tearDown()
{
	delete mDexConverter;
}

void DEX_Converter_UT::test_DexFileGenerater_tc_01()
{
	string jsondata = "{\"vminfo\":[{\"Asset_No\":\"CRANE--VER 15822\",\"Cash\":0,\"Coins_in_CashBox\":0,\"Coins_to_Tubes\":2900,\"Comm_Id\":\"3142983500\",\"Serial_No\":\"295750\",\"Timestamp\":1368550052,\"Version\":\"V1/1\"}]}";
	mDexConverter->DexFileGenerater(jsondata);
}

void DEX_Converter_UT::test_ReadvminfoObject_tc_02()
{
	string jsondata = "{\"vminfo\":[{\"Asset_No\":\"CRANE--VER 15822\",\"Cash\":0,\"Coins_in_CashBox\":0,\"Coins_to_Tubes\":2900,\"Comm_Id\":\"3142983500\",\"Serial_No\":\"295750\",\"Timestamp\":1368550052,\"Version\":\"V1/1\"}]}";
	QString JsonData1 = jsondata.c_str();
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	QJsonObject jsonObject = JsonResponse.object();
	mDexConverter->ReadvminfoObject(jsonObject);
}

void DEX_Converter_UT::test_ReadeventinfoObject_tc_03()
{
	string jsondata = "{\"eventinfo\":[{\"Duration_of_Event\":0,\"Event_Identity\":\"LAST CLEAR\",\"Timestamp\":1368550872},{\"Duration_of_Event\":0,\"Event_Identity\":\"EGS\",\"Timestamp\":1368551197},{\"Duration_of_Event\":7,\"Event_Identity\":\"REFILL\",\"Timestamp\":1398436926}]}";
	QString JsonData1 = jsondata.c_str();
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	QJsonObject jsonObject = JsonResponse.object();
	mDexConverter->ReadeventinfoObject(jsonObject);
}

void DEX_Converter_UT::test_ReadproductvendinfoObject_tc_04()
{
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	QString JsonData1 = jsondata.c_str();
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	QJsonObject jsonObject = JsonResponse.object();
	mDexConverter->ReadproductvendinfoObject(jsonObject);
}

void DEX_Converter_UT::test_DexPaidvendsales_tc_05()
{
	mDexConverter->DexPaidvendsales();
}

void DEX_Converter_UT::test_DexCashInflow_tc_06()
{
	mDexConverter->DexCashInflow();
}

void DEX_Converter_UT::test_DexCashOutflow_tc_07()
{
	mDexConverter->DexCashOutflow();
}

void DEX_Converter_UT::test_Dexvalueintubes_tc_08()
{
	mDexConverter->Dexvalueintubes();
}

void DEX_Converter_UT::test_DexCashlesssalesinfo_tc_09()
{
	mDexConverter->DexCashlesssalesinfo();
}

void DEX_Converter_UT::test_DexProductinfosales_tc_10()
{
	mDexConverter->DexProductinfosales();
}

void DEX_Converter_UT::test_RecordIntegrityCheck_tc_11()
{
	mDexConverter->RecordIntegrityCheck();
}

void DEX_Converter_UT::test_NoofIncludedSets_tc_12()
{
	mDexConverter->NoofIncludedSets();
}

void DEX_Converter_UT::test_DexTrailer_tc_13()
{
	mDexConverter->DexTrailer();
}

void DEX_Converter_UT::test_Clear_Memory_tc_14()
{
	mDexConverter->Clear_Memory();
}

void DEX_Converter_UT::test_ReadvminfoObject_tc_15()
{
	string jsondata = "{\"vminfo\":[]}";
	QString JsonData1 = jsondata.c_str();
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	QJsonObject jsonObject = JsonResponse.object();
	mDexConverter->ReadvminfoObject(jsonObject);
}

void DEX_Converter_UT::test_ReadeventinfoObject_tc_16()
{
	string jsondata = "{\"eventinfo\":[]}";
	QString JsonData1 = jsondata.c_str();
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	QJsonObject jsonObject = JsonResponse.object();
	mDexConverter->ReadeventinfoObject(jsonObject);
}

#if 0
void DEX_Converter_UT::test_DexFileGenerater_tc_02()
{
	string  TestAssetNo = "CRANE--VER 15822";
	string jsondata = "{\"vminfo\":[{\"Asset_No\":\"CRANE--VER 15822\",\"Cash\":0,\"Coins_in_CashBox\":0,\"Coins_to_Tubes\":2900,\"Comm_Id\":\"3142983500\",\"Serial_No\":\"295750\",\"Timestamp\":1368550052,\"Version\":\"V1/1\"}]}";
	mDexConverter->DexFileGenerater(jsondata);
	CPPUNIT_ASSERT_EQUAL(0, TestAssetNo.compare(mDexConverter->AssetNo.toStdString()));
}

void DEX_Converter_UT::test_DexFileGenerater_tc_03()
{
	string  TestComm_ID = "31429835";
	string jsondata = "{\"vminfo\":[{\"Asset_No\":\"CRANE--VER 15822\",\"Cash\":0,\"Coins_in_CashBox\":0,\"Coins_to_Tubes\":2900,\"Comm_Id\":\"3142983500\",\"Serial_No\":\"295750\",\"Timestamp\":1368550052,\"Version\":\"V1/1\"}]}";
	mDexConverter->DexFileGenerater(jsondata);
	CPPUNIT_ASSERT_EQUAL(-1, TestComm_ID.compare(mDexConverter->Comm_ID.toStdString()));
}

void DEX_Converter_UT::test_DexFileGenerater_tc_04()
{
	string  TestSerial_No = "295751";
	string jsondata = "{\"vminfo\":[{\"Asset_No\":\"CRANE--VER 15822\",\"Cash\":0,\"Coins_in_CashBox\":0,\"Coins_to_Tubes\":2900,\"Comm_Id\":\"3142983500\",\"Serial_No\":\"295750\",\"Timestamp\":1368550052,\"Version\":\"V1/1\"}]}";
	mDexConverter->DexFileGenerater(jsondata);
	CPPUNIT_ASSERT_EQUAL(1, TestSerial_No.compare(mDexConverter->Serialno.toStdString()));
}

/*************/
void DEX_Converter_UT::test_DexFileGenerater_tc_05()
{
	int TestRefillTime = 1398436926;
	string jsondata = "{\"eventinfo\":[{\"Duration_of_Event\":0,\"Event_Identity\":\"LAST CLEAR\",\"Timestamp\":1368550872},{\"Duration_of_Event\":0,\"Event_Identity\":\"EGS\",\"Timestamp\":1368551197},{\"Duration_of_Event\":7,\"Event_Identity\":\"REFILL\",\"Timestamp\":1398436926}]}";
	mDexConverter->DexFileGenerater(jsondata);
	CPPUNIT_ASSERT_EQUAL(TestRefillTime, mDexConverter->RefillTimeUTC);
}

void DEX_Converter_UT::test_DexFileGenerater_tc_06()
{
	string TestEvent_Identity = "REFILL";
	string jsondata = "{\"eventinfo\":[{\"Duration_of_Event\":0,\"Event_Identity\":\"LAST CLEAR\",\"Timestamp\":1368550872},{\"Duration_of_Event\":0,\"Event_Identity\":\"EGS\",\"Timestamp\":1368551197},{\"Duration_of_Event\":7,\"Event_Identity\":\"REFILL\",\"Timestamp\":1398436926}]}";
	mDexConverter->DexFileGenerater(jsondata);
	CPPUNIT_ASSERT_EQUAL(0, TestEvent_Identity.compare(mDexConverter->EventID.toStdString()));
}

void DEX_Converter_UT::test_DexFileGenerater_tc_07()
{
	string TestEvent_Identity = "Egs";
	string jsondata = "{\"eventinfo\":[{\"Duration_of_Event\":0,\"Event_Identity\":\"LAST CLEAR\",\"Timestamp\":1368550872},{\"Duration_of_Event\":0,\"Event_Identity\":\"EGS\",\"Timestamp\":1368551197},{\"Duration_of_Event\":7,\"Event_Identity\":\"REFILL\",\"Timestamp\":1398436926}]}";
	mDexConverter->DexFileGenerater(jsondata);
	CPPUNIT_ASSERT_EQUAL(-1, TestEvent_Identity.compare(mDexConverter->EventID.toStdString()));
}

void DEX_Converter_UT::test_DexFileGenerater_tc_08()
{
	string TestEvent_Identity = "refill";
	string jsondata = "{\"eventinfo\":[{\"Duration_of_Event\":0,\"Event_Identity\":\"LAST CLEAR\",\"Timestamp\":1368550872},{\"Duration_of_Event\":0,\"Event_Identity\":\"EGS\",\"Timestamp\":1368551197},{\"Duration_of_Event\":7,\"Event_Identity\":\"REFILL\",\"Timestamp\":1398436926}]}";
	mDexConverter->DexFileGenerater(jsondata);
	CPPUNIT_ASSERT_EQUAL(1, TestEvent_Identity.compare(mDexConverter->EventID.toStdString()));
}
#endif
/***********************/

void DEX_Converter_UT::test_All_Transaction_Summary_tc_01()
{
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	mDexConverter->All_Transaction_Summary(jsondata);
}

void DEX_Converter_UT::test_All_Transaction_Summary_tc_02()
{
	float TestPaidVendValueInit = 100;
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	mDexConverter->All_Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(TestPaidVendValueInit, mDexConverter->PaidVendValueInit);
}

void DEX_Converter_UT::test_All_Transaction_Summary_tc_03()
{
	float TestTotalCashlessValueInit = 200;
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":2,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	mDexConverter->All_Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(TestTotalCashlessValueInit, mDexConverter->TotalCashlessValueInit);
}

void DEX_Converter_UT::test_All_Transaction_Summary_tc_04()
{
	float TestPaidVendValueRefill = 100;
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	mDexConverter->All_Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(TestPaidVendValueRefill, mDexConverter->PaidVendValueRefill);
}

void DEX_Converter_UT::test_All_Transaction_Summary_tc_05()
{
	float TestFreeVendValueInit = 0;
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	mDexConverter->All_Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(TestFreeVendValueInit, mDexConverter->FreeVendValueInit);
}

void DEX_Converter_UT::test_All_Transaction_Summary_tc_06()
{
	float TestTestVendValueInit = 0;
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	mDexConverter->All_Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(TestTestVendValueInit, mDexConverter->TestVendValueInit);
}

void DEX_Converter_UT::test_All_Transaction_Summary_tc_07()
{
	float TestTotalCoinsValueInit = 0;
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	mDexConverter->All_Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(TestTotalCoinsValueInit, mDexConverter->TotalCoinsValueInit);
}

void DEX_Converter_UT::test_All_Transaction_Summary_tc_08()
{
	float TestTotalCashlessValueInit = 100;
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	mDexConverter->All_Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(TestTotalCashlessValueInit, mDexConverter->TotalCashlessValueInit);
}

void DEX_Converter_UT::test_All_Transaction_Summary_tc_09()
{
	float TestTotalBillsValueInit = 0;
	string jsondata = "{\"product_vend_info\":[{\"CampaignId\":1,\"ColumnID\":\"1\",\"Discount_Ratio\":0.5,\"InternalKey\":10001,\"PaymentSource\":1,\"ProductSold\":1,\"Product_price\":100,\"SalePrice\":100,\"ShelfID\":\"A\",\"Tax\":1,\"Timestamp\":1368550052,\"Transaction_ID\":40001,\"Product_ID\":730706,\"VendType\":0,\"VendorID\":\"2039\"}]}";
	mDexConverter->All_Transaction_Summary(jsondata);
	CPPUNIT_ASSERT_EQUAL(TestTotalBillsValueInit, mDexConverter->TotalBillsValueInit);
}


CPPUNIT_TEST_SUITE_REGISTRATION(DEX_Converter_UT);

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
	ofstream xmlFileOut("Test_Results_DEX_Converter_UT.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	getchar();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}

