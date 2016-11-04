// MessageHandler_UT.cpp : Defines the entry point for the console application.
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

#include "MessageHandler.h"
#include "MessageQueue.h"


using namespace CppUnit;
using namespace std;

class MessageHandler_UT :
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(MessageHandler_UT);
	CPPUNIT_TEST(test_getInstanceSingleton);
	CPPUNIT_TEST(test_MessageHandler_Init);
	CPPUNIT_TEST(test_MessageHandler_Init_Failure);
	CPPUNIT_TEST(test_MessageHandler_ProcessMessage);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test_getInstanceSingleton(void);
	void test_MessageHandler_Init(void);
	void test_MessageHandler_Init_Failure(void);
	void test_MessageHandler_ProcessMessage(void);
//	void test_MessageHandler_cleaunp(void);

private:
	MessageHandler *m_MessageHandler;
};

void MessageHandler_UT::setUp()
{
	m_MessageHandler = MessageHandler::getInstance();
}

void MessageHandler_UT::tearDown()
{
	delete m_MessageHandler;
}


/**** Below functions are static callback functions and are not tested separately rather covered during Init 
static void onConnect(void* context, MQTTAsync_successData* response);
static void onConnectFailure(void* context, MQTTAsync_failureData* response);
static void onDisconnect(void* context, MQTTAsync_successData* response);
static void onSend(void* context, MQTTAsync_successData* response);
static void connlost(void *context, char *cause);
static void onSubscribe(void* context, MQTTAsync_successData* response);
static void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
****/

void MessageHandler_UT::test_getInstanceSingleton(void)
{
	int isSameFlag = 0;
	//  Ensure always we obtain singleton instance
	MessageHandler *newDTCInstance;
	newDTCInstance = MessageHandler::getInstance();
	
	if (newDTCInstance == m_MessageHandler)
	isSameFlag =1;

	CPPUNIT_ASSERT_EQUAL(1, isSameFlag);
}

void MessageHandler_UT::test_MessageHandler_Init()
{
	m_MessageHandler->Init();
}

void MessageHandler_UT::test_MessageHandler_Init_Failure()
{
	//rename("./TelemetryConfiguration.xml", "./TelmetryConfiguration_org.xml");

	//rename("./TelemetryConfiguration_fake.xml", "./TelmetryConfiguration.xml");
/*
	MQTTAsync_disconnectOptions disconnectOpts = MQTTAsync_disconnectOptions_initializer;
	disconnectOpts.onSuccess = onDisconnect;
	disconnectOpts.context = m_clientInstance;

	if ((retCode = MQTTAsync_disconnect(m_MessageHandler->m_clientInstance, &disconnectOpts)) != MQTTASYNC_SUCCESS)
	{
		printf("[MessageHandler] ERROR:: Failed to disconnect the client, return code %d\n", retCode);		
	}
	
	while (1 == MQTTAsync_isConnected(m_MessageHandler->m_clientInstance))
	{
		// Wait until the disconnect is successful
	}

	// Destroy the client instance
	MQTTAsync_destroy(&m_MessageHandler->m_clientInstance);
*/
	m_MessageHandler->Init();
}

void MessageHandler_UT::test_MessageHandler_ProcessMessage() 
{
	MessageList *messageListInstance = MessageList::getInstance();

	// Lock the mutex before filling the message queue
	messageListInstance->m_queueMutex.lock();

	// Fill the message queue with message
	messageListInstance->Add(1, "Sample Message");

	// Unlock the mutex after completing the filling
	messageListInstance->m_queueMutex.unlock();	

	m_MessageHandler->ProcessMessage();
}

/*********************/

CPPUNIT_TEST_SUITE_REGISTRATION(MessageHandler_UT);

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
	ofstream xmlFileOut("Test_Results_Messagehandler_UT.xml");
	XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	getchar();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}
