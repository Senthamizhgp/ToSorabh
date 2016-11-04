#include <iostream>
#include <string>
#include <list>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

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

#include "wifilib.h"
#include "channel_selector_ut.h"
#include "channel_selector.h"
#include "hashmaplib.h"
#include "dbconnector.h"
#include "ConfigurationReader.h"

using namespace CppUnit;
using namespace std;

char ifname[30];
char *en_name;
char *wifimac;

class TestWifiAPI :
	public CppUnit::TestFixture
{
	/* WiFi Channel and Mode Manager */
	CPPUNIT_TEST_SUITE(TestWifiAPI);

	CPPUNIT_TEST(test_wifi_rfkill_unblock_tc_01);

	CPPUNIT_TEST(test_wifi_get_nuc_mac_addr_tc_01);
	CPPUNIT_TEST(test_wifi_get_nuc_mac_addr_tc_02);
	CPPUNIT_TEST(test_wifi_get_nuc_mac_addr_tc_03);
	CPPUNIT_TEST(test_wifi_get_nuc_mac_addr_tc_04);

	CPPUNIT_TEST(test_wifi_set_monitor_mode_tc_01);
	CPPUNIT_TEST(test_wifi_set_monitor_mode_tc_02);
	CPPUNIT_TEST(test_wifi_set_monitor_mode_tc_03);

	CPPUNIT_TEST(test_wifi_set_channel_tc_01);
	CPPUNIT_TEST(test_wifi_set_channel_tc_02);
	CPPUNIT_TEST(test_wifi_set_channel_tc_03);
	CPPUNIT_TEST(test_wifi_set_channel_tc_04);

	CPPUNIT_TEST(test_wifi_set_freq_tc_01);
	CPPUNIT_TEST(test_wifi_set_freq_tc_02);
	CPPUNIT_TEST(test_wifi_set_freq_tc_03);
	CPPUNIT_TEST(test_wifi_set_freq_tc_04);

	CPPUNIT_TEST(test_wifi_channel_to_freq_tc_01);
	CPPUNIT_TEST(test_wifi_channel_to_freq_tc_02);
	CPPUNIT_TEST(test_wifi_channel_to_freq_tc_03);
	CPPUNIT_TEST(test_wifi_channel_to_freq_tc_04);
	CPPUNIT_TEST(test_wifi_channel_to_freq_tc_05);

	CPPUNIT_TEST(test_wifi_get_state_tc_01);
	CPPUNIT_TEST(test_wifi_get_state_tc_02);
	CPPUNIT_TEST(test_wifi_get_state_tc_03);
	CPPUNIT_TEST(test_wifi_get_state_tc_04);
	CPPUNIT_TEST(test_wifi_get_state_tc_05);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test_wifi_rfkill_unblock_tc_01(void);

	void test_wifi_get_nuc_mac_addr_tc_01(void);
	void test_wifi_get_nuc_mac_addr_tc_02(void);
	void test_wifi_get_nuc_mac_addr_tc_03(void);
	void test_wifi_get_nuc_mac_addr_tc_04(void);

	void test_wifi_set_monitor_mode_tc_01(void);
	void test_wifi_set_monitor_mode_tc_02(void);
	void test_wifi_set_monitor_mode_tc_03(void);

	void test_wifi_set_channel_tc_01(void);
	void test_wifi_set_channel_tc_02(void);
	void test_wifi_set_channel_tc_03(void);
	void test_wifi_set_channel_tc_04(void);

	void test_wifi_set_freq_tc_01(void);
	void test_wifi_set_freq_tc_02(void);
	void test_wifi_set_freq_tc_03(void);
	void test_wifi_set_freq_tc_04(void);

	void test_wifi_channel_to_freq_tc_01(void);
	void test_wifi_channel_to_freq_tc_02(void);
	void test_wifi_channel_to_freq_tc_03(void);
	void test_wifi_channel_to_freq_tc_04(void);
	void test_wifi_channel_to_freq_tc_05(void);

	void test_wifi_get_state_tc_01(void);
	void test_wifi_get_state_tc_02(void);
	void test_wifi_get_state_tc_03(void);
	void test_wifi_get_state_tc_04(void);
	void test_wifi_get_state_tc_05(void);
};

