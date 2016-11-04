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
*	@file				SPInterface.h
*	
*	@brief			This file contains declaration of all the data structures and  APIs which will be exported by the HLAPI DLL
*	
*	@copyright		2010-2015 All rights reserved
*	@version			Revision: 2.0
*	@date				05 Dec 2013
*	
*	\par	Software License Agreement:
*	
*		The software supplied herewith by Intel's vendor to Intel for Sunset Pass Project for use exclusively
*		on PC. The software is owned by the Intel, and is protected under applicable copyright laws.
*	
*	
*	\par	Change History:
*	
*	Rev   |  Description                                                 |  Author         |  Date
*	----  |  -----------                                                 |  ---------------|  -------
*	0.01  | Created file with all APIs                                   | SW_HLAPI_DEV_1  |  05 Dec 2013
*	0.02  | extern "C" added to make HLAPI compatible with non C++ applications  | SW_HLAPI_DEV_1  |  05 Mar 2014
*	0.02  | API names of get/set property and get capability changed according to their type, due to addition of extern "c" | SW_HLAPI_DEV_1  |  05 Mar 2014
*	0.02  | Macro name changed to SP_EVENT_NOTIFICATION and SP_UPGRADE_NOTIFICATION in place of EVENT_NOTIFICATION and SP_UPGRADE_NOTIFICATION respectively | SW_HLAPI_DEV_1  |  07 Mar 2014
*	0.02  | Prototype of the APIs modified to avoid using STL data types like "string", "list" etc. | SW_HLAPI_DEV_1  |  13 Mar 2014
*	0.02  | API name changed to spSystemOpen and spSystemClose in place of spSystemInit and spSystemDeInit respectively | SW_HLAPI_DEV_1  |  14 Mar 2014
*	0.02  | Used stdint.h and its data types instead of normal data types | SW_HLAPI_DEV_1  |  15 Mar 2014
*	0.03  | SPDataBuff structure added to store the byte array, the SPString structure is of same type | SW_HLAPI_DEV_1  |  27 March 2014
*	0.03  | Macro name changed to SP_MAX_BUFF_SIZE in place of SP_MAX_STRING_SIZE | SW_HLAPI_DEV_1  |  03 April 2014
*	0.03  | Device sub types SP_DEV_SENSOR_TEMPERATURE, SP_DEV_SENSOR_CURRENT and SP_DEV_SENSOR_VOLTAGE removed and generic sensor subtypes SP_DEV_SENSOR_GEN_I2C and SP_DEV_SENSOR_GEN_GPIO added instead | SW_HLAPI_DEV_1  |  04 April 2014
*	0.03  | Generic sensor related properties SP_DEV_PROP_GEN_SENSOR_UNIT and SP_DEV_PROP_GEN_SENSOR_TYPE added | SW_HLAPI_DEV_1  |  04 April 2014
*	0.03  | New API @ref spSensorGetValueA added to read data from I2C based generic sensor devices | SW_HLAPI_DEV_2  |  04 April 2014
*	0.03  | Prototype of the @ref spIDDispenseItem API changed | SW_HLAPI_DEV_1  |  04 April 2014
*	0.03  | Temperature sensor is a type of Generic sensor and that does not generate any event so that is removed from the list of devices who can generate events  | SW_HLAPI_DEV_1  |  04 April 2014
*	0.03  | New device types SP_DEV_CP and SP_DEV_LD added for Cashless Payment and Line Display devices | SW_HLAPI_DEV_1  |  07 April 2014
*	0.03  | Cashless Payment related properties SP_DEV_PROP_CP_SERIAL_NUMBER, SP_DEV_PROP_CP_PRODUCT_NUMBER and SP_DEV_PROP_CP_AMOUNT added | SW_HLAPI_DEV_1  |  07 April 2014
*	0.03  | Operation type enum named SP_CP_OPERATION_REQ added for Cashless Payment device | SW_HLAPI_DEV_1  |  07 April 2014
*	0.03  | Cashless payment and Line display APIs added | SW_HLAPI_DEV_1  |  07 April 2014
*	0.03  | SP_UPGRADE_TYPE enum is removed, not required now | SW_HLAPI_DEV_1  |  08 April 2014
*	0.03  | HLAPI API supports upgrade and notification of SunsetPass firmware only. Prototype of SP_UPGRADE_NOTIFICATION and spUpgrade API is modified accordingly | SW_HLAPI_DEV_2  |  08 April 2014
*	1.00  | Property @ref SP_DEV_PROP_CP_CURRENCY_CODE added for cashless Payment device | SW_HLAPI_DEV_2  |  10 April 2014
*	1.00  | Prototype of @ref spGetVersion API changed | SW_HLAPI_DEV_2  |  14 April 2014
*	1.00  | New API @ref spDevGetPropertyLN added to read unsigned long type properties | SW_HLAPI_DEV_2  |  14 April 2014
*	1.1  | Changed Name of old API @ref spSensorSetValue to @ref spSensorSetValueN | SW_HLAPI_DEV_2  |  19 June 2014
*	1.1  | New API @ref spSensorSetValue added to write data to generic sensor devices | SW_HLAPI_DEV_2  |  19 June 2014
*	1.1  | Added support for Generic sensors based on SPI,OneWire and Analog Port | SW_HLAPI_DEV_2  |  19 June 2014
*	1.1  | Added support for Automatic firmware upgrade in spSystemOpen | SW_HLAPI_DEV_2  |  19 June 2014
*	1.1  | Added support for ibutton based generic onewire sensor | SW_HLAPI_DEV_2  |  08 July 2014
*	1.1  | Added New API @ref spKLSetPosition to set position on keylock device | SW_HLAPI_DEV_2  |  09 July 2014
*	1.2  | Added Support for USD device | SW_HLAPI_DEV_2  |  30 July 2014
*	1.3  | Klocwork issues resolved | SW_HLAPI_DEV_2  |  04 Sept 2014
*	2.0  | Added support for ccTalk and ProtocolA based coin acceptor and dispenser | SW_HLAPI_DEV_2  |  24 Sept 2014
*	2.0  | Added support for read cash count capability | SW_HLAPI_DEV_2  |  25 Sept 2014
*	2.0  | New return type @ref SP_DISP_ACT_AMT_NOT_SUPPORTED added for @ref spCDDispenseChange API. | SW_HLAPI_DEV_2  |  26 Sept 2014
*	3.0  | Added support for activate ccTalk based multiple hoppers at a time | SW_HLAPI_DEV_2  |  27 Oct 2014
*	3.1  | Sensor timestamp support added for I2C, 1-wire, SPI, GPI and Voltage based generic sensor. | SW_HLAPI_DEV_2  |  28 Oct 2014
*	3.2  | New return type @ref SP_E_PORT_ACQUIRED added to support sequential port access for shared port. | SW_HLAPI_DEV_2  |  27 Nov 2014
*	3.2  | FAN consistency related bug reported by Trevor solved. | SW_HLAPI_DEV_2  |  27 Nov 2014
*	3.2  | Removed hard coded MDB Poll codes from firmware and passed them to it by HLAPI | SW_HLAPI_DEV_2  |  27 Nov 2014
*	3.2  | New property @ref SP_DEV_PROP_FAN_DEFAULT_SPEED added to return default speed specified in XML | SW_HLAPI_DEV_2  |  28 Nov 2014
*	4.0  | Final gold release | SW_HLAPI_DEV_2  |  16 Mar 2015
*	4.1  | Added support for SecureHead MSR device (Cashless payment type) | SW_HLAPI_DEV_2  |  05 May 2015
*	4.1  | Size of @ref SP_MAX_BUFF_SIZE changed from 512 bytes to 1024 bytes | SW_HLAPI_DEV_2  |  05 May 2015
*	4.2  | Added support for Kiosk3 Contactless device (Cashless payment type) | SW_HLAPI_DEV_2  |  10 May 2015
*	4.2  | Added spSensorSetValueNGPIO | SW_HLAPI_DEV_2  |  25 May 2015
*/

#if !defined(SP_INTERFACE_H)
#define SP_INTERFACE_H

#include <stdint.h>

#ifdef _WIN32
#define DLL_EXPORT __declspec (dllexport)			///< DLL export macro
#elif __linux__
#define DLL_EXPORT
#endif

#define HLAPI_VERSION	"5.00"							///< Current version of HLAPI DLL
#define SP_MAX_BUFF_SIZE (1024)						///< Max size of character array to pass data between application/library
#define SP_MAX_NO_OF_DEVICES (256)					///< Max number of devices that can be present in the system (or configuration file)

/**
* @defgroup		grpDataStructures Data Structures
* This group contains resources common to all modules
*/
/**
* @addtogroup	grpDataStructures
* @{
*/

/**
	 ENUM Device Type
*/
enum SP_DEVICE_TYPE
{
	SP_DEV_CA,											///< Coin acceptor
	SP_DEV_CD,											///< Coin dispenser
	SP_DEV_BA,											///< Bill acceptor
	SP_DEV_BD,											///< Bill dispenser
	SP_DEV_ID,											///< Item dispenser
	SP_DEV_KL,											///< Key lock
	SP_DEV_LIGHT,										///< Light
	SP_DEV_SENSOR,										///< Sensors
	SP_DEV_DEX,											///< DEX
	SP_DEV_MS,											///< Motion sensor
	SP_DEV_PM,											///< Power manager
	SP_DEV_SSD,											///< Seven segment display
	SP_DEV_CP,											///< Cashless payment
	SP_DEV_LD,											///< Line display
	SP_DEV_INVALID										///< Invalid device type
};

