
/******************************************************************************
*
* INTEL CONFIDENTIAL
* Copyright 2016 Intel Corporation All Rights Reserved.
*
* The source code contained or described herein and all documents related to the
* source code ("Material") are  owned by Intel Corporation or its suppliers or
* licensors. Title to the Material remains with Intel Corporation or its suppliers
* and licensors. The Material contains trade secrets and proprietary and
* confidential information of Intel or its suppliers and licensors. The Material
* is protected by worldwide  copyright and trade secret laws and treaty provisions.
* No part of the Material may be used, copied, reproduced, modified, published,
* uploaded, posted, transmitted, distributed, or disclosed in any way without
* Intel's prior express written permission.
*
* No license under any patent, copyright, trade secret or other intellectual
* property right is granted to or conferred upon you by disclosure or delivery of
* the Materials, either expressly, by implication,  inducement, estoppel or
* otherwise. Any license under such intellectual property rights must be express
* and approved by Intel in writing.
*
******************************************************************************/

/**
 *
 *	@file		dataobject.cpp
 *
 *	@brief		This is model class to hold bluetooth device information.
 *				by the Blutooth module.
 *
 *
 */

#include "dataobject.h"
#include <vector>
#include "QtCore/qdebug.h"
#include <iostream>

using namespace std;
/**
 * @brief
 * default Constructor method to create dataobject Object.
 */
DataObject::DataObject()
{

}


/**
 * @brief
 *  Constructor method to create dataobject Object.
 */
DataObject::DataObject(const QString &name, const QString &mac, const QString &timestamp, const short &rssi)
{
    a_name = name;
    a_mac = mac;
    a_timestamp = timestamp;
    a_startTime=timestamp;
    a_rssi=rssi;
}



/**
 * @brief
 *  returns name of bluetooth device.
 */
 QString DataObject::name()
{
    return  a_name;
}

void DataObject::setName(const QString &name)
{
    if (a_name !=  name) {
        a_name = name;
        //emit nameChanged();
    }
}
/**
 * @brief
 *  returns RSSI signal value of bluetooth device.
 */
short int  DataObject::rssi()
{
   return  a_rssi;
}

void DataObject::setRssi(const short int  &rssi)
{
   if (a_rssi !=  rssi) {
        a_rssi = rssi;
        //emit nameChanged();
   }
}
/**
 * @brief
 *  returns bluetooth address of bluetooth device.
 */
QString DataObject::mac() const
{
    return  a_mac;
}

void DataObject::setMac(const QString &mac)
{
    if (a_mac !=  mac  ) {
         a_mac = mac;
    }
}
/**
 * @brief
 *  returns detected timestamp  of bluetooth device.
 */
QString DataObject::timestamp() const
{
    return  a_timestamp;
}


QString DataObject::startTime() {
   return  a_startTime;
}

void DataObject::setStartTime(const QString &start_time){
    if (a_startTime !=  start_time  ) {
         a_startTime = start_time;
     //   emit timestampChanged();
    }
}



void DataObject::setTimestamp(const QString &timestamp)
{
    if (a_timestamp !=  timestamp  ) {
         a_timestamp = timestamp;
     //   emit timestampChanged();
    }
}

bool DataObject::operator==(DataObject& rhs)
{
    if (a_name.compare(rhs.name())!=0)
        return false;
    if (a_mac.compare( rhs.mac())!=0)
        return false;
    return true;
}
