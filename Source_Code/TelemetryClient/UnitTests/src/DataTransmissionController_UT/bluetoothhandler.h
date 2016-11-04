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

#define MAX_BUF_LEN		255


enum STATUS_CODE {
    SUCCESS = 0,		// /< Success
    BT_FAILURE = 1,		// /< Failed to perform the Scanning
                // operation
    DB_FAILURE = 2,		// /< DB failure on using QtSql methods

};

class BluetoothHandler {

  public:
    BluetoothHandler();
    STATUS_CODE     init();

    ~BluetoothHandler();
private:
   // Bluetooth b;
};















#endif // BLUETOOTHHANDLER_H
