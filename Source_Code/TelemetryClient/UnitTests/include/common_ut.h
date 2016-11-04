/*
 * common_stubs.h
 */

#ifndef COMMON_STUBS_H
#define COMMON_STUBS_H

#include <stdint.h>
#include <string>

#include <QtCore/qstring.h>
#include <QtCore/qmetatype.h>
#include "QtCore/qtimer.h"
#include "QtBluetooth/qbluetoothglobal.h"
#include "QtCore/qcoreapplication.h"
#include "QtBluetooth/qbluetoothlocaldevice.h"

#include "SPInterface.h"

#ifdef DLL_EXPORT
#undef DLL_EXPORT
#define DLL_EXPORT
#endif

#ifdef DLL_PUBLIC
#undef DLL_PUBLIC
#define DLL_PUBLIC
#endif

#ifdef DLL_LOCAL
#undef DLL_LOCAL
#define DLL_LOCAL
#endif

using namespace std;

/* mock class for Configuration Reader */
class mockConfigurationReader1 {
public:
	string m_DatabaseHostName;						///< Holds hostname for the database
	string m_DatabaseUserName;						///< Holds username for the database
	string m_DatabasePassword;						///< Holds password for the database
	string m_DatabaseName;							///< Holds name of the database
unsigned short m_DatabasePort;
int m_BLEAcquisitionFreq;
	static mockConfigurationReader1* getInstance();
};

class mockConfigurationReader {
public:
	string m_DatabaseHostName;						///< Holds hostname for the database
	string m_DatabaseUserName;						///< Holds username for the database
	string m_DatabasePassword;						///< Holds password for the database
	string m_DatabaseName;							///< Holds name of the database
	unsigned short m_DatabasePort;		
	static mockConfigurationReader* getInstance();
};

/*class mockQBluetoothAddress {
private:
	QString _address;
public:
	void set_address(QString a);
	QString address();
	QString toString();
};*/

class mockQBluetoothDeviceInfo {
private:
	QString _name;
	QBluetoothAddress _address;
	int _rssi;
public:
	void set_name(QString n);
	void set_address(QBluetoothAddress a);
	void set_rssi(int r);
	QString name();
	QBluetoothAddress address();
	int rssi();
};


/* mock Sunset Pass functions */
SP_STATUS_CODE mockspDevOpen(int32_t id);
SP_STATUS_CODE mockspDevSetPropertyB(int32_t nDeviceID, SP_DEV_PROPERTIES eProp, bool bPropertyValue);
SP_STATUS_CODE mockspDevRegCallback(int32_t nDeviceID, SP_EVENT_NOTIFICATION eventNotification);
SP_STATUS_CODE mockspDevClose(int32_t nDeviceID);

#endif