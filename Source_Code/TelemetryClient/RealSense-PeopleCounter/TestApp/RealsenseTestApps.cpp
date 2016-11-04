// RealsenseTestApps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "C:\Users\intelnuc\Desktop\Hari\RealSense-dll-complete\Intel_Intelligent_Vending_and_Smart_Shelf\src\RealSensePeopleCounter\RealSensePeopleCounter.h"
#include <thread>
#include <iostream>

void task1(std::string msg)
{
	while (true) {
		std::cout << "task1 says: ";
		std::this_thread::sleep_for(std::chrono::seconds(1));

	}
}
int main()
{
	std::cout << "hi";


	//Creating a thread for Wi-Fi module
	std::thread t1(task1, "Hello");

	//Creating a thread for Bluetooth module
	//std::thread BLEHandlerThread(BLEENTRYPOINT);
	//RealSense rsb;


	// Create a thread for the Telemetry client
	//MessageHandler *m_messageHandlerinstance = MessageHandler::getInstance();
 std::thread t2(&RealSense::init_RS_PeopleCounter, new RealSense());






//	rsb.init_RS_PeopleCounter();
	    
	for (int i = 0;i < 100;i++) {
		
	}
	//rsb.exit_RS_PeopleCounter();
	std::cout << "bye";
	while(true);
    return 0;
}

