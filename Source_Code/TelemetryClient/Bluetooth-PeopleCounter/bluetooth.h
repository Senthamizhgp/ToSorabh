

#ifndef BLSCANNER_H
#define BLSCANNER_H
/*
 * System headers
 */
#include <map>
#include <list>
#include <iostream>
#include <string.h>

/*
 * Qt headers for QtSql Database
 */
#include "QtCore/qtimer.h"
#include "QtBluetooth/qbluetoothglobal.h"
#include "QtCore/qcoreapplication.h"
#include "QtBluetooth/qbluetoothlocaldevice.h"
#include "QtBluetooth/qbluetoothdevicediscoveryagent.h"
#include "QtCore/qdebug.h"
#include "QtSql/qsql.h"
#include "QtSql/qsqlquery.h"
#include <QtSql/qsqlrecord.h>
#include <QtSql/qsqlfield.h>
#include "QtSql/qsqlerror.h"
#include <QMap>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusMessage>
/*
 * Own headers
 */
#ifndef UNIT_TEST
#include "ConfigurationReader.h"
#endif				// !UNIT_TEST

//#include "common.h"
#include "dataobject.h"

#ifdef UNIT_TEST
#include <iostream>
#include "common_ut.h"

#define QBluetoothDeviceInfo			mockQBluetoothDeviceInfo
#define	ConfigurationReader				mockConfigurationReader1
#endif				// UNIT_TEST

#ifndef UNIT_TEST
#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_DLL
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllexport))
#else
#define DLL_PUBLIC __declspec(dllexport)	// Note: actually gcc
                        // seems to also supports
                        // this syntax.
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllimport))
#else
#define DLL_PUBLIC __declspec(dllimport)	// Note: actually gcc
                        // seems to also supports
                        // this syntax.
#endif
#endif
#define DLL_LOCAL
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif

#endif
#endif

#define MAX_BUF_LEN		255
#ifndef UNIT_TEST
enum STATUS_CODE {
    SUCCESS = 0,		//  Success
    BT_FAILURE = 1,		//  Failed to perform the Scanning
    DB_FAILURE = 2,		//  DB failure on using QtSql methods

};
#endif
#define BLE_DB_CONNECTION_NAME "BLE_DB_CONNECTION_NAME"

typedef struct arguments {
    std::string key;
    std::string value;
} arguments;

class DLL_PUBLIC Bluetooth:public QObject {
#ifndef UNIT_TEST
    Q_OBJECT
#endif
  public:
    Bluetooth();
    #ifndef UNIT_TEST
    STATUS_CODE     init();  
    #endif

	bool 			execInsertionQuery(string strQuery, struct arguments arg[], int argCount);
	bool 			execUpdateQuery(string strQuery, struct arguments arg[], int argCount);
	bool 			execSelectQuery(const string &strQuery, map<string, string> &arguments, QList<QMap<QString, QVariant> > &records);
    void			addDevice(QBluetoothDeviceInfo device);
                   ~Bluetooth();


#ifndef UNIT_TEST
                    signals:void finished();
#endif
    public          slots:void startDeviceDiscovery();
#ifndef UNIT_TEST
    void            reScan();
#endif
    void            scanFinished();
    void            printError(QBluetoothDeviceDiscoveryAgent::
                   	Error error);

#ifdef UNIT_TEST
    int             devicelistsize = 0;
    int             IsdbOpen = 0;
    QString         ScanTime;
#endif

  private:
    QBluetoothDeviceDiscoveryAgent * discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    std::vector < DataObject * >dataList;
    std::map < QString, DataObject * >deviceList;
    QString         scanTime;
    QList<QBluetoothDeviceInfo>  listOfDevices;
    QSqlDatabase    db;
    int             acquitionFrequency;
    #ifndef UNIT_TEST
                    QCoreApplication *app;
        #endif
};
#endif