/**
	 ENUM Device subtype
*/
enum SP_DEVICE_SUBTYPE
{
	SP_DEV_SUBTYPE_MDB = 0,							///< MDB based device
	SP_DEV_SUBTYPE_USB,								///< USB based device
	SP_DEV_ID_MOTOR,									///< Motor based Item Dispenser
	SP_DEV_ID_USD,										///< USD based Item Dispenser
	SP_DEV_KEYLOCK_MANUAL,							///< Manual Keylock
	SP_DEV_KEYLOCK_ELECTRONIC,						///< Electronic Keylock
	SP_DEV_LIGHT_ALL,									///< Light device
	SP_DEV_SENSOR_FAN,								///< FAN sensor
	SP_DEV_SENSOR_GEN_I2C,							///< I2C based generic sensor
	SP_DEV_SENSOR_GEN_SPI,							///< SPI based generic sensor
	SP_DEV_SENSOR_GEN_ONEWIRE,						///< OneWire based generic sensor
	SP_DEV_SENSOR_GEN_VOLTAGE,						//< Analog port based generic sensor
	SP_DEV_SENSOR_GEN_GPIO,							///< GPIO based generic sensor
	SP_DEV_DEX_ALL,									///< DEX device
	SP_DEV_MS_ALL,										///< Motion sensor device
	SP_DEV_PM_ALL,										///< Power manager device
	SP_DEV_SSD_ALL,									///< Seven segment device
	SP_DEV_SUBTYPE_CCTALK,							///< CCTALK based device
	SP_DEV_SUBTYPE_PROTOCOL_A						///< Protocol-A based device
};

/**
	ENUM Device properties
*/

/**
Property ID                             |Type          |Accessibility |Supported Devices
:---------------------------------------|:------------:|:-----------: |:---------------------------------
SP_DEV_PROP_PHYSICAL_DEVICE_DESCRIPTION |SPString      |Read-only     |All                               
SP_DEV_PROP_PHYSICAL_DEVICE_NAME        |SPString      |Read-only     |All                               
SP_DEV_PROP_DEVICE_ACQUIRED             |bool          |Read-only     |All                               
SP_DEV_PROP_DEVICE_ENABLED              |bool          |Read-Write    |All                               
SP_DEV_PROP_CURRENCY_CODE               |SPString      |Read-only     |SP_DEV_CA, SP_DEV_BA              
SP_DEV_PROP_DEPOSIT_CASH_LIST           |SPString      |Read-only     |SP_DEV_CA, SP_DEV_BA              
SP_DEV_PROP_DEPOSIT_COUNTS              |SPString      |Read-only     |SP_DEV_CA, SP_DEV_BA              
SP_DEV_PROP_DEPOSIT_AMOUNT              |int32_t       |Read-only     |SP_DEV_CA, SP_DEV_BA              
SP_DEV_PROP_DEPOSIT_STATUS              |int32_t       |Read-only     |SP_DEV_CA, SP_DEV_BA              
SP_DEV_PROP_REAL_TIME_DATA_ENABLED      |bool          |Read-Write    |SP_DEV_CA, SP_DEV_BA              
SP_DEV_PROP_FULL_STATUS                 |int32_t       |Read-only     |SP_DEV_CA, SP_DEV_BA              
SP_DEV_PROP_DISPENSER_STATUS            |int32_t       |Read-only     |SP_DEV_CD, SP_DEV_ID              
SP_DEV_PROP_MAX_SLOTS                   |int32_t       |Read-only     |SP_DEV_ID                         
SP_DEV_PROP_MAX_LIGHTS                  |int32_t       |Read-only     |SP_DEV_LIGHT                      
SP_DEV_PROP_MS_SENSOR_STATUS            |bool          |Read-only     |SP_DEV_MS                         
SP_DEV_PROP_KEY_POSITION                |int32_t       |Read-only     |SP_DEV_KL                         
SP_DEV_PROP_CP_SERIAL_NUMBER            |SPString      |Read-only     |SP_DEV_CP                         
SP_DEV_PROP_CP_PRODUCT_NUMBER           |unsigned long |Read-only     |SP_DEV_CP                         
SP_DEV_PROP_CP_AMOUNT                   |unsigned long |Read-write    |SP_DEV_CP                         
SP_DEV_PROP_CP_CURRENCY_CODE            |int32_t       |Read-write    |SP_DEV_CP                         
SP_DEV_PROP_GEN_SENSOR_UNIT             |SPString      |Read-only     |SP_DEV_SENSOR                     
SP_DEV_PROP_GEN_SENSOR_TYPE             |SPString      |Read-only     |SP_DEV_SENSOR  
SP_DEV_PROP_CP_MSR_KSN                  |SPString      |Read-only     |SP_DEV_CP 
SP_DEV_PROP_CP_MSR_ALL_SETTINGS         |SPString      |Read-only     |SP_DEV_CP
SP_DEV_PROP_CP_MSR_SECURITY_LEVEL		|SPString      |Read-only     |SP_DEV_CP
SP_DEV_PROP_CP_MSR_DEFAULTS             |int32_t       |Write	      |SP_DEV_CP
SP_DEV_PROP_CP_MSR_READING				|int32_t       |Write		  |SP_DEV_CP
SP_DEV_PROP_CP_MSR_DECODING_METHOD      |int32_t       |Write		  |SP_DEV_CP
SP_DEV_PROP_CP_MSR_TRACK                |int32_t       |Write		  |SP_DEV_CP
SP_DEV_PROP_CP_MSR_KEY_MANAGEMENT_TYPE  |int32_t       |Write		  |SP_DEV_CP
SP_DEV_PROP_CP_MSR_ENCRYPTION           |int32_t       |Write		  |SP_DEV_CP


**/

enum SP_DEV_PROPERTIES
{
	SP_DEV_PROP_PHYSICAL_DEVICE_DESCRIPTION,	///< String describing physical device
	SP_DEV_PROP_PHYSICAL_DEVICE_NAME,			///< Device name string
	SP_DEV_PROP_DEVICE_ACQUIRED,				///< Flag to indicate if device is acquired or not
	SP_DEV_PROP_DEVICE_ENABLED,					///< Flag to indicate if device is enabled or not
	SP_DEV_PROP_CURRENCY_CODE,					///< String describing current currency supported by device
	SP_DEV_PROP_DEPOSIT_CASH_LIST,				///< String describing supported cash denominations. It consists of ASCII numeric comma delimited values which denote the units of the coins/bills.
	SP_DEV_PROP_DEPOSIT_COUNTS,					///< String describing cash accepted by the cash units. Cash units inside the string are the same as the @ref SP_DEV_PROP_DEPOSIT_CASH_LIST property and are in the same order.
	SP_DEV_PROP_DEPOSIT_AMOUNT,					///< Number equal to total amount of deposited cash
	SP_DEV_PROP_DEPOSIT_STATUS,					///< Current status of the cash acceptance operation. It may be one of the @ref SP_DEPOSIT_STATUS values
	SP_DEV_PROP_REAL_TIME_DATA_ENABLED,			///< Flag to indicate if real time data event reporting is enable or not
	SP_DEV_PROP_FULL_STATUS,					///< Full status of the cash slots. It may be one of the @ref SP_CASH_SLOT_STATUS values
	SP_DEV_PROP_DISPENSER_STATUS,				///< Current status of the dispenser. It may be one of the @ref SP_DISPENSER_STATUS values
	SP_DEV_PROP_MAX_SLOTS,						///< Maximum number of slots that the Item dispenser can support
	SP_DEV_PROP_MAX_LIGHTS,						///< Maximum number of lights that the Light device can support
	SP_DEV_PROP_MS_SENSOR_STATUS,				///< Motion sensor status (i.e. presence of motion)
	SP_DEV_PROP_KEY_POSITION,					///< Current Key position of Keylock
	SP_DEV_PROP_CP_SERIAL_NUMBER,				///< Serial number of cashless payment device
	SP_DEV_PROP_CP_PRODUCT_NUMBER,				///< Product number of cashless payment device
	SP_DEV_PROP_CP_AMOUNT,						///< Amount to make payment with cashless payment device
	SP_DEV_PROP_CP_REVALUE_AMOUNT,				///< Revalue amount to be transferred back to the payment device
	SP_DEV_PROP_CP_CURRENCY_CODE,				///< Currency code pf cashless payment device. Currently two codes(i.e., 840 for USD and 978 for EURO) are supported.
	SP_DEV_PROP_GEN_SENSOR_UNIT,				///< Unit of Generic sensor. The value of the property will be copied from the XML configuration file
	SP_DEV_PROP_GEN_SENSOR_TYPE,				///< Type of Generic I2C or GPIO based sensor. The value of the property will be copied from the XML configuration file
	SP_DEV_PROP_FAN_DEFAULT_SPEED,				///< Default speed of FAN sensor specified in XML 
	SP_DEV_PROP_CP_MSR_KSN,                     ///< KSN for SecureHead MSR  (For DUPKT only)
	SP_DEV_PROP_CP_MSR_ALL_SETTINGS,            ///< Current configuration of Secure Head MSR Device
	SP_DEV_PROP_CP_MSR_SECURITY_LEVEL,			///< Security Level for SecureHead MSR
	SP_DEV_PROP_CP_MSR_DEFAULTS,                ///< Restore Defaults for Secure Head MSR device (Note: Arguement for this property is ignored).
	SP_DEV_PROP_CP_MSR_READING,					///< Enable/Disable MSR Reading (Note : Valid values are 0x30 & 0x31)
	SP_DEV_PROP_CP_MSR_DECODING_METHOD,         ///< Allow bi-directional/forward/backward MSR reading (Note : Valid values are 0x31, 0x32, 0x33 & 0x34)
	SP_DEV_PROP_CP_MSR_TRACK,                   ///< Track to be read and decoded (Note : Valid values are 0x31, 0x32, ...., 0x39)
	SP_DEV_PROP_CP_MSR_KEY_MANAGEMENT_TYPE,     ///< Fixed/DUPKT Key Management type (Note : Valid values are 0x30 & 0x31)
	SP_DEV_PROP_CP_MSR_ENCRYPTION,              ///< Disable encryption or select AES/TDES encryption algorithm as per requirement (Note : Valid values are 0x30, 0x31 & 0x32)
	SP_DEV_PROP_CP_COUNTRY_CODE,				///< Country code of cashless payment device as per ISO 3166
	SP_DEV_PROP_CP_AMOUNT_AUTH,					///< Authorized Amount for Kiosk3 device
	SP_DEV_PROP_CP_TRANSACTION_DATE,			///< Transaction Date
	SP_DEV_PROP_CP_TRANSACTION_TIME				///< Transaction Time	
};

