/******************************************************************************************
 *
 * INTEL CONFIDENTIAL
 * Copyright 2015 Intel Corporation All Rights Reserved.
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
*******************************************************************************************/


/**
*
*  @file                LLAPIExport.h

*  @brief               This file contains all ENUMs and structures and definitions and API declaration, that will be exported in LLAPI DLL file.
*
*   \b  Intel_Sunset_Pass_Module
*
*  @copyright           2010-2015 All rights reserved
*  @version             Revision: 5.0
*                       Gold Release
*  @date                16/03/2015
*
*
*  \par  Software License Agreement:
*
*     The software supplied herewith by Intel's vendor to Intel for Sunset Pass Project for use exclusively
*     on PC. The software is owned by the Intel, and is protected under applicable copyright laws.
*
*
*  \par  Change History:
*
*  Rev   |  Description                                                 |  Author         |  Date
*  ----  |  -----------                                                 |  ---------------|  -------
*  0.01  |  Created file with all APIs                                  |  SW_LLAPI_DEV_1 |  21/10/2013
*  1.00  |  Alpha Release                                               |  SW_LLAPI_DEV_1 |  03/01/2014
*  2.00  |  Beta Release                                                |  SW_LLAPI_DEV_1 |  28/02/2014
*  2.01  |  Beta Release                                                |  SW_LLAPI_DEV_1 |  20/03/2014
*  3.00  |  Gold Release                                                |  SW_LLAPI_DEV_1 |  17/04/2014
*  3.20  |  Klocwork compliance                                         |  SW_LLAPI_DEV_1 |  15/09/2014
*  3.30  |  Phase B Beta Part 2                                         |  SW_LLAPI_DEV_1 |  22/09/2014
*  4.00  |  Phase B Gold                                                |  SW_LLAPI_DEV_1 |  22/09/2014
*  4.10  |  Phase B Gold with timestamp support							|  SW_LLAPI_DEV_1 |  11/11/2014
*  5.00  |  Phase B Final Gold Release									|  SW_LLAPI_DEV_1 |  17/03/2015
*  5.10  |  Phase B Amendement Local Port Release						|  SW_LLAPI_DEV_1 |  29/05/2015
*  5.20  |  serRead Bug Resolved Related to TimeOut						|  SW_LLAPI_DEV_1 |  12/06/2015
*/


#ifndef EXPORT_H_
#define EXPORT_H_

/**
* @defgroup    GENERAL GENERAL
*
* @brief       This is general group that contains resources common to all modules
*
*              This module contains all Enums, Structures, and Definitions, which is used by
*              all modules.
*
*/
/**
* @addtogroup  GENERAL
* @{
*/

/*****************************************************************************************************
 *                               GENERAL DEFINE
 ****************************************************************************************************/
//#define _DEBUG
//#define DLL_EXPORT 1                    ///<Disable this for DLL API Import
#ifdef _WIN32
#ifdef DLL_EXPORT
#define DLL_API __declspec(dllexport)     ///<DLL export macro
#else
#define DLL_API __declspec(dllimport)     ///<DLL import Macro
#endif
#endif

#ifdef __linux__
#define DLL_API
#endif

#include <list>
using namespace std;
/*****************************************************************************************************
 *                               GENERAL TYPEDEF
 ****************************************************************************************************/

typedef unsigned char         sUINT8;     ///<8 bit variable
typedef signed char           sINT8;      ///<7 bit variable
typedef unsigned char*        sUINT8P;    ///<8 bit variable pointer
typedef const unsigned char*  sCUINT8P;   ///<8 bit constant variable pointer
typedef short int             sINT16;     ///<15 bit variable
typedef unsigned short        sUINT16;    ///<16 bit variable
typedef unsigned short*       sUINT16P;   ///<16 bit variable pointer
typedef long                  sINT32;     ///<31 bit variable
typedef unsigned long         sUINT32;    ///<32 bit variable
typedef unsigned long*        sUINT32P;   ///<32 bit variable pointer
typedef const unsigned long*  sCUINT32P;  ///<32 bit variable pointer
typedef float                 sFLOAT;     ///<float variable
typedef float*                sFLOATP;   ///<float variable pointer
typedef double                sDOUBLE;    ///<double variable
typedef unsigned char         sBOOL;      ///<8 bit variable
typedef void*                 sVOIDP;     ///<memory pointer
typedef const void*           sCVOIDP;    ///<memory pointer

/*****************************************************************************************************
 *                               GENERAL ENUM
 ****************************************************************************************************/

/**
*  ENUM for different ports of SunSet Pass Module
*/
enum PortID
{
   S_MDB = 1,                    ///<MDB port for MDB bus
   S_RS232_485_1 = 2,            ///<RS232/RS485 port 1
   S_RS232_485_2 = 3,            ///<RS232/RS485 port 2
   S_DEX = 4,                    ///<DEX port
   S_I2C = 5,                    ///<I2C port
   S_PWM_1 = 6,                  ///<Pulse Width Modulation port 1
   S_PWM_2 = 7,                  ///<Pulse Width Modulation port 2
   S_TACH_1 = 8,                 ///<Tachometer 1
   S_TACH_2 = 9,                 ///<Tachometer 2
   S_SPI = 10,                   ///<SPI port 1
   S_CAN = 11,                   ///<Controller Area Network port
   S_MOTOR = 12,                 ///<Motor
   S_SEVEN_SEGMENT = 13,         ///<Seven Segment
   S_FIRMWARE_UPGRADE = 14,      ///<Firmware upgrade
   S_GPO = 15,                   ///<general purpose output
   S_GPI = 16,                   ///<general purpose input
   S_POWER_MANAGEMENT = 17,      ///<power management
   S_EVENT_MANAGEMENT = 18,      ///<event management
   S_ERROR_MANAGEMENT = 19,      ///<error management
   S_CCTALK = 20,                ///<ccTalk
   S_WIRE_1 = 21,                ///<1- wire
   S_PROTOCOL_A = 22,            ///<protocol A
   S_ANALOG = 23,                ///<Analog Voltage measurement
   S_COMM_CHANNEL = 24,          ///<Communication
   S_RTC = 25,                   ///<RTC
   S_I2C_LOCAL_PORT_0 = 26,      ///<I2C 0 on Local Port
   S_I2C_LOCAL_PORT_1 = 27,      ///<I2C 1 on Local Port
   S_UART_LOCAL_PORT_1 = 28,     ///<UART 1 on Local Port
   S_UART_LOCAL_PORT_2 = 29,     ///<UART 2 on Local Port
   S_SPI_LOCAL_PORT = 30,		 ///<8 channel USB Relay Module
   S_USB_RELAY_MODULE = 31,		 ///<8 channel USB Relay Module
   S_GPIO_LOCAL_PORT = 32,		 ///<General purpose input/output Port connected on Embedded PC
   S_PWM_LOCAL_PORT = 33,		 ///<Pulse Width Modulation Port connected on Embedded PC
   S_ANALOG_LOCAL_PORT = 34,	 ///<Analog Voltage measurement Port connected on Embedded PC
   NUMBER_OF_PORTS = 35,			 ///<Total no of ports
};

/**
 * ENUM for ERROR CODE which will be return value of LLAPI
 */
enum returnStatus
{
   S_SUCCESS = 0,                   ///<Task completed successfully
   S_ERROR_FAILURE = 1,             ///<Task is not completed, due to some reason
   S_ERROR_PORT_OPEN = 2,           ///<Error occurred while opening PC channel driver port
   S_ERROR_PORT_CLOSE = 3,          ///<Error occurred while closing PC channel driver port
   S_ERROR_CONFIG_PORT = 4,         ///<Error occurred while configuring PC channel driver port
   S_ERROR_READ_BUFFER = 5,         ///<Error occurred while reading data from internal port buffer to local port of embedded PC
   S_ERROR_WRITE_BUFFER = 6,        ///<Error occurred while writing data to internal port buffer or to local port of embedded PC
   S_ERROR_ARGUMENT = 7,            ///<API argument is not valid
   S_ERROR_CHANNEL_INVALID = 8,     ///<Proper channel is not selected
   S_ERROR_TIME_OUT = 9,            ///<Waiting Time is over
   S_ERROR_SEND = 10,               ///<Error occurred while sending command to Sunset Pass Module, due to lack of acknowledgment from Sunset Pass Module or timeout.
   S_ERROR_SEQUENCE = 11,           ///<Violating task sequence I.e. configure then open etc.
   S_ERROR_PORT_ACQUIRED = 14,      ///<Error occurred while trying to acquire handle, acquired by else before and not released yet
   S_ERROR_HANDLE_MISMATCH = 15,    ///<Error occurred while port handle supplied as argument is not matching with handle acquired
   S_BOOT_MODE = 16,                ///<Microcontroller is in Boot Mode
   S_ERROR_DIRECTION_SET = 17,     ///<Error occurred while setting direction of local port GPIO pin
   S_ERROR_DIRECTION_GET = 18,		///<Error occurred while getting direction of local port GPIO pin
   S_ERROR_ENABLE_INTERRUPT = 19,  ///<Error occurred while enable the GPIO pin interrupt
   S_ERROR_DISABLE_INTERRUPT =20,  ///<Error occurred while disable the GPIO pin interrupt
   S_ERROR_MONITOR_EVENT = 21,		///<Error occurred while monitor the GPIO pin interrupt
   S_ERROR_POLL_TIMEOUT = 22,    ///<Error occurred while poll timeout
   S_ERROR_DIRECTION_INVALID = 23, ///<Error occured while Invalid direction detected
   S_ERROR_PINVAL_SP_INVALID = 24,	///<Error occured while more than 8-bit wide Pin Number for SPM Board
   
   S_ERROR_INSTANCE_ACTIVATE = 25,   ///<Error occurred while activating the instance of the controller
   S_ERROR_INSTANCE_CREATE = 26,	  ///<Error occurred while getting the instance of the controller
   S_ERROR_PIN_OPEN = 27,			  ///<Error occurred while opening the gpio controller pin
   S_ERROR_PIN_READ_LOW = 28,		 ///<Error occurred while reading the low value of the gpio controller pin
};


/**
* ENUM for SMBus Alert Pin number
*/
enum SMBusAlertPin
{
	S_SMBUS_ALERT_POLLING = -1,	
	S_SMBUS_ALERT_PIN_MIN = 0,
	S_SMBUS_ALERT_PIN1 = 1,
	S_SMBUS_ALERT_PIN2 = 2,
	S_SMBUS_ALERT_PIN3 = 3,
	S_SMBUS_ALERT_PIN4 = 4,
	S_SMBUS_ALERT_PIN5 = 5,
	S_SMBUS_ALERT_PIN6 = 6,
	S_SMBUS_ALERT_PIN7 = 7,
	S_SMBUS_ALERT_PIN8 = 8,
	S_SMBUS_ALERT_PIN9 = 9,
	S_SMBUS_ALERT_PIN10 = 10,
	S_SMBUS_ALERT_PIN11 = 11,
	S_SMBUS_ALERT_PIN12 = 12,
	S_SMBUS_ALERT_PIN13 = 13,
	S_SMBUS_ALERT_PIN14 = 14,
	S_SMBUS_ALERT_PIN15 = 15,
	S_SMBUS_ALERT_PIN16 = 16,
	S_SMBUS_ALERT_PIN17 = 17,
	S_SMBUS_ALERT_PIN18 = 18,
	S_SMBUS_ALERT_PIN19 = 19,
	S_SMBUS_ALERT_PIN20 = 20,
	S_SMBUS_ALERT_PIN21 = 21,
	S_SMBUS_ALERT_PIN22 = 22,
	S_SMBUS_ALERT_PIN23 = 23,
	S_SMBUS_ALERT_PIN24 = 24,
	S_SMBUS_ALERT_PIN25 = 25,
	S_SMBUS_ALERT_PIN26 = 26, 	
	S_SMBUS_ALERT_PIN_MAX = 27,
};


/**
* @}
*/

