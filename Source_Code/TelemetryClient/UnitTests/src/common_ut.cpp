#include <iostream>
#include <fstream>

#include "common_ut.h"

mockConfigurationReader *m_configInstance;
bool mockspDevOpenFlag = true;
bool mockspDevSetPropertyBFlag = true;
bool mockspDevRegCallbackFlag = true;
bool mockspDevClosenFlag = true;

mockConfigurationReader* mockConfigurationReader::getInstance()
{
	return m_configInstance;
}

static mockConfigurationReader1 *m_configInstance1;

mockConfigurationReader1* mockConfigurationReader1::getInstance()
{
	m_configInstance1 = new mockConfigurationReader1();

	m_configInstance1->m_DatabaseHostName = "localhost";
	m_configInstance1->m_DatabaseUserName = "root";
	m_configInstance1->m_DatabasePassword = "root123";
	m_configInstance1->m_DatabaseName = "sensor_db";
	m_configInstance1->m_DatabasePort = 3306;

	return m_configInstance1;
}


SP_STATUS_CODE mockspDevOpen(int32_t nDeviceID)
{
	if (mockspDevOpenFlag)
		return SP_SUCCESS;
	else
		return SP_E_INVALID_SYS_STATE;
}

SP_STATUS_CODE mockspDevSetPropertyB(int32_t nDeviceID, SP_DEV_PROPERTIES eProp, bool bPropertyValue)
{
	if (mockspDevSetPropertyBFlag)
		return SP_SUCCESS;
	else
		return SP_E_INVALID_SYS_STATE;
}

SP_STATUS_CODE mockspDevRegCallback(int32_t nDeviceID, SP_EVENT_NOTIFICATION eventNotification)
{
	if (mockspDevRegCallbackFlag)
		return SP_SUCCESS;
	else
		return SP_E_INVALID_SYS_STATE;
}

SP_STATUS_CODE mockspDevClose(int32_t nDeviceID)
{
	if (mockspDevClosenFlag)
		return SP_SUCCESS;
	else
		return SP_E_INVALID_SYS_STATE;
}


/*---------------------SETTER----------------------*/
void mockQBluetoothDeviceInfo::set_name(QString n)
{
	_name = n;
}

void mockQBluetoothDeviceInfo::set_address(QBluetoothAddress a)
{
	_address = a;
}

void mockQBluetoothDeviceInfo::set_rssi(int r)
{
	_rssi = r;
}

/*--------------------getter------------------------*/
QString mockQBluetoothDeviceInfo::name()
{
	return _name;
}

QBluetoothAddress mockQBluetoothDeviceInfo::address()
{
	return _address;
}

int mockQBluetoothDeviceInfo::rssi()
{
	return _rssi;
}

/*------------------------------------------------------------------------------------*/
/*
void mockQBluetoothAddress::set_address(QString a)
{
	_address = a;
}

QString mockQBluetoothAddress::address()
{
	return _address;
}

QString mockQBluetoothAddress::toString()
{
	const QString a = _address;
	return a;
}*/