/**
	ENUM Types of capabilities supported by all devices
*/
enum SP_DEV_CAPABILITIES
{
	SP_DEV_CAP_REAL_TIME_DATA,						///< Capability to report real time data event
	SP_DEV_CAP_FULL_SENSOR,							///< Capability to report FULL condition
	SP_DEV_CAP_NEAR_FULL_SENSOR,					///< Capability to report NEAR FULL condition
	SP_DEV_CAP_JAM_SENSOR,							///< Capability to report JAM condition
	SP_DEV_CAP_PAUSE_DEPOSIT,						///< Capability to PAUSE cash acceptance
	SP_DEV_CAP_EMPTY_SENSOR,						///< Capability to report EMPTY condition
	SP_DEV_CAP_NEAR_EMPTY_SENSOR,					///< Capability to report NEAR EMPTY condition
	SP_DEV_CAP_INDIVIDUAL_SLOT_STATUS,			///< Capability to provide individual slot statue
	SP_DEV_CAP_ALARM,									///< Alarm capability
	SP_DEV_CAP_BLINK,									///< Lights blink capability
	SP_DEV_CAP_COLOR,									///< Lights color capability
	SP_DEV_CAP_ADJUST_CASH_COUNT,					///< Capability of the device to set initial amounts after initial setup, or to adjust cash counts after replenishment or removal, such as a paid in or paid out operation
	SP_DEV_CAP_READ_CASH_COUNT						///< Capability of the device to manage cash count
};

/**
	ENUM Current status of the cash acceptance
*/
enum SP_DEPOSIT_STATUS
{
	SP_DEPOSIT_START,									///< Cash acceptance started
	SP_DEPOSIT_END,									///< Cash acceptance stopped
	SP_DEPOSIT_COUNT,									///< Counting or repaying the deposited money
	SP_DEPOSIT_JAM,									///< A mechanical fault has occurred
	SP_DEPOSIT_PAUSE,									///< Cash acceptance paused
	SP_DEPOSIT_RESUME									///< Cash acceptance resumed
};

/**
	ENUM Current status of acceptor device cash slots
*/
enum SP_CASH_SLOT_STATUS
{
	SP_CASH_SLOT_STATUS_OK,							///< All cash slots are neither nearly full nor full
	SP_CASH_SLOT_STATUS_FULL,						///< Some cash slots are full
	SP_CASH_SLOT_STATUS_NEARFULL					///< Some cash slots are nearly full
};

/**
	ENUM Current status of dispenser device slots
*/
enum SP_DISPENSER_STATUS
{
	SP_DISPENSER_STATUS_OK,							///< All slots are neither nearly empty nor empty
	SP_DISPENSER_STATUS_EMPTY,						///< Some slots are empty
	SP_DISPENSER_STATUS_NEAREMPTY,				///< Some slots are nearly empty
	SP_DISPENSER_STATUS_JAM,						///< A mechanical fault has occurred
};

/**
	ENUM Power manager mode
*/
enum SP_POWER_STATE
{
	SP_POWER_STATE_SLEEP = 1,						///< Power manager sleep mode
	SP_POWER_STATE_WAKEUP,							///< Power manager wakeup mode
};

/**
	ENUM How to dispense item from Motor based Item Dispenser
*/
enum SP_RUN_MOTOR_BY
{
	SP_RUN_MOTOR_BY_TIME = 0x00,					///< Rotate motor for the duration specified
	SP_RUN_MOTOR_BY_ROTATION = 0x01				///< Rotate motor for specified number of rotations
};

/**
	ENUM Status codes
*/
enum SP_STATUS_CODE
{
	SP_SUCCESS = 0,									///< Success
	SP_DISP_ACT_AMT_NOT_SUPPORTED,				///< Dispense Change API successfully executed but actual amount is not supported by device
	SP_E_INVALID_SYS_STATE = -1999,				///< API call is not allowed in current system state
	SP_E_CLOSED,										///< An attempt was made to access a closed Device
	SP_E_CLAIMED,										///< An attempt was made to access a Physical Device that is already in use
	SP_E_DISABLED,										///< Cannot perform this operation while the Device is disabled
	SP_E_ILLEAGAL,										///< An attempt was made to perform an illegal or unsupported operation with the Device, or an invalid parameter value was used in XML or internal
	SP_E_NOHARDWARE,									///< The Physical Device is not connected to the system or is not powered on
	SP_E_OFFLINE,										///< The Physical Device is off-line
	SP_E_FAILURE,										///< Failed to perform the requested operation
	SP_E_VMC_COMM_CHA_INIT_FAILED,				///< Failed to establish communication to vending expansion card due to SPM device powered off, No/wrong communication channel exists, No/corrupt boot loader exists, No upgrade files found while upgrading system.
	SP_E_TIMEOUT,										///< Timeout while waiting for the response from the Physical Device or timeout while waiting for some pre-condition. For SecureHead MSR device, timeout can also indicate that MSR reading is disabled.
	SP_E_BUSY,											///< Device or system is busy. User application have to retry after some time
	SP_E_DEPRECATED,									///< Requested operation is deprecated
	SP_E_NOFIRMWARE,									///< Firmware not present on VMC
	SP_E_SOFTWARE_LLAPI_INIT_FAILED,					///< Failed to initialize software (LLAPI)
	SP_E_INVALID_CONFIG_PARAMETER,				///< Invalid XML parameter found
	SP_E_INVALID_ARGUMENT,							///< Invalid input arguments to the API
	SP_E_INVALID_PROPERTY,							///< Unsupported property (or type) for a device or it not writable
	SP_E_INVALID_CAPABILITY,						///< Unsupported capability (or type) for a device or it not writable
	SP_E_FIRST_HANDSHAKE_FAILED,					///< DEX device first handshake failed
	SP_E_SECOND_HANDSHAKE_FAILED,					///< DEX device second handshake failed
	SP_E_THIRD_HANDSHAKE_FAILED,					///< DEX device third handshake failed
	SP_E_DATA_TRANSFER_FAILED,						///< Data transfer between VMC and DEX device failed
	SP_E_PORT_ACQUIRED,								///< An attempt was made to access shared port when one device already acquired the port.
	SP_E_NOT_SUPPORTED,								///< Feature/Command not supported
};

/**
	ENUM Type of events that HLAPI can generate
*/
enum SP_EVENT_TYPE
{
	SP_EVE_DATA,										///< Data event
	SP_EVE_ERROR,										///< Error event
	SP_EVE_STATUS,										///< Status event
};

/**
	ENUM Status Events
*/
enum SP_STATUS_EVENT
{
	SP_SE_DEV_OK,										///< Device is in healthy condition
	SP_SE_DEV_RESET,									///< Device has detected an Reset condition and has returned to its power-on idle condition
	SP_SE_DEV_CONNECTED,								///< Device connected
	SP_SE_DEV_DISCONNECTED,							///< Device disconnected
	SP_SE_MS_MOTION_ON,								///< Motion detected
	SP_SE_MS_MOTION_OFF,								///< No motion
	SP_SE_KL_POSITION_CHANGE,						///< Key-lock position changed
	SP_SE_SENSOR_REACHED_THRESHOLD,				///< Threshold reached in monitoring devices. e.g. current, voltage, temperature etc.
	SP_SE_DEX_SEND_AUDIT_DATA,						///< Send audit data to DEX device
	SP_SE_DEX_READ_CONFIG_DATA,					///< Read configuration data from DEX device
	SP_SE_GEN_GPI_SENSOR_VALUE_CHANGE			///< GPI value change with timestamp for generic GPI based sensor
};

/**
	ENUM Data Events
*/
enum SP_DATA_EVENT
{
	SP_DE_CASH_ACCEPT,								///< Cash accepted
	SP_DE_READER_CONFIG,							///< Reader Config
	SP_DE_DISPLAY_REQ,								///< Display Request
	SP_DE_BEGIN_SESSION,							///< Begin Session
	SP_DE_SESSION_CANCEL_REQ,						///< Session Cancel Req
	SP_DE_VEND_APPROVED,							///< Vend Approved
	SP_DE_VEND_DENIED,								///< Vend Denied
	SP_DE_END_SESSION,								///< End Session
	SP_DE_STATUS_CANCELLED,							///< Cancelled
	SP_DE_STATUS_PERIPHERAL_ID,						///< Peripheral Id
	SP_DE_CASH_DISPENSE,							///< Cash dispensed
	SP_DE_REVALUE_APPROVED,							///< Revalue Approved
	SP_DE_REVALUE_DENIED,							///< Revalue Denied
	SP_DE_REVALUE_LIMIT_AMOUNT						///< Revalue Limit Amount
};

/**
	ENUM Error Events
*/
enum SP_ERROR_EVENT
{
	SP_EE_DEV_COMM_FAILURE,							///< Failed to communicate with device 
	SP_EE_DEV_TUBE_JAM,								///< A tube payout attempt has resulted in jammed condition
	SP_EE_CASH_JAM_IN_ACCEPTANCE_PATH,			///< Cash has been jammed in acceptor path
	SP_EE_DEV_SENSOR_DEFECT,						///< Sensor Problem - One of the sensors has failed to provide its response
	SP_EE_DEV_MOTOR_DEFECT,							///< Defective Motor - One of the motors has failed to perform its expected assignment
	SP_EE_DEV_ROM_CHECKSUM_ERR,					///< ROM Checksum Error - The validator's internal checksum does not match the calculated checksum
	SP_EE_CASH_REJECT,								///< Cash rejected
	SP_EE_CASH_BOX_REMOVED,							///< Cash box is removed/opened
	SP_EE_DEX_COMM_ID_MISMATCH,					///< Communication id received from DEX device and xml mismatch
	SP_EE_DEX_REV_LEVEL_MISMATCH,					///< Revision level received from DEX device and xml mismatch
	SP_EE_CP_MALFUNCTION_ERROR,					///< Malfunction Error
	SP_EE_CP_CMD_OUT_SEQ						///< Command Out Seq
};