extern "C"
{

/*****************************************************************************************************
 *                               VERSION
 ****************************************************************************************************/
/**
* @defgroup    VERSION VERSION
*
* @brief       LLAPI version related module
*
*              This module contains API that returns current version of LLAPI
*/
/**
* @addtogroup  VERSION
* @{
*/

/**
 -----------------------------------------------------------------------------------------------------------------------
 llapiVersionRead
 -----------------------------------------------------------------------------------------------------------------------
*  This function will return current version of LLAPI DLL.
*
*  @date                27/11/2013
*  @author              SW_LLAPI_DEV_1
*
*  @return              version number of LLAPI DLL
*                       \n It will be a pointer of string of 5 character
*
************************************************************************************************************************
*/
DLL_API sUINT8P llapiVersionRead();

/**
* @}
*/

/*****************************************************************************************************
 *                               MODULE_HANDLE
 ****************************************************************************************************/
/**
* @defgroup    MODULE_HANDLE  MODULE_HANDLE
*
* @brief       module related to acquire and release handle.
*
*              This is the handle module, mainly responsible for acquiring and releasing handle for any particular
*              port. handle mechanism is used to control the multiple access to particular port. For example once 
               the handle is acquired by one application for MDB port, none of the other application can acquire
               the handle for same MDB port. Once the first application releases the handle then and then only
               the second application can acquired the handle for the same port. handle is mandatory parameter for 
               accessing any of the LLAPI for particular port.
*/
/**
* @addtogroup  MODULE_HANDLE
* @{
*/

/**
 -----------------------------------------------------------------------------------------------------------------------
 acquireHandle
 -----------------------------------------------------------------------------------------------------------------------
*  This function will acquire the handle for particular port. This handle will be used for accessing
*  any of the LLAPI for that particular port 
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 None
*
*  @param ePortId       Provide the port ID for which handle need to be acquired. The valid ports are provided in Enum
*                       portID, Refer this Enum @ref PortID
*  @param *uipHandle    Reference pointer for receiving the handle for respective port, pointer type should be sUINT16
*                       \n memory allocation should be done by caller
*
*  @return              Refer this Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus acquireHandle(PortID ePortId, sUINT16* uipHandle, list<sUINT16> lListofSubPorts = list<sUINT16>());

/**
 -----------------------------------------------------------------------------------------------------------------------
 releaseHandle
 -----------------------------------------------------------------------------------------------------------------------
*  This function will release the handle for particular port. The application will require the acquired 
*  handle for that particular port for releasing the same 
*  
*  @date                AUG/09/2013
*  @author              SW_LLAPI_DEV_1
*  @pre                 Should have the handle of port, which the application need to release
*
*  @param ePortId       Provide the port ID for which handle need to be released. 
*                       The valid ports are provided in Enum PortID, Refer this Enum @ref PortID
*  @param uiHandle      Provide the handle of that particular port acquired
*
*  @return              Refer this Enum @ref returnStatus

************************************************************************************************************************
*/
DLL_API returnStatus releaseHandle(PortID ePortId, sUINT16 uiHandle);

/**
-----------------------------------------------------------------------------------------------------------------------
handleMatch
-----------------------------------------------------------------------------------------------------------------------
*  This function will useful to validate uiHandle value corresponding to PortID.
*
*  @date                APR/04/2016
*  @author              SW_LLAPI_DEV_1
*  @pre                 Should have the handle of port
*
*  @param ePortId       Provide the port ID for which handle need to be released.
*                       The valid ports are provided in Enum PortID, Refer this Enum @ref PortID
*  @param uiHandle      Provide the handle of that particular port acquired
*
*  @return              Refer this Enum @ref returnStatus

************************************************************************************************************************
*/
DLL_API returnStatus handleMatch(PortID ePortId, sUINT16 uiHandle);


/**
* @}
*/

/*****************************************************************************************************
 *                               RTC
 ****************************************************************************************************/
/**
* @defgroup    RTC RTC
*
* @brief       Real Time Clock
*
*              This module contains API that Get and Set RTC of SPM
*/
/**
* @addtogroup  RTC
* @{
*/

/*****************************************************************************************************
 *                               RTC STRUCTURE
 ****************************************************************************************************/
/**
RTC structure
*/
struct rtc
{
   sUINT8 year;            ///<must be 1 to 99
   sUINT8 month;           ///<must be 1 to 12
   sUINT8 date;            ///<must be 1 to 31
   sUINT8 week_day;        ///<must be 0 to 6
   sUINT8 hour;            ///<must be 0 to 23
   sUINT8 minute;          ///<must be 0 to 59
   sUINT8 second;          ///<must be 0 to 59
   sUINT16 millisecond;    ///<must be 0 to 999
};

/**
 -----------------------------------------------------------------------------------------------------------------------
 rtcSet
 -----------------------------------------------------------------------------------------------------------------------
*  This function will configure SunSet Pass Module RTC.User can set year, month, date, week_day, hour, minute, second.
*  User must set millisecond to 0. Because Intel_SPM cannot set millisecond.
*
*  @date                   27/11/2013
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of RTC port acquired earlier
*  @param spRtc            RTC port configuration structure, refer structure @ref rtc
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus rtcSet(sUINT16 uiHandle, const rtc* spRtc);

/**
 -----------------------------------------------------------------------------------------------------------------------
 rtcGet
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get value of SunSet Pass Module RTC
*
*  @date                   11/10/2014
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of RTC port acquired earlier
*  @param spRtc            structure pointer where RTC value will be stored, refer structure @ref rtc
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus rtcGet(sUINT16 uiHandle, rtc* spRtc);

/**
* @}
*/

/*****************************************************************************************************
*                                         DEX
 ****************************************************************************************************/
/**
* @defgroup    DEX DEX
*
* @brief       DEX module is responsible for performing task related to DEX port. 
*
*              This module contains API implementation to access DEX port on SunSetPass Module.\n
*              DEX port is used for Audit data collection and configuration setting.\n
*              User can use read and write API to communicate with DEX device connected to DEX port.\n
*              <B>Sequence to use DEX API is:</B>\n
*              Configure \n
*              Enable \n
*              Data transfer\n
*              Disable\n
*/
/**
* @addtogroup  DEX
* @{
*/

/*****************************************************************************************************
 *                               DEX PORT STRUCTURE
 ****************************************************************************************************/
/**
   DEX port configuration structure
*/
struct dexConfigure
{
   sUINT32 ulDexBaudRate;  ///< Speed of data transfer\n value should be 9600
   sUINT8 ucDexDatabits;   ///< bits in data \n value should be 8
   sUINT8 ucDexParity;     ///< Parity bit selection, refer enum @ref serParity
   sUINT8 ucDexStopbits;   ///< Stop bit selection, refer enum @ref serStopbits
   sUINT8 ucDexBusTimeout; ///< time out after which, to activate bus, dummy data is send by Sunset Pass Module. It is in Second
};

/***********************************************************************************************************************
 *                               DEX PORT API
 ***********************************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 dexEnable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will enable Sunset Pass Module SERIAL port where DEX connector is connected.
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 DEX port should be configured
*
*  @param uiHandle      The handle of DEX port acquired earlier
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus dexEnable(sUINT16 uiHandle);

/**
 -----------------------------------------------------------------------------------------------------------------------
 dexDisable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will disable Sunset Pass Module SERIAL port where DEX connector is connected.
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 DEX port should be configured and enabled
*
*  @param uiHandle      The handle of DEX port acquired earlier
*
*  @return              Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus dexDisable(sUINT16 uiHandle);

/**
 -----------------------------------------------------------------------------------------------------------------------
 dexSetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function will configure Sunset Pass Module SERIAL port where DEX connector is connected.
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of DEX port acquired earlier
*  @param spDexConfigure   DEX port configuration structure, refer structure @ref dexConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus dexSetConfiguration(sUINT16 uiHandle, const dexConfigure* spDexConfigure);

/**
 -----------------------------------------------------------------------------------------------------------------------
 dexGetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get configuration of Sunset Pass Module SERIAL port where DEX connector is connected.
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    DEX port should be configured
*
*  @param uiHandle         The handle of DEX port acquired earlier
*  @param *spDexConfigure  structure pointer where DEX port configuration value will be stored, refer structure @ref dexConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus dexGetConfiguration(sUINT16 uiHandle, dexConfigure* spDexConfigure);

/**
 -----------------------------------------------------------------------------------------------------------------------
 dexWrite
 -----------------------------------------------------------------------------------------------------------------------
*  This function will write data to Sunset Pass Module SERIAL port where DEX connector is connected.
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          DEX port should be configured and enabled
*
*  @param uiHandle               The handle of DEX port acquired earlier
*  @param cpDexWriteData         memory pointer of data array to write to DEX port
*                                \n memory allocation should be done by caller
*  @param uiDexWriteDataLength   data length in bytes
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus dexWrite(sUINT16 uiHandle, sCUINT8P cpDexWriteData, sUINT16 uiDexWriteDataLength);

/**
 -----------------------------------------------------------------------------------------------------------------------
 dexRead
 -----------------------------------------------------------------------------------------------------------------------
*  This function will read data from SunSet Pass Module SERIAL port where DEX connector is connected.
*
*  @date                         27/11/2013
*  @author                       SW_LLAPI_DEV_1
*  @pre                          DEX port should be configured and enabled
*
*  @param uiHandle               The handle of DEX port acquired earlier
*  @param cpDexReadData          memory pointer of data array to copy read data 
*                                \n memory allocation should be done by caller
*  @param uiDexReadDataLength    data length in bytes of read from port
*  @param uipDexActualReadLength Actual data length in bytes, read from port
*  @param uiDexReadTimeOutInMS   time in millisecond to wait before return if data is not available
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus dexRead(sUINT16 uiHandle, sUINT8P cpDexReadData, sUINT16 uiDexReadDataLength, sUINT16P uipDexActualReadLength,
                     sUINT32 uiDexReadTimeOutInMS);
/**
* @}
*/


/*****************************************************************************************************
*                                         EVENT
 ****************************************************************************************************/
/**
* @defgroup    EVENT EVENT
*
* @brief       Event module is responsible for performing task related to Event and Error coming from Sunset Pass Module
*
*              This module contains API implementation of management of event coming from Sunset Pass Module.\n
*              Whenever Sunset Pass Module want to inform LLAPI about unexpected condition or data captured from device.\n
*              It will give LLAPI event with event ID and data related to event. LLAPI will call User's callback function to inform it\n
*              about the Event/Error.\n
*              <B>Sequence to use Event module is:</B>\n
*              Register callback function \n
*              Start capturing \n
*              Stop capturing \n
*/             
/**
* @addtogroup  EVENT
* @{
*/

/*****************************************************************************************************
 *                               EVENT ENUM
 ****************************************************************************************************/
/**
   Event ID, return to user application from which, it recognize the event from Sunset Pass Module
*/
enum emmEventID
{
    RESET_EVENT = 0,                                        ///< Reserved for future
    MDB_DATA,                                               ///< There is Some MDB data on MDB PORT
    DEX_PIN_CONNECTED,                                      ///< There is Some change on DEX pin
    DEX_PIN_DIS_CONNECTED,                                  ///< There is Some change on DEX pin
    GPI_PIN,                                                ///< There is Some change on GPI pin
    MTR_STOPPED,                                            ///< Motor has been stopped
    MDB_DEVICE_NOT_CONNECTED,                               ///< No MDB device connected
    CCTALK_DATA,                                            ///< There is some ccTalk data on ccTalk port
    CCTALK_DEVICE_NOT_CONNECTED,                            ///< No ccTalk device connected
    PROTOCOL_A_DATA,                                        ///< There is some Protocol A data on Protocol A port
	SMBUS_ALERT_DATA,										///< SMBus Alert Slave Address
	LOCAL_PORT_GPI_PIN = 11,								///< There is some change on Local PC GPI pin
};

/**
   Error ID, return to user application from which, it recognize the error from Sunset Pass Module
*/
enum emmErrorID
{
    /***** System & PC Communication Error Code from 0 to 99*******/

    RESET_ERROR = 0,                                        ///< Reserved for future
    EVENT_QUEUE_FULL,                                       ///< Event queue of the system is getting Full
    SOF_GETTING_CORRUPTTED,                                 ///< Start for frame getting corrupted after receive
    PCC_INVALID_PORT_RECEIVED,                              ///< Invalid PORT address received from PC
    PCC_RECEIVE_BUFFER_OVERRUN,                             ///< Receive data frame is more than PC RX buffer
    PCC_SW_CHANGED,                                         ///< Run time changing the PORT section switch for PC Communication
    PCC_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< PCCOMM Internal System Event Error

    /***** RS232 Error Code from 100 to 199*******/

    RS232_STATE_NOT_ON_FOR_WRITE = 100,                     ///< State of RS232 is not ON for Write
    RS232_STATE_NOT_ON_FOR_STOP,                            ///< State of RS232 is not ON for STOP
    RS232_STATE_NOT_CONFIGURED_FOR_START,                   ///< State of RS232 is not CONFIGURED for START
    RS232_INVALID_STOP_BIT,                                 ///< Invalid Stop Bit for UART - RS232
    RS232_INVALID_PARITY,                                   ///< Invalid parity for UART - RS232
    RS232_INVALID_DATA_BITS,                                ///< Invalid Data Bits for UART - RS232
    RS232_STATE_ON_FOR_CONFIGURE,                           ///< State of RS232 is ON so cannot be configured
    RS232_RX_BUFFER_OVERRUN,                                ///< Receive Buffer OverRun
    RS232_SW_CHANGED,                                       ///< Run time changing the PORT section switch for RS232, RS485
    RS232_INVALID_TASK_RECEIVED,                            ///< RS232 Invalid task received
    RS232_INTERNAL_SYSTEM_EVENT_ERROR,                      ///< RS232 Internal System Event Error

    /***** FAN Error Code from 200 to 299*******/

    TACH_UNKNOWM_SYS_EVENT = 200,                           ///< An unknown Event for TACH Detected
    PWM_UNKNOWM_SYS_EVENT,                                  ///< An unknown Event for PWM Detected
    TACH_NOT_WORKING,                                       ///< Tach Not working
    TACH_INVALID_TASK_RECEIVED,                             ///< TACH Invalid task received
    TACH_INTERNAL_SYSTEM_EVENT_ERROR,                       ///< TACH Internal System Event Error
    TACH_IN_USE,                                            ///< Another Tach is in use

    /***** MOTOR Error Code from 300 to 399*******/

    MTR_INVALID_GPO_PORT_RECEIVED = 300,                    ///< Invalid GPI PORT address received from PC
    MTR_INVALID_ON_CONDITION_PORT_RECEIVED,                 ///< Invalid ON Condition received from PC
    MTR_VOLTAGE_THRESHOLD_LIMIT,                            ///< Voltage Threshold limit reached
    MTR_CURRENT_THRESHOLD_LIMIT,                            ///< Current Threshold limit reached
    MTR_INVALID_TASK_RECEIVED,                              ///< Motor Invalid task received
    MTR_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< Motor Internal System Event Error
    MTR_ALREADY_STARTED,                                    ///< Motor is already running

   /***** GPO & GPI Error Code from 400 to 499*******/

    GPO_INVALID_PIN_NUMBER = 400,                           ///< Invalid PIN number for GPO module
    GPI_INVALID_PIN_NUMBER,                                 ///< Invalid PIN number for GPI module
    GPO_INVALID_DATA_BITS,                                  ///< Invalid Data Bits for GPO module
    GPO_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< GPO Internal System Event Error
    GPI_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< GPI Internal System Event Error

    /***** I2C Error Code from 500 to 599*******/

    I2C_INVALID_TASK_RECEIVED = 500,                        ///< Invalid task ID for I2C module
    I2C_IS_NOT_ENABLED,                                     ///< I2C module is not enabled
    I2C_BUS_NOT_IDLE,                                       ///< I2C module is not idle
    I2C_UNABLE_TO_START,                                    ///< I2C module unable to send start condition
    I2C_UNABLE_TO_STOP,                                     ///< I2C module unable to stop transfer
    I2C_BYTE_NOT_ACKNOWLEDGE,                               ///< I2C module sent byte was not acknowledged
    I2C_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< I2C Internal System Event Error

    /***** Seven Segment Error Code from 600 to 699*******/

    SEG_INVALID_DATA_RECEIVED = 600,                        ///< Seven segment Invalid data received
    SEVEN_SEGMENT_INVALID_TASK_RECEIVED,                    ///< Seven segment Invalid task received
    SEVEN_SEGMENT_INTERNAL_SYSTEM_EVENT_ERROR,              ///< Seven segment Internal System Event Error

    /***** SPI Error Code from 700 to 799*******/

    SPI_STATE_NOT_ON_FOR_WRITE = 700,                       ///< State of SPI is not ON for Write
    SPI_STATE_NOT_ON_FOR_READ,                              ///< State of SPI is not ON for Read
    SPI_STATE_NOT_ON_FOR_STOP,                              ///< State of SPI is not ON for STOP
    SPI_STATE_NOT_CONFIGURED_FOR_START,                     ///< State of SPI is not CONFIGURED for START
    SPI_INVALID_MODE,                                       ///< Invalid MODE for SPI - SPI
    SPI_INVALID_CONFIG,                                     ///< Invalid config for SPI - SPI
    SPI_STATE_ON_FOR_CONFIGURE,                             ///< State of SPI is ON so cannot be configured
    SPI_RX_BUFFER_OVERRUN,                                  ///< Receive Buffer OverRun
    SPI_INVALID_TASK_RECEIVED,                              ///< Invalid task ID for SPI module
    SPI_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< SPI Internal System Event Error

    /***** DEX Error Code from 800 to 899*******/

    DEX_STATE_NOT_ON_FOR_WRITE = 800,                       ///< State of DEX is not ON for Write
    DEX_STATE_NOT_ON_FOR_STOP,                              ///< State of DEX is not ON for STOP
    DEX_STATE_NOT_CONFIGURED_FOR_START,                     ///< State of DEX is not CONFIGURED for START
    DEX_INVALID_STOP_BIT,                                   ///< Invalid Stop Bit for UART - DEX
    DEX_INVALID_PARITY,                                     ///< Invalid parity for UART - DEX
    DEX_INVALID_DATA_BITS,                                  ///< Invalid Data Bits for UART - DEX
    DEX_STATE_ON_FOR_CONFIGURE,                             ///< State of DEX is ON so cannot be configured
    DEX_RX_BUFFER_OVERRUN,                                  ///< Receive Buffer OverRun
    DEX_INVALID_TASK_RECEIVED,                              ///< Dex Invalid task received
    DEX_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< Dex Internal System Event Error

    /***** CAN Error Code from 900 to 999*******/

    CAN_INVALID_TASK_RECEIVED = 900,                        ///< Invalid task ID for CAN module
    CAN_IS_NOT_ENABLED,                                     ///< State of CAN is not ON for Write
    CAN_STATE_NOT_ON_FOR_STOP,                              ///< State of CAN is not ON for STOP
    CAN_STATE_NOT_CONFIGURED_FOR_START,                     ///< State of CAN is not CONFIGURED for START
    CAN_INVALID_MODE,                                       ///< Invalid MODE for CAN - CAN
    CAN_INVALID_CONFIG,                                     ///< Invalid config for CAN - CAN
    CAN_INVALID_DATA_LENGTH,                                ///< Invalid Data Bits for UART - CAN
    CAN_STATE_ON_FOR_CONFIGURE,                             ///< State of CAN is ON so cannot be configured
    CAN_RX_BUFFER_OVERRUN,                                  ///< Receive Buffer OverRun
    CAN_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< CAN Internal System Event Error

    /***** MDB Error Code from 1000 to 1099*******/
    
    MDB_STATE_NOT_ON_FOR_WRITE = 1000,                       ///< State of MDB is not ON for Write
    MDB_STATE_NOT_ON_FOR_STOP,                              ///< State of MDB is not ON for STOP
    MDB_STATE_NOT_CONFIGURED_FOR_START,                     ///< State of MDB is not CONFIGURED for START
    MDB_INVALID_STOP_BIT,                                   ///< Invalid Stop Bit for UART - MDB
    MDB_INVALID_PARITY,                                     ///< Invalid parity for UART - MDB
    MDB_INVALID_DATA_BITS,                                  ///< Invalid Data Bits for UART - MDB
    MDB_STATE_ON_FOR_CONFIGURE,                             ///< State of MDB is ON so cannot be configured
    MDB_RX_BUFFER_OVERRUN,                                  ///< Receive Buffer OverRun
    MDB_INVALID_TASK_RECEIVED,                              ///< MDB Invalid task received
    MDB_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< MDB Internal System Event Error
    MDB_DEVICE_NACK_RECEIVED,                               ///< MDB Device NACK received

    /***** Firmware Error Code from 1100 to 1199*******/

    FWR_INVALID_TASK_RECEIVED = 1100,                       ///< Invalid task ID for Firmware
    FWR_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< FWR Internal System Event Error

    /***** I2C Error Code from 1200 to 1299*******/

    ONE_WIRE_INVALID_TASK_RECEIVED = 1200,                       ///< Invalid task ID for 1-Wire module
    ONE_WIRE_BUS_NOT_IDLE,                                       ///< 1-Wire module is not idle
    ONE_WIRE_UNABLE_TO_START,                                    ///< 1-Wire module unable to send start condition
    ONE_WIRE_UNABLE_TO_STOP,                                     ///< 1-Wire module unable to stop transfer
    ONE_WIRE_BYTE_NOT_ACKNOWLEDGE,                               ///< 1-Wire module sent byte was not acknowledged
    ONE_WIRE_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< 1-Wire Internal System Event Error
    ONE_WIRE_DEVICE_NOT_CONNECTED,                                ///< 1-Wire Device is not connected

    /***** Analog Error code from 1300 to 1399******/
    ANL_INVALID_TASK_RECEIVED = 1300,                           ///< Invalid task ID for Analog Module

    /***** ccTalk Error Code from 1400 to 1499*******/

    CCTALK_STATE_NOT_ON_FOR_WRITE = 1400,                       ///< State of ccTalk is not ON for Write
    CCTALK_STATE_NOT_ON_FOR_STOP,                              ///< State of ccTalk is not ON for STOP
    CCTALK_STATE_NOT_CONFIGURED_FOR_START,                     ///< State of ccTalk is not CONFIGURED for START
    CCTALK_INVALID_STOP_BIT,                                   ///< Invalid Stop Bit for UART - ccTalk
    CCTALK_INVALID_PARITY,                                     ///< Invalid parity for UART - ccTalk
    CCTALK_INVALID_DATA_BITS,                                  ///< Invalid Data Bits for UART - ccTalk
    CCTALK_STATE_ON_FOR_CONFIGURE,                             ///< State of ccTalk is ON so cannot be configured
    CCTALK_RX_BUFFER_OVERRUN,                                  ///< Receive Buffer OverRun
    CCTALK_INVALID_TASK_RECEIVED,                              ///< ccTalk Invalid task received
    CCTALK_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< ccTalk Internal System Event Error
    CCTALK_DEVICE_NACK_RECEIVED,                               ///< ccTalk Device NACK received

    PROTOCOL_A_STATE_NOT_ON_FOR_WRITE = 1500,                      ///< State of ProtocolA is not ON for Write
    PROTOCOL_A_STATE_NOT_ON_FOR_STOP,                              ///< State of ProtocolA is not ON for STOP
    PROTOCOL_A_STATE_NOT_CONFIGURED_FOR_START,                     ///< State of ProtocolA is not CONFIGURED for START
    PROTOCOL_A_INVALID_STOP_BIT,                                   ///< Invalid Stop Bit for UART - ProtocolA
    PROTOCOL_A_INVALID_PARITY,                                     ///< Invalid parity for UART - ProtocolA
    PROTOCOL_A_INVALID_DATA_BITS,                                  ///< Invalid Data Bits for UART - ProtocolA
    PROTOCOL_A_STATE_ON_FOR_CONFIGURE,                             ///< State of ProtocolA is ON so cannot be configured
    PROTOCOL_A_RX_BUFFER_OVERRUN,                                  ///< Receive Buffer OverRun
    PROTOCOL_A_INVALID_TASK_RECEIVED,                              ///< ProtocolA Invalid task received
    PROTOCOL_A_INTERNAL_SYSTEM_EVENT_ERROR,                        ///< ProtocolA Internal System Event Error
    PROTOCOL_A_DEVICE_NACK_RECEIVED,                               ///< ProtocolA Device NACK received

};



/*****************************************************************************************************
 *                               EVENT STRUCTURE
 ****************************************************************************************************/
/**
   Event "Call back" Data structure, to send as argument in "Call back" function
*/

struct emmEventCallBackData
{
    sUINT8P ucpData;             ///<Pointer of Memory of Data array received with event
    sUINT32 ulDataSize;          ///<size of data received from Sunset Pass Module with event
    sUINT32 ulEventId;           ///<event ID, refer @ref emmEventID
    sBOOL bEventType;            ///<Type of event, if 1 It is Error event, If 0 It is Data Event.
};

/*****************************************************************************************************
 *                               EVENT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 emmRegisterEventCallback
 -----------------------------------------------------------------------------------------------------------------------
*  This function will register callback function, which will be called when event occurs in Sunset Pass Module.
*
*  @date                               21/10/13
*  @author                             SW_LLAPI_DEV_1
*  @pre                                None
*
*  @param uiHandle                     The handle of event module acquired earlier
*  @param emmEventCallbackFunction     the pointer of callback function
*  @param bEventFrom                   If this argument is 0, informs Call back function that, this event is from LLAPI
*  @param sEmmEventCallBackData        pointer of structure of data received from Sunset Pass Module with event
*
*  @return                             Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus emmRegisterEventCallback(sUINT16 uiHandle, void (*emmEventCallbackFunction)(sBOOL bEventFrom, void 
                                                                      *sEmmEventCallBackData));

/**
 -----------------------------------------------------------------------------------------------------------------------
 emmStartEventCapture
 -----------------------------------------------------------------------------------------------------------------------
*  This function will start capturing event and call callback function on event.
*  \n before event capture start, Sunset Pass Module will not send any Event & Error.
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 Callback function should be registered
*
*  @param uiHandle      The handle of event module acquired earlier
*  @param bLocalOnly	Start event capture only for local port if bLocalOnly is true
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus emmStartEventCapture(sUINT16 uiHandle, sBOOL bLocalOnly);

/**
 -----------------------------------------------------------------------------------------------------------------------
 emmStopEventCapture
 -----------------------------------------------------------------------------------------------------------------------
*  This function will stop capturing event.
*  \n after event capture stop, Sunset Pass Module will not send any Event & Error.
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 Callback function should be registered and capturing started
*
*  @param uiHandle      The handle of event module acquired earlier
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus emmStopEventCapture(sUINT16 uiHandle);

/**
-----------------------------------------------------------------------------------------------------------------------
emmEnableLocalPortGPIOEventCapture
-----------------------------------------------------------------------------------------------------------------------
*  This function will enable Local port gpio for event capture.
*
*  @date                05/04/16
*  @author              SW_LLAPI_DEV_1
*
*  @param ePortId				Port ID Of Device
*  @param ucLocalPCGpioNmber	GPIO number
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus emmEnableLocalPortGPIOEventCapture(PortID ePortId, sUINT16 ucLocalPCGpioNmber);

/**
-----------------------------------------------------------------------------------------------------------------------
emmDisableLocalPortGPIOEventCapture
-----------------------------------------------------------------------------------------------------------------------
*  This function will disable Local port gpio for event capture.
*
*  @date                05/04/16
*  @author              SW_LLAPI_DEV_1
*
*  @param ePortId				Port ID Of Device
*  @param ucLocalPCGpioNmber	GPIO number
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus emmDisableLocalPortGPIOEventCapture(PortID ePortId, sUINT16 ucLocalPCGpioNmber);

/**
* @}
*/

/*****************************************************************************************************
*                                         FIRMWARE_UPGRADE
 ****************************************************************************************************/
/**
* @defgroup    FIRMWARE_UPGRADE FIRMWARE_UPGRADE
*
* @brief       Firmware Upgrade module is responsible for performing task related to firmware upgrade of Sunset Pass Module
*
*               Firmware of Sunset Pass Module can be upgraded using LLAPI.\n
*               Firmware has version no. for tracking the current version.\n
*               This module has API that can read current version of Firmware and upgrade the firmware.\n
*               For upgrade Firmware, User should have valid firmware Hex file.\n
*/             
/**
* @addtogroup  FIRMWARE_UPGRADE
* @{
*/
/*****************************************************************************************************
 *                               FIRMWARE_UPGRADE STRUCTURE
 ****************************************************************************************************/
/**
   structure that contains firmware and boot-loader version of Sunset Pass Module
*/
struct firmwareVersion
{
   sUINT8 ucpFrmVersion[5];     ///<Firmware Version
   sUINT8 ucpBtlVersion[5];     ///<Boot-loader Version
};

/*****************************************************************************************************
 *                               FIRMWARE_UPGRADE API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 frmVersionCheck
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to check current version of firmware and boot-loader of Sunset Pass Module
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          firmware and boot-loader should be loaded to Sunset Pass Module.
*
*  @param uiHandle               The handle of firmware upgrade module acquired earlier
*  @param spFirmwareVersion      pointer of structure to store firmware and boot-loader version
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus frmVersionCheck(sUINT16 uiHandle, firmwareVersion* spFirmwareVersion);

/**
 -----------------------------------------------------------------------------------------------------------------------
 frmBootMode
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to put SunSet Pass Module in Boot mode to upgrade firmware.
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          none
*
*  @param uiHandle               The handle of firmware upgrade module acquired earlier
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus frmBootMode(sUINT16 uiHandle);

/**
 -----------------------------------------------------------------------------------------------------------------------
 frmUpgrade
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to upgrade current firmware of Sunset Pass Module
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          none
*
*  @param uiHandle               The handle of firmware upgrade module acquired earlier
*  @param cpFrmFilePath          path of firmware hex file
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus frmUpgrade(sUINT16 uiHandle, sUINT8P cpFrmFilePath);

/**
* @}
*/

/*****************************************************************************************************
*                                      GPIO
****************************************************************************************************/
/**
* @defgroup    GPIO GPIO
*
* @brief       GPIO module is responsible for performing task related to General Purpose Output port of Sunset Pass Module.
*
*              This module has API that can enable the Local embedded PC's GPIO port before actual read/write.
*/
/**
* @addtogroup  GPIO
* @{
*/
/*****************************************************************************************************
*                               GPIO PORT API
****************************************************************************************************/
/**
-----------------------------------------------------------------------------------------------------------------------
gpioEnable
-----------------------------------------------------------------------------------------------------------------------
*  This function will enable the Local embedded PC's GPIO port before actual read/write.
*
*  @date                   18/02/16
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of GPIO port acquired earlier
*  @param eGpioPortId      The port ID
*  @param ucLocalPCGpioNmber   Linux or Windows Specific GPIO number
*
*  @return                 Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus gpioEnable(sUINT16 uiHandle, PortID eGpioPortId, sUINT16 ucLocalPCGpioNmber);
/**
* @}
*/

/*****************************************************************************************************
*                                         GPI
 ****************************************************************************************************/
/**
* @defgroup    GPI GPI
*
* @brief       GPI module is responsible for performing task related to General Purpose Input port of Sunset Pass Module.
*
*              This file has API that can read current status of input pin on Sunset Pass module.\n
*/             
/**
* @addtogroup  GPI 
* @{
*/
/*****************************************************************************************************
 *                               GPI PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 gpioRead
 -----------------------------------------------------------------------------------------------------------------------
*  This function will read status of SunSet Pass Module general purpose input pin.
*
*  @date                   27/11/2013
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of GPI port acquired earlier
*  @param ucGpiPinNumber   pin number of input pin\n it can be a SPM board or Local embedded PC
*  @param *bpGpiValue      memory pointer where status of pin will be stored.\n it will be 0 or 1
*  @param bWithRtc         Get RTC time stamp or not
*  @param *spRtc           memory pointer where date and time of RTC will be stored.It is time of GPI read 
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus gpioRead(PortID piGpoPort, sUINT16 uiHandle, sUINT16 ucGpiPinNumber, sBOOL* bpGpiValue, sBOOL bWithRtc, rtc* spRtc);

/**
* @}
*/

/*****************************************************************************************************
*                                      GPO
 ****************************************************************************************************/
/**
* @defgroup    GPO GPO
*
* @brief       GPO module is responsible for performing task related to General Purpose Output port of Sunset Pass Module.
*
*              This module has API that can set status of output pin of Sunset Pass module.
*              it will set or reset the pin
*/             
/**
* @addtogroup  GPO 
* @{
*/
/*****************************************************************************************************
 *                               GPO PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 gpioWrite
 -----------------------------------------------------------------------------------------------------------------------
*  This function will write value to Sunset Pass Module general purpose output pin 1 or 0.
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of GPO port acquired earlier
*  @param ucGpoPinNumber   pin number of output pin\n it can be a SPM board or Local embedded PC
*  @param bGpoValue        value of pin to set\n 0 = reset and 1 = set
*
*  @return                 Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus gpioWrite(PortID piGpoPort, sUINT16 uiHandle, sUINT16 ucGpoPinNumber, sBOOL bGpoValue);
/**
* @}
*/

/*****************************************************************************************************
*                                      GPIO
****************************************************************************************************/
/**
* @defgroup    GPIO GPIO
*
* @brief       GPIO module is responsible for performing task related to General Purpose Output port of Sunset Pass Module.
*
*              This module has API that can set direction for specified Local embedded PC's GPIO port.
*/
/**
* @addtogroup  GPIO
* @{
*/
/*****************************************************************************************************
*                               GPIO PORT API
****************************************************************************************************/
/**
-----------------------------------------------------------------------------------------------------------------------
gpioSetDirection
-----------------------------------------------------------------------------------------------------------------------
*  This function will set direction for specified Local embedded PC's GPIO port.
*
*  @date                   18/02/16
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of GPIO port acquired earlier
*  @param ucLocalPCGpioNmber   Linux or Windows Specific GPIO number
*  @param *cpDirection   Linux or Windows Specific set direction string pointer for specified Local embedded PC's GPIO port
*
*  @return                 Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus gpioSetDirection(sUINT16 nPortHandle, PortID eGpioPortId, sUINT16 ucLocalPCGpioNmber, const sINT8 *cpDirection);

/**
* @}
*/

/*****************************************************************************************************
*                                      GPIO
****************************************************************************************************/
/**
* @defgroup    GPIO GPIO
*
* @brief       GPIO module is responsible for performing task related to General Purpose Output port of Sunset Pass Module.
*
*              This module has API that can get direction for specified Local embedded PC's GPIO port.
*/
/**
* @addtogroup  GPIO
* @{
*/
/*****************************************************************************************************
*                               GPIO PORT API
****************************************************************************************************/
/**
-----------------------------------------------------------------------------------------------------------------------
gpioGetDirection
-----------------------------------------------------------------------------------------------------------------------
*  This function will get direction for specified Local embedded PC's GPIO port.
*
*  @date                   18/02/16
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of GPIO port acquired earlier
*  @param bLocalPCGpioDir   Linux or Windows Specific get direction for specified Local embedded PC's GPIO port
*  @param ucLocalPCGpioNmber   Linux or Windows Specific GPIO number
*
*  @return                 Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus gpioGetDirection(sUINT16 uiHandle, PortID eGpioPortId, sUINT16 ucGpiPinNumber, sBOOL* bpGpioDirection, sBOOL bWithRtc, rtc* spRtc);
/**
* @}
*/


/*****************************************************************************************************
*                                      GPIO
****************************************************************************************************/
/**
* @defgroup    GPIO GPIO
*
* @brief       GPIO module is responsible for performing task related to General Purpose Output port of Sunset Pass Module.
*
*              This module has API that can disable the Local embedded PC's GPIO port before actual read/write.
*/
/**
* @addtogroup  GPIO
* @{
*/
/*****************************************************************************************************
*                               GPIO PORT API
****************************************************************************************************/
/**
-----------------------------------------------------------------------------------------------------------------------
gpioDisable
-----------------------------------------------------------------------------------------------------------------------
*  This function will disable the Local embedded PC's GPIO port before actual read/write.
*
*  @date                   18/02/16
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of GPIO port acquired earlier
*  @param eGpioPortId      The port ID.
*  @param ucLocalPCGpioNmber   Linux or Windows Specific GPIO number
*
*  @return                 Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus gpioDisable(sUINT16 uiHandle, PortID eGpioPortId, sUINT16 ucLocalPCGpioNmber);
/**
* @}
*/

/*****************************************************************************************************
*                                         I2C
 ****************************************************************************************************/
/**
* @defgroup    I2C I2C
*
* @brief       I2C module is responsible for performing task related to communication on I2C port of Sunset Pass Module.
*
*              This module has API that can communicate to I2C device connected to Sunset Pass Module.\n
*              \n <B>Sequence to use I2C API is:</B>\n
*              Enable \n
*              Data transfer\n
*              Disable\n
*/             
/**
* @addtogroup  I2C
* @{
*/
/*****************************************************************************************************
*                               I2C PORT STRUCTURE
****************************************************************************************************/
/**
structure containing I2C bus speed configuration
*/
struct i2cConfigure
{
	sUINT32 uiI2cBusSpeed;    ///<I2C serial clock rate in Hz if in master mode
};


   
/*****************************************************************************************************
 *                               I2C PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 i2cEnable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will enable Sunset Pass Module I2C port.
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 none
*
*  @param uiHandle      The handle of I2C port acquired earlier
*  @param eI2cPortId    I2C port ID if more than one is available i.e. I2C_1
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus i2cEnable(sUINT16 uiHandle, PortID eI2cPortId);

/**
 -----------------------------------------------------------------------------------------------------------------------
 i2cDisable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will disable Sunset Pass Module I2C port.
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 I2C port should be enabled
*
*  @param uiHandle      The handle of I2C port acquired earlier
*  @param eI2cPortId    I2C port ID if more than one is available i.e. I2C_1
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus i2cDisable(sUINT16 uiHandle, PortID eI2cPortId);

/**
-----------------------------------------------------------------------------------------------------------------------
i2cSetConfiguration
-----------------------------------------------------------------------------------------------------------------------
*  This function will set bus rate of SunSet Pass Module I2C port.
*
*  @date					22/01/16
*  @author					SW_LLAPI_DEV_1
*  @pre						I2C port should not be enabled
*
*  @param uiHandle			The handle of I2C port acquired earlier
*  @param eI2cPortId		I2C port ID if more than one is available i.e. I2C_1
*  @param spI2cConfigure	Constant struct pointer of i2c configuration
*
*  @return					Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/

DLL_API returnStatus i2cSetConfiguration(sUINT16 uiHandle, PortID eI2cPortId, const i2cConfigure* spI2cConfigure);

/**
-----------------------------------------------------------------------------------------------------------------------
i2cGetConfiguration
-----------------------------------------------------------------------------------------------------------------------
*  This function will get bus rate of SunSet Pass Module I2C port.
*
*  @date					22/01/16
*  @author					SW_LLAPI_DEV_1
*  @pre						I2C port should not be disabled
*
*  @param uiHandle			The handle of I2C port acquired earlier
*  @param eI2cPortId		I2C port ID if more than one is available i.e. I2C_1
*  @param spI2cConfigure	Struct pointer of i2c configuration
*
*  @return					Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus i2cGetConfiguration(sUINT16 uiHandle, PortID eI2cPortId, i2cConfigure* spI2cConfigure);

/**
 -----------------------------------------------------------------------------------------------------------------------
 i2cWrite
 -----------------------------------------------------------------------------------------------------------------------
*  This function will write data to Sunset Pass Module I2C port.
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          I2C port should be enabled
*
*  @param uiHandle               The handle of I2C port acquired earlier
*  @param eI2cPortId             I2C port ID if more than one is available i.e. I2C_1
*  @param cpI2cWriteData         memory pointer of data array to write to I2C port
*                                \n memory allocation should be done by caller
*  @param uiI2cWriteDataLength   data length in bytes
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus i2cWrite(sUINT16 uiHandle, PortID eI2cPortId, sCUINT8P cpI2cWriteData, sUINT16 uiI2cWriteDataLength);

/**
 -----------------------------------------------------------------------------------------------------------------------
 i2cRead
 -----------------------------------------------------------------------------------------------------------------------
*  This function will read data from SunSet Pass Module I2C port
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          I2C port should be enabled
*
*  @param uiHandle               The handle of I2C port acquired earlier
*  @param eI2cPortId             I2C port ID if more than one is available i.e. I2C_1
*  @param cpI2cReadData          memory pointer of data array to copy read data 
*                                \n memory allocation should be done by caller
*  @param cpI2cRequestReadData   memory pointer of data array to request for read to I2C port
*  @param uiI2cReadDataLength    data length in bytes of read from port
*  @param uiI2cRequestDataLength data length in bytes of request read data to I2C port
*  @param bRepeatedStart		 If true, use repeated start between write and read command
*  @param bWithRtc               Get RTC time stamp or not
*  @param *spRtc                 memory pointer where date and time of RTC will be stored.It is time of I2C read 
*                                \n memory allocation should be done by caller
*  @param uiI2cReadTimeOutInMS   time in millisecond to wait before return if data is not available
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus i2cRead(sUINT16 uiHandle, PortID eI2cPortId, sUINT8P cpI2cReadData, sUINT8P cpI2cRequestReadData, 
                     sUINT16 uiI2cReadDataLength, sUINT16 uiI2cRequestDataLength, sBOOL bRepeatedStart, sBOOL bWithRtc, rtc* spRtc, sUINT32 uiI2cReadTimeOutInMS);

/**
-----------------------------------------------------------------------------------------------------------------------
i2cEnableSMBusAlert
-----------------------------------------------------------------------------------------------------------------------
*  This function will enable SMBus Alert on SunSet Pass Module
*
*  @date                         22/02/16
*  @author                       SW_LLAPI_DEV_1
*  @pre                          I2C port should be enabled
*
*  @param uiHandle               The handle of I2C port acquired earlier
*  @param eI2cPortId             I2C port ID if more than one is available i.e. I2C_1
*  @param ucAlertPin			 Alert Pin or type Refer Enum @ref SMBusAlertPin
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus i2cEnableSMBusAlert(sUINT16 uiHandle, PortID eI2cPortId, SMBusAlertPin ucAlertPin);

/**
-----------------------------------------------------------------------------------------------------------------------
i2cDisableSMBusAlert
-----------------------------------------------------------------------------------------------------------------------
*  This function will disable SMBus Alert on SunSet Pass Module
*
*  @date                         22/02/16
*  @author                       SW_LLAPI_DEV_1
*  @pre                          I2C port should be enabled
*
*  @param uiHandle               The handle of I2C port acquired earlier
*  @param eI2cPortId             I2C port ID if more than one is available i.e. I2C_1
*  @param ucAlertPin			 Alert Pin or type Refer Enum @ref SMBusAlertPin
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus i2cDisableSMBusAlert(sUINT16 uiHandle, PortID eI2cPortId, SMBusAlertPin ucAlertPin);

/**
* @}
*/

/*****************************************************************************************************
*                                         MOTOR
 ****************************************************************************************************/
/**
* @defgroup    MOTOR MOTOR 
*
* @brief       Event module is responsible for performing task related to controlling of motor, connected to Sunset Pass Module.
*
*              This module contains API implementation to Sunset Pass Module motor functioning.\n
*              API will be used to measure voltage and current of motor supply.\n
*              It will be used to ON and OFF motors.\n
*              Motor supply voltage and current threshold will be checked by Sunset Pass Module and acted upon it.\n
*              User will get event on over/under current/voltage.\n
*              \n <B>Sequence to use MOTOR API is:</B>\n
*              Configure \n
*              on \n
*/             
/**
* @addtogroup  MOTOR
* @{
*/
/*****************************************************************************************************
 *                               MOTOR PORT STRUCTURE
 ****************************************************************************************************/
/**
structure containing high and low threshold of voltage and current for safe motor operation
*/
struct mtrConfigure  
{
   sUINT32 uiMtrUpThresholdVoltage;    ///<Highest voltage ADC Voltage for motor working 
   sUINT32 uiMtrDowmThresholdVoltage;  ///<Lowest voltage ADC Voltage for motor working 
   sUINT32 uiMtrUpThresholdCurrent;    ///<Highest current ADC Voltage for motor working 
   sUINT32 uiMtrDowmThresholdCurrent;  ///<Lowest current ADC Voltage for motor working 
};

/**
   motor control structure
*/
struct mtrOnData
{
   sUINT32 ulGpioOnFor;             ///<time or rotation of motor for which GPIO will be set
   sUINT8P ucGpioPinNumber;         ///<array of pin number to set
   sUINT8 ucGpioOnCondition;        ///<If 0 "ulGpioOnFor" will be time for on in MS and if 1 "ulGpioOnFor" will be motor rotation
   sUINT8 ucNoOfGpio;               ///<no of GPIO to set
   sUINT8 ucGpiInterruptPinNumber;  ///<pin number of general purpose input pin which is used for motor rotation encoder interrupt
};

/*****************************************************************************************************
 *                               MOTOR PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
mtrSetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function will configure up and down threshold value of current and voltage to safe run motor
*  if current or voltage value will violate this limit Sunset Pass Module will stop motor working.
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of MOTOR port acquired earlier
*  @param spMtrConfigure   MOTOR port configuration structure, refer structure @ref mtrConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus mtrSetConfiguration(sUINT16 uiHandle, const mtrConfigure* spMtrConfigure);

/**
 -----------------------------------------------------------------------------------------------------------------------
 mtrGetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get configuration of up and down threshold value of current and voltage in Sunset Pass Module
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    MOTOR port should be configured
*
*  @param uiHandle         The handle of MOTOR port acquired earlier
*  @param *spMtrConfigure  structure pointer where MOTOR port configuration value will be stored, refer structure @ref mtrConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus mtrGetConfiguration(sUINT16 uiHandle, mtrConfigure* spMtrConfigure);

/**
 -----------------------------------------------------------------------------------------------------------------------
 mtrOn
 -----------------------------------------------------------------------------------------------------------------------
*  This function will set GPIO pin of Sunset Pass Module to control motor function. GPIO will be set for given timeout value.
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          MOTOR port should be configured
*
*  @param uiHandle               The handle of MOTOR port acquired earlier
*  @param spMtrOn              GPIO on structure, refer structure @ref mtrOnData
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus mtrOn(sUINT16 uiHandle, const mtrOnData* spMtrOn);

/**
 -----------------------------------------------------------------------------------------------------------------------
 mtrRead
 -----------------------------------------------------------------------------------------------------------------------
*  This function will read last captured value of current and voltage in Sunset Pass Module
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          MOTOR port should be configured and enabled
*
*  @param uiHandle               The handle of MOTOR port acquired earlier
*  @param uipMtrVoltage          memory pointer to copy voltage of motor supply last captured
*                                \n memory allocation should be done by caller
*  @param uipMtrCurrent          memory pointer to copy current of motor supply last captured 
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus mtrRead(sUINT16 uiHandle, sUINT16* uipMtrVoltage, sUINT16* uipMtrCurrent);

/**
* @}
*/

/*****************************************************************************************************
*                                   CHANNEL_SELECTION
 ****************************************************************************************************/
/**
* @defgroup    CHANNEL_SELECTION CHANNEL_SELECTION
*
* @brief       Channel Selection module is responsible for selecting communication channel between PC and Sunset Pass Module. 
*
*              This module is used to select one of the three channel to communicate with Sunset Pass Module, USB, RS232, ETHERNET
*              At any time, only one channel is active.
*/             
/**
* @addtogroup  CHANNEL_SELECTION
* @{
*/

/*****************************************************************************************************
 *                               CHANNEL SELECTION ENUM
 ****************************************************************************************************/
/**
   enum for channel to communicate between PC and Sunset Pass Module
*/
enum channel
{
   NO_CHANNEL = 0,            ///<channel not selected
   SERIAL_CHANNEL = 2,        ///<RS232 channel 
   ETHERNET_CHANNEL = 3,      ///<Ethernet TCP/IP channel 
   USB_CHANNEL = 1            ///<USB CDC class channel
};
/*****************************************************************************************************
 *                               CHANNEL SELECTION STRUCTURE
 ****************************************************************************************************/
/**
structures of serial channel configuration data
this structure will select channel and give channel's configuration data
*/
struct channelSelectSerial
{
   channel eChannel;                ///<channel to be select for communication, refer enum @ref channel
   sUINT8P ucpPortName;             ///<SERIAL port name i.e. "COM1"
};

/**
structures of Ethernet channel configuration data
this structure will select channel and give channel's configuration data
*/
struct channelSelectEthernet
{
   channel eChannel;                ///<channel to be select for communication, refer enum @ref channel
   sUINT8P cpServerName;            ///<name or IP address of TCP server on Sunset Pass Module
   sUINT16 uiPortNumber;            ///<port number of TCP server on Sunset Pass Module
};

/**
structures of USB channel configuration data
this structure will select channel and give channel's configuration data
*/
struct channelSelectUsb
{
   channel eChannel;                ///<channel to be select for communication, refer enum @ref channel
   sUINT8P ucpPortName;             ///<SERIAL port name i.e. "COM1" created by USB CDC driver
};

/*****************************************************************************************************
 *                               CHANNEL SELECTION API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 chnSelect
 -----------------------------------------------------------------------------------------------------------------------
*  This function will close active channel if available and enable selected channel for communication with Sunset Pass Module
*  \n This function also handshake with Sunset Pass Module to test channel and return appropriate result
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 none
*
*  @param uiHandle      The handle of channel selection module acquired earlier
*  @param ucpChnSelect  structure to select channel, refer structure @ref channelSelectSerial
*                       \n memory allocation should be done by caller
*
*  @return              Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus chnSelect(sUINT16 uiHandle, void* ucpChnSelect);

/**
 -----------------------------------------------------------------------------------------------------------------------
 startChannel
 -----------------------------------------------------------------------------------------------------------------------
*  This function will start selected channel for communication between PC and SunSet Pass Module
*
*  @date             27/11/2013
*  @author           SW_LLAPI_DEV_1
*  @pre              channel should be selected
*
*  @return           Refer this Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus startChannel();

/**
 -----------------------------------------------------------------------------------------------------------------------
 stotChannel
 -----------------------------------------------------------------------------------------------------------------------
*  This function will stop active channel for communication between PC and SunSet Pass Module
*
*  @date             27/11/2013
*  @author           SW_LLAPI_DEV_1
*  @pre              channel should be selected and enabled
*
*  @return           Refer this Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus stopChannel();

/**
* @}
*/

/*****************************************************************************************************
*                                         SERIAL
 ****************************************************************************************************/
/**
* @defgroup    SERIAL SERIAL
*
* @brief       Serial module is responsible for performing task related to RS232 and RS485 port of Sunset Pass Module.
*
*              This file has API that can communicate to RS232 and RS485 device connected to Sunset Pass Module.\n
*              This file has API that configures RS232 and RS485 port.\n
*              This port can be used as one of RS232 port or RS485 port\n
*              \n <B>Sequence to use SERIAL API is:</B>\n
*              Configure \n
*              Enable \n
*              Data transfer\n
*              Disable\n
*/             
/**
* @addtogroup  SERIAL
* @{
*/

/*****************************************************************************************************
 *                               SERIAL PORT ENUM
 ****************************************************************************************************/
/**
   enum for UART port communication parity bit selection
*/
enum serParity
{
   NONE_PARITY = 0,  ///<no parity
   ODD_PARITY = 1,   ///<odd parity
   EVEN_PARITY = 2   ///<even parity
};

/**
   enum for UART port communication stop bit selection
*/
enum serStopbits
{
   ONE_STOP_BIT = 0,
   TWO_STOP_BIT = 1
};


#define COM_VALUE_LENGTH	  50					  ///<COM port string length for Accessing particular port (i.e Local Port Module, USB Relay, etc...)

/*****************************************************************************************************
 *                               SERIAL PORT STRUCTURE
 ****************************************************************************************************/
/**
   UART port configuration structure
*/
struct serConfigure
{
   sUINT32 ulSerBaudRate;  ///< Speed of data transfer \n value should be 2400, 9600, 19200, 38400, 56000 or 115200
   sUINT8 ucSerDatabits;   ///< bits in data \n value should be 5, 6, 7 or 8
   sUINT8 ucSerParity;     ///< Parity bit selection, refer enum @ref serParity
   sUINT8 ucSerStopbits;   ///< Stop bit selection, refer enum @ref serStopbits
};

/*****************************************************************************************************
 *                               SERIAL PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 serEnable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will enable Sunset Pass Module UART port.
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 SERIAL port should be configured
*
*  @param uiHandle      The handle of SERIAL port acquired earlier
*  @param eSerPortId    SERIAL port id\n RS232_485_1, RS232_485_2, RS232_485_3
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus serEnable(sUINT16 uiHandle, PortID eSerPortId);

/**
 -----------------------------------------------------------------------------------------------------------------------
 serDisable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will disable Sunset Pass Module UART port .
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 SERIAL port should be configured and enabled
*
*  @param uiHandle      The handle of SERIAL port acquired earlier
*  @param eSerPortId    SERIAL port id\n RS232_485_1, RS232_485_2, RS232_485_3
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus serDisable(sUINT16 uiHandle, PortID eSerPortId);

/**
 -----------------------------------------------------------------------------------------------------------------------
 serSetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function will configure Sunset Pass Module UART port
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of SERIAL port acquired earlier
*  @param eSerPortId       SERIAL port id\n RS232_485_1, RS232_485_2, RS232_485_3
*  @param spSerConfigure   SERIAL port configuration structure, refer structure @ref serConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/

DLL_API returnStatus serSetConfiguration(sUINT16 uiHandle, PortID eSerPortId, const serConfigure* spSerConfigure);

/**
 -----------------------------------------------------------------------------------------------------------------------
 serGetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get configuration of Sunset Pass Module UART port
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    SERIAL port should be configured
*
*  @param uiHandle         The handle of SERIAL port acquired earlier
*  @param eSerPortId       SERIAL port id\n RS232_485_1, RS232_485_2, RS232_485_3
*  @param *spSerConfigure  structure pointer where SERIAL port configuration value will be stored, refer structure @ref serConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus serGetConfiguration(sUINT16 uiHandle, PortID eSerPortId, serConfigure* spSerConfigure);

/**
 -----------------------------------------------------------------------------------------------------------------------
 serWrite
 -----------------------------------------------------------------------------------------------------------------------
*  This function will write data to Sunset Pass Module UART port
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          SERIAL port should be configured and enabled
*
*  @param uiHandle               The handle of SERIAL port acquired earlier
*  @param eSerPortId             SERIAL port id\n RS232_485_1, RS232_485_2, RS232_485_3
*  @param cpSerWriteData         memory pointer of data array to write to SERIAL port
*                                \n memory allocation should be done by caller
*  @param uiSerWriteDataLength   data length in bytes
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus serWrite(sUINT16 uiHandle, PortID eSerPortId, sCUINT8P cpSerWriteData, sUINT16 uiSerWriteDataLength);

/**
 -----------------------------------------------------------------------------------------------------------------------
 serRead
 -----------------------------------------------------------------------------------------------------------------------
*  This function will read data from Sunset Pass Module SERIAL port
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          SERIAL port should be configured and enabled
*
*  @param uiHandle               The handle of SERIAL port acquired earlier
*  @param eSerPortId             SERIAL port id\n RS232_485_1, RS232_485_2, RS232_485_3
*  @param cpSerReadData          memory pointer of data array to copy read data 
*                                \n memory allocation should be done by caller
*  @param uiSerReadDataLength    data length in bytes of read from port
*  @param uiSerReadTimeOutInMS   time in millisecond to wait before return if data is not available
*  @param uipActualDataRead      Actual data length read from port
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus serRead(sUINT16 uiHandle, PortID eSerPortId, sUINT8P cpSerReadData, sUINT16 uiSerReadDataLength, sUINT32 uiSerReadTimeOutInMS, sUINT16P uipActualDataRead);

/**
* @}
*/

/*****************************************************************************************************
*                                         MDB
 ****************************************************************************************************/
/**
* @defgroup    MDB MDB
*
* @brief       MDB module is responsible for performing task related to MDB port of Sunset Pass Module.
*
*              This file has API that can communicate to MDB device connected to Sunset Pass Module.\n
*              This file has API that send command to MDB device and Get response from it.\n
*              User can configure polling rate, Start polling of device.\n
*              Polling will be done by Sunset Pass Module, Its response will come to user by Event.\n
*              \n <B>Sequence to use MDB API is:</B>\n
*              Configure \n
*              Execute command \n
*/             
/**
* @addtogroup  MDB
* @{
*/

/*****************************************************************************************************
 *                               MDB PORT STRUCTURE
 ****************************************************************************************************/
/**
MDB command structure containing command frame, pointer to receive response frame and response timeout.
*/

struct mdbCommand
{
   sUINT8P cpMdbCommandFrame;           ///<pointer to command frame array
   sUINT8P cpMdbResponse;               ///<Pointer to get response frame array
   sUINT32 ulMdbCommandResponseTimeOut; ///<timeout in Millisecond to wait for response before return
   sUINT16 uiMdbResponseSize;           ///<response size in bytes
   sUINT8 ucMdbCommandFrameSize;        ///<size of command frame in bytes
};

/**
MDB port configuration structure
*/
struct mdbConfig
{
   serConfigure spMdbPortConfig;    ///<UART port configuration where MDB connector is connected, refer structure @ref serConfigure
   sUINT8P ucpMdbDeviceAddress;     ///<pointer to array of MDB device addresses attached to MDB bus
   sUINT8 ucMdbPollRate;            ///<common polling rate for all MDB devices in seconds\n Every MDB device will be polled by Sunset Pass Module within this Poll time
   sUINT8 ucMdbNoOfDevice;          ///<no of device attached to MDB bus
};

/*****************************************************************************************************
 *                               MDB PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 mdbSetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function will configure Sunset Pass Module for MDB port
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of MDB port acquired earlier
*  @param spMdbConfig      MDB port configuration structure, refer structure @ref mdbConfig
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus mdbSetConfiguration(sUINT16 uiHandle, const mdbConfig* spMdbConfig);

/**
 -----------------------------------------------------------------------------------------------------------------------
 mdbGetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get configuration of Sunset Pass Module MDB port
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of MDB port acquired earlier
*  @param spMdbConfig      structure pointer where MDB port configuration value will be stored, refer structure @ref mdbConfig
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus mdbGetConfiguration(sUINT16 uiHandle, mdbConfig* spMdbConfig);

/**
 -----------------------------------------------------------------------------------------------------------------------
 mdbExecuteCommand
 -----------------------------------------------------------------------------------------------------------------------
*  This function will write command to Sunset Pass Module MDB port and get response from MDB port.
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          MDB port should be configured
*
*  @param uiHandle               The handle of MDB port acquired earlier
*  @param sMdbCommand            memory pointer of MDB command structure, refer structure @ref mdbCommand
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus mdbExecuteCommand(sUINT16 uiHandle, mdbCommand* sMdbCommand);

/**
 -----------------------------------------------------------------------------------------------------------------------
 mdbPollStart
 -----------------------------------------------------------------------------------------------------------------------
*  This function will start polling of device attached to MDB bus.
*  Polling will be done by Sunset Pass Module and poll response is given by event to application
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          MDB port should be configured
*
*  @param uiHandle               The handle of MDB port acquired earlier
*  @param ucMdbDeviceAddress     address of MDB device to start polling of that device
*  @param ucMdbPollCmd           command of MDB device for polling that device
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus mdbPollStart(sUINT16 uiHandle, sUINT8 ucMdbDeviceAddress, sUINT8 ucMdbPollCmd); 

/**
 -----------------------------------------------------------------------------------------------------------------------
 mdbPollStop
 -----------------------------------------------------------------------------------------------------------------------
*  This function will stop polling of device attached to MDB bus.
*
*  @date                         21/10/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          MDB port should be configured and Polling of that device should be started
*
*  @param uiHandle               The handle of MDB port acquired earlier
*  @param ucMdbDeviceAddress     address of MDB device to stop polling of that device
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus mdbPollStop(sUINT16 uiHandle, sUINT8 ucMdbDeviceAddress);

/**
* @}
*/

/*****************************************************************************************************
*                                         CCTALK
 ****************************************************************************************************/
/**
* @defgroup    ccTalk ccTalk
*
* @brief       ccTalk module is responsible for performing task related to ccTalk port of Sunset Pass Module.
*
*              This file has API that can communicate to ccTalk device connected to Sunset Pass Module.\n
*              This file has API that send command to ccTalk device and Get response from it.\n
*              User can configure polling rate, Start polling of device.\n
*              Polling will be done by Sunset Pass Module, Its response will come to user by Event.\n
*              \n <B>Sequence to use ccTalk API is:</B>\n
*              Configure \n
*              Execute command \n
*/
/**
* @addtogroup  ccTalk
* @{
*/

/*****************************************************************************************************
 *                               CCTALK PORT STRUCTURE
 ****************************************************************************************************/
/**
ccTalk command structure containing command frame, pointer to receive response frame and response timeout.
*/

struct cctCommand
{
   sUINT8P cpCctCommandFrame;           ///<pointer to command frame array
   sUINT8P cpCctResponse;               ///<Pointer to get response frame array
   sUINT32 ulCctCommandResponseTimeOut; ///<timeout in Millisecond to wait for response before return
   sUINT16 uiCctResponseSize;           ///<response size in bytes
   sUINT8 ucCctCommandFrameSize;        ///<size of command frame in bytes
};

/**
ccTalk device configuration structure
*/
struct cctDevice
{
   sUINT8 ucAddress;               ///<ccTalk device address
   sUINT8 ucChecksumType;          ///<ccTalk checksum type 0 = Checksum byte, 1 = 16 bit CRC CCIT checker, 2 = 16 bit CRC with Encryption
   sUINT8 ucEncryptionKey;         ///<Key for encryption. if checksum type is CRC with encryption
};

/**
ccTalk port configuration structure
*/
struct cctConfig
{
   sUINT8 ucCctVmcAddress;          ///<Address of VMC
   serConfigure spCctPortConfig;    ///<UART port configuration where ccTalk connector is connected, refer structure @ref serConfigure
   cctDevice* spCctDevice;          ///<pointer to array of ccTalk device structure attached to ccTalk bus
   sUINT8 ucCctPollRate;            ///<common polling rate for all ccTalk devices in seconds\n Every ccTalk device will be polled by Sunset Pass Module within this Poll time
   sUINT8 ucCctNoOfDevice;          ///<no of device attached to ccTalk bus
};


/*****************************************************************************************************
 *                               CCTALK PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 cctSetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function will configure Sunset Pass Module for ccTalk port
*
*  @date                   19/08/14
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of ccTalk port acquired earlier
*  @param spCctConfig      ccTalk port configuration structure, refer structure @ref cctConfig
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus cctSetConfiguration(sUINT16 uiHandle, const cctConfig* spCctConfig);

/**
 -----------------------------------------------------------------------------------------------------------------------
 cctGetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get configuration of Sunset Pass Module ccTalk port
*
*  @date                   20/08/14
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of ccTalk port acquired earlier
*  @param spCctConfig      structure pointer where ccTalk port configuration value will be stored, refer structure @ref cctConfig
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus cctGetConfiguration(sUINT16 uiHandle, cctConfig* spCctConfig);

/**
 -----------------------------------------------------------------------------------------------------------------------
 cctExecuteCommand
 -----------------------------------------------------------------------------------------------------------------------
*  This function will write command to Sunset Pass Module ccTalk port and get response from ccTalk port.
*
*  @date                         20/08/14
*  @author                       SW_LLAPI_DEV_1
*  @pre                          ccTalk port should be configured
*
*  @param uiHandle               The handle of ccTalk port acquired earlier
*  @param sCctCommand            memory pointer of ccTalk command structure, refer structure @ref cctCommand
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus cctExecuteCommand(sUINT16 uiHandle, cctCommand* sCctCommand);

/**
 -----------------------------------------------------------------------------------------------------------------------
 cctPollStart
 -----------------------------------------------------------------------------------------------------------------------
*  This function will start polling of device attached to ccTalk bus.
*  Polling will be done by Sunset Pass Module and poll response is given by event to application
*
*  @date                         20/08/14
*  @author                       SW_LLAPI_DEV_1
*  @pre                          ccTalk port should be configured
*
*  @param uiHandle               The handle of ccTalk port acquired earlier
*  @param ucCctDeviceAddress     address of ccTalk device to start polling of that device
*  @param ucCctPollCmd           ccTalk device poll command to start polling of that device
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus cctPollStart(sUINT16 uiHandle, sUINT8 ucCctDeviceAddress, sUINT8 ucCctPollCmd);

/**
 -----------------------------------------------------------------------------------------------------------------------
 cctPollStop
 -----------------------------------------------------------------------------------------------------------------------
*  This function will stop polling of device attached to ccTalk bus.
*
*  @date                         20/08/14
*  @author                       SW_LLAPI_DEV_1
*  @pre                          ccTalk port should be configured and Polling of that device should be started
*
*  @param uiHandle               The handle of ccTalk port acquired earlier
*  @param ucCctDeviceAddress     address of ccTalk device to stop polling of that device
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus cctPollStop(sUINT16 uiHandle, sUINT8 ucCctDeviceAddress);

/**
* @}
*/

/*****************************************************************************************************
*                                         PROTOCOL A
 ****************************************************************************************************/
/**
* @defgroup    ProtocolA ProtocolA
*
* @brief       ProtocolA module is responsible for performing task related to ProtocolA port of Sunset Pass Module.
*
*              This file has API that can communicate to ProtocolA device connected to Sunset Pass Module.\n
*              This file has API that send reply to ccTalk device command\n
*              User can configure Start replying polling from device.\n
*              Polling reply will be done by Sunset Pass Module, Necessary Command will be send to user by Event.\n
*              \n <B>Sequence to use ccTalk API is:</B>\n
*              Configure \n
*              Start Poll\n
*              Reply to command \n
*/
/**
* @addtogroup  ProtocolA
* @{
*/

/*****************************************************************************************************
 *                               PROTOCOL A PORT STRUCTURE
 ****************************************************************************************************/
/**
Protocol A reply structure containing reply data and command
*/
struct praReply
{
   sUINT8 cpCommand;             ///<Command for that reply should be given
   sUINT8 cpPraReply;            ///<Reply data
   sUINT32 ulPraCommandTimeOut;  ///<timeout in Millisecond to wait for command before return
};

/**
Protocol A port configuration structure
*/
struct praConfig
{
   serConfigure spPraPortConfig;         ///<UART port configuration where Protocol A connector is connected.
};


/*****************************************************************************************************
 *                               PROTOCOL A PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 praSetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function will configure Sunset Pass Module for Protocol A port
*
*  @date                   19/08/14
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of Protocol A port acquired earlier
*  @param spPraConfig      Protocol A port configuration structure, refer structure @ref praConfig
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus praSetConfiguration(sUINT16 uiHandle, const praConfig* spPraConfig);


/**
 -----------------------------------------------------------------------------------------------------------------------
 praGetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get configuration of Sunset Pass Module Protocol A port
*
*  @date                   20/08/14
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of Protocol A port acquired earlier
*  @param spPraConfig      structure pointer where Protocol A port configuration value will be stored, refer structure @ref praConfig
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus praGetConfiguration(sUINT16 uiHandle, praConfig* spPraConfig);

/**
 -----------------------------------------------------------------------------------------------------------------------
 praReplyToCommand
 -----------------------------------------------------------------------------------------------------------------------
*  This function will send reply to SPM,  which should be sent in response of a particular command from Protocol A device
*
*  @date                         20/08/14
*  @author                       SW_LLAPI_DEV_1
*  @pre                          Protocol A port should be configured
*
*  @param uiHandle               The handle of Protocol A port acquired earlier
*  @param sPraReply              memory pointer of Protocol A reply structure to particular command, refer structure @ref praReply
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus praReplyToCommand(sUINT16 uiHandle, praReply* sPraReply);

/**
 -----------------------------------------------------------------------------------------------------------------------
 praStartPollAck
 -----------------------------------------------------------------------------------------------------------------------
*  This function will Start Acknowledging command from Protocol A device.
*  Polling Replying will be done by Sunset Pass Module
*
*  @date                         20/08/14
*  @author                       SW_LLAPI_DEV_1
*  @pre                          Protocol A port should be configured
*
*  @param uiHandle               The handle of Protocol A port acquired earlier
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus praStartPollAck(sUINT16 uiHandle);

/**
 -----------------------------------------------------------------------------------------------------------------------
 praStopPollAck
 -----------------------------------------------------------------------------------------------------------------------
*  This function will stop polling of device attached to Protocol A bus.
*
*  @date                         20/08/14
*  @author                       SW_LLAPI_DEV_1
*  @pre                          Protocol A port should be configured and Polling should be started
*
*  @param uiHandle               The handle of Protocol A port acquired earlier
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus praStopPollAck(sUINT16 uiHandle);

/**
* @}
*/

/*****************************************************************************************************
*                                         POWER_MANAGEMENT
 ****************************************************************************************************/
/**
* @defgroup    POWER_MANAGEMENT POWER_MANAGEMENT
*
* @brief       Power Management module is responsible for performing task related to Power Management of Sunset Pass Module.
*
*              This module is used to manage power requirement of sunset pass module by making Sunset Pass Module controller in Sleep mode
*/             
/**
* @addtogroup  POWER_MANAGEMENT
* @{
*/
/*****************************************************************************************************
 *                               POWER_MANAGEMENT ENUM
 ****************************************************************************************************/
/**
   different state of Sunset Pass Module for different power requirement
*/
enum pmmState
{
   SLEEP = 1,     ///<Sunset Pass Module sleep mode
   WAKEUP = 2     ///<Sunset Pass Module active mode
};
/*****************************************************************************************************
 *                               POWER_MANAGEMENT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 pmmSetState
 -----------------------------------------------------------------------------------------------------------------------
*  This function will set different power management state of Sunset Pass Module
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 none
*
*  @param uiHandle      The handle of power management module acquired earlier
*  @param ePmmState     state of Sunset Pass Module to be set, refer @ref pmmState
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus pmmSetState(sUINT16 uiHandle, pmmState ePmmState);

/**
* @}
*/

/*****************************************************************************************************
*                                         PWM
 ****************************************************************************************************/
/**
* @defgroup    PWM PWM
*
* @brief       PWM module is responsible for performing task related to PWM port of Sunset Pass Module.
*
*              This module has API that can send PWM signal to device connected to Sunset Pass Module.
*              \n <B>Sequence to use PWM API is:</B>\n
*              Configure \n
*              Enable \n
*              Disable\n
*/             
/**
* @addtogroup  PWM
* @{
*/
/*****************************************************************************************************
 *                               PWM PORT STRUCTURE
 ****************************************************************************************************/
 /**
 Macro for time in NSs
 */

#define TIME_NS 1000000000

 /**
PWM port configuration structure
*/
struct pwmConfigure
{
   sUINT32 ulPwmPulseFrequency;   ///< PWM pulse frequency In Hertz
   sUINT8 ucPwmPulseDutyCycle;   ///<  duty cycle of pulse between 0 to 100
};

/*****************************************************************************************************
 *                               PWM PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 pwmStart
 -----------------------------------------------------------------------------------------------------------------------
*  This function will enable Sunset Pass Module PWM port
*  \n start outputting PWM pulse
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 PWM port should be configured
*
*  @param uiHandle      The handle of PWM port acquired earlier
*  @param ePwmPortId    PWM port id, can be PWM_1 or PWM_2 or S_PWM_LOCAL_PORT
*  @param ucChannelNo   PWM Channel No
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus pwmStart(sUINT16 uiHandle, PortID ePwmPortId, sUINT8 ucChannelNo);

/**
 -----------------------------------------------------------------------------------------------------------------------
 pwmStop
 -----------------------------------------------------------------------------------------------------------------------
*  This function will disable Sunset Pass Module PWM port
*  \n stop outputting PWM pulse
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 PWM port should be configured and started
*
*  @param uiHandle      The handle of PWM port acquired earlier
*  @param ePwmPortId    PWM port id, can be PWM_1 or PWM_2  or S_PWM_LOCAL_PORT
*  @param ucChannelNo   PWM Channel No
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus pwmStop(sUINT16 uiHandle, PortID ePwmPortId, sUINT8 ucChannelNo);

/**
 -----------------------------------------------------------------------------------------------------------------------
 pwmSetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function will configure Sunset Pass Module PWM port
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of PWM port acquired earlier
*  @param ePwmPortId       PWM port id, can be PWM_1 or PWM_2  or S_PWM_LOCAL_PORT
*  @param ucChannelNo      PWM Channel No
*  @param spPwmConfigure   PWM port configuration structure, refer structure @ref pwmConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus pwmSetConfiguration(sUINT16 uiHandle, PortID ePwmPortId, pwmConfigure* spPwmConfigure, sUINT8 ucChannelNo);

/**
 -----------------------------------------------------------------------------------------------------------------------
 pwmGetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get configuration of Sunset Pass Module PWM port
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    PWM port should be configured
*
*  @param uiHandle         The handle of PWM port acquired earlier
*  @param ePwmPortId       PWM port id, can be PWM_1 or PWM_2  or S_PWM_LOCAL_PORT
*  @param ucChannelNo      PWM Channel No
*  @param *spPwmConfigure  structure pointer where PWM port configuration value will be stored, refer structure @ref pwmConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus pwmGetConfiguration(sUINT16 uiHandle, PortID ePwmPortId, pwmConfigure* spPwmConfigure, sUINT8 ucChannelNo);

/**
 -----------------------------------------------------------------------------------------------------------------------
pwmEnable
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to Start the PWM channel of Sunset Pass Module PWM port
*
*  @date                   28/03/2016
*  @author                 SW_LLAPI_DEV_1
*  @pre                    PWM port should be configured
*
*  @param ePwmPortId       S_PWM_LOCAL_PORT
*  @param ucChannelNo      PWM Channel No
*
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus pwmEnable(PortID ePwmPortId, sUINT8 ucChannelNo);

/**
 -----------------------------------------------------------------------------------------------------------------------
pwmDisable
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to  Stop thw PWM channel of Sunset Pass Module PWM port
*
*  @date                   28/03/2016
*  @author                 SW_LLAPI_DEV_1
*  @pre                    PWM port should be configured
*
*  @param ePwmPortId       S_PWM_LOCAL_PORT
*  @param ucChannelNo      PWM Channel No
*
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus pwmDisable(PortID ePwmPortId, sUINT8 ucChannelNo);

/**
* @}
*/

/*****************************************************************************************************
*                                         SEVEN_SEGMENT
 ****************************************************************************************************/
/**
* @defgroup    SEVEN_SEGMENT SEVEN_SEGMENT
*
* @brief       Seven Segment module is responsible for performing task related to Seven Segment Display in Sunset Pass Module.
*
*              This module has API that can write numeric data to Seven Segment Display in Sunset Pass Module.
*/             
/**
* @addtogroup  SEVEN_SEGMENT
* @{
*/
/*****************************************************************************************************
 *                               SEVEN_SEGMENT PORT API
 ****************************************************************************************************/
/**
-----------------------------------------------------------------------------------------------------------------------
segWrite
-----------------------------------------------------------------------------------------------------------------------
*  This function will write 4 digit numeric value to seven segment display
*
*  @date                      21/10/13
*  @author                    SW_LLAPI_DEV_1
*  @pre                       none
*
*  @param uiHandle            The handle of seven segment port acquired earlier
*  @param ucpSegWriteData     pointer of 4 byte array which contain hex value of each digit.\n To enable dot after digit 7th bit should be 1
*                             \n memory allocation should be done by caller
*
*  @return                    Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus segWrite(sUINT16 uiHandle, sCUINT8P ucpSegWriteData);

/**
* @}
*/

/*****************************************************************************************************
*                                         SPI
 ****************************************************************************************************/
/**
* @defgroup    SPI SPI
*
* @brief       SPI module is responsible for performing task related to SPI port in Sunset Pass Module.
*
*              This module has API that can communicate to SPI device connected to Sunset Pass Module.
*              User can also configure SPI port depending on Device connected.
*              \n <B>Sequence to use SPI API is:</B>\n
*              Configure \n
*              Enable \n
*              Data transfer\n
*              Disable\n
*/             
/**
* @addtogroup  SPI
* @{
*/

/*****************************************************************************************************
 *                               SPI PORT ENUM
 ****************************************************************************************************/
/**
enum for SPI mode
*/
enum spiMode
{
   RISING_EDGE_LOW_POLARITY = 0,       ///<clock is active low and data is captured on rising edge of clock
   RISING_EDGE_HIGH_POLARITY = 1,      ///<clock is active high and data is captured on rising edge of clock
   FALLING_EDGE_LOW_POLARITY = 2,      ///<clock is active low and data is captured on falling edge of clock
   FALLING_EDGE_HIGH_POLARITY = 3      ///<clock is active high and data is captured on falling edge of clock
};
/*****************************************************************************************************
 *                               SPI PORT STRUCTURE
 ****************************************************************************************************/
/**
SPI port configuration structure
*/
struct spiConfigure
{
   sUINT32 ulSpiBaudrate;           ///<SPI clock frequency in Hz if in master mode 
   sUINT8 ucSpiMode;                ///<SPI mode, refer @ref spiMode
   sUINT8 ucSpiCS;                  ///<SPI Chip select : 0 Active low, 1 Active High
};

/*****************************************************************************************************
 *                               SPI PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 spiEnable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will enable Sunset Pass Module SPI port
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 SPI port should be configured
*
*  @param uiHandle      The handle of SPI port acquired earlier
*  @param eSpiPortId    SPI port ID if more than one is available i.e. SPI_1
*
*  @return              Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus spiEnable(sUINT16 uiHandle, PortID eSpiPortId);

/**
 -----------------------------------------------------------------------------------------------------------------------
 spiDisable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will disable Sunset Pass Module SPI port 
*
*  @date                21/10/13
*  @author              SW_LLAPI_DEV_1
*  @pre                 SPI port should be configured and enabled
*
*  @param uiHandle      The handle of SPI port acquired earlier
*  @param eSpiPortId    SPI port ID if more than one is available i.e. SPI_1
*
*  @return              Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus spiDisable(sUINT16 uiHandle, PortID eSpiPortId);

/**
 -----------------------------------------------------------------------------------------------------------------------
 spiSetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function will configure Sunset Pass Module SPI port
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    none
*
*  @param uiHandle         The handle of SPI port acquired earlier
*  @param eSpiPortId       SPI port ID if more than one is available i.e. SPI_1
*  @param spSpiConfigure   SPI port configuration structure, refer structure @ref spiConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus spiSetConfiguration(sUINT16 uiHandle, PortID eSpiPortId, const spiConfigure* spSpiConfigure);

/**
 -----------------------------------------------------------------------------------------------------------------------
 spiGetConfiguration
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get configuration of Sunset Pass Module SPI port 
*
*  @date                   21/10/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    SPI port should be configured
*
*  @param uiHandle         The handle of SPI port acquired earlier
*  @param eSpiPortId       SPI port ID if more than one is available i.e. SPI_1
*  @param *spSpiConfigure  structure pointer where SPI port configuration value will be stored, refer structure @ref spiConfigure
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus spiGetConfiguration(sUINT16 uiHandle, PortID eSpiPortId, spiConfigure* spSpiConfigure);

/**
 -----------------------------------------------------------------------------------------------------------------------
 spiWrite
 -----------------------------------------------------------------------------------------------------------------------
*  This function will write data to Sunset Pass Module SPI port
*
*  @date                         27/11/2013
*  @author                       SW_LLAPI_DEV_1
*  @pre                          SPI port should be configured and enabled
*
*  @param uiHandle               The handle of SPI port acquired earlier
*  @param eSpiPortId             SPI port ID if more than one is available i.e. SPI_1
*  @param cuipSpiWriteData       memory pointer of data array to write to SPI port
*                                \n memory allocation should be done by caller
*  @param uiSpiWriteDataLength   data length in size of array
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus spiWrite(sUINT16 uiHandle, PortID eSpiPortId, sCUINT8P cuipSpiWriteData, sUINT16 uiSpiWriteDataLength);

/**
 -----------------------------------------------------------------------------------------------------------------------
 spiRead
 -----------------------------------------------------------------------------------------------------------------------
*  This function will send READ command to SunSet Pass Module and then wait for data
*  \n SunSet Pass Module will return data read from SPI port of SunSet Pass Module
*
*  @date                         27/11/2013
*  @author                       SW_LLAPI_DEV_1
*  @pre                          SPI port should be configured and enabled
*
*  @param uiHandle               The handle of SPI port acquired earlier
*  @param eSpiPortId             SPI port ID if more than one is available i.e. SPI_1   
*  @param uipSpiWriteData        memory pointer of data array to write to SPI port before read 
*                                \n This data is for continuous operation of SPI.
*                                \n memory allocation should be done by caller
*  @param uiSpiWriteDataLength   data length in size of array write to SPI port
*  @param uipSpiReadData         memory pointer of data array to copy read data 
*                                \n memory allocation should be done by caller
*  @param uiSpiReadDataLength    data length in size of array of read from SPI port
*  @param bWithRtc               Get RTC time stamp or not
*  @param *spRtc                 memory pointer where date and time of RTC will be stored.It is time of SPI read 
*                                \n memory allocation should be done by caller
*  @param uiSpiReadTimeOutInMS   time in millisecond to wait before return if data is not available
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus spiRead(sUINT16 uiHandle, PortID eSpiPortId, sUINT8P uipSpiWriteData, sUINT16 uiSpiWriteDataLength,
                     sUINT8P uipSpiReadData, sUINT16 uiSpiReadDataLength, sBOOL bWithRtc, rtc* spRtc, sUINT32 uiSpiReadTimeOutInMS);


/**
* @}
*/

/*****************************************************************************************************
*                                         TACH
 ****************************************************************************************************/
/**
* @defgroup    TACH TACH
*
* @brief       TACH module is responsible for performing task related to measure tachometer output
*
*              This module has API that can measure tachometer output signal connected to Sunset Pass Module.
*/
/**
* @addtogroup  TACH
* @{
*/
/*****************************************************************************************************
 *                               TACH PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 tchGetFrequency
 -----------------------------------------------------------------------------------------------------------------------
*  This function will give Tachometer pulse frequency to measure speed of Fan.
*
*  @date                            21/10/13
*  @author                          SW_LLAPI_DEV_1
*  @pre                             none
*
*  @param uiHandle                  The handle of TACH module acquired earlier
*  @param eTchPortId                TACH port id, can be TACH_1 or TACH_2
*  @param uiTchFreq                 memory pointer to give frequency to application\n frequency will be in Hz
*                                   \n memory allocation should be done by caller
*  @param uiTchReadTimeOutInMS      timeout to wait before return if data is not available
*
*  @return                          Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus tchGetFrequency(sUINT16 uiHandle, PortID eTchPortId, sUINT16* uiTchFreq, sUINT32 uiTchReadTimeOutInMS);
/**
* @}
*/


/*****************************************************************************************************
*                                         ONE WIRE
 ****************************************************************************************************/
/**
* @defgroup    OneWire OneWire
*
* @brief       OneWire module is responsible for communication with 1-Wire devices connected to SPM
*
*              This module has API that can Read and Write data to 1-Wire devices connected to Sunset Pass Module.
*/
/**
* @addtogroup  OneWire
* @{
*/
/*****************************************************************************************************
 *                               ONE WIRE STRUCTURE
 ****************************************************************************************************/
/**
One Wire address structure
*/
struct owrAddress
{
   sUINT8 ucOwrNoOFAddress;          ///<No of addresses of one wire device connected
   sUINT8 address[256];             ///<array of addresses
};

/*****************************************************************************************************
 *                                      ONE WIRE API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 owrReset
 -----------------------------------------------------------------------------------------------------------------------
*  This function will reset all devices connected to 1-Wire bus on SPM
*
*  @date                            26/05/14
*  @author                          SW_LLAPI_DEV_1
*  @pre                             none
*
*  @param uiHandle                  The handle of OneWire module acquired earlier
*
*  @return                          Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus owrReset(sUINT16 uiHandle);

/**
 -----------------------------------------------------------------------------------------------------------------------
 owrSearch
 -----------------------------------------------------------------------------------------------------------------------
*  This function will search address of all devices connected to 1-Wire bus on SPM
*
*  @date                              26/05/14
*  @author                            SW_LLAPI_DEV_1
*  @pre                               none
*
*  @param uiHandle                    The handle of OneWire module acquired earlier
*  @param spOwrAddress                memory pointer of data array to copy addresses
*                                     \n memory allocation should be done by caller
*  @param uiOwrSearchTimeOutInMS      time in millisecond to wait before return if data is not available
*
*  @return                             Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus owrSearch(sUINT16 uiHandle, owrAddress* spOwrAddress, sUINT32 uiOwrSearchTimeOutInMS);

/**
 -----------------------------------------------------------------------------------------------------------------------
 owrWrite
 -----------------------------------------------------------------------------------------------------------------------
*  This function will write given data all devices connected to 1-Wire bus on SPM
*
*  @date                            26/05/14
*  @author                          SW_LLAPI_DEV_1
*  @pre                             none
*
*  @param uiHandle                  The handle of OneWire module acquired earlier
*  @param cuipOwrWriteData          memory pointer of data array to write to 1-Wire bus
*                                   \n memory allocation should be done by caller
*  @param uiOwrWriteDataLength      data length in size of array
*
*  @return                          Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus owrWrite(sUINT16 uiHandle, sCUINT8P cuipOwrWriteData, sUINT16 uiOwrWriteDataLength);

/**
 -----------------------------------------------------------------------------------------------------------------------
 owrRead
 -----------------------------------------------------------------------------------------------------------------------
*  This function will Read data from device connected to 1-Wire bus on SPM
*
*  @date                            26/05/14
*  @author                          SW_LLAPI_DEV_1
*  @pre                             none
*
*  @param uiHandle                  The handle of OneWire module acquired earlier
*  @param uipOwrReadData            memory pointer of data array to copy read data 
*                                   \n memory allocation should be done by caller
*  @param uiOwrReadDataLength       data length in size of array of read from 1-Wire bus
*  @param bWithRtc                  Get RTC time stamp or not
*  @param *spRtc                    memory pointer where date and time of RTC will be stored.It is time of SPI read 
*                                   \n memory allocation should be done by caller
*  @param uiOwrReadTimeOutInMS      time in millisecond to wait before return if data is not available
*
*  @return                          Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus owrRead(sUINT16 uiHandle, sUINT8P uipOwrReadData, sUINT16 uiOwrReadDataLength, sBOOL bWithRtc, rtc* spRtc, 
                     sUINT32 uiOwrReadTimeOutInMS);

/*****************************************************************************************************
 *                               DEVICESEARCH STRUCTURE
 ****************************************************************************************************/
struct sDevice
{
	sUINT16 uiOwrLastDiscrepancy;                              // Last discrepancy of 1-Wire device
	sUINT16 uiOwrLastFamilyDiscrepancy;                        // Last family discrepancy of 1-Wire device
	sUINT16 uiOwrLastDeviceFlag;                               // Flag to check last device
};

/**
-----------------------------------------------------------------------------------------------------------------------
localPortI2COwrSearchdevice
-----------------------------------------------------------------------------------------------------------------------
*  This function will search one wire From I2C port
*
*  @date                        15/02/16
*  @author                      SW_LLAPI_DEV_1
*  @pre                         I2C port should be enabled
*
*  @param uiHandle              The handle of I2C port acquired earlier
*  @param eI2cPortId            I2C port ID
*  @param ucSlaveAddress        Slave Adress of I2C Bridge
*  @param spOwrAddress         	Address of slave devices found
*
*  @return                      Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus localPortI2COwrSearchDevice(sUINT16 uiHandle, PortID eI2cPortId, sUINT8 ucSlaveAddress, owrAddress* spOwrAddress);
/**
 -----------------------------------------------------------------------------------------------------------------------
 localPortI2COwrFirst
 -----------------------------------------------------------------------------------------------------------------------
* This routine finds first device on 1-Wire network
*
*  @date                        15/02/16
*  @author                      SW_LLAPI_DEV_1
*  @pre                         I2C port should be enabled
*
*  @param uiHandle              The handle of I2C port acquired earlier
*  @param eI2cPortId            I2C port ID
*  @param ucSlaveAddress        Slave Adress of I2C Bridge
*  @param ucpOwrROMbit    		ROM address
*  @param spDev					Structure for one wire device search state
*  @return                      Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus localPortI2COwrFirst(sUINT16 uiHandle, PortID eI2cPortId, sUINT8 ucSlaveAddress, sUINT8P ucpOwrROMbit, sDevice* spDev);

/**
 -----------------------------------------------------------------------------------------------------------------------
 localPortI2COwrNext
 -----------------------------------------------------------------------------------------------------------------------
* This routine finds next device up to last devices on 1-Wire network
*
*  @date                        15/02/16
*  @author                      SW_LLAPI_DEV_1
*  @pre                         I2C port should be enabled
*
*  @param uiHandle              The handle of I2C port acquired earlier
*  @param eI2cPortId            I2C port ID
*  @param ucSlaveAddress        Slave Adress of I2C Bridge
*  @param ucpOwrROMbit    		ROM address
*  @param spDev					Structure for one wire device search state
*  @return                      Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus localPortI2COwrNext(sUINT16 uiHandle, PortID eI2cPortId, sUINT8 ucSlaveAddress, sUINT8P ucpOwrROMbit, sDevice* spDev);

/**
 -----------------------------------------------------------------------------------------------------------------------
 localPortI2COwrSearch
 -----------------------------------------------------------------------------------------------------------------------
* This routine perform general search to find attached devices on the 1-Wire bus
*
*  @date                        15/02/16
*  @author                      SW_LLAPI_DEV_1
*  @pre                         I2C port should be enabled
*
*  @param uiHandle              The handle of I2C port acquired earlier
*  @param eI2cPortId            I2C port ID
*  @param ucSlaveAddress        Slave Adress of I2C Bridge
*  @param ucpOwrROMbit    		ROM address
*  @param spDev					Structure for one wire device search state
*  @return                      Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus localPortI2COwrSearch(sUINT16 uiHandle, PortID eI2cPortId, sUINT8 ucSlaveAddress, sUINT8P ucpOwrROMbit, sDevice* spDev);

/**
 -----------------------------------------------------------------------------------------------------------------------
 localPortI2COwrSearchTriplet
 -----------------------------------------------------------------------------------------------------------------------
*  This routine will write one bit and reads two bit From one wire device
*
*  @date                        15/02/16
*  @author                      SW_LLAPI_DEV_1
*  @pre                         I2C port should be enabled
*
*  @param uiHandle              The handle of I2C port acquired earlier
*  @param eI2cPortId            I2C port ID
*  @param ucSlaveAddress        Slave Adress of I2C Bridge
*  @param ucpIdBit      		id bits read by triplet command   
*  @param ucSearchDir    		search direction set or reset
*  @return                      Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus localPortI2COwrSearchTriplet(sUINT16 uiHandle, PortID eI2cPortId, sUINT8 ucSlaveAddress, sUINT8P ucpIdBit, sUINT8 ucSearchDir);
/**
 -----------------------------------------------------------------------------------------------------------------------
 localPortI2COwrCalcCrc
 -----------------------------------------------------------------------------------------------------------------------
* This routine calculates CRC for read data
*
*  @date                        15/02/16
*  @author                      SW_LLAPI_DEV_1
*  @pre                         I2C port should be enabled
*
*  @param ucpData               Input data to calcualte CRC
*  @param ucBytereadlength      Length of input data 	
*  @param ucpCrc1     	        CRC bit
*  @return                      Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API void localPortI2COwrCalcCrc(sUINT8P ucpData, sUINT8 ucByteReadLength, sUINT8P ucpCrc);
/**
* @}
*/

/*****************************************************************************************************
 *                               ANALOG
 ****************************************************************************************************/
/**
* @defgroup    ANALOG ANALOG
*
* @brief       Analog voltage measurement
*
*              This module contains API that returns voltage at Analog port of SPM
*/
/**
* @addtogroup  ANALOG
* @{
*/

/**
 -----------------------------------------------------------------------------------------------------------------------
 anlReadVoltage
 -----------------------------------------------------------------------------------------------------------------------
*  This function will read value of voltage at Analog port of SunSet Pass Module
*
*  @date                         27/11/2014
*  @author                       SW_LLAPI_DEV_1
*
*  @param uiHandle               The handle of Analog port acquired earlier
*  @param uipAnlVoltage          memory pointer to copy voltage at Analog port of SunSet Pass Module
*                                \n memory allocation should be done by caller
*  @param bWithRtc               Get RTC time stamp or not
*  @param *spRtc                 memory pointer where date and time of RTC will be stored.It is time of I2C read 
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus anlReadVoltage(sUINT16 uiHandle, sUINT16* uipAnlVoltage, sBOOL bWithRtc, rtc* spRtc);

/**
 -----------------------------------------------------------------------------------------------------------------------
 localAnalogReadVoltage
 -----------------------------------------------------------------------------------------------------------------------
*  This function will read raw data and conversion scale for Analog port of local machine
*
*  @date                         20/02/2016
*  @author                       SW_LLAPI_DEV_1
*
*  @param uiHandle               The handle of local Analog port acquired earlier
*  @param ipRawAdcData           memory pointer to copy raw data of Analog channel of local machine
*                                \n memory allocation should be done by caller
*  @param fpRawAdcScale          memory pointer to copy raw data of Analog channel of local machine
*                                \n memory allocation should be done by caller
*  @param iChaNum				 local port adc channel number
*  @param iDevNum				 local port adc device number
*  @param bWithRtc               Get RTC time stamp or not
*  @param *spRtc                 memory pointer where date and time of RTC will be stored.It is time of I2C read 
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus localAnalogReadVoltage(sUINT16 uiHandle, sINT16 *ipRawAdcData, sFLOAT *fpRawAdcScale, sUINT8 iChaNum, sUINT8 iDevNum, sBOOL bWithRtc, rtc* spRtc);

/**
* @}
*/

/*****************************************************************************************************
*                                         CAN
 ****************************************************************************************************/
/**
* @defgroup    CAN CAN
*
* @brief       CAN module is responsible for performing task related to communication on CAN bus
*
*              This module has API that can communicate over CAN bus with CAN device connected to Sunset Pass Module.
*              \n <B>Sequence to use CAN API is:</B>\n
*              Enable \n
*              Configure \n
*              Data transfer\n
*              Disable\n
*/
/**
* @addtogroup  CAN
* @{
*/
/*****************************************************************************************************
 *                               CAN PORT ENUMS
 ****************************************************************************************************/
/**
Specifies the status of the CAN Remote Transmit Request (RTR) feature for a CAN TX Channel.

This enumeration specifies the status of the CAN Remote Transmit Request 
feature for a CAN TX Channel. The RTR feature allows a node on the CAN Bus
to request a transmission from another node on the bus. The responding node
in this case should have a RTR enabled TX Channel in order to be able to
respond to this request.
*/
enum canTxRtr
{
   CAN_TX_RTR_ENABLED,     ///< CAN TX Channel RTR Feature is enabled.
   CAN_TX_RTR_DISABLED     ///< CAN TX Channel RTR Feature is disabled.
};

/**
Specifies the priority of a TX channel.
This enumeration identifies the available TX channel priorities. A TX channel
has its own natural priority order, which determines priority when two or more
TX channels are assigned the same priority level. Channel 1 has higher natural
priority than Channel 0 and Channel 2 has a natural priority than Channel 1, and
so on.
*/
enum canTxChannelPriority
{
   CAN_LOWEST_PRIORITY,
   CAN_LOW_MEDIUM_PRIORITY,
   CAN_HIGH_MEDIUM_PRIORITY,
   CAN_HIGHEST_PRIORITY
};

/**
Specifies the CAN ID Type

This enumeration specifies the two CAN ID types: Standard and Extended. The Standard
Type ID is 11 bits long and the Extended ID is 29 bits long. This enumeration then
specifies the type of the ID specified while configuring Filter and Filter masks

*/
enum canIdType
{
   CAN_EID,    ///< CAN Extended ID
   CAN_SID     ///< CAN Standard ID
};

/**
Enables the Data Only Receive Mode or Full Receive Mode of a CAN Receive Channel

This enumeration specifies the status of the CAN Receive Channel Data Only
feature. If the feature is enabled, the CAN module will store only the data
payload portion of the received CAN message. If the Full Receive mode is 
specified, the CAN module stores the entire CAN message (ID field plus data
payload). The RX channel can either in Data Only mode or Full Receive Mode
*/
enum canRxDataMode
{
   CAN_RX_DATA_ONLY,          ///< CAN RX Channel Data Only Mode is enabled.
   CAN_RX_FULL_RECEIVE        ///< CAN RX Channel Full Receive Mode is enabled.
};
/**
This enumeration identifies the available filters masks in CAN module.
*/
enum canFilterMask
{
   CAN_FILTER_MASK0,   ///< CAN Filter Mask 0
   CAN_FILTER_MASK1,   ///< CAN Filter Mask 1
   CAN_FILTER_MASK2,   ///< CAN Filter Mask 2
   CAN_FILTER_MASK3,   ///< CAN Filter Mask 3
};

/*****************************************************************************************************
 *                               CAN PORT STRUCTURE
 ****************************************************************************************************/
/**
Configuration of number of bit Time quanta in a CAN bit.

This data structure provides an interface to the CAN bit configuration parameters.
Note that the bit configuration affects the CAN Bus Speed. The total number of 
time quanta in a bit is between 8 and 25 inclusive.  Refer to the CAN Bus 
specification for more details on  setting up the time quanta in a bit.

This data type is used with the CANSetSpeed() function.
*/
struct canBitConfig
{
   /// Number of Time quanta in Phase Segment 2.
   /// This value can be between 1 and 8
   sUINT8 phaseSeg2Tq;

   /// Number of Time quanta in Phase Segment 1.
   /// This value can be between 1 and 8
   sUINT8 phaseSeg1Tq;

   /// Number of Time quanta in Propagation Segment 2.
   /// This value can be between 1 and 8
   sUINT8 propagationSegTq;

   /// This determines if the Phase Segment 2  value is
   /// specified by code or set automatically by the module.
   /// TRUE - Phase Segment 2 can be set in code.
   /// FALSE - Phase Segment 2 is set by module.
   sBOOL phaseSeg2TimeSelect;

   /// This determines the number of times a bit is sampled by
   /// the CAN module..
   /// TRUE - bit is sampled 3 times.
   /// FALSE - bit is sampled once.
   sBOOL sample3Time;

   /// This determines the Synchronization jump width Time quanta.
   /// This value should be between 1 and 4
   sUINT8 syncJumpWidth;
};

/**
Structure that contain CAN Transmission Channel configuration data
*/
struct canConfigTx
{
   canTxRtr eCanTxRtr;                          ///<Enables disables Remote Transmit Request @ref canTxRtr
   canTxChannelPriority eCanTxChannelPriority;  ///<Specifies the priority of the TX channel
   sUINT8 ucCanChannel;                         ///<Identifies the desired CAN Channel. This value should be between 0 and 31
   sUINT8 ucCanChannelSize;                     ///<Size of the channel in messages. This value should be between 1 and 32
};

/**
Structure that contain CAN reception Channel configuration data
*/
struct canConfigRx
{
   sUINT32 ulCanFilterId;                  ///<A value in the range 0x0 to 0x7FF for SID filter type or 0x0 to 0x1FFFFFFF for EID filter type
   canIdType eCanIdType;                   ///<Specifies the CAN ID Type @ref canIdType
   sUINT8 ucCanFilterMask;                 ///<Select Filter Mask, Value should be 0 to 3
   canRxDataMode eCanRxDataMode;           ///<Enables the Data Only Receive Mode or Full Receive Mode of a CAN Receive Channel
   sUINT8 ucCanChannel;                    ///<Identifies the desired CAN RX channel
   sUINT8 ucCanChannelSize;                ///<Specifies the number of received messages that the channel can buffer before it overflows This should be a value between 1 and 32.
};

/**
Defines the structure of the SID word section of the RX message

This data structure represents the SID section of the CAN RX message. The data
data structure is an element of the CANRxMessageBuffer data structure
*/
struct canRxMsgSID
{
   /// SID of the Received CAN Message.
   sUINT32 uSID:11;

   /// Filter which accepted this message.
   sUINT32 uFILHIT:5;

   /// Time stamp of the received message. This is
   /// valid only if the Time stamping is enabled.
   sUINT32 uCMSGTS:16;
};

/**
Defines the structure of the EID word section of the TX and RX message
This data structure represents the EID section of the CAN TX and RX message. 
The data structure is an element of the CANMessageBuffer
data structures
*/
struct canMsgEID
{
   /// Data Length Control. Specifies the size of the
   /// data payload section of the CAN packet. Valid
   /// values are 0x0 - 0x8.
   sUINT32 uDLC:4;

   /// Reserved bit. Should be always 0.
   sUINT32 uRB0:1;
   ///Padding
   sUINT32 :3;

   /// Reserved bit. Should be always 0.
   sUINT32 uRB1:1;

   /// Remote Transmit Request bit. Should be set for
   /// RTR messages, clear otherwise.
   sUINT32 uRTR:1;

   /// CAN TX and RX Extended ID field. Valid values
   /// are in range 0x0 - 0x3FFFF.
   sUINT32 uEID:18;

   /// Identifier bit. If 0 means that message is SID.
   /// If 1 means that message is EID type.
   sUINT32 uIDE:1;

   /// Substitute Remote request bit. This bit should
   /// always be clear for an EID message. It is ignored
   /// for an SID message.
   sUINT32 uSRR:1;
   ///Padding
   sUINT32 :2;
};


/**
Defines the structure of the CAN Message Buffer
*/
struct canMessageBuffer
{
   canRxMsgSID sCanRxMsgSID;      ///< This is SID portion of the CAN message.
   canMsgEID sCanMsgEID;          ///< This is EID portion of the CAN message
   sUINT8 ucCanData[8];           ///< This is the data payload section of the received message.
};

/*****************************************************************************************************
 *                               CAN PORT API
 ****************************************************************************************************/
/**
 -----------------------------------------------------------------------------------------------------------------------
 canSetSpeed
 -----------------------------------------------------------------------------------------------------------------------
*  This function will set speed of CAN bus data transfer.
*
*  @date                   29/12/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    None
*
*  @param uiHandle         The handle of CAN port acquired earlier
*  @param spCanBitConfig   Configuration of number of bit Time quanta in a CAN bit @ref canBitConfig
*  @param ulCanBusSpeed    The desired CAN bus speed specified in bits per second
*
*  @return                 Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus canSetSpeed(sUINT16 uiHandle, const canBitConfig* spCanBitConfig, sUINT32 ulCanBusSpeed);

/**
 -----------------------------------------------------------------------------------------------------------------------
 canConfigureChannelForRx
 -----------------------------------------------------------------------------------------------------------------------
*  This routine configures a CAN channel for Receive operation
*  This routine configures a CAN channel for receive operation. A receive channel
*  can be either a full CAN message receive channel, which receives an entire CAN
*  message (Arbitration field + Data field) or a data only message channel, which 
*  receives only the data payload section of the message. A receive channel can 
*  buffer up to 32 messages. The number of messages to buffer (i.e. the size of the
*  channel) is set by the channelSize parameter.
*
*  @date                   29/12/13
*  @author                 SW_LLAPI_DEV_1
*  @pre                    None
*
*  @param uiHandle         The handle of CAN port acquired earlier
*  @param sCanConfigRx     Structure that contain CAN reception Channel configuration data @ref canConfigRx
*                          \n memory allocation should be done by caller
*
*  @return                 Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus canConfigureChannelForRx(sUINT16 uiHandle, const canConfigRx* sCanConfigRx);

/**
 -----------------------------------------------------------------------------------------------------------------------
 canConfigureChannelForTx
 -----------------------------------------------------------------------------------------------------------------------
*  This routine configures a CAN Channel for transmission. The size of the channel specifies the number of messages that the 
*  channel can buffer. The channel is a First In First Out (FIFO) type of buffer. The Remote Transmit Request feature allows the 
*  transmit channel to start transmitting when an associated filter has received a message. The TX channel priority determines the 
*  priority as compared to other TX channels. If two TX channels have the same priority, the natural channel priority
*  determines which channel transmits first.
*
*  @date                   27/11/2013
*  @author                 SW_LLAPI_DEV_1
*  @pre                    CAN port should be configured and enabled
*
*  @param uiHandle         The handle of CAN port acquired earlier
*  @param spCanConfigTx    Structure that contain CAN Transmission Channel configuration data @ref canConfigTx
*
*  @return                 Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus canConfigureChannelForTx(sUINT16 uiHandle, const canConfigTx* spCanConfigTx);

/**
 -----------------------------------------------------------------------------------------------------------------------
 canGetRxMessage
 -----------------------------------------------------------------------------------------------------------------------
*  This function is used to get receive message of Sunset Pass Module CAN port
*
*  @date                         27/11/2013
*  @author                       SW_LLAPI_DEV_1
*  @pre                          CAN port should be configured
*
*  @param uiHandle               The handle of CAN port acquired earlier
*  @param ucCanRxChannel         Identifies the desired CAN Channel. This value should be between 0 and 31
*  @param sCanRxMessageBuffer    structure pointer where CAN port message data will be stored, refer structure @ref canMessageBuffer
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus canGetRxMessage(sUINT16 uiHandle, sUINT8 ucCanRxChannel, canMessageBuffer* sCanRxMessageBuffer);

/**
 -----------------------------------------------------------------------------------------------------------------------
 canSendTxMessage
 -----------------------------------------------------------------------------------------------------------------------
*  This function will write message to Sunset Pass Module CAN port
*
*  @date                         29/12/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          None
*
*  @param uiHandle               The handle of CAN port acquired earlier
*  @param ucCanTxChannel         Identifies the desired CAN Channel. This value should be between 0 and 31
*  @param sCanTxMessageBuffer    structure pointer with CAN port message data, refer structure @ref canMessageBuffer
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus canSendTxMessage(sUINT16 uiHandle, sUINT8 ucCanTxChannel, const canMessageBuffer* sCanTxMessageBuffer);

/**
 -----------------------------------------------------------------------------------------------------------------------
 canEnable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will enable CAN module of Sunset Pass
*
*  @date                         29/12/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          None
*
*  @param uiHandle               The handle of CAN port acquired earlier
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus canEnable(sUINT16 uiHandle);


/**
 -----------------------------------------------------------------------------------------------------------------------
 canDisable
 -----------------------------------------------------------------------------------------------------------------------
*  This function will disable CAN module of Sunset Pass
*
*  @date                         29/12/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          None
*
*  @param uiHandle               The handle of CAN port acquired earlier
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus canDisable(sUINT16 uiHandle);

/**
 -----------------------------------------------------------------------------------------------------------------------
 canConfigureFilterMaskBits
 -----------------------------------------------------------------------------------------------------------------------
*  This routine configures a CAN Filter Mask. The mask bits determine which
*  message ID bits are ignored and compared during the filtering process. This routine will configure
*  4 type of Mask bits for use with RX channel.
*
*  @date                         29/12/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          None
*
*  @param uiHandle               The handle of CAN port acquired earlier
*  @param ulpCanFilterMaskBits   Pointer, That contain 4 Value of Mask bits. A value in the range 0x0 to 0x7FF for SID range  or 0x0 to 0x1FFFFFFF
*                                \n for the EID range. Each set bit will mean that the corresponding bit in the filter will be compared to 
*                                \n the corresponding bit in the message ID. A clear mask bit means the corresponding bit in the incoming message 
*                                \n ID field will be ignored.
*                                \n memory allocation should be done by caller
*
*  @return                       Refer Enum @ref returnStatus
*
************************************************************************************************************************
*/
DLL_API returnStatus canConfigureFilterMaskBits(sUINT16 uiHandle, const sUINT32* ulpCanFilterMaskBits);

/**
 -----------------------------------------------------------------------------------------------------------------------
 setComValue
 -----------------------------------------------------------------------------------------------------------------------
*  This function will set COM port value for specific module to access it
*
*  @date                         29/12/13
*  @author                       SW_LLAPI_DEV_1
*  @pre                          None
*
*  @param ePortId                port id
*  @param cpComValue			 string of com port value
*  @param uiComDataLength	     Length of comport value string
*
*  @return                       Refer Enum @ref returnStatus
************************************************************************************************************************
*/
DLL_API returnStatus setComValue(PortID ePortId, sUINT8P cpComValue, sUINT8 uiComDataLength);
/**
* @}
*/

}

#endif //EXPORT_H_