class TestHashmapAPI :
	public CppUnit::TestFixture
{
	/* Hashmap */
	CPPUNIT_TEST_SUITE(TestHashmapAPI);

	CPPUNIT_TEST(test_hashmap_update_elem_tc_01);
	CPPUNIT_TEST(test_hashmap_update_elem_tc_02);
	CPPUNIT_TEST(test_hashmap_update_elem_tc_03);
	CPPUNIT_TEST(test_hashmap_update_elem_tc_04);
	CPPUNIT_TEST(test_hashmap_update_elem_tc_05);

	CPPUNIT_TEST(test_hashmap_length_tc_01);
	CPPUNIT_TEST(test_hashmap_length_tc_02);

	CPPUNIT_TEST(test_hashmap_remove_elem_tc_01);
	CPPUNIT_TEST(test_hashmap_remove_elem_tc_02);
	CPPUNIT_TEST(test_hashmap_remove_elem_tc_03);
	CPPUNIT_TEST(test_hashmap_remove_elem_tc_04);
	CPPUNIT_TEST(test_hashmap_remove_elem_tc_05);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:


	void test_hashmap_update_elem_tc_01(void);
	void test_hashmap_update_elem_tc_02(void);
	void test_hashmap_update_elem_tc_03(void);
	void test_hashmap_update_elem_tc_04(void);
	void test_hashmap_update_elem_tc_05(void);

	void test_hashmap_length_tc_01(void);
	void test_hashmap_length_tc_02(void);

	void test_hashmap_remove_elem_tc_01(void);
	void test_hashmap_remove_elem_tc_02(void);
	void test_hashmap_remove_elem_tc_03(void);
	void test_hashmap_remove_elem_tc_04(void);
	void test_hashmap_remove_elem_tc_05(void);
};

class TestDbConnectorAPI :
	public CppUnit::TestFixture
{
	/*WiFi dbconnector */
	CPPUNIT_TEST_SUITE(TestDbConnectorAPI);

	CPPUNIT_TEST(test_init_database_connector_tc_01);
	CPPUNIT_TEST(test_add_record_to_database_tc_02);
	CPPUNIT_TEST(test_free_database_connector_tc_03);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test_init_database_connector_tc_01(void);
	void test_add_record_to_database_tc_02(void);
	void test_free_database_connector_tc_03(void);
};


void TestWifiAPI::setUp(void)
{
	/* Reset / unblock the WiFi RF */
	if(wifi_rfkill_unblock() != 0)
	{
		printf("\nCould not reset / unblock the WiFi RF\n");
		exit(-1);
	}
	/* setup functions for WiFi channel and mode manager test cases */
	ConfigurationReader *configReader;
	configReader = ConfigurationReader::getInstance();

	memset(ifname, 0, sizeof(ifname));
	if(configReader)
		sprintf(ifname, "%s", configReader->m_WiFiDeviceID.c_str());
	else
	{
		printf("\nWiFi Device ID could not be get\n");
		exit(-1);
	}

	/* get the wifi device mac address */
	wifimac = getenv("WIFI_MAC_ADDR");
	if(wifimac == NULL)
	{
		printf("\nPlease export WIFI_MAC_ADDR env variable\n");
		printf("E.g export the WiFi device MAC as export WIFI_MAC_ADDR=08:00:27:cc:4c:9a\n");
		exit(-1);
	}

	/* get the ethernet interface */
	en_name = getenv("ETH_IF_NAME");
	if(en_name == NULL)
	{
		printf("\nPlease export ETH_IF_NAME env variable\n");
		printf("E.g export the wired ethernet device as export ETH_IF_NAME=enp0s9\n");
		exit(-1);
	}
}

void TestWifiAPI::tearDown(void)
{
	/* tear down functions for  WiFi channel and mode manager test cases */
}

void TestHashmapAPI::setUp(void)
{
	/* tear down functions for hashmap test cases */
	hashmap_initialize();
}

void TestHashmapAPI::tearDown(void)
{
	/* tear down functions for hashmap test cases */
	hashmap_uninitialize();
}

void TestDbConnectorAPI::setUp(void)
{
	/* tear down functions for dbconnector test cases */
}

void TestDbConnectorAPI::tearDown(void)
{
	/* tear down functions for dbconnector test cases */
}


/**
* @brief Testcase for wifi_rfkill_unblock
*/
void TestWifiAPI:: test_wifi_rfkill_unblock_tc_01(void)
{
	CPPUNIT_ASSERT_EQUAL(0, wifi_rfkill_unblock());
}