/**
	ENUM Operation requests for cashless payment device
*/
enum SP_CP_OPERATION_REQ
{
	SP_CP_OPERATION_DEBIT = 0,						///< Debit operation
	SP_CP_OPERATION_CREDIT,							///< Credit operation
	SP_CP_OPERATION_ISO2,							///< TODO
	SP_CP_OPERATION_ANNULATION,					///< Annulation of a previous transaction
	SP_CP_OPERATION_DUPLICATE,						///< Duplicate of a previous transaction
	SP_CP_OPERATION_READ_TRACK_DATA			///< Read Track Data from Cashless Payment Device
};

/**
	Container to store device specific details
*/
struct DeviceDetail
{
	int32_t m_nDeviceID;								///< Device id: "device_id" attribute from MachineConfiguration, required by all the device specific APIs
	int32_t m_nID;									///< Auto generated unique id of device
	SP_DEVICE_TYPE m_eDeviceType;					///< Type of the device
	SP_DEVICE_SUBTYPE m_eDeviceSubType;			///< Device subtype
	bool m_bDeviceIsPowerdOn;						///< Flag to indicate if device is powered on or not
	char m_sName[SP_MAX_BUFF_SIZE];					///< User Friendly Device Name
	char m_sDeviceDescription[SP_MAX_BUFF_SIZE];///< Description / model name of the device
};

/**
	Container to pass character buffer
*/
struct SPDataBuff
{
	char pcData [SP_MAX_BUFF_SIZE];						///< Data buffer
	int32_t nDataSize;										///< Actual size of the data buffer
};

/**
	Container to pass string character buffer
*/
typedef SPDataBuff SPString;

/**
	Container to store the list of @ref DeviceDetail
*/
struct SPDeviceList
{
	DeviceDetail structDeviceList [SP_MAX_NO_OF_DEVICES];	///< Device details array
	int32_t nDeviceListSize;								///< Actual number of devices present in the array
};

/**
	Container to store the RTC time of system
*/
struct SPRTC
{
	unsigned char cYear;					///< Year value. It must be 1 to 99
	unsigned char cMonth;				///< Month value. It must be 1 to 12
	unsigned char cDate;					///< Date value. It must be 1 to 31
	unsigned char cWeekDay;				///< Week day value. It must be 0 to 6
	unsigned char cHour;					///< Hour value. It must be 0 to 23
	unsigned char cMinute;				///< Minute value. It must be 0 to 59
	unsigned char cSecond;				///< Second value. It must be 0 to 59
	uint16_t nMillisecond;				///< Millisecond value. It must be 0 to 999
};

/**
List of events that can be generated by the device

Device                 |nDeviceID     | eEventType        | nEventData1        | nEventData2            | pvEventData3 | nReserved   |
:----------------------|:-------------|:----------------- |:-------------------|:---------------------- |:----------- |:----------- |
Coin Acceptor          |deiceId       |@ref SP_EVE_DATA   |@ref SP_DE_CASH_ACCEPT|Value of accepted coin  |NA           |NA           |
Coin Acceptor          |deviceId      |@ref SP_EVE_STATUS |@ref SP_SE_DEV_DISCONNECTED \n @ref SP_SE_DEV_RESET|NA                      |NA           |NA           |
Coin Acceptor          |deviceId      |@ref SP_EVE_ERROR  |@ref SP_EE_CASH_REJECT \n @ref SP_EE_DEV_SENSOR_DEFECT \n @ref SP_EE_DEV_TUBE_JAM \n @ref SP_EE_DEV_ROM_CHECKSUM_ERR \n @ref SP_EE_CASH_JAM_IN_ACCEPTANCE_PATH|NA                      |NA           |NA           |
Coin Dispenser         |deviceId      |@ref SP_EVE_STATUS |@ref SP_SE_DEV_DISCONNECTED \n @ref SP_SE_DEV_RESET|NA                      |NA           |NA           |
Coin Dispenser         |deviceId      |@ref SP_EVE_ERROR  |@ref SP_EE_DEV_SENSOR_DEFECT \n @ref SP_EE_DEV_TUBE_JAM \n @ref SP_EE_DEV_ROM_CHECKSUM_ERR \n @ref SP_EE_CASH_JAM_IN_ACCEPTANCE_PATH|NA                      |NA           |NA           |
Bill Acceptor          |deiceId       |@ref SP_EVE_DATA   |@ref SP_DE_CASH_ACCEPT|Value of accepted bill  |NA           |NA           |
Bill Acceptor          |deviceId      |@ref SP_EVE_STATUS |@ref SP_SE_DEV_DISCONNECTED \n @ref SP_SE_DEV_RESET|NA         |NA           |NA           |
Bill Acceptor          |deviceId      |@ref SP_EVE_ERROR  |@ref SP_EE_DEV_SENSOR_DEFECT \n @ref SP_EE_CASH_BOX_REMOVED \n @ref SP_EE_DEV_ROM_CHECKSUM_ERR \n @ref SP_EE_CASH_JAM_IN_ACCEPTANCE_PATH \n @ref SP_EE_DEV_COMM_FAILURE \n @ref SP_EE_DEV_MOTOR_DEFECT \n @ref SP_EE_CASH_REJECT |NA           |NA           |NA           |
Key Lock               |deviceId      |@ref SP_EVE_STATUS |@ref SP_SE_KL_POSITION_CHANGE|Current key position|NA           |NA           |
Motion Sensor          |deviceId      |@ref SP_EVE_STATUS |@ref SP_SE_MS_MOTION_ON \n @ref SP_SE_MS_MOTION_OFF|NA                      |NA           |NA           |
Item Dispenser         |deviceId      |@ref SP_EVE_ERROR  |@ref SP_EE_DEV_MOTOR_DEFECT |NA                      |NA           |NA           |
Item Dispenser         |deviceId      |@ref SP_EVE_STATUS |@ref SP_SE_DEV_DISCONNECTED |NA                      |NA           |NA           |
DEX                    |deviceId      |@ref SP_EVE_STATUS |@ref SP_SE_DEV_DISCONNECTED \n @ref SP_SE_DEV_CONNECTED \n @ref SP_SE_DEX_SEND_AUDIT_DATA \n @ref SP_SE_DEX_READ_CONFIG_DATA|NA           |NA           |NA           |
Generic GPI Sensor     |deviceId      |@ref SP_EVE_STATUS |@ref SP_SE_GEN_GPI_SENSOR_VALUE_CHANGE |Changed value of GPI pin defined in XML |@ref SPRTC         |NA           |
SENSOR                 |deviceId      |@ref SP_EVE_STATUS |@ref SP_SE_DEV_DISCONNECTED \n @ref SP_SE_DEV_CONNECTED |NA                      |NA           |NA           |
*/

typedef void (*SP_EVENT_NOTIFICATION) (int32_t nDeviceId, int32_t eEventType, int32_t nEventData1, int32_t nEventData2, void *pvEventData3, int32_t nReserved);
///< Prototype of the event notification callback function

/**
* @}
*/

