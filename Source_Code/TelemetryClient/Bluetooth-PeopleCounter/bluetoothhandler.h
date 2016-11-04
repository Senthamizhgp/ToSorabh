#ifndef BLUETOOTHHANDLER_H
#define BLUETOOTHHANDLER_H

/*
 * System headers
 */
#include <map>
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
#include "QtSql/qsqldatabase.h"
#include "QtSql/qsqlquery.h"
#include "QtSql/qsqlerror.h"
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusMessage>
/*
 * Own headers
 */
#ifndef UNIT_TEST
#include "ConfigurationReader.h"
#endif				// !UNIT_TEST
#include "bluetooth.h"

//#include "common.h"
#include "dataobject.h"

#ifdef UNIT_TEST
#include <iostream>
#include "common_ut.h"

#define QBluetoothDeviceInfo			mockQBluetoothDeviceInfo
#define QBluetoothAddress				mockQBluetoothAddress
#define	ConfigurationReader				mockConfigurationReader
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




   DLL_PUBLIC STATUS_CODE     init_BLE();
   DLL_PUBLIC  void exit_BLE();
















#endif // BLUETOOTHHANDLER_H