/**
* @brief Testcase for wifi_get_nuc_mac_addr
*/
void TestWifiAPI::test_wifi_get_nuc_mac_addr_tc_01(void)
{
	uint8_t mac[30];

	CPPUNIT_ASSERT_EQUAL(0, wifi_get_nuc_mac_addr(ifname, mac) );

	CPPUNIT_ASSERT_EQUAL(0, strcasecmp(wifimac, (char*)mac));
}

/**
* @brief Testcase for wifi_get_nuc_mac_addr
* with interface parameter set to NULL
*/
void TestWifiAPI::test_wifi_get_nuc_mac_addr_tc_02(void)
{
	uint8_t mac[30];

	/* Negative test-case with ifname NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_get_nuc_mac_addr(NULL, mac) );
}

/**
* @brief Testcase for wifi_get_nuc_mac_addr
* with MAC address array parameter set to NULL
*/
void TestWifiAPI::test_wifi_get_nuc_mac_addr_tc_03(void)
{
	/* Negative test-case with MAC address array is NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_get_nuc_mac_addr(ifname, NULL) );
}

/**
* @brief Testcase for wifi_get_nuc_mac_addr
* with valid non WiFi interface
*/
void TestWifiAPI::test_wifi_get_nuc_mac_addr_tc_04(void)
{
	uint8_t mac[30];

	memset(mac, 0, 30);

	/* Negative test-case with non WiFi interface */
	CPPUNIT_ASSERT_EQUAL(0, wifi_get_nuc_mac_addr(en_name, mac) );
}

/**
* @brief Testcase for wifi_set_monitor_mode
*/
void TestWifiAPI::test_wifi_set_monitor_mode_tc_01(void)
{
	CPPUNIT_ASSERT_EQUAL(0, wifi_set_monitor_mode(ifname) );
}

/**
* @brief Testcase for wifi_set_monitor_mode with a invalid
* interface name parameter
*/
void TestWifiAPI::test_wifi_set_monitor_mode_tc_02(void)
{
	/* Negative test-case with a invalid ifname*/
	CPPUNIT_ASSERT_EQUAL(-1, wifi_set_monitor_mode("invalid") );
}

/**
* @brief Testcase for test wifi_set_monitor_mode with
* with interface name parameter set to NULL
*/
void TestWifiAPI::test_wifi_set_monitor_mode_tc_03(void)
{
	/* Negative test-case with ifname set to NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_set_monitor_mode(NULL) );
}

/**
* @brief Testcase for wifi_set_channel
*/
void TestWifiAPI::test_wifi_set_channel_tc_01(void)
{
	uint32_t channel = 1;

	CPPUNIT_ASSERT_EQUAL(0, wifi_set_channel(ifname, channel) );
}

/**
* @brief Testcase for wifi_set_channel with a invalid channel
*/
void TestWifiAPI::test_wifi_set_channel_tc_02(void)
{
	uint32_t channel_invalid = 0;

	/* Negative test-case with invalid channel */
	CPPUNIT_ASSERT_EQUAL(-2, wifi_set_channel(ifname, channel_invalid) );
}

/**
* @brief Testcase for wifi_set_channel with interface name parameter set to NULL
*/
void TestWifiAPI::test_wifi_set_channel_tc_03(void)
{
	uint32_t channel = 1;

	/* Negative test-case with ifname NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_set_channel(NULL, channel) );
}

/**
* @brief Testcase for wifi_set_channel with non WiFi interface
*/
void TestWifiAPI::test_wifi_set_channel_tc_04(void)
{
	uint32_t channel = 1;

	/* Negative test-case with non WiFi interface */
	CPPUNIT_ASSERT_EQUAL(-1, wifi_set_channel(en_name, channel) );
}

/**
* @brief Testcase for wifi_set_freq
*/
void TestWifiAPI::test_wifi_set_freq_tc_01(void)
{
	double freq = 2.417;
	wifi_freq_flag_t flag = WIFI_FREQ_AUTO;

	CPPUNIT_ASSERT_EQUAL(0, wifi_set_freq(ifname, freq, flag) );
}


/**
* @brief Testcase for wifi_set_freq with invalid frequency parameter.
*/
void TestWifiAPI::test_wifi_set_freq_tc_02(void)
{
//TODO
//Strange behaviour

	double freq_invalid = 1.3;
	wifi_freq_flag_t flag = WIFI_FREQ_AUTO;

	/* Negative test-case with invalid wifi frequency of 1.3GHz */
	CPPUNIT_ASSERT_EQUAL(0, wifi_set_freq(ifname, freq_invalid, flag) );
}

/**
* @brief Testcase for wifi_set_freq with interface
* parameter set to NULL
*/
void TestWifiAPI::test_wifi_set_freq_tc_03(void)
{
	double freq_valid = 2.417;
	wifi_freq_flag_t flag = WIFI_FREQ_AUTO;

	/* Negative test-case with ifname NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_set_freq(NULL, freq_valid, flag) );
}

/**
* @brief Testcase for wifi_set_freq with valid non WiFi interface
*/
void TestWifiAPI::test_wifi_set_freq_tc_04(void)
{
	double freq_valid = 2.417;
	wifi_freq_flag_t flag = WIFI_FREQ_AUTO;

	/* Negative test-case with a non WiFi interface */
	CPPUNIT_ASSERT_EQUAL(-1, wifi_set_freq(en_name, freq_valid, flag) );
}

/**
* @brief Testcase for wifi_channel_to_freq.
*/
void TestWifiAPI::test_wifi_channel_to_freq_tc_01(void)
{
	double freq;
	int32_t channel = 1;

	CPPUNIT_ASSERT_EQUAL(0, wifi_channel_to_freq(ifname, channel, &freq) );
}

/**
* @brief Testcase for wifi_channel_to_freq with interface parameter
* set to NULL
*/
void TestWifiAPI::test_wifi_channel_to_freq_tc_02(void)
{
	double freq;
	int32_t channel = 1 ;

	/* Negative test-case with interface parameter as NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_channel_to_freq(NULL, channel, &freq) );
}

/**
* @brief Testcase for wifi_channel_to_freq with frequency parameter
* set to NULL
*/
void TestWifiAPI::test_wifi_channel_to_freq_tc_03(void)
{
	int32_t channel = 1;

	/* Negative test-case with frequency NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_channel_to_freq(ifname, channel, NULL) );
}

/**
* @brief Testcase for wifi_channel_to_freq with a invalid channel
*/
void TestWifiAPI::test_wifi_channel_to_freq_tc_04(void)
{
	double freq;
	int32_t channel = 0;

	/* Negative test-case with invalid channel */
	CPPUNIT_ASSERT_EQUAL(-2, wifi_channel_to_freq(ifname, channel, &freq) );
}

/**
* @brief Testcase for wifi_channel_to_freq
* with valid non WiFi interface
*/
void TestWifiAPI::test_wifi_channel_to_freq_tc_05(void)
{
	double freq;
	int32_t channel = 1;

	/* Negative test-case with a non WiFi interface */
	CPPUNIT_ASSERT_EQUAL(-1, wifi_channel_to_freq(en_name, channel, &freq) );
}

/**
 * @brief Testcase for wifi_get_state
 */
void TestWifiAPI::test_wifi_get_state_tc_01(void)
{
	long flags;

	CPPUNIT_ASSERT_EQUAL(0, wifi_get_state(ifname, &flags) );
}

/**
* @brief Testcase for wifi_get_state interface parameter set to NULL
*/
void TestWifiAPI::test_wifi_get_state_tc_02(void)
{
	long flags;

	/* Negative test-case with interface NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_get_state(NULL, &flags) );
}

/**
* @brief Testcase for wifi_get_state with flags parameter set to NULL
*/
void TestWifiAPI::test_wifi_get_state_tc_03(void)
{
	/* Negative test-case with flags as NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_get_state(ifname, NULL) );
}

/**
* @brief Testcase for wifi_get_state with valid non WiFi interface
*/
void TestWifiAPI::test_wifi_get_state_tc_04(void)
{
	long flags;

	/* Negative test-case with non WiFi interface */
	CPPUNIT_ASSERT_EQUAL(0, wifi_get_state(en_name, &flags) );
}

/**
* @brief Testcase for wifi_get_state with non WiFi interface and flags
* parameter as NULL
*/
void TestWifiAPI::test_wifi_get_state_tc_05(void)
{
	/* Negative test-case with non WiFi interface and flags parameter as NULL*/
	CPPUNIT_ASSERT_EQUAL(-EINVAL, wifi_get_state(en_name, NULL) );
}

/**
* @brief Testcase for hashmap_update_elem with hashmap uninitialised.
*/
void TestHashmapAPI::test_hashmap_update_elem_tc_01(void)
{
	uint8_t  key[30] = "58:91:cf:99:0c:96";
	wifi_user_record_data_t data;
	size_t len = sizeof(wifi_user_record_data_t);

	memset(&data, 0, len);

	/* un-initialize the hashmap first because its initialized in TestHashmapAPI::setUp */
	hashmap_uninitialize();

	/* Negative test-case for hashmap_update_elem */
	CPPUNIT_ASSERT_EQUAL(-6, hashmap_update_elem(key, &data, len) );

	/* re-initialize the hashmap because it is uninitialized in TestHashmapAPI::tearDown */
	hashmap_initialize();
}

/**
* @brief Testcase for hashmap_remove_elem with hashmap uninitialised.
*/
void TestHashmapAPI::test_hashmap_remove_elem_tc_01(void)
{
        size_t len = sizeof(wifi_user_record_data_t);
        uint8_t key[30] = "58:91:cf:99:0c:96";
        wifi_user_record_data_t data;

	memset(&data, 0, len);

	/* un-initialize the hashmap first because its initialized in TestHashmapAPI::setUp */
	hashmap_uninitialize();

	/* Negative test-case for hashmap_remove_elem */
        CPPUNIT_ASSERT_EQUAL(-6, hashmap_remove_elem(key, &data, len) );

	/* re-initialize the hashmap because it is uninitialized in TestHashmapAPI::tearDown */
	hashmap_initialize();
}


/**
* @brief Testcase for hashmap_length with no elements
*/
void TestHashmapAPI::test_hashmap_length_tc_01(void)
{
	CPPUNIT_ASSERT_EQUAL(0, hashmap_length() );
}

/**
* @brief Testcase for hashmap_update_elem to add 2 records
*/
void TestHashmapAPI::test_hashmap_update_elem_tc_02(void)
{
	uint8_t  key1[30] = "58:91:cf:99:0c:96";
	uint8_t  key2[30] = "58:91:cf:99:0c:98";
	wifi_user_record_data_t data;
	size_t len = sizeof(wifi_user_record_data_t);

	memset(&data, 0, len);

	CPPUNIT_ASSERT_EQUAL(0, hashmap_update_elem(key1, &data, len) );
	CPPUNIT_ASSERT_EQUAL(0, hashmap_update_elem(key2, &data, len) );
}

/**
* @brief Testcase for hashmap_length for hashmap with one element
*/
void TestHashmapAPI::test_hashmap_length_tc_02(void)
{
	uint8_t  key[30] = "58:91:cf:99:0c:96";
	wifi_user_record_data_t data;
	size_t len = sizeof(wifi_user_record_data_t);

	memset(&data, 0, len);

	CPPUNIT_ASSERT_EQUAL(0, hashmap_update_elem(key, &data, len) );

	CPPUNIT_ASSERT_EQUAL(1, hashmap_length() );
}

/**
* @brief Testcase for hashmap_update_elem with key parameter as NULL
*/
void TestHashmapAPI::test_hashmap_update_elem_tc_03(void)
{
	wifi_user_record_data_t data;
	size_t len = sizeof(wifi_user_record_data_t);

	memset(&data, 0, len);

	/* Negative test-case for hashmap_remove_elem with key parameter NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, hashmap_update_elem(NULL, &data, len) );
}

/**
* @brief Testcase for hashmap_update_elem with userdata record parameter as NULL.
*/
void TestHashmapAPI::test_hashmap_update_elem_tc_04(void)
{
	size_t len = sizeof(wifi_user_record_data_t);
	uint8_t  key[30] = "58:91:cf:99:0c:96";

	/* Negative test-case for hashmap_remove_elem with data parameter NULL */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, hashmap_update_elem(key, NULL, len) );
}

/**
* @brief Testcase for hashmap_update_elem with userdata record length zero.
*/
void TestHashmapAPI::test_hashmap_update_elem_tc_05(void)
{
	wifi_user_record_data_t data;
	uint8_t  key[30] = "58:91:cf:99:0c:96";

	memset(&data, 0, sizeof(wifi_user_record_data_t));

	/* Negative test-case for hashmap_remove_elem with len parameter as 0 */
	CPPUNIT_ASSERT_EQUAL(-EINVAL, hashmap_update_elem(key, &data, 0) );
}

/**
* @brief Testcase for hashmap_remove_elem with invalid key.
*/
void TestHashmapAPI::test_hashmap_remove_elem_tc_02(void)
{
	uint8_t  key[30] = "58:91:cf:99:0c:96";
	uint8_t  invalid_key[30] = "88:11:cf:29:0c:36";
	wifi_user_record_data_t data;
	size_t len = sizeof(wifi_user_record_data_t);

	memset(&data, 0, len);

	CPPUNIT_ASSERT_EQUAL(0, hashmap_update_elem(key, &data, len) );

	memset(&data, 0, len);

        CPPUNIT_ASSERT_EQUAL(-3, hashmap_remove_elem(invalid_key, &data, len) );
}

/**
* @brief Testcase for hashmap_remove_elem with data parameter set to NULL
*/
void TestHashmapAPI::test_hashmap_remove_elem_tc_03(void)
{
	uint8_t  key[30] = "58:91:cf:99:0c:96";
	wifi_user_record_data_t data;
	size_t len = sizeof(wifi_user_record_data_t);

	memset(&data, 0, len);

	CPPUNIT_ASSERT_EQUAL(0, hashmap_update_elem(key, &data, len) );

	memset(&data, 0, len);

        CPPUNIT_ASSERT_EQUAL(-EINVAL, hashmap_remove_elem(key, NULL, len) );
}

/**
* @brief Testcase for hashmap_remove_elem with data length set to zero.
*/
void TestHashmapAPI::test_hashmap_remove_elem_tc_04(void)
{
	uint8_t  key[30] = "58:91:cf:99:0c:96";
	wifi_user_record_data_t data;
	size_t len = sizeof(wifi_user_record_data_t);

	memset(&data, 0, len);

	CPPUNIT_ASSERT_EQUAL(0, hashmap_update_elem(key, &data, len) );

	memset(&data, 0, len);

        CPPUNIT_ASSERT_EQUAL(-EINVAL, hashmap_remove_elem(key, &data, 0) );
}

/**
* @brief Testcase for hashmap_remove_elem
*/
void TestHashmapAPI::test_hashmap_remove_elem_tc_05(void)
{
	uint8_t  key[30] = "58:91:cf:99:0c:96";
	wifi_user_record_data_t data;
	size_t len = sizeof(wifi_user_record_data_t);

	memset(&data, 0, len);

	CPPUNIT_ASSERT_EQUAL(0, hashmap_update_elem(key, &data, len) );

	memset(&data, 0, len);

        CPPUNIT_ASSERT_EQUAL(0, hashmap_remove_elem(key, &data, len) );
}

/**
* @brief Testcase for init_database_connector to initialise database connection.
*/
void TestDbConnectorAPI::test_init_database_connector_tc_01(void)
{
	CPPUNIT_ASSERT_EQUAL(0, init_database_connector() );
}

/**
* @brief Testcase for add_record_database to add record to database.
*/
void TestDbConnectorAPI::test_add_record_to_database_tc_02(void)
{
	wifi_user_record_data record = {1, -20, "58:91:cf:99:0c:96", "28:90:cf:92:11:96", time(NULL), time(NULL), 4};
	CPPUNIT_ASSERT_EQUAL(0, add_record_database(&record) );
}

/**
* @brief Testcase for free_database_connector to free database connection.
*/
void TestDbConnectorAPI::test_free_database_connector_tc_03(void)
{
	free_database_connector();
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestWifiAPI );
CPPUNIT_TEST_SUITE_REGISTRATION( TestHashmapAPI );
CPPUNIT_TEST_SUITE_REGISTRATION( TestDbConnectorAPI );

int main(int argc, char* argv[])
{
	// informs test-listener about testresults
	CPPUNIT_NS::TestResult testresult;

	// register listener for collecting the test-results
	CPPUNIT_NS::TestResultCollector collectedresults;
	testresult.addListener (&collectedresults);

	// register listener for per-test progress output
	CPPUNIT_NS::BriefTestProgressListener progress;
	testresult.addListener (&progress);

	// insert test-suite at test-runner by registry
	CPPUNIT_NS::TestRunner testrunner;
	testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
	testrunner.run(testresult);

	// output results in compiler-format
	CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
	compileroutputter.write ();

	// Output XML for Jenkins CPPunit plugin
	ofstream xmlFileOut("cppTestWifi.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}