extern "C"
{

/**
* @defgroup		grpGeneralApis General APIs
* This section describes two set of APIs, one which are common to whole system and provides functionalities like 
* system initialization, system de-initialization, firmware/software upgrade, second which are specific to the 
* devices and provides functionalities like open/close, get capabilities, get/set properties, register notification 
* call back etc.
*
* Following is typical calling sequence of the General APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* @ref spGetVersion [Optional]\n
* @ref spRegUpgradeNotifiocation [Optional]\n
* <b>....</b>\n
* <b>....</b>\n
* Module specific API calls\n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*
*/
/**
* @addtogroup	grpGeneralApis
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spSystemOpen
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Initialize the system
*
*									This is the first API to be called by the application. Initializes all the modules and 
*									resources which will be used by the system in all the subsequent APIs.\n\n
*
*									This API will parse the Configuration file to read all the system and device specific configurations. 
*									The API will initialize communication with the Vending Expansion Card via configured channel 
*									in the configuration file. The API will also do the power on sequence for devices which are 
*									specified as "active" in configuration file. Once the device power on sequence is completed, the API will 
*									generate a list of device which user application can access via @ref spGetDevicesDetail API.\n\n
*
*									If this API is called by an application, it is mandatory to call @ref spSystemClose in order
*									to free the resources allocated by the API.
*
*	@date							10 Dec 2013
*	@author
*	@pre							None
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSystemOpen ();

/**
-----------------------------------------------------------------------------------------------------------------------
spSystemClose
-----------------------------------------------------------------------------------------------------------------------
*	@brief						De-initialize the system
*
*									This is the last API to be called by the application. De-initialize all the modules and 
*									resources which were initialized and allocated by the spSystemOpen API.
*									Application must call this API if @ref spSystemOpen is already been called.
*
*	@date							10 Dec 2013
*	@author
*	@pre							None
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSystemClose ();

/**
-----------------------------------------------------------------------------------------------------------------------
spSystemPinConfiguration
-----------------------------------------------------------------------------------------------------------------------
*	@brief						System Pin Configuration for Local Port
*
*									This API to be called after spSystemOpen() by the application.
*
*	@date							24 Feb 2016
*	@author
*	@pre							None
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSystemPinConfiguration();

/**
-----------------------------------------------------------------------------------------------------------------------
spSetSystemRTC
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Set PC time to Vending Expansion IO Board. User application
*									must set nMillisecond parameter of @ref SPRTC to 0.
*
*	@date							28 Oct 2014
*	@author
*	@pre							Application must call this API if @ref spSystemOpen is already been called. 
*
*	@param[in] pstructRTC	Struct holds the RTC time to set as system RTC time.
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSetSystemRTC (SPRTC& pstructRTC);

/**
-----------------------------------------------------------------------------------------------------------------------
spGetSystemRTC
-----------------------------------------------------------------------------------------------------------------------
*	@brief						To get current time of the system.
*
*	@date							28 Oct 2014
*	@author
*	@pre							Application must call this API if @ref spSystemOpen is already been called.
*
*	@param[out] pstructRTC	Struct holds the current RTC time returned by Vending Expansion IO Board.
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spGetSystemRTC (SPRTC& pstructRTC);

/**
-----------------------------------------------------------------------------------------------------------------------
spGetDevicesDetail
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get the details of devices present in the system (i.e. Machine configuration file)
*
*									Fills the application given array with the devices which are present in the system with their
*									types, power-on status etc. Refer @ref DeviceDetail for the information which these API provides.\n\n
*
*									Application can call other device specific APIs (i.e. spDevOpen, spDevClose etc) for the 
*									devices which are successfully powered on, application can use @ref DeviceDetail's 
*									m_bDeviceIsPowerdOn flag to check if device is successfully powered on or not.\n\n
*
*									All the device specific APIs will need device Id (m_nDeviceID parameter of @ref DeviceDetail 
*									structure) as an argument so that the HLAPI can identify particular device internally.
*
*	@date							10 Dec 2013
*	@author
*	@pre							System is initialized
*
*	@param[out] devicesList	Array of devices (of type @ref DeviceDetail) which are present in the system
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spGetDevicesDetail (SPDeviceList *devicesList);

/**
-----------------------------------------------------------------------------------------------------------------------
spGetVersion
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Read various version information
*
*									Fills application given strings with the firmware, software and HLAPI version information.
*									API will also fill size information in the respective size variables.
*
*	@date							10 Dec 2013
*	@author
*	@pre							System is initialized
*
*	@param[out] pstructFwVersion	Current firmware version
*	@param[out] pstructSoftVersion Current software version
*	@param[out] pstructHLAPIVersion Current HLAPI version
*
*	@return						Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spGetVersion (SPString *pstructFwVersion, SPString *pstructSoftVersion, SPString *pstructHLAPIVersion);
	
/**
-----------------------------------------------------------------------------------------------------------------------
spDevRegCallback
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Register device event notification callback for the device with specified device id
*
*									HLAPI will call the application given function when physical device generates some events, refer 
*									@ref SP_EVENT_NOTIFICATION for the details about the callback function prototype.
*
*									Different devices can generate various events and application should register the event notification 
*									handler in order to receive these events. HLAPI will ignore the events from the physical device
*									if corresponding event handler is not registered by the application. Refer @ref SP_EVENT_NOTIFICATION 
*									for the details about the events particular type of device can send.
*
*	@date							10 Dec 2013
*	@author
*	@pre							System is initialized and device with the id is active
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eventNotification Pointer to the function, which will be called by HLAPI when physical device generates the event
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevRegCallback (int32_t nDeviceID, SP_EVENT_NOTIFICATION eventNotification);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevOpen
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Open the device of specified device id
*
*									This is the first API that should be called by the application in order to use particular device
*
*	@date							10 Dec 2013
*	@author
*	@pre							System is initialized and device with the id is active
*
*	@param[in] nDeviceID		Id of the target device to be opened, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevOpen (int32_t nDeviceID);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevClose
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Close the device of specified device id
*
*									Successful call to the application will close the communication with the device, @ref spDevOpen
*									should be called in order to use the device again
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device to be closed, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevClose (int32_t nDeviceID);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevGetCap
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get the specified capability value of the specified device
*
*									Read the value of capability of Boolean type
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eCap				Id of the capability, whose value is to be read
*	@param[out] bCapValue		Capability value, to be filled by the API
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevGetCap (int32_t nDeviceID, SP_DEV_CAPABILITIES eCap, bool& bCapValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevGetPropertyB
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get the specified property value of the specified device
*
*									Read the value of the properties of Boolean type
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eProp			Id of the property whose value is to be read
*	@param[out] bPropertyValue	Property value, to be filled by the API
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevGetPropertyB (int32_t nDeviceID, SP_DEV_PROPERTIES eProp, bool &bPropertyValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevGetPropertyN
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get the specified property value of the specified device
*
*									Read the value of the properties of integer type
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eProp			Id of the property whose value is to be read
*	@param[out] nPropertyValue	Property value, to be filled by the API
*
*	@return	 				Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevGetPropertyN (int32_t nDeviceID, SP_DEV_PROPERTIES eProp, int32_t& nPropertyValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevGetPropertyLN
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get the specified property value of the specified device
*
*									Read the value of the properties of unsigned long integer type
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eProp			Id of the property whose value is to be read
*	@param[out] nPropertyValue	Property value, to be filled by the API
*
*	@return	 				Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevGetPropertyLN (int32_t nDeviceID, SP_DEV_PROPERTIES eProp, unsigned long & nPropertyValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevGetPropertyS
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get the specified property value of the specified device
*
*									Read the value of the properties of string type
*
*	@date							10 Dec 2013
*	@author 				
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eProp			Id of the property whose value is to be read
*	@param[out] pstructPropertyValue	Property value, to be filled by the API
*
*	@return	 				Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevGetPropertyS (int32_t nDeviceID, SP_DEV_PROPERTIES eProp, SPString *pstructPropertyValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevSetPropertyB
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Update the specified property value of the specified device
*
*									Update the value of the properties of Boolean type
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eProp			Id of the property whose value is to be updated
*	@param[in] bPropertyValue Property value, to be given by application

*	@return	 				Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevSetPropertyB (int32_t nDeviceID, SP_DEV_PROPERTIES eProp, bool bPropertyValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevSetPropertyN
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Update the specified property value of the specified device
*
*									Update the value of the properties of integer type
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eProp			Id of the property whose value is to be updated
*	@param[in] nPropertyValue Property value, to be given by application
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevSetPropertyN (int32_t nDeviceID, SP_DEV_PROPERTIES eProp, int32_t nPropertyValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevSetPropertyLN
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Update the specified property value of the specified device
*
*									Update the value of the properties of integer type
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eProp			Id of the property whose value is to be updated
*	@param[in] nPropertyValue Property value, to be given by application
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevSetPropertyLN (int32_t nDeviceID, SP_DEV_PROPERTIES eProp, unsigned long nPropertyValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spDevSetPropertyS
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Update the specified property value of the specified device
*
*									Update the value of the properties of string type
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eProp			Id of the property whose value is to be updated
*	@param[in] pstructPropertyValue Property value, to be given by application
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDevSetPropertyS (int32_t nDeviceID, SP_DEV_PROPERTIES eProp, SPString *pstructPropertyValue);
/**
* @}
*/

/**
* @defgroup		rgpCoinAcceptor Coin Acceptor
* Coin Acceptor device APIs allows user application to start/stop the cash acceptance, read the cash present in 
* the Coin Acceptor, do the initial setup etc. The device also provides APIs thru which application can read various 
* properties of the device like full/jam condition, currency code, physical device name etc. These APIs also allows user 
* application to read various device capabilities.
*
* Following is typical calling sequence of the Coin Acceptor APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen \n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevRegCallback \n
* @ref spDevSetPropertyB [Optional] \n
* @ref spDevSetPropertyN [Optional] \n
* @ref spCABeginDeposit \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spDevGetPropertyN [Optional] \n
* @ref spDevGetPropertyS [Optional] \n
* @ref spDevGetCap [Optional] \n
* @ref spCAFixDeposit \n
* @ref spCAEndDeposit \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/
/**
* @addtogroup	rgpCoinAcceptor
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spCABeginDeposit
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Start cash acceptance
*
*									API will initialize deposit amount (@ref SP_DEV_PROP_DEPOSIT_COUNTS) and deposit counts 
*									(@ref SP_DEV_PROP_DEPOSIT_AMOUNT) properties with zero. After calling this method, cash 
*									acceptance is reported by @ref SP_EVE_DATA until @ref spCAFixDeposit is called while the
*									deposit process is not paused.
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCABeginDeposit (int32_t nDeviceID);

/**
-----------------------------------------------------------------------------------------------------------------------
spCAReadCashCnt
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Read number of coins present in the Coin Acceptor
*
*									Application can call this API to read the coins present in the Coin Acceptor, cash count 
*									will be stored in a string as specified in the unified POS. Each unit in string matches
*									a unit in the cash list (@ref SP_DEV_PROP_DEPOSIT_CASH_LIST) property, and is in the same 
*									order.\n
*									For example, if currency is Euro and string is set to: 5:10,10:20,20:30,50:40,100:50,200:60 
*									then there would be 10 five cent coins, 20 ten cent coins, 30 twenty cent coins, 40 fifty cent coins,
*									50 one euro(hundred cent) coins and 60 two euro (two hundred cent) coins in the Coin Acceptor
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[out] pstructCashCntString	The cash count data, to be updated by the API
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCAReadCashCnt (int32_t nDeviceID, SPString *pstructCashCntString);

/**
-----------------------------------------------------------------------------------------------------------------------
spCAAdjustCashCnt
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Adjust initial amount of cash in the Coin Acceptor
*
*									Application can call this API with the cash amount with which initial setup will be done. 
*									The format of the cash count will be stored in a string as specified in the unified POS.\n
*
*									For example, if currency is Euro and string is set to: 5:10,10:20,20:30,50:40,100:50,200:60 
*									then  after successful execution of this API, there would be 10 five cent coins, 20 ten cent coins, 
*									30 twenty cent coins, 40 fifty cent coins, 50 one euro(hundred cent) coins and 60 two euro (two hundred cent) 
*									coins in the Coin Acceptor
*
*									Particular device's capability to perform this operation can be checked by reading the @ref SP_DEV_CAP_ADJUST_CASH_COUNT capability
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[out] pstructCashCntString	Device to be initialized with the amount specified by the string
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCAAdjustCashCnt (int32_t nDeviceID, SPString *pstructCashCntString);

/**
-----------------------------------------------------------------------------------------------------------------------
spCAFixDeposit
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Update the properties to reflect current values in the Coin Acceptor
*
*									After starting the cash acceptance, application can call this API before reading details 
*									about the cash present in the Coin Acceptor
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened and cash acceptance is started
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCAFixDeposit (int32_t nDeviceID);

/**
-----------------------------------------------------------------------------------------------------------------------
spCAPauseDeposit
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Suspend or resume the process of cash acceptance
*
*									Application can call the API to pause or resume the cash acceptance. Once paused by calling 
*									the API with bControl argument equal to true, cash acceptance will remain paused until the 
*									method is called again with bControl argument equal to false.
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened and cash acceptance is started
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] bControl		true/false to Suspend/Resume the operation respectively
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCAPauseDeposit (int32_t nDeviceID, bool bControl);

/**
-----------------------------------------------------------------------------------------------------------------------
spCAEndDeposit
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Stop the cash acceptance
*
*									Application can call this API to stop the cash acceptance operation. Before calling this method,
*									the application must calculate the difference between the amount of the deposit and the amount
*									required
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened and cash acceptance is started
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 				Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCAEndDeposit (int32_t nDeviceID);

/**
* @}
*/
	
/**
* @defgroup		grpCoinDispenser Coin Dispenser
* Coin dispenser device APIs will allow user application to dispense the cash, read the cash present in the coin 
* dispenser, do the initial setup etc. The device also provides APIs thru which application will be able to read 
* various properties of the device like full/jam condition, currency code, physical device name etc. These APIs 
* will also allow user application to read various capabilities.
*
* Following is typical calling sequence of the Coin Dispenser APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen \n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevRegCallback \n
* @ref spDevSetPropertyB [Optional] \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spDevGetCap [Optional] \n
* @ref spCDDispenseChange \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/
/**
* @addtogroup	grpCoinDispenser
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spCDReadCashCnt
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Read number of coins present in the Coin Dispenser
*
*									Application can call this API to read the coins present in the Coin Dispenser, cash count 
*									will be stored in a string as specified in the unified POS. Each unit in string matches
*									a unit in the cash list (@ref SP_DEV_PROP_DEPOSIT_CASH_LIST) property, and is in the same 
*									order.\n
*									For example, if currency is Euro and string is set to: 5:10,10:20,20:30,50:40,100:50,200:60 
*									then there would be 10 five cent coins, 20 ten cent coins, 30 twenty cent coins, 40 fifty cent coins,
*									50 one euro(hundred cent) coins and 60 two euro (two hundred cent) coins in the Coin Dispenser
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[out] pstructCashCntString	The cash count data, to be updated by the API
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCDReadCashCnt (int32_t nDeviceID, SPString *pstructCashCntString);

/**
-----------------------------------------------------------------------------------------------------------------------
spCDAdjustCashCnt
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Adjust initial amount of cash in the Coin Dispenser
*
*									Application can call this API with the cash amount with which initial setup will be done. 
*									The format of the cash count will be stored in a string as specified in the unified POS.\n
*									
*									For example, if currency is Euro and string is set to: 5:10,10:20,20:30,50:40,100:50,200:60 
*									then after successful execution of this API, there would be 10 five cent coins, 20 ten cent coins,
*									30 twenty cent coins, 40 fifty cent coins, 50 one euro(hundred cent) coins and 60 two euro (two hundred cent)
*									coins in the Coin Dispenser
*
*									Particular device's capability to perform this operation can be checked by reading the @ref SP_DEV_CAP_ADJUST_CASH_COUNT capability
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[out] pstructCashCntString	Device to be initialized with the amount specified by the string
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCDAdjustCashCnt (int32_t nDeviceID, SPString *pstructCashCntString);

/**
-----------------------------------------------------------------------------------------------------------------------
spCDDispenseChange
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Dispense change
*
*									Application can call the API to dispense cash amount specified
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nAmount		Amount to be dispensed
*	@param[out] nActualAmtPaid	Amount actually dispensed
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCDDispenseChange (int32_t nDeviceID, int32_t nAmount, int32_t &nActualAmtPaid);
	
/**
* @}
*/

/**
* @defgroup		grpBillAcceptor Bill Acceptor
* Bill Acceptor device APIs will allow user application to accept/validate bills, read the cash present in the Bill 
* Acceptor, do the initial setup etc. The device also provides APIs thru which application will be able to read various 
* properties of the device like full/jam condition, currency code, physical device name etc. These APIs will also allow 
* user application to read various capabilities.
*
* Following is typical calling sequence of the Bill Acceptor APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevRegCallback \n
* @ref spDevSetPropertyB [Optional] \n
* @ref spBABeginDeposit\n
* @ref spDevGetPropertyB [Optional] \n
* @ref spDevGetCap [Optional] \n
* @ref spBAFixDeposit\n
* @ref spBAEndDeposit\n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/
/**
* @addtogroup	grpBillAcceptor
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spBABeginDeposit
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Start cash acceptance
*
*									API will initialize deposit amount (@ref SP_DEV_PROP_DEPOSIT_COUNTS) and deposit counts 
*									(@ref SP_DEV_PROP_DEPOSIT_AMOUNT) properties with zero. After calling this method, cash 
*									acceptance is reported by @ref SP_EVE_DATA until @ref spBAFixDeposit is called while the
*									deposit process is not paused.
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spBABeginDeposit (int32_t nDeviceID);

/**
-----------------------------------------------------------------------------------------------------------------------
spBAReadCashCnt
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Read number of bills present in the Bill Acceptor
*
*									Application can call this API to read the bills present in the Bill Acceptor, cash count 
*									will be stored in a string as specified in the unified POS. Each unit in string matches
*									a unit in the cash list (@ref SP_DEV_PROP_DEPOSIT_CASH_LIST) property, and is in the same 
*									order.\n
*									For example, if currency is Euro and string is set to: 500:10,1000:20
*									then there would be 10 five euro(five hundred cent) bills, 20 ten euro(thousand cent) bills 
*									in the Bill Acceptor
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[out] pstructCashCntString	The cash count data, to be updated by the API
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spBAReadCashCnt (int32_t nDeviceID, SPString *pstructCashCntString);

/**
-----------------------------------------------------------------------------------------------------------------------
spBAAdjustCashCnt
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Adjust initial amount of cash in the Bill Acceptor
*
*									Application can call this API with the cash amount with which initial setup will be done. 
*									The format of the cash count will be stored in a string as specified in the unified POS.\n
*
*									For example, if currency is Euro and string is set to: 500:10,1000:20
*									then after successful execution of this API,there would be 10 five euro(five hundred cent) bills,
*									20 ten euro(thousand cent) bills in the Bill Acceptor.
*
*									Particular device's capability to perform this operation can be checked by reading the @ref SP_DEV_CAP_ADJUST_CASH_COUNT capability
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[out] pstructCashCntString	Device to be initialized with the amount specified by the string
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spBAAdjustCashCnt (int32_t nDeviceID, SPString *pstructCashCntString);

/**
-----------------------------------------------------------------------------------------------------------------------
spBAFixDeposit
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Update the properties to reflect current values in the Bill Acceptor
*
*									After starting the cash acceptance, application can call this API before reading details 
*									about the cash present in the Bill Acceptor
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened and cash acceptance is started
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spBAFixDeposit (int32_t nDeviceID);

/**
-----------------------------------------------------------------------------------------------------------------------
spBAPauseDeposit
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Suspend or resume the process of cash acceptance
*
*									Application can call the API to pause or resume the cash acceptance. Once paused by calling 
*									the API with bControl argument equal to true, cash acceptance will remain paused until the 
*									method is called again with bControl argument equal to false.
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened and cash acceptance is started
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] bControl		true/false to Suspend/Resume the operation respectively
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spBAPauseDeposit (int32_t nDeviceID, bool bControl);

/**
-----------------------------------------------------------------------------------------------------------------------
spBAEndDeposit
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Stop the cash acceptance
*
*									Application can call this API to stop the cash acceptance operation. Before calling this method,
*									the application must calculate the difference between the amount of the deposit and the amount
*									required
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened and cash acceptance is started
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spBAEndDeposit (int32_t nDeviceID);

/**
* @}
*/

/**
* @defgroup		grpBillDispenser Bill Dispenser
* Bill dispenser device APIs will allow user application to dispense the cash, read the cash present in the Bill 
* dispenser, do the initial setup etc. The device also provides APIs thru which application will be able to read 
* various properties of the device like full/jam condition, currency code, physical device name etc. These APIs 
* will also allow user application to read various capabilities.
*
* Following is typical calling sequence of the Coin Dispenser APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen \n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevRegCallback \n
* @ref spDevSetPropertyB [Optional] \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spDevGetCap [Optional] \n
* @ref spBDDispenseChange \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/
/**
* @addtogroup	grpBillDispenser
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spBDReadCashCnt
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Read number of coins present in the Bill Dispenser
*
*									Application can call this API to read the bills present in the Bill Dispenser, cash count 
*									will be stored in a string as specified in the unified POS. Each unit in string matches
*									a unit in the cash list (@ref SP_DEV_PROP_DEPOSIT_CASH_LIST) property, and is in the same 
*									order.\n
*									For example, if currency is Euro and string is set to: 500:10,1000:20
*									then there would be 10 five euro(five hundred cent) bills, 20 ten euro(thousand cent) bills 
*									in the Bill Dispenser
*
*	@date							09 Dec 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[out] pstructCashCntString	The cash count data, to be updated by the API
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spBDReadCashCnt (int32_t nDeviceID, SPString *pstructCashCntString);

/**
-----------------------------------------------------------------------------------------------------------------------
spBDAdjustCashCnt
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Adjust initial amount of cash in the Bill Dispenser
*
*									Application can call this API with the cash amount with which initial setup will be done. 
*									The format of the cash count will be stored in a string as specified in the unified POS.\n
*									
*									For example, if currency is Euro and string is set to: 500:10,1000:20
*									then after successful execution of this API,there would be 10 five euro(five hundred cent) bills,
*									20 ten euro(thousand cent) bills in the Bill Acceptor.
*
*									Particular device's capability to perform this operation can be checked by reading the @ref SP_DEV_CAP_ADJUST_CASH_COUNT capability
*
*	@date							09 Dec 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[out] pstructCashCntString	Device to be initialized with the amount specified by the string
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spBDAdjustCashCnt (int32_t nDeviceID, SPString *pstructCashCntString);

/**
-----------------------------------------------------------------------------------------------------------------------
spBDDispenseChange
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Dispense change
*
*									Application can call the API to dispense cash amount specified
*
*	@date							09 Dec 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nAmount		Amount to be dispensed
*	@param[out] nActualAmtPaid	Amount actually dispensed
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spBDDispenseChange (int32_t nDeviceID, int32_t nAmount, int32_t &nActualAmtPaid);
	
/**
* @}
*/

/**
* @defgroup		grpItemDispenser Item Dispenser
* Item Dispenser device APIs will allow user application to dispense item(s) specified by item number and item count 
*
* Following is typical calling sequence of the Bill Acceptor APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevRegCallback \n
* @ref spDevSetPropertyB [Optional] \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spDevGetCap [Optional] \n
* @ref spIDDispenseItem\n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/

/**
* @addtogroup	grpItemDispenser
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spIDDispenseItem
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Dispense item
*
*	@date							4 Apr 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nItemCount	Number of items to be dispensed
*	@param[in] nItem			Item number to be dispensed.
*	@param[out] nActualItemDispensed	Items actually dispensed
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spIDDispenseItem (int32_t nDeviceID, int32_t nItemCount, int32_t nItem, int32_t &nActualItemDispensed);

/**
* @}
*/

/**
* @defgroup		grpLights Lights
* This API will allow application to control the Lights
*	
* Following is typical calling sequence of the Light APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spDevGetCap [Optional] \n
* @ref spLightsSwitchOnOff\n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/
/**
* @addtogroup	grpLights
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spLightsSwitchOnOff
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Control lights
*
*									Application can call the API to control the Lights. Basic usage of the API is to switch on/off
*									the Light. API also allows application to change blink on/off cycle, color etc.
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nEnable		Switch on/Switch off the light
*	@param[in] nBlinkOnCycle A zero (0) value indicates no blink cycle. A positive value indicates the blink on cycle 
*									time in milliseconds. Negative values are not allowed
*	@param[in] nBlinkOffCycle A zero (0) value indicates no blink cycle. A positive value indicates the blink off cycle 
*									time in milliseconds. Negative values are not allowed
*	@param[in] nColor			Specifies the color of the light, must be one of the colors defined by CapColor (TODO)
*	@param[in] nAlarm			Specifies the used alarm type, must be one of the alarms defined by CapAlarm (TODO)
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/

DLL_EXPORT SP_STATUS_CODE spLightsSwitchOnOff (int32_t nDeviceID, bool nEnable, int32_t nBlinkOnCycle, int32_t nBlinkOffCycle, 
																int32_t nColor, int32_t nAlarm);
	
/**
* @}
*/

/**
* @defgroup		grpPowerManager Power Manager
* This module lists all the APIs to interact with the Power Manager
*	
* Following is typical calling sequence of the Power Manager APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spPMChangeMode\n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/
/**
* @addtogroup	grpPowerManager
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spPMChangeMode
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Change Power Manager mode
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nPowerMode	Switch to this Power Manager mode (@ref SP_POWER_STATE)
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spPMChangeMode (int32_t nDeviceID, SP_POWER_STATE nPowerMode);

/**
* @}
*/

/**
* @defgroup		grpSensor Sensors
* This module lists all the APIs to interact with Sensors present in the system.\n\n
*
* The HLAPI supports following type of sensors,\n
* -# <b> PWM based FAN sensor : </b> Application can use the sensor APIs to set and get the speed of the FAN.\n
* -# <b> I2C/GPIO based generic sensor : </b> Variety of generic sensors with I2C/GPIO interface can be connected with the Sunset Pass module. 
*			Since variety of I2C and GPIO based sensors can be connected to Sunset Pass module, application can read value of @ref SP_DEV_PROP_GEN_SENSOR_TYPE 
*			property to get the type of the sensor which is connected on the hardware. HLAPI will return the sensor type string directly copied from the configuration 
*			XML file. If applicable, application can read @ref SP_DEV_PROP_GEN_SENSOR_UNIT property to get the unit of the sensor, this unit string 
*			will be copied from the XML as well.\n
*			The generic sensors will not generate any kind of events.
*
* The generic sensors are read only sensors so @ref spSensorSetValue API is not supported for these kind of sensors. For I2C based sensors application needs to 
* call @ref spSensorGetValueA API to read the value from the sensor. The API fills the application given buffer array with the data 
* received from the device. HLAPI will perform logical AND operation with the received data bytes based on the mask provided in XML file, before filling 
* the appilcation given array. HLAPI is blind to the data coming from the I2C sensor and application should have the knowledge about each received byte.\n\n
*
* For GPIO based sensor application needs to call @ref spSensorGetValue API.\n\n
*
* Following is typical calling sequence of the Sensor APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevGetPropertyS [Optional] \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spSensorSetValue\n
* @ref spSensorGetValue or @ref spSensorGetValueA\n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/
/**
* @addtogroup	grpSensor
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spSensorSetValue
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Set Sensor value
*
*									Application can call this API to set the Sensor value. In this API,HLAPI will perform write
*									sequence mentioned in configuration file.
*
*	@date							19 Jun 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorSetValue (int32_t nDeviceID);

/**
-----------------------------------------------------------------------------------------------------------------------
spSensorSetValueN
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Set Sensor value
*
*									Application can call this API to set the Sensor value
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nValue			Value to be set
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorSetValueN (int32_t nDeviceID, int32_t nValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spSensorSetValueNGPIO
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Set Sensor value
*
*									Application can call this API to set the Sensor value over USB relay
*
*	@date							25 May 2015
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nValue			Value to be set
*	@param[in] nGPIONumber		GPIO Pin Number
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorSetValueNGPIO(int32_t nDeviceID, int32_t nValue, int32_t nGPIONumber);

/**
-----------------------------------------------------------------------------------------------------------------------
spSensorGetValue
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get Sensor value
*
*									Application can call this API to get the Sensor value
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID				Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] bIsTimeStampReq		Timestamp required or not flag. 
*											True - Timestamp will be returned in argument passed.
*											False - Timestamp will not be returned in argument passed.
*	@param[out] pstructRTC			Struct to hold system time returned.
*	@param[out] nValue				Value to be updated by the API
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorGetValue (int32_t nDeviceID, bool bIsTimeStampReq, SPRTC& pstructRTC, int32_t& nValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spSensorGetValueF
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get Sensor value
*
*									Application can call this API to get the Sensor value
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID			Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] bIsTimeStampReq		Timestamp required or not flag. 
*									True - Timestamp will be returned in argument passed.
*									False - Timestamp will not be returned in argument passed.
*	@param[out] pstructRTC			Struct to hold system time returned.
*	@param[out] nValue				Value to be updated by the API
*
*	@return	 						Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorGetValueF(int32_t nDeviceID, bool bIsTimeStampReq, SPRTC& pstructRTC, float& nValue);

/**
-----------------------------------------------------------------------------------------------------------------------
spSensorGetValueA
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get Sensor value
*
*									Application can call this API to get the response bytes from the Generic I2C sensor\n
*									This API will return an array containing response of I2C sensor.
*
*	@date							04 April 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID				Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] bIsTimeStampReq		Timestamp required or not flag. 
*											True - Timestamp will be returned in argument passed.
*											False - Timestamp will not be returned in argument passed.
*	@param[out] pstructRTC			Struct to hold system time returned.
*	@param[out] pStructDataBuff	Buffer to be updated by the API
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorGetValueA (int32_t nDeviceID, bool bIsTimeStampReq, SPRTC& pstructRTC, SPDataBuff& pStructDataBuff);

/**
-----------------------------------------------------------------------------------------------------------------------
spSensorGetSMBusAlert
-----------------------------------------------------------------------------------------------------------------------
*	@brief							Get Alert asserted SMBus Slave Address 
*
*									Application can call this API to get slave address of all Alert asserted SMBus device \n
*									This API will return an array containing slave address of all Alert asserted SMBus device.
*
*	@date							22 February 2016
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID			Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@param[out] pStructDataBuff		Buffer to be updated by the API
*
*	@return	 						Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorGetSMBusAlert(int32_t nDeviceID, SPDataBuff& pStructDataBuff);

/**
-----------------------------------------------------------------------------------------------------------------------
spSensorSetSMBusAlertState
-----------------------------------------------------------------------------------------------------------------------
*	@brief							Start - Stop SMBus Alert
*
*									Application can call this API to start or stop SMBus Alert \n
*
*	@date							22 February 2016
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID			Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@param[in] bEnableSMBusAlert	true - Enable SMBus Alert, false - Disable SMBus Alert
*
*	@return	 						Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorSetSMBusAlertState(int32_t nDeviceID, bool bEnableSMBusAlert);
/**
-----------------------------------------------------------------------------------------------------------------------
spSensorGetAddress
-----------------------------------------------------------------------------------------------------------------------
*	@brief							Get I2C to One Wire Sensor Address
**									Application can call this API to get the response bytes from the Generic I2C sensor\n
*									This API will return an array containing response of I2C sensor.
*
*	@date							05 March 2016
*   @author
*	@pre							Device is opened
*
*	@param[in] nDeviceID				Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] bIsTimeStampReq		Timestamp required or not flag.
*											True - Timestamp will be returned in argument passed.
*											False - Timestamp will not be returned in argument passed.
*	@param[out] pstructRTC			Struct to hold system time returned.
*	@param[out] pStructDataBuff	Buffer to be updated by the API
*
*   @return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorGetAddress(int32_t nDeviceID, bool bIsTimeStampReq, SPRTC& pstructRTC, SPDataBuff& pStructDataBuff);

/**
-----------------------------------------------------------------------------------------------------------------------
spSensorGetConvertedValue
-----------------------------------------------------------------------------------------------------------------------
*	@brief							Get converted value of sensor reading using linear conversion formula ax+b
*
*	@date							07 March 2016
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID			Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@param[in] pStructDataBuff		Buffer to pass sensor reading values with length. Max length should be 4(sizeof int).
*
*	@param[out] fConvertedValue		Converted value of sensor reading
*
*	@param[in] isLittleEndian		Endianess of pStructDataBuff.pcData data buffer value.
*									true - little endian, false - big endian.
*
*	@return	 						Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSensorGetConvertedValue(int32_t nDeviceID, SPDataBuff& pStructDataBuff, float& fConvertedValue, bool isLittleEndian);
/**
* @}
*/

/**
* @defgroup		grpDex Dex
* The DEX APIs will allow user application to read configuration data or to send audit data to the Data Carrier (DC) 
* device. An API supports functionality to receive a callback when DEX device is connected to a DEX port. 
* 
* HLAPI itself handles the handshaking as per the DEX protocol. Whenever device is connected to the VMC HLAPI will perform
* the first handshake and it will also send appropriate @ref SP_EVE_STATUS event to the user application. The second 
* and third handshake sequences will be carried out as part of the @ref spDexGetConfigData and @ref spDexSendAuditData APIs.
*
* Following is typical calling sequence of the DEX device APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spDevRegCallback \n
* @ref spDexGetConfigData or @ref spDexSendAuditData \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/
/**
* @addtogroup	grpDex
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spDexGetConfigData
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Get the configuration data from Data Carrier device
*
*									Application can call this API to get the configuration data from the Data Carrier. The API 
*									does not validate configuration data from Data Carrier. It will just pass the data to user 
*									application
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened and @ref SP_SE_DEX_READ_CONFIG_DATA status event is sent to application
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[out] pcConfigData Application given buffer which will be filled by configuration data received from DC
*	@param[out] nConfigDataSize Size of buffer written in pcConfigData
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDexGetConfigData (int32_t nDeviceID, char *pcConfigData, int32_t& nConfigDataSize);

/**
-----------------------------------------------------------------------------------------------------------------------
spDexSendAuditData
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Send audit data to DC device
*
*									Application can call this API to send the audit data to the DC device block by block based
*									on the Block size. This API is not responsible to validate format of audit data. It is calling 
*									application's responsibility to provide audit data as per format specified by DEX protocol.
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened and @ref SP_SE_DEX_SEND_AUDIT_DATA  status event is sent to application
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] pcAuditData	Application given audit data
*	@param[in] nAuditDataSize Size of the audit data
*	@param[in] nBlockSize	Block size
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spDexSendAuditData (int32_t nDeviceID, char *pcAuditData, int32_t nAuditDataSize, int32_t nBlockSize);

/**
* @}
*/

/**
* @defgroup		grpSSD Seven Segment Display
* The Seven segment display APIs will allow user application to display number provided from user application on the 
* seven segment display. Vending Expansion IO board supports 4 seven segment displays. User application can display 
* different numbers such as 0001, 12.13, 1.234, 1234 etc. Four digit positive integer or float number is supported.
*
* Following is typical calling sequence of the Seven Segment Display APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spSSDDisplay \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*/
/**
* @addtogroup	grpSSD
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spSSDDisplay
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Display string on the seven segment display
*
*									Application can call this API to display the specified string on the Seven Segment Display.
*									For example, if API is called with the string "1.115" then the Seven Segment Display would
*									display 1.115
*
*	@date							10 Dec 2013
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] pstructDispString string to be displayed on seven segment display
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spSSDDisplay (int32_t nDeviceID, SPString *pstructDispString);

/**
* @}
*/

/**
* @defgroup		grpMotionSensor Motion Sensor
* The Motion Sensor APIs will allow user application to interact with the Motion sensor device. The motion sensor only 
* generates notification events in case of motion thru the Event notification interface.
*
* Following is typical calling sequence of the Motion Sensor APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevGetPropertyB [Optional] \n
* @ref spDevRegCallback \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*
*/
/**
* @addtogroup	grpMotionSensor
* @{
*/

/**
* @}
*/

/**
* @defgroup		grpCashlessPayment Cashless Payment
* The Cashless Payment APIs will allow user application to interact with the Cashless Payment device. Application can 
* perform cash less transaction by using these APIs\n\n
*
* Following is typical calling sequence of the Cashless Payment APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevRegCallback \n
* @ref spDevSetPropertyLN with @ref SP_DEV_PROP_CP_AMOUNT \n
* @ref spDevSetPropertyN with @ref SP_DEV_PROP_CP_CURRENCY_CODE \n 
* @ref spDevGetPropertyS with @ref SP_DEV_PROP_CP_SERIAL_NUMBER [Optional] \n
* @ref spDevGetPropertyLN with @ref SP_DEV_PROP_CP_PRODUCT_NUMBER [Optional] \n
* @ref spCPDoTransaction \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*
*/

/**
* @addtogroup	grpCashlessPayment
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spCPDoTransaction
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Do transaction
*									
*									Application can call this API to send payment request to cashless payment device
*									and make payment based on operation type @ref SP_CP_OPERATION_REQ.
*									Application needs to set the amount of the transaction by setting the @ref SP_DEV_PROP_CP_AMOUNT 
*									property using @ref spDevSetPropertyN API
*
*	@date							07 April 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] eOperation	Type of transaction, one out of @ref SP_CP_OPERATION_REQ
*	@param[in] pstructUserData	Buffer to be given by the application
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spCPDoTransaction(int32_t nDeviceID, SP_CP_OPERATION_REQ eOperation, SPString *pstructUserData);

/**
* @}
*/

/**
* @defgroup		grpLineDisplay Line Display
* The Line Display APIs will allow user application to interact with the line display device. Application can display or 
* clear messages and can set backlight of the device.\n\n
*
* Following is typical calling sequence of the Line Display APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevRegCallback \n
* @ref spLDChangeBackLight \n
* @ref spLDDisplayMessage \n
* @ref spLDClearMessage \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*
*/

/**
* @addtogroup	grpLineDisplay
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spLDChangeBackLight
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Change the backlight level
*
*									Application can call this API to set percentage backlight level
*
*	@date							07 April 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nBackLightLevel	Percentage backlight level
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spLDChangeBackLight(int32_t nDeviceID, int32_t nBackLightLevel);

/**
-----------------------------------------------------------------------------------------------------------------------
spLDDisplayMessage
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Display messages
*
*									Application can call this API to display string on the Line Display
*
*	@date							07 April 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID			Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nAddress				Shell number from where to start displaying message
*	@param[in] pstructMsgString		Message to be displayed on device
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spLDDisplayMessage (int32_t nDeviceID, int32_t nAddress,SPString *pstructMsgString);

/**
-----------------------------------------------------------------------------------------------------------------------
spLDClearMessage
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Clear messages
*
*									Application can call this API to clear message from the line display
*
*	@date							07 April 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID			Id of the target device, the target device's Id can be found from @ref DeviceDetail
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spLDClearMessage (int32_t nDeviceID);

/**
* @}
*/

/**
* @defgroup		grpKeyLock KeyLock
* The KeyLock API will allow user application to interact with the GPO based Keylock device. Application can set 
* positions ,i.e.mentioned in configuration file , of the device.\n\n
*
* Following is typical calling sequence of the KeyLock API, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevRegCallback \n
* @ref spKLSetPosition \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*
*/

/**
* @addtogroup	grpKeyLock
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spKLSetPosition
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Set the position
*
*									Application can call this API to set position of device. HLAPI will parse configuration file
*									to get position details and set mentioned GPO for that position.
*
*	@date							08 July 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nPos			Position specified in configuration file.
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spKLSetPosition(int32_t nDeviceID, int32_t nPos);

/**
* @}
*/

/**
* @defgroup		grpSpecialAPIs SpecialAPIs
* The Special APIs will allow user application to interact with the interface on which respective device is connected. Application can execute 
* commands by command id, i.e.mentioned in configuration file , on interface and parse response of that.\n\n
*
* Following is typical calling sequence of the Special APIs, \n
* @ref spSystemOpen \n
* @ref spGetDevicesDetail \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spDevOpen\n
* @ref spDevSetPropertyB with @ref SP_DEV_PROP_DEVICE_ENABLED \n
* @ref spDevRegCallback \n
* @ref spExecSpecialMDBCommand \n
* @ref spDevClose \n
* <b>....</b>\n
* <b>....</b>\n
* @ref spSystemClose
*
*/

/**
* @addtogroup	grpSpecialAPIs
* @{
*/

/**
-----------------------------------------------------------------------------------------------------------------------
spExecSpecialMDBCommand
-----------------------------------------------------------------------------------------------------------------------
*	@brief						Execute special MDB command
*
*									Application can call this API to execute MDB command of device. HLAPI will parse configuration 
*									file to get Command information from command id passed as an argument and then execute that 
*									command. This API will return response of this command in to buffer passed as an argument.
*
*	@date							29 July 2014
*	@author
*	@pre							Device is opened
*
*	@param[in] nDeviceID		Id of the target device, the target device's Id can be found from @ref DeviceDetail
*	@param[in] nCmdId			Id of command defined in configuration file.
*	@param[out] structResponseBuff	Response of command execution.
*
*	@return	 					Refer this Enum @ref SP_STATUS_CODE
*
************************************************************************************************************************
*/
DLL_EXPORT SP_STATUS_CODE spExecSpecialMDBCommand(int32_t nDeviceID, int32_t nCmdId, SPDataBuff &structResponseBuff);

/**
* @}
*/

}
#endif  // _SP_INTERFACE_H
