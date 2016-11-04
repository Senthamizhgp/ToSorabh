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
*	@file		DexConverter.cpp
*
*	@brief
*		To converts sales, stock and event information to DEX file
*		It performs following actions:
*			- Check for DEX processing unit is enabled
*			- If enabled converts sales, stock and event information to DEX file format
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Intel Smart Shelf
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

/* System headers */

/* Own headers */
#include "DexConverter.h"
#include <time.h>

#ifdef _WIN32
	#define STRCPY(dest, src) strcpy_s(dest, src.size() + 1, src.c_str());	
#elif __linux__
	#define STRCPY(dest, src) strncpy(dest, src.c_str(), src.size() + 1);
#endif

using namespace std;

/* Global and Static initializations */
DexConverter* DexConverter::instance = NULL;
const char* DEXCONFIGURATION_XML_PATH = "./DEXConfiguration.xml";
std::map<std::string, std::map<std::string, std::string>>m_DEXConfigParams;
string m_DEXBlockIdentifier;
const xmlChar* m_DEXDataElement;

#define CRLF "\n"
#define VMFUNCTIONIDENTIFIER "VA"
#define VMTRANSMISSIONNO "1"
#define TRANSACTIONSETHDR "001"
#define TRANSACTIONSETCTRLNO "0001"
#define BILLVALIDATOR "Bill_Validator"
#define COINMECHANISM "Coin_Mechanism"
#define DEFAULTVALUE "0"

/**
-------------------------------------------------------------------------------
DexConverter
-------------------------------------------------------------------------------
*   @brief		Constructor initializing the member variables and required inits.
*
*	@param		none
*
*	@return		None
*
*/
DexConverter::DexConverter() :
	crc(0),
	CRCqstringtostr(""),
	delimiter("*"),
	dexBuffer(""),
	CRCBuffer(""),
	Tofile(""),
	RecentSaleTimeStamp(0),
	LastSaleProductSelectionCnt(0),
	CoinsToTubes(0),
	CoinsInCashBox(0),
	BillsInCashBox(0),
	CountLine(0),
	PaidVendCount(0),
	FreeVendCount(0),
	TestVendCount(0),
	PaidVendCountRefill(0),
	FreeVendCountRefill(0),
	TestVendCountRefill(0),
	TCardSalesCountInit(0),
	TCashSalesCountInit(0),
	TCoinSalesCountInit(0),
	TCoinSalesCountRefill(0),
	TCardSalesCountRefill(0),
	TCashSalesCountRefill(0),
	RefillTimeUTC(0),
	ReadCount(0),
	AuditTableCoinsInCashBoxInit(0),
	AuditTableCoinsToTubesInit(0),
	AuditTableCashSalesInit(0),
	AuditTableCardSalesInit(0),
	PaidVendValueRefill(0),
	FreeVendValueRefill(0),
	TestVendValueRefill(0),
	PaidVendValueInit(0),
	FreeVendValueInit(0),
	TestVendValueInit(0),
	TotalBillsValueInit(0),
	TotalCoinsValueInit(0),
	TotalCashlessValueInit(0),
	TotalCoinsValueRefill(0),
	TotalCashlessValueRefill(0),	
	TotalBillsValueRefill(0),
	TotalTokenValueInit(0),
	TTokenSalesCountInit(0),
	TTokenSalesCountRefill(0),
	TotalTokenValueRefill(0)
{
	// Perform all the initialization
	// Initialized only at the first invocation of ctor
	static bool initFlag = false;

	// True only for the first instance of DataAcquisition
	if (true != initFlag)
	{
		DEXConfigParametersXML();

		// To ensure the getConnection is called only once
		initFlag = true;
	}
}


/**
-------------------------------------------------------------------------------
~DexConverter
-------------------------------------------------------------------------------
*   @brief		Destructor performing the cleanup like deleting allocated memory, dereferencing etc..
*
*	@param		none
*
*	@return		None
*
*/
DexConverter::~DexConverter()
{
	// Define all the code cleanup and memory delete
}

/**
------------------------------------------------------------------------------ -
getInstance
------------------------------------------------------------------------------ -
*	@brief		To obtain the singleton instance
*
*	@param		none
*
*	@return		Instance of the DexConverter
*
*/
DexConverter* DexConverter::getInstance()
{
	static DexConverter dexInstance;
	return &dexInstance;
}

/**
-------------------------------------------------------------------------------
DexFileGenerater
-------------------------------------------------------------------------------
*   @brief		To parse JSON string and to convert converts sales, stock and event information to DEX file format
*
*	@param		none
*
*	@return		None
*
*/
QString DexConverter::DexFileGenerater(string jsondata)
{
	dexBuffer.clear();	
	QString JsonData1 = jsondata.c_str();
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	QJsonObject jsonObject = JsonResponse.object();

	// Process the sales and stock info to calculate DEX data
	ReadvminfoObject(jsonObject);
	ReadVmDeviceInfo(jsonObject);
	ReadCashInInfo(jsonObject);
	ReadAuditInfo(jsonObject);
	ReadCoinMechanismInfo(jsonObject);

	ReadeventinfoObject(jsonObject);
	NoofReads();
	ReadproductvendinfoObject(jsonObject);		
	All_Transaction_Summary(jsondata);
	DexVendsales(jsonObject);
	DexTokensales();
	DexCashInflow();
	DexCashOutflow();
	BlockIdentifierLS();
	Dexvalueintubes();
	DexCashlesssalesinfo();
	RecentSale();
	DexProductinfosales(jsonObject);
	BlockIdentifierLE();
	PriceList();
	ReadConfigurationData(jsonObject);
	RecordIntegrityCheck();
	NoofIncludedSets();
	DexTrailer();

	Clear_Memory();
	//WriteToFile(); /// For Debug purpose
	CRCBuffer = "";

	return dexBuffer;
}

/**
-------------------------------------------------------------------------------
BlockIdentifierLS
-------------------------------------------------------------------------------
*   @brief		Block Identifier LS
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::BlockIdentifierLS()
{
	Tofile = "LS" + delimiter + "0100\n";
	CountLine++;
	CRCBuffer += "0100";
	dexBuffer.append(Tofile);
}

/**
-------------------------------------------------------------------------------
BlockIdentifierLE
-------------------------------------------------------------------------------
*   @brief		Block Identifier LE
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::BlockIdentifierLE()
{
	Tofile = "LE" + delimiter + "0100\n";
	CountLine++;
	CRCBuffer += "0100";
	dexBuffer.append(Tofile);
}

/**
-------------------------------------------------------------------------------
NoofReads
-------------------------------------------------------------------------------
*   @brief		Block Identifier for No of Reads
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::NoofReads()
{
	QString EA301;
	if (std::stoi(m_DEXConfigParams["EA3"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["EA301"]["enable"]))
		{
			EA301 = QString::number(ReadCount);
		}
		Tofile = "EA3" + delimiter + EA301 + CRLF;
		CountLine++;
		CRCBuffer += EA301;
		dexBuffer.append(Tofile);
	}
}

/**
-------------------------------------------------------------------------------
RecentSale
-------------------------------------------------------------------------------
*   @brief		Block Identifier PA5
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::RecentSale()
{
	QString RSaleDate;
	QString RSaleTime;
	QString PA5_SaleDate;
	QString PA5_RSaleTime;
	QString PA5_LastSaleSelectionCnt;

	if (std::stoi(m_DEXConfigParams["PA5"]["enable"]))
	{		
		getUTCTime(RecentSaleTimeStamp, RSaleDate, RSaleTime);
		if (std::stoi(m_DEXConfigParams["PA501"]["enable"]))
		{
			PA5_SaleDate = RSaleDate;
		}
		if (std::stoi(m_DEXConfigParams["PA502"]["enable"]))
		{
			PA5_RSaleTime = RSaleTime;
		}
		if (std::stoi(m_DEXConfigParams["PA503"]["enable"]))
		{
			PA5_LastSaleSelectionCnt = QString::number(LastSaleProductSelectionCnt);
		}			
	}
	
	Tofile = "PA5" + delimiter + PA5_SaleDate + delimiter + PA5_RSaleTime + delimiter 
		+ PA5_LastSaleSelectionCnt + CRLF;
	CountLine++;
	CRCBuffer += PA5_SaleDate + PA5_RSaleTime;
	dexBuffer.append(Tofile);
}

/**
-------------------------------------------------------------------------------
WriteToFile
-------------------------------------------------------------------------------
*   @brief		To print and to write DEX content in a file (For debugging purpose only)
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::WriteToFile()
{
	fstream Dexfile;
	Dexfile.open("D:/Intel/Dex_Format.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	if (!Dexfile)
		cout << "error opening file for writing." << endl;
	//	std::cout << " Current Time :: " << currentDateTime() << std::endl;
	//	Dexfile << "#####   Current Time ::  " << currentDateTime() << "     ######" << endl << endl;
	std::cout << "------------   DEX report !! START !! --------------  " << endl << dexBuffer.toUtf8().constData() << endl;
	std::cout << "------------   DEX report !! END !! ----------------  " << endl << endl;
	Dexfile << dexBuffer.toUtf8().constData();
	Dexfile.close();
}

/**
-------------------------------------------------------------------------------
All_Transaction_Summary
-------------------------------------------------------------------------------
*   @brief		To process product_vend_info for transaction
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::All_Transaction_Summary(string jsondata)
{
	int PVendType = 0;
	int PVTimeUTC = 0;
	int PPrice = 0;
	int ProductSold = 0;
	int PPaymentSource = 0;
	QString LastSaleProductId;

	QString JsonData1 = jsondata.c_str();
	QMap <QString, QVariant> innermap;
	QList<QMap <QString, QVariant>> listofmap;
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	QJsonObject jsonObject = JsonResponse.object();
	QJsonValue ProductVend = jsonObject.value(QString("product_vend_info"));
	if (ProductVend.isArray())
	{
		QJsonArray ProductVendArray = jsonObject["product_vend_info"].toArray();
		foreach(const QJsonValue & value, ProductVendArray)
		{
			QJsonObject Productobj = value.toObject();
			PVendType = Productobj["VendType"].toInt();
			PVTimeUTC = Productobj["Timestamp"].toInt();
			PPrice = Productobj["SalePrice"].toInt();
			ProductSold = Productobj["ProductSold"].toInt();
			PPaymentSource = Productobj["PaymentSource"].toInt();
			RecentSaleTimeStamp = PVTimeUTC;
			LastSaleProductId = Productobj["Product_ID"].toString();

			// Vend type
			switch (PVendType)
			{
			case 0:			// Paid vend
				PaidVendCount += 1;
				PaidVendValueInit += (PPrice*ProductSold);
				break;
			case 1:			// Free vend
				FreeVendCount += 1;
				FreeVendValueInit += (PPrice*ProductSold);
				break;
			case 2:			// Test vend
				TestVendCount += 1;
				TestVendValueInit += (PPrice*ProductSold);
				break;
			default:
				break;
			}
			//cash flow manipulation based on paymentsource
			switch (PPaymentSource)
			{
			case 0:			// coin
				TCoinSalesCountInit += 1;
				TotalCoinsValueInit += (PPrice*ProductSold);
				break;
			case 1:			// cashless i.e card
				TCardSalesCountInit += 1;
				TotalCashlessValueInit += (PPrice*ProductSold);
				break;
			case 2:			// cash
				TCashSalesCountInit += 1;
				TotalBillsValueInit += (PPrice*ProductSold);
				break;
			case 3:			// Token vend
				TTokenSalesCountInit += 1;
				TotalTokenValueInit += (PPrice*ProductSold);
				break;
			default:
				break;
			}
			// Cash flow calculation after Refill
			if (PVTimeUTC >= RefillTimeUTC)
			{
				// Vend type after refill
				switch (PVendType)
				{
				case 0:			// Paid vend ; 
					PaidVendCountRefill += 1;
					PaidVendValueRefill += (PPrice*ProductSold);

					ProductinfoIter1 = ProductMap1.find(LastSaleProductId.toStdString());
					if (ProductinfoIter1 != ProductMap1.end())
					{
						int PaidSalesCntAfterRefill = 0;
						PaidSalesCntAfterRefill = ProductMap1[LastSaleProductId.toStdString()]["PaidSalesCntAfterRefill"].toInt();
						PaidSalesCntAfterRefill += ProductSold;
						ProductMap1[LastSaleProductId.toStdString()]["PaidSalesCntAfterRefill"] = PaidSalesCntAfterRefill;
					}
					break;
				case 1:			// Free vend
					FreeVendCountRefill += 1;
					FreeVendValueRefill += (PPrice*ProductSold);
					break;
				case 2:			// Test vend
					TestVendCountRefill += 1;
					TestVendValueRefill += (PPrice*ProductSold);
					break;
				default:
					break;
				}
				//cash flow manipulation based on paymentsource after refill
				switch (PPaymentSource)
				{
				case 0:			// coin
					TCoinSalesCountRefill += 1;
					TotalCoinsValueRefill += (PPrice*ProductSold);
					break;
				case 1:			// cashless i.e card
					TCardSalesCountRefill += 1;
					TotalCashlessValueRefill += (PPrice*ProductSold);
					break;
				case 2:			// cash
					TCashSalesCountRefill += 1;
					TotalBillsValueRefill += (PPrice*ProductSold);
					break;
				case 3:			// Token vend
					TTokenSalesCountRefill += 1;
					TotalTokenValueRefill += (PPrice*ProductSold);
					break;
				default:
					break;
				}
			}
		}

		foreach(const QJsonValue & value, ProductVendArray)
		{
			QJsonObject Productobj = value.toObject();			
			if (Productobj["Product_ID"].toString() == LastSaleProductId)
			{				
				LastSaleProductSelectionCnt++;				
			}
		}		
	}
}

/**
-------------------------------------------------------------------------------
IntToStringConvertion
-------------------------------------------------------------------------------
*   @brief		Method to convert Integer value To String
*
*	@param		none
*
*	@return		None
*
*/
QString DexConverter::IntToStringConvertion(int Value)
{
	QString Tocharcon;
	std::string Tostringcon;
	Tostringcon = std::to_string(Value);				// to convert int to string
	Tocharcon = Tostringcon.c_str();
	return Tocharcon;
}

/**
-------------------------------------------------------------------------------
crc16
-------------------------------------------------------------------------------
*   @brief		CRC calculation
*
*	@param		none
*
*	@return		None
*
*/
unsigned short DexConverter::crc16(unsigned short crcVal, unsigned char data)
{
	static const unsigned short crctable[] = { 
		0x0000,0xCC01,0xD801,0x1400,0xF001,0x3C00,0x2800,0xE401,
		0xA001,0x6C00,0x7800,0xB401,0x5000,0x9C01,0x8801,0x4400 };

	crcVal = crcVal ^ ((unsigned short)data & 0x00FF); // Data & CRC shift right, ie LSB first

	crcVal = (crcVal >> 4) ^ crctable[crcVal & 0xF]; // Process 8-bits, 2 rounds of 4-bits
	crcVal = (crcVal >> 4) ^ crctable[crcVal & 0xF]; // Right shifting polynomial

	return(crcVal);
}

/**
-------------------------------------------------------------------------------
calculateCRC
-------------------------------------------------------------------------------
*   @brief		To calculate CRC checksum
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::calculateCRC(char *data)
{
	crc = 0x0000; // Initial
	while (*data) 
	{
		crc = crc16(crc, (unsigned char)*data++); // Cumulate the CRC value
	}
	// Calculate CRC 16
	crc = crc16(crc, 0x03); 
}

/**
-------------------------------------------------------------------------------
DexCashInflow
-------------------------------------------------------------------------------
*   @brief		Blockidentifier for Cash In
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::DexCashInflow()
{
	QString CA301, CA302, CA303, CA304, CA305, CA306, CA307, CA308, CA309, CA310;
	if (std::stoi(m_DEXConfigParams["CA3"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["CA301"]["enable"]))
		{
			CA301 = QString::number(TotalCoinsValueRefill + TotalBillsValueRefill);
		}
		if (std::stoi(m_DEXConfigParams["CA302"]["enable"]))
		{
			CA302 = QString::number(TotalCoinsValueRefill);
		}
		if (std::stoi(m_DEXConfigParams["CA303"]["enable"]))
		{		
			CA303 = QString::number(TotalCoinsValueRefill+ CoinsToTubes);
		}
		if (std::stoi(m_DEXConfigParams["CA304"]["enable"]))
		{
			CA304 = QString::number(TotalBillsValueRefill);
		}
		if (std::stoi(m_DEXConfigParams["CA305"]["enable"]))
		{
			CA305 = QString::number(AuditTableCoinsToTubesInit + AuditTableCoinsInCashBoxInit  + AuditTableCashSalesInit);
		}
		if (std::stoi(m_DEXConfigParams["CA306"]["enable"]))
		{
			CA306 = QString::number(AuditTableCoinsInCashBoxInit);
		}
		if (std::stoi(m_DEXConfigParams["CA307"]["enable"]))
		{
			CA307 = QString::number(AuditTableCoinsToTubesInit);
		}
		if (std::stoi(m_DEXConfigParams["CA308"]["enable"]))
		{
			CA308 = QString::number(AuditTableCashSalesInit);
		}
		Tofile = "CA3" + delimiter + CA301 + delimiter + CA302 + delimiter + CA303 + delimiter + CA304 + 
			delimiter + CA305 + delimiter + CA306 + delimiter + CA307 + delimiter + CA308 + CRLF;
		CountLine++;
		CRCBuffer += CA301 + CA302 + CA303 + CA304 + CA305 + CA306 + CA307;
		dexBuffer.append(Tofile);
	}
}

/**
-------------------------------------------------------------------------------
DexVendsales
-------------------------------------------------------------------------------
*   @brief		Blockidentifier to show vend sales
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::DexVendsales(QJsonObject jsonObject)
{
	QString VA101;
	QString VA102;
	QString VA103;
	QString VA104;
	QString VA201;
	QString VA202;
	QString VA203;
	QString VA204;
	QString VA301;
	QString VA302;
	QString VA303;
	QString VA304;
	int PaidvendInitAmt = 0;
	int PVCount = 0;

	QJsonValue SalesInfoArray = jsonObject.value(QString("salesinfo"));
	if (SalesInfoArray.isArray())
	{
		QJsonArray SalesInfojsonArray = jsonObject["salesinfo"].toArray();
		if (SalesInfojsonArray.isEmpty())
		{
			cout << "[DEXConverter::ReadSalesInfo] No data - SalesInfo table" << endl;
		}
		foreach(const QJsonValue & value, SalesInfojsonArray)
		{
			QJsonObject SalesInfoobj = value.toObject();
			PaidvendInitAmt += SalesInfoobj["PaidvendvalueInit"].toInt();
			PVCount += SalesInfoobj["Paidvendsales"].toInt();
		}
	}

	if (std::stoi(m_DEXConfigParams["VA1"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["VA101"]["enable"]))
		{
			VA101 = QString::number(PaidvendInitAmt);
		}
		if (std::stoi(m_DEXConfigParams["VA102"]["enable"]))
		{
			VA102 = QString::number(PVCount);
		}
		if (std::stoi(m_DEXConfigParams["VA103"]["enable"]))
		{
			VA103 = QString::number(PaidVendValueRefill);
		}
		if (std::stoi(m_DEXConfigParams["VA104"]["enable"]))
		{
			VA104 = QString::number(PaidVendCountRefill);
		}
		Tofile = "VA1" + delimiter + VA101 + delimiter + VA102 + delimiter + VA103 + delimiter + VA104 + CRLF;
		CountLine++;
		CRCBuffer += VA101 + VA102 + VA103 + VA104;
		dexBuffer.append(Tofile);
	}

	if (std::stoi(m_DEXConfigParams["VA2"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["VA201"]["enable"]))
		{
			VA201 = QString::number(TestVendValueInit);
		}
		if (std::stoi(m_DEXConfigParams["VA202"]["enable"]))
		{
			VA202 = QString::number(TestVendCount);
		}
		if (std::stoi(m_DEXConfigParams["VA203"]["enable"]))
		{
			VA203 = QString::number(TestVendValueRefill);
		}
		if (std::stoi(m_DEXConfigParams["VA204"]["enable"]))
		{
			VA204 = QString::number(TestVendCountRefill);
		}
		Tofile = "VA2" + delimiter + VA201 + delimiter + VA202 + delimiter + VA203 + delimiter + VA204 + CRLF;
		CountLine++;
		CRCBuffer += VA201 + VA202 + VA203 + VA204;
		dexBuffer.append(Tofile);
	}

	if (std::stoi(m_DEXConfigParams["VA3"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["VA301"]["enable"]))
		{
			VA301 = QString::number(FreeVendValueInit);
		}
		if (std::stoi(m_DEXConfigParams["VA302"]["enable"]))
		{
			VA302 = QString::number(FreeVendCount);
		}
		if (std::stoi(m_DEXConfigParams["VA303"]["enable"]))
		{
			VA303 = QString::number(FreeVendValueRefill);
		}
		if (std::stoi(m_DEXConfigParams["VA304"]["enable"]))
		{
			VA304 = QString::number(FreeVendCountRefill);
		}
		Tofile = "VA3" + delimiter + VA301 + delimiter + VA302 + delimiter + VA303 + delimiter + VA304 + CRLF;
		CountLine++;
		CRCBuffer += VA301 + VA302 + VA303 + VA304;
		dexBuffer.append(Tofile);
	}
}


/**
-------------------------------------------------------------------------------
DexTokensales
-------------------------------------------------------------------------------
*   @brief		Blockidentifier to show Token sales
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::DexTokensales()
{
	QString TA201;
	QString TA202;
	QString TA203;
	QString TA204;

	if (std::stoi(m_DEXConfigParams["TA2"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["TA201"]["enable"]))
		{
			TA201 = QString::number(TotalTokenValueInit);
		}
		if (std::stoi(m_DEXConfigParams["TA202"]["enable"]))
		{
			TA202 = QString::number(TTokenSalesCountInit);
		}
		if (std::stoi(m_DEXConfigParams["TA203"]["enable"]))
		{
			TA203 = QString::number(TotalTokenValueRefill);
		}
		if (std::stoi(m_DEXConfigParams["TA204"]["enable"]))
		{
			TA204 = QString::number(TTokenSalesCountRefill);
		}
		Tofile = "TA2" + delimiter + TA201 + delimiter + TA202 + delimiter + TA203 + delimiter + TA204 + CRLF;
		CountLine++;
		CRCBuffer += TA201 + TA202 + TA203 + TA204;
		dexBuffer.append(Tofile);
	}
}


/**
-------------------------------------------------------------------------------
DexCashlesssalesinfo
-------------------------------------------------------------------------------
*   @brief		Blockidentifier to show Cashless sales info
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::DexCashlesssalesinfo()
{
	QString DA201, DA202, DA203, DA204;
	if (std::stoi(m_DEXConfigParams["DA2"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["DA201"]["enable"]))
		{
			DA201 = QString::number(TotalCashlessValueInit);
		}
		if (std::stoi(m_DEXConfigParams["DA202"]["enable"]))
		{
			DA202 = QString::number(TCardSalesCountInit);
		}
		if (std::stoi(m_DEXConfigParams["DA203"]["enable"]))
		{
			DA203 = QString::number(TotalCashlessValueRefill);
		}
		if (std::stoi(m_DEXConfigParams["DA204"]["enable"]))
		{
			DA204 = QString::number(TCardSalesCountRefill);
		}
		Tofile = "DA2" +delimiter + DA201 + delimiter + DA202 + delimiter + DA203 + delimiter + DA204 + CRLF;
		CountLine++;
		CRCBuffer += DA201 + DA202 + DA203 + DA204;
		dexBuffer.append(Tofile);
	}
}

/**
-------------------------------------------------------------------------------
DexCashlesssalesinfo
-------------------------------------------------------------------------------
*   @brief		Blockidentifier to show Cashless sales info
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::DexProductinfosales(QJsonObject jsonObject)
{
	int salePrice = 0;
	QString shelfId = "";
	QString columnId = "";
	QString PA101;
	QString PA102;
	QString PA201;
	QString PA202;
	QString PA203;
	QString PA204;

	for (ProductinfoIter1 = ProductMap1.begin(); ProductinfoIter1 != ProductMap1.end(); ProductinfoIter1++)
	{
		ProdInnerMapIter1 = ProductinfoIter1->second.find("SalePrice");
		salePrice = ProdInnerMapIter1->second.toInt();

		ProdInnerMapIter1 = ProductinfoIter1->second.find("ShelfID");
		
		//qDebug() << "->" << ProdInnerMapIter1->first.c_str() << "=" << ProdInnerMapIter1->second.toString().toStdString().c_str() << endl;
		shelfId = ProdInnerMapIter1->second.toString();

		ProdInnerMapIter1 = ProductinfoIter1->second.find("ColumnID");
		//qDebug() << ProdInnerMapIter1->first.c_str() << "=" << ProdInnerMapIter1->second.toString().toStdString().c_str() << endl;
		columnId = ProdInnerMapIter1->second.toString();
		
		// Block identifier PA1
		if (std::stoi(m_DEXConfigParams["PA1"]["enable"]))
		{
			if (std::stoi(m_DEXConfigParams["PA101"]["enable"]))
			{
			//	PA101 = QString::number(ProductinfoIter1->first);
				//PA101 = ProductinfoIter1->first.c_str();
				PA101 = shelfId + columnId;
				qDebug() << PA101.toStdString().c_str() << endl;
			}
			if (std::stoi(m_DEXConfigParams["PA102"]["enable"]))
			{
				PA102 = QString::number(salePrice);
			}
			Tofile = "PA1"+ delimiter + PA101 + delimiter + PA102 + CRLF;
			CountLine++;
			CRCBuffer += PA101 + PA102;
			dexBuffer.append(Tofile);
		}

		ProdInnerMapIter1 = ProductinfoIter1->second.find("ProductSold");
		// Block identifier PA2
		if (std::stoi(m_DEXConfigParams["PA2"]["enable"]))
		{
			if (std::stoi(m_DEXConfigParams["PA201"]["enable"]))
			{
			//	PA201 = QString::number(ProdInnerMapIter1->second.toInt());
				PA201 = QString::number(ReadSalesInfo(jsonObject, PA101));
			}

			if (std::stoi(m_DEXConfigParams["PA202"]["enable"]))
			{
			//	PA202 = QString::number(ProdInnerMapIter1->second.toInt() * SalePrice);
				PA202 = QString::number(ReadSalesInfoPaidAmtInit(jsonObject, PA101));
			}

			ProdInnerMapIter1 = ProductinfoIter1->second.find("PaidSalesCntAfterRefill");
			if (std::stoi(m_DEXConfigParams["PA203"]["enable"]))
			{		
				PA203 = QString::number(ProdInnerMapIter1->second.toInt());
			}

			if (std::stoi(m_DEXConfigParams["PA204"]["enable"]))
			{				
				PA204 = QString::number(ProdInnerMapIter1->second.toInt() * salePrice);
			}
			Tofile = "PA2" + delimiter + PA201 + delimiter + PA202 + delimiter + PA203 + delimiter + PA204 + CRLF;
			CountLine++;
			CRCBuffer += PA201 + PA202 + PA203 + PA204;
			dexBuffer.append(Tofile);
		}
	}
}

/**
-------------------------------------------------------------------------------
ReadSalesInfoPaidAmtInit
-------------------------------------------------------------------------------
*   @brief		To read SalesInfo object for PaidAmtInit
*
*	@param		none
*
*	@return		None
*
*/
double DexConverter::ReadSalesInfoPaidAmtInit(QJsonObject jsonObject, QString Productstockinfo)
{
	double ProductPaidAmtInit = 0;
	QJsonValue SalesInfoArray = jsonObject.value(QString("salesinfo"));
	if (SalesInfoArray.isArray())
	{
		QJsonArray SalesInfojsonArray = jsonObject["salesinfo"].toArray();
		if (SalesInfojsonArray.isEmpty())
		{
			cout << "[DEXConverter::ReadSalesInfoPaidAmtInit] No data - SalesInfo table" << endl;
		}
		foreach(const QJsonValue & value, SalesInfojsonArray)
		{
			QJsonObject SalesInfoobj = value.toObject();
			QString Productid = SalesInfoobj["Product_ID"].toString();
			if (Productid == Productstockinfo)
			{
				ProductPaidAmtInit = SalesInfoobj["PaidvendvalueInit"].toDouble();
				break;				
			}
		}
	}
	return ProductPaidAmtInit;
}

/**
-------------------------------------------------------------------------------
ReadSalesInfo
-------------------------------------------------------------------------------
*   @brief		To read SalesInfo object
*
*	@param		none
*
*	@return		None
*
*/
int DexConverter::ReadSalesInfo(QJsonObject jsonObject, QString Productstockinfo)
{
	int Paidvendsalescount = 0;
	QJsonValue SalesInfoArray = jsonObject.value(QString("salesinfo"));
	if (SalesInfoArray.isArray())
	{
		QJsonArray SalesInfojsonArray = jsonObject["salesinfo"].toArray();
		if (SalesInfojsonArray.isEmpty())
		{
			cout << "[DEXConverter::ReadSalesInfo] No data - SalesInfo table" << endl;
		}
		foreach(const QJsonValue & value, SalesInfojsonArray)
		{
			QJsonObject SalesInfoobj = value.toObject();
			QString Productid = SalesInfoobj["Product_ID"].toString();
			if (Productid == Productstockinfo)
			{
				Paidvendsalescount = SalesInfoobj["Paidvendsales"].toInt();				
			}
		}
	}
	return Paidvendsalescount;
}

/**
-------------------------------------------------------------------------------
PriceList 
-------------------------------------------------------------------------------
*   @brief		Blockidentifier for Price List
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::PriceList()
{
	QString LA101;
	QString LA102;
	QString LA103;
	QString LA104;
	QString LA105;
	for (ProductinfoIter1 = ProductMap1.begin(); ProductinfoIter1 != ProductMap1.end(); ProductinfoIter1++)
	{
		if (std::stoi(m_DEXConfigParams["LA1"]["enable"]))
		{
			if (std::stoi(m_DEXConfigParams["LA101"]["enable"]))
			{
				// TODO - Price List number not clear
				//	PA101 = QString::number(ProductinfoIter1->first);				
			}
			if (std::stoi(m_DEXConfigParams["LA102"]["enable"]))
			{
				LA102 = ProductinfoIter1->first.c_str();
			}
			if (std::stoi(m_DEXConfigParams["LA103"]["enable"]))
			{
				ProdInnerMapIter1 = ProductinfoIter1->second.find("SalePrice");
				LA103 = QString::number(ProdInnerMapIter1->second.toInt());
			}
			if (std::stoi(m_DEXConfigParams["LA104"]["enable"]))
			{

			}
			if (std::stoi(m_DEXConfigParams["LA105"]["enable"]))
			{

			}
			Tofile = "LA1" + delimiter + LA101 + delimiter + LA102 + delimiter + LA103 + delimiter + LA104 + delimiter + LA105 + CRLF;
			CountLine++;
			CRCBuffer += "";
			dexBuffer.append(Tofile);
		}
	}
}


/**
-------------------------------------------------------------------------------
DexCashOutflow
-------------------------------------------------------------------------------
*   @brief		Blockidentifier Cash Out
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::DexCashOutflow() 
{
	if (std::stoi(m_DEXConfigParams["CA4"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["CA401"]["enable"]))
		{
			// not applicable in current implementation
		}
		Tofile = "CA4" + delimiter + CRLF;
		CountLine++;
		dexBuffer.append(Tofile);
	}
}

/**
-------------------------------------------------------------------------------
Dexvalueintubes
-------------------------------------------------------------------------------
*   @brief		Blockidentifier for value in Tube contents
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::Dexvalueintubes()
{
	QString lCoinsToTubes;
	if (std::stoi(m_DEXConfigParams["CA15"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["CA1501"]["enable"]))
		{
			lCoinsToTubes = QString::number(CoinsToTubes);
		}
		Tofile = "CA15" + delimiter + lCoinsToTubes + CRLF;
		CountLine++;
		CRCBuffer += lCoinsToTubes;
		dexBuffer.append(Tofile);
	}
}

/**
-------------------------------------------------------------------------------
RecordIntegrityCheck
-------------------------------------------------------------------------------
*   @brief		RecordIntegrityCheck calculation
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::RecordIntegrityCheck()
{
	int crcLength = 0;
	QString DexCRC;
	char crcsum[2500];

	if (std::stoi(m_DEXConfigParams["G85"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["G8501"]["enable"]))
		{
			CRCqstringtostr = CRCBuffer.toStdString();
			crcLength = CRCqstringtostr.length();						

			STRCPY(crcsum, CRCqstringtostr);

			calculateCRC(crcsum);			// Calculate CRC

			std::stringstream CRCString;
			CRCString << std::hex << crc;
			std::string Result(CRCString.str());
			DexCRC = QString::fromStdString(Result);
		}
		Tofile = "G85" + delimiter + DexCRC + CRLF;
		CountLine++;
		dexBuffer.append(Tofile);
	}
}

/**
-------------------------------------------------------------------------------
DexTrailer
-------------------------------------------------------------------------------
*   @brief		DEX application trailer block identifier
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::DexTrailer()
{
	QString TranCtrlNo;
	QString NoIncludedSets;
	if (std::stoi(m_DEXConfigParams["DXE"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["DXE01"]["enable"]))
		{
			TranCtrlNo = "1";
		}
		if (std::stoi(m_DEXConfigParams["DXE02"]["enable"]))
		{
			NoIncludedSets = "1";
		}
		Tofile = "DXE" + delimiter + TranCtrlNo + delimiter + NoIncludedSets + CRLF;
		dexBuffer.append(Tofile);
	}
}

/**
-------------------------------------------------------------------------------
DEXConfigParametersXML
-------------------------------------------------------------------------------
*   @brief		To read DEX Configuration Parameters from XML file
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::DEXConfigParametersXML()
{
	xmlDocPtr doc = NULL;
	xmlNodePtr node_ptr = NULL;

	//Read the xml file to be parsed
//	cout << "DEX file path: " << DEXCONFIGURATION_XML_PATH << endl;
	doc = xmlReadFile(DEXCONFIGURATION_XML_PATH, NULL, XML_PARSE_NOBLANKS);
	if (doc == NULL) {
		fprintf(stderr, "Could not read file...\n");
	}
	node_ptr = xmlDocGetRootElement(doc);
	traverseDEX_XMLTree(node_ptr);
//	printMap();			// for debugging purpose
	//Close the xml file and clean parser
	xmlFreeDoc(doc);
	xmlCleanupParser();
}

void DexConverter::traverseDEX_XMLTree(xmlNode *a_node)
{
	xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE && !xmlStrcmp(cur_node->name, (const xmlChar *)"DataSegment"))
		{				
			m_DEXBlockIdentifier = (char*)xmlGetProp(cur_node, (const xmlChar *)"BlockIdentifier");
			if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"enable"))
			{
				m_DEXConfigParams[m_DEXBlockIdentifier]["enable"] = "1";
			}
			else if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)"state"), (const xmlChar *)"disable"))
			{
				m_DEXConfigParams[m_DEXBlockIdentifier]["enable"] = "0";
			}
		}
		else if (cur_node->type == XML_ELEMENT_NODE && !xmlStrcmp(cur_node->name, (const xmlChar *)"DataElement"))
		{
			for (xmlAttrPtr attr = cur_node->properties; attr != NULL; attr = attr->next)
			{
				m_DEXDataElement = attr->name;

				if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)m_DEXDataElement), (const xmlChar *)"enable"))
				{
					m_DEXConfigParams[(char*)m_DEXDataElement]["enable"] = "1";
				}
				else if (!xmlStrcmp(xmlGetProp(cur_node, (const xmlChar*)m_DEXDataElement), (const xmlChar *)"disable"))
				{
					m_DEXConfigParams[(char*)m_DEXDataElement]["enable"] = "0";
				}
			}
		}
		traverseDEX_XMLTree(cur_node->children);
	}
}

/*
------------------------------------------------------------------------------ -
PrintMap
------------------------------------------------------------------------------ -
*@brief		To print the internal maps used(For debugging purpose only)
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::printMap()
{
	typedef std::map<std::string, std::string> inner_map;
	typedef std::map<std::string, inner_map> outer_map;

	for (outer_map::iterator i = m_DEXConfigParams.begin(), iend = m_DEXConfigParams.end(); i != iend; ++i)
	{
		inner_map &innerMap = i->second;
		std::cout << "---- " << i->first << " ----" << std::endl;
		for (inner_map::iterator j = innerMap.begin(), jend = innerMap.end(); j != jend; ++j)
		{
			std::cout << j->first << " :: " << j->second << std::endl;
		}
		std::cout << std::endl;
	}
}

/**
-------------------------------------------------------------------------------
NoofIncludedSets
-------------------------------------------------------------------------------
*   @brief		To calculate No of Included Sets in the Dex report
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::NoofIncludedSets()
{
	QString TranSetCtrlNo;
	QString CLine;
	std::string Tostringcon;
	if (std::stoi(m_DEXConfigParams["SE"]["enable"]))
	{
		if (std::stoi(m_DEXConfigParams["SE01"]["enable"]))
		{
			Tostringcon = std::to_string(CountLine);			// to convert int to string
			CLine = Tostringcon.c_str();						// to convert string to char
		}
		if (std::stoi(m_DEXConfigParams["SE02"]["enable"]))
		{
			TranSetCtrlNo = "0001";
		}
		Tofile = "SE" + delimiter + CLine + delimiter + TranSetCtrlNo + CRLF;
		dexBuffer.append(Tofile);
	}
}

/**
-------------------------------------------------------------------------------
Readproductvendinfo
-------------------------------------------------------------------------------
*   @brief		To read roductvendinfo object content
*
*	@param		none
*
*	@return		None
*
*/
void DexConverter::ReadproductvendinfoObject(QJsonObject jsonObject)
{
//	int ProductID = 0;
	string ProductID;	
	int PSaleprice = 0;
	int ProductSold = 0;
	QVariant TProdSold;
	QJsonValue ProductVend = jsonObject.value(QString("product_vend_info"));
	if (ProductVend.isArray())
	{
		QJsonArray ProductVendArray = jsonObject["product_vend_info"].toArray();
		foreach(const QJsonValue & value, ProductVendArray)
		{
			QJsonObject Productobj = value.toObject();						
			ProductID = Productobj["Product_ID"].toString().toStdString();
			ProductinfoIter1 = ProductMap1.find(ProductID);

			if (ProductinfoIter1 != ProductMap1.end())
			{
				ProductSold = Productobj["ProductSold"].toInt();
				ProdInnerMapIter1 = ProductinfoIter1->second.find("ProductSold");
				TProdSold = ProdInnerMapIter1->second.toInt() + ProductSold;
				ProductMap1[ProductID]["ProductSold"] = TProdSold;
			}
			else
			{
				ProductSold = Productobj["ProductSold"].toInt();
				PSaleprice = Productobj["SalePrice"].toInt();
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("ProductSold", ProductSold));
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("SalePrice", PSaleprice));
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("PaidSalesCntAfterRefill", 0));
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("ShelfID", Productobj["ShelfID"].toString()));
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("ColumnID", Productobj["ColumnID"].toString()));
				ProductMap1.insert(map<std::string, ProdInnerInfo1>::value_type(ProductID, ProdInnerMap1));
				ProdInnerMap1.clear();
			}
		}
	}
}

/**
-------------------------------------------------------------------------------
ReadConfigurationData
-------------------------------------------------------------------------------
* @brief To read Configuration Data object content
*
* @param none
*
* @return None
*
*/
void DexConverter::ReadConfigurationData(QJsonObject jsonObject)
{
	QString MA501;
	QString MA502;
	QString MA503;
	QString MA504;
	QString MA505;
	QString MA506;
	QString MA507;
	QString MA508;
	QString MA509;
	QString MA510;
	QString MA511;

	QJsonValue ConfigurationDataArray = jsonObject.value(QString("configurationdata"));

	if (ConfigurationDataArray.isArray())
	{
		QJsonArray jsonArray = jsonObject["configurationdata"].toArray();
		if (jsonArray.isEmpty())
		{
			cout << "[DexConverter::ReadConfigurationData] No data - configurationdata table" << endl;
		}
		foreach(const QJsonValue & value, jsonArray)
		{
			QJsonObject configobj = value.toObject();
			if (std::stoi(m_DEXConfigParams["MA5"]["enable"]))
			{
				if (std::stoi(m_DEXConfigParams["MA501"]["enable"]))
				{
					MA501 = configobj["Block_Identifier"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA502"]["enable"]))
				{
					MA502 = configobj["Configuration_Data"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA503"]["enable"]))
				{
					MA503 = configobj["OptionalField2"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA504"]["enable"]))
				{
					MA504 = configobj["OptionalField3"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA505"]["enable"]))
				{
					MA505 = configobj["OptionalField4"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA506"]["enable"]))
				{
					MA506 = configobj["OptionalField5"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA507"]["enable"]))
				{
					MA507 = configobj["OptionalField6"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA508"]["enable"]))
				{
					MA508 = configobj["OptionalField7"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA509"]["enable"]))
				{
					MA509 = configobj["OptionalField8"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA510"]["enable"]))
				{
					MA510 = configobj["OptionalField9"].toString();
				}
				if (std::stoi(m_DEXConfigParams["MA511"]["enable"]))
				{
					MA511 = configobj["OptionalField10"].toString();
				}
				Tofile = "MA5" + delimiter + MA501 + delimiter + MA502 + delimiter + MA503 + delimiter + MA504 + delimiter + MA505 + delimiter +
					MA506 + delimiter + MA507 + delimiter + MA508 + delimiter + MA509 + delimiter + MA510 + delimiter + MA511 + CRLF;
				CRCBuffer += MA501 + MA502 + MA503 + MA504 + MA505 + MA506 + MA507 + MA508 + MA509 + MA510 + MA511;
				CountLine++;
				dexBuffer.append(Tofile);
			}	
		}
	}
}

/**
-------------------------------------------------------------------------------
ReadeventinfoObject
-------------------------------------------------------------------------------
* @brief To read eventinfo object content
*
* @param none
*
* @return None
*
*/

void DexConverter::ReadeventinfoObject(QJsonObject jsonObject)
{
	QString EventID;
	QString EDate;
	QString ETime;
	QString EReportDate;
	QString EReportTime;
	QString EA1_EDuration;
	QString EA2_EDuration;
	int EventTime = 0;
	int EA2_EventDuration = 0;
	int EA1_EventDuration = 0;
	// QString EActive = "";
	QString EventState;
	QString CurrentState = "0";
	QString EA2EventID;
	QJsonValue EventArray = jsonObject.value(QString("eventinfo"));
	QString EA1_UserDefinedValue;
	QString EA2_UserDefinedValue;
	QString eventDate;
	QString eventTime;


	if (EventArray.isArray())
	{
		QJsonArray jsonArray = jsonObject["eventinfo"].toArray();
		if (jsonArray.isEmpty())
		{
			cout << "[DexConverter::ReadeventinfoObject] No data - eventinfo table" << endl;
			// return READ_VMINFO_FAILURE;
		}
		
		foreach(const QJsonValue & value, jsonArray)
		{
			QJsonObject eventobj = value.toObject();
			QString EventActivity = eventobj["EventCurrentState"].toString();			

			EventID = eventobj["Event_Identity"].toString();
			if (EventID == "POWER OUTAGE")
			{
				if (std::stoi(m_DEXConfigParams["EA7"]["enable"]))
				{
					// # CHECK
					Tofile = "EA7" + delimiter + "1" + delimiter + "1" + CRLF;
					CRCBuffer += EventID;
					CountLine++;
					dexBuffer.append(Tofile);
				}
			}

			if ((EventID != "POWER OUTAGE") && ("ACTIVE" == EventActivity || "INACTIVE" == EventActivity))
			{				
				if (std::stoi(m_DEXConfigParams["EA2"]["enable"]))
				{
					if (std::stoi(m_DEXConfigParams["EA201"]["enable"]))
					{
						EA2EventID = eventobj["Event_Identity"].toString();
					}
					if (std::stoi(m_DEXConfigParams["EA202"]["enable"]))
					{
						//EventTime = eventobj["Timestamp"].toInt();
						//ETimeStamp = IntToStringConvertion(EventTime);
					}
					if (std::stoi(m_DEXConfigParams["EA203"]["enable"]))
					{
						// not applicable in current implementation
					}

					if (std::stoi(m_DEXConfigParams["EA204"]["enable"]))
					{
						// USER DEFINED FIELD
						// # TODO : Shall be handled in future
						EA2_UserDefinedValue = "";
					}

					if (std::stoi(m_DEXConfigParams["EA205"]["enable"]))
					{
						EventState = eventobj["EventCurrentState"].toString();
						if (EventState == "ACTIVE")
						{
							CurrentState = "1";
						}
						else
						{
							CurrentState = "0";
						}
					}

					if (std::stoi(m_DEXConfigParams["EA206"]["enable"]))
					{
						EA2_EventDuration = eventobj["Duration_of_Event"].toInt();
						EA2_EDuration = IntToStringConvertion(EA2_EventDuration);
					}

					Tofile = "EA2" + delimiter + EA2EventID + delimiter + delimiter + delimiter + EA2_UserDefinedValue +
						delimiter + CurrentState + delimiter + EA2_EDuration + CRLF;
					CRCBuffer += EA2EventID + CurrentState + EA2_EDuration;
					CountLine++;
					dexBuffer.append(Tofile);
				}
			}
			else if (EventID != "POWER OUTAGE")
			{
				if (std::stoi(m_DEXConfigParams["EA1"]["enable"]))
				{
					EventTime = eventobj["Timestamp"].toInt();
					getUTCTime(EventTime, EDate, ETime);

					if (std::stoi(m_DEXConfigParams["EA101"]["enable"]))
					{
						EventID = eventobj["Event_Identity"].toString();
						if (EventID == "REFILL")
						{
							RefillTimeUTC = eventobj["Timestamp"].toInt();
						}
					}

					if (std::stoi(m_DEXConfigParams["EA102"]["enable"]))
					{
						EReportDate = EDate;
					}

					if (std::stoi(m_DEXConfigParams["EA103"]["enable"]))
					{
						EReportTime = ETime;
					}

					if (std::stoi(m_DEXConfigParams["EA104"]["enable"]))
					{
						EA1_EventDuration = eventobj["Duration_of_Event"].toInt();
						EA1_EDuration = IntToStringConvertion(EA1_EventDuration);
					}
					if (std::stoi(m_DEXConfigParams["EA105"]["enable"]))
					{
						// Event duration in milliseconds not used
					}
					if (std::stoi(m_DEXConfigParams["EA106"]["enable"]))
					{
						// USER DEFINED FIELD
						// # TODO : Shall be handled in future
						EA1_UserDefinedValue = "";
					}

					Tofile = "EA1" + delimiter + EventID + delimiter + EReportDate + delimiter + 
						EReportTime + delimiter + EA1_EDuration + CRLF;
					CRCBuffer += EventID + EReportDate + EReportTime + EA1_EDuration;
					CountLine++;
					dexBuffer.append(Tofile);
				}
			}
			else
			{
				// Do nothing
			}
		}
	}
}

/**
-------------------------------------------------------------------------------
ReadAuditInfo
-------------------------------------------------------------------------------
*   @brief		To read Audit info object content
*
*	@param		jsonObject
*
*	@return		None
*
*/
void DexConverter::ReadAuditInfo(QJsonObject jsonObject)
{
	QJsonValue AuditinfoArray = jsonObject.value(QString("auditinfo"));
	if (AuditinfoArray.isArray())
	{
		QJsonArray AuditinfojsonArray = jsonObject["auditinfo"].toArray();
		if (AuditinfojsonArray.isEmpty())
		{
			cout << "[DEXConverter::ReadAuditInfo] No data - auditinfo table" << endl;
		}
		foreach(const QJsonValue & value, AuditinfojsonArray)
		{
			QJsonObject Auditinfoobj = value.toObject();
			AuditTableCoinsInCashBoxInit = Auditinfoobj["Coins_in_CashBox"].toInt();
			AuditTableCoinsToTubesInit = Auditinfoobj["Coins_to_Tubes"].toInt();
			AuditTableCashSalesInit = Auditinfoobj["Cash"].toInt();
			AuditTableCardSalesInit = Auditinfoobj["card_sales"].toInt();
			ReadCount = Auditinfoobj["NoofReads"].toInt();
		}
		/*qDebug() << "CoinsInCashBox : " << AuditTableCoinsInCashBoxInit;
		qDebug() << "CoinsToTubes : " << AuditTableCoinsToTubesInit;
		qDebug() << "CashSales : " << AuditTableCashSalesInit;
		qDebug() << "CardSales : " << AuditTableCardSalesInit;*/
	}
}

/**
-------------------------------------------------------------------------------
ReadCashInInfo
-------------------------------------------------------------------------------
*   @brief		To read cashInInfo object content
*
*	@param		jsonObject
*
*	@return		None
*
*/
void DexConverter::ReadCashInInfo(QJsonObject jsonObject)
{
	int V_BillToRecycle = 0;
	QString BillToRecycler;
	//int Timestamp = 0;
	//float CoinsInCashBox = 0;
	int	initBills = 0;
	int BillsAfterReset = 0;
	int CoinsAfterReset = 0;
	QString CA1001;
	QString CA1002;
	QString CA1003;
	QString CA1004;

	QJsonValue VMArray = jsonObject.value(QString("cashin_info"));
	if (VMArray.isArray())
	{
		QJsonArray VMjsonArray = jsonObject["cashin_info"].toArray();
		if (VMjsonArray.isEmpty())
		{
			cout << "[DEXConverter::ReadCashInInfo] No data - cashin_info table" << endl;
		}
		foreach(const QJsonValue & value, VMjsonArray)
		{
			QJsonObject VMobj = value.toObject();
			//Timestamp = VMobj["Timestamp"].toInt(); // To be used in future
			CoinsInCashBox = VMobj["Coins_in_CashBox"].toInt();
			CoinsToTubes = VMobj["Coins_to_Tubes"].toInt();
			BillsInCashBox = VMobj["Bills_in_CashBox"].toInt();

			if (std::stoi(m_DEXConfigParams["CA10"]["enable"]))
			{
				if (std::stoi(m_DEXConfigParams["CA1001"]["enable"]))
				{
					CoinsAfterReset = VMobj["Coins_to_Tubes"].toInt();
					CA1001 = IntToStringConvertion(CoinsAfterReset);
				}
				if (std::stoi(m_DEXConfigParams["CA1002"]["enable"]))
				{
					// $ CHECK - Fill the initial value based on audit info
					CA1002 = IntToStringConvertion(CoinsToTubes);
				}
				if (std::stoi(m_DEXConfigParams["CA1003"]["enable"]))
				{					
					BillsAfterReset = VMobj["Cash_to_Recycler"].toInt();
					CA1003 = IntToStringConvertion(BillsAfterReset);
				}
				if (std::stoi(m_DEXConfigParams["CA1004"]["enable"]))
				{
					// $ CHECK - Fill the initial value based on audit info
					initBills = VMobj["Cash_to_Recycler"].toInt();
					CA1004 = IntToStringConvertion(initBills);
				}
				Tofile = "CA10" + delimiter + CA1001 + delimiter + CA1002 + delimiter + CA1003 + delimiter + CA1004 + CRLF;
				CountLine++;
				CRCBuffer += CA1001 + CA1002 + CA1003 + CA1004;
				dexBuffer.append(Tofile);
			}
			
			if (std::stoi(m_DEXConfigParams["CA22"]["enable"]))
			{
				if (std::stoi(m_DEXConfigParams["CA2201"]["enable"]))
				{
					V_BillToRecycle = VMobj["Cash_to_Recycler"].toInt();
					BillToRecycler = IntToStringConvertion(V_BillToRecycle);
				}

				Tofile = "CA22" + delimiter + BillToRecycler + CRLF;
				CountLine++;
				CRCBuffer += BillToRecycler;
				dexBuffer.append(Tofile);
			}
		}
	}
}

/**
-------------------------------------------------------------------------------
ReadCoinMechanismInfo
-------------------------------------------------------------------------------
*   @brief		To read CoinMechanismInfo object content
*
*	@param		jsonObject
*
*	@return		None
*
*/
void DexConverter::ReadCoinMechanismInfo(QJsonObject jsonObject)
{
	QString CA1701;
	QString CA1702;
	QString CA1703;
	QString CA1704;
	QString CA1705;
	QString CA1706;

	QJsonValue cmechanismArray = jsonObject.value(QString("coinmechanism_info"));
	if (cmechanismArray.isArray())
	{
		QJsonArray CoinMechjsonArray = jsonObject["coinmechanism_info"].toArray();
		if (CoinMechjsonArray.isEmpty())
		{
			cout << "[DEXConverter::ReadCoinMechanismInfo] No data - coinmechanism_info table" << endl;
		}
		foreach(const QJsonValue & value, CoinMechjsonArray)
		{
			QJsonObject CoinMechobj = value.toObject();

			if (std::stoi(m_DEXConfigParams["CA17"]["enable"]))
			{
				if (std::stoi(m_DEXConfigParams["CA1701"]["enable"]))
				{					
					CA1701 = IntToStringConvertion(CoinMechobj["Coin_Type_Number"].toInt());
				}
				if (std::stoi(m_DEXConfigParams["CA1702"]["enable"]))
				{
					CA1702 = IntToStringConvertion(CoinMechobj["Coin_value"].toInt());
				}
				if (std::stoi(m_DEXConfigParams["CA1703"]["enable"]))
				{					
					CA1703 = IntToStringConvertion(CoinMechobj["Number_of_Coins_in_Tube"].toInt());
				}
				if (std::stoi(m_DEXConfigParams["CA1704"]["enable"]))
				{
					CA1704 = IntToStringConvertion(CoinMechobj["Coin_Insertion_during_Manual_Fill"].toInt());
				}
				if (std::stoi(m_DEXConfigParams["CA1705"]["enable"]))
				{
					CA1705 = IntToStringConvertion(CoinMechobj["Coins_Dispensed_during_Manual_Invent"].toInt());
				}
				if (std::stoi(m_DEXConfigParams["CA1706"]["enable"]))
				{
					CA1706 = IntToStringConvertion(CoinMechobj["IsCoinTubeFull"].toInt());
				}

				Tofile = "CA17" + delimiter + CA1701 + delimiter + CA1702 + delimiter +
					CA1703 + delimiter + CA1704 + delimiter + CA1705 + delimiter + CA1706 + CRLF;
				CountLine++;
				CRCBuffer += CA1701 + CA1702 +	CA1703 + CA1704 + CA1705 + CA1706;
				dexBuffer.append(Tofile);
			}			
		}
	}
}

/**
-------------------------------------------------------------------------------
ReadVmDeviceInfo
-------------------------------------------------------------------------------
*   @brief		To read vmdevice object content
*
*	@param		jsonObject
*
*	@return		None
*
*/
void DexConverter::ReadVmDeviceInfo(QJsonObject jsonObject)
{
	QString BillValidatorSerialNo;
	QString BillValidatorModelNo;
	int BillValidatorSWVer = 0;
	QString BValidatorSWVer;
	QString CChangerSerialNo;
	QString CMechanismModelNo;
	int CMechanismSWVer = 0;
	QString CMechSWVer;
	QString CA1UserDefinedValue;
	QString BA1UserDefinedValue;
	QString CA1AssetNo;
	QString BA1AssetNo;

	QJsonValue VMArray = jsonObject.value(QString("vmdevice"));
	if (VMArray.isArray())
	{
		QJsonArray VMjsonArray = jsonObject["vmdevice"].toArray();
		if (VMjsonArray.isEmpty())
		{
			cout << "[DEXConverter::ReadVmDeviceInfo] No data - vmdevice table" << endl;
		}
		foreach(const QJsonValue & value, VMjsonArray)
		{
			QJsonObject VMobj = value.toObject();
			if ((std::stoi(m_DEXConfigParams["BA1"]["enable"])) &&
				(VMobj["VM_Device_Type"].toString() == BILLVALIDATOR))
			{
				if (std::stoi(m_DEXConfigParams["BA101"]["enable"]))
				{
					BillValidatorSerialNo = VMobj["Serial_No"].toString();
				}
				if (std::stoi(m_DEXConfigParams["BA102"]["enable"]))
				{
					BillValidatorModelNo = VMobj["Model_No"].toString();
				}
				if (std::stoi(m_DEXConfigParams["BA103"]["enable"]))
				{
					BillValidatorSWVer = VMobj["SW_Version"].toInt();
					BValidatorSWVer = IntToStringConvertion(BillValidatorSWVer);
				}
				if (std::stoi(m_DEXConfigParams["BA104"]["enable"]))
				{
					// USER DEFINED FIELD
					// # TODO : Shall be handled in future
					BA1UserDefinedValue = "";
				}
				if (std::stoi(m_DEXConfigParams["BA105"]["enable"]))
				{					
					BA1AssetNo = VMobj["VM_Device_Asset_No"].toString();
				}
				Tofile = "BA1" + delimiter + BillValidatorSerialNo + delimiter + BillValidatorModelNo 
					+ delimiter + BValidatorSWVer + delimiter + BA1UserDefinedValue + delimiter + BA1AssetNo + CRLF;
				CountLine++;
				CRCBuffer += BillValidatorSerialNo + BillValidatorModelNo + BValidatorSWVer
					+ BA1UserDefinedValue + BA1AssetNo;
				dexBuffer.append(Tofile);
			}

			if ((std::stoi(m_DEXConfigParams["CA1"]["enable"])) &&
				(VMobj["VM_Device_Type"].toString() == COINMECHANISM))
			{
				if (std::stoi(m_DEXConfigParams["CA101"]["enable"]))
				{
					CChangerSerialNo = VMobj["Serial_No"].toString();
				}
				if (std::stoi(m_DEXConfigParams["CA102"]["enable"]))
				{
					CMechanismModelNo = VMobj["Model_No"].toString();
				}
				if (std::stoi(m_DEXConfigParams["CA103"]["enable"]))
				{
					CMechanismSWVer = VMobj["SW_Version"].toInt();
					CMechSWVer = IntToStringConvertion(CMechanismSWVer);
				}
				if (std::stoi(m_DEXConfigParams["CA104"]["enable"]))
				{				
					// USER DEFINED FIELD
					// # TODO : Shall be handled in future
					CA1UserDefinedValue = "";

				}
				if (std::stoi(m_DEXConfigParams["CA105"]["enable"]))
				{
					CA1AssetNo = VMobj["VM_Device_Asset_No"].toString();
				}
				Tofile = "CA1" + delimiter + CChangerSerialNo + delimiter + CMechanismModelNo + delimiter 
					+ CMechSWVer + delimiter + CA1UserDefinedValue + delimiter + CA1AssetNo + CRLF;
				CountLine++;
				CRCBuffer += CChangerSerialNo + CMechanismModelNo + CMechSWVer 
					+ CA1UserDefinedValue + CA1AssetNo;
				dexBuffer.append(Tofile);
			}
		}
	}
}

/**
-------------------------------------------------------------------------------
ReadvminfoObject
-------------------------------------------------------------------------------
*   @brief		To read vminfo object content
*
*	@param		jsonObject
*
*	@return		None
*
*/
void DexConverter::ReadvminfoObject(QJsonObject jsonObject)
{
	QString Comm_ID;
	QString Version;
	QString AssetNo;
	QString Serialno;
	QString ModelNo;
	int MachineBuildStd = 0;
	QString MacBuildStd;
	QString MacLocation;
	QString MachineLocation;
	QString ST01;
	QString ST02;
	QString ANCurrencyCode;
	int CurrencyDecimalPt;
	int CurrencyCode;
	QString CurrDecimalPt;
	QString CurrCode;
	QString FunctionIdentifier;
	QString TransmissionNo;
	int VMDateTime;
	QString UserDefinedValue;
	QString VMDate;
	QString VMTime;
	QString VMReportDate;
	QString VMReportTime;

	QJsonValue VMArray = jsonObject.value(QString("vminfo"));
	if (VMArray.isArray())
	{
		QJsonArray VMjsonArray = jsonObject["vminfo"].toArray();
		if (VMjsonArray.isEmpty())
		{
			cout << "[DEXConverter::ReadvminfoObject] No data - vminfo table" << endl;
		}
		foreach(const QJsonValue & value, VMjsonArray)
		{
			QJsonObject VMobj = value.toObject();
		//	CoinsToTubes = VMobj["Coins_to_Tubes"].toInt();
			if (std::stoi(m_DEXConfigParams["DXS"]["enable"]))
			{
				if (std::stoi(m_DEXConfigParams["DXS01"]["enable"]))
				{
					Comm_ID = VMobj["Comm_Id"].toString();
				}
				if (std::stoi(m_DEXConfigParams["DXS02"]["enable"]))
				{
					FunctionIdentifier = VMFUNCTIONIDENTIFIER;
				}
				if (std::stoi(m_DEXConfigParams["DXS03"]["enable"]))
				{
					Version = VMobj["Version"].toString();
				}
				if (std::stoi(m_DEXConfigParams["DXS04"]["enable"]))
				{
					TransmissionNo = VMTRANSMISSIONNO;
				}
				Tofile = "DXS" + delimiter + Comm_ID + delimiter + FunctionIdentifier + delimiter + Version + 
					delimiter + TransmissionNo + CRLF;
				dexBuffer.append(Tofile);
			}

			if (std::stoi(m_DEXConfigParams["ST"]["enable"]))
			{
				if (std::stoi(m_DEXConfigParams["ST01"]["enable"]))
				{
					ST01 = TRANSACTIONSETHDR;
				}
				if (std::stoi(m_DEXConfigParams["ST02"]["enable"]))
				{
					ST02 = TRANSACTIONSETCTRLNO;
				}
				Tofile = "ST" + delimiter + ST01 + delimiter + ST02 + CRLF;
				CountLine++;
				CRCBuffer = ST01 + ST02;
				dexBuffer.append(Tofile);
			}

			if (std::stoi(m_DEXConfigParams["ID1"]["enable"]))
			{
				if (std::stoi(m_DEXConfigParams["ID101"]["enable"]))
				{
					Serialno = VMobj["Serial_No"].toString();
				}
				if (std::stoi(m_DEXConfigParams["ID102"]["enable"]))
				{
					ModelNo = VMobj["Model_No"].toString();					
				}
				if (std::stoi(m_DEXConfigParams["ID103"]["enable"]))
				{
					MachineBuildStd = VMobj["Machine_Build_Std"].toInt();
					MacBuildStd = IntToStringConvertion(MachineBuildStd);
				}
				if (std::stoi(m_DEXConfigParams["ID104"]["enable"]))
				{
					MacLocation = VMobj["Machine_Location"].toString();
				}
				if (std::stoi(m_DEXConfigParams["ID105"]["enable"]))
				{
					// USER DEFINED FIELD
					// # TODO : Shall be handled in future
					UserDefinedValue = "";
				}
				if (std::stoi(m_DEXConfigParams["ID106"]["enable"]))
				{
					AssetNo = VMobj["Asset_No"].toString();
				}
				Tofile = "ID1" + delimiter + Serialno + delimiter + ModelNo + delimiter + MacBuildStd + delimiter + MacLocation 
					+ delimiter + UserDefinedValue + delimiter + AssetNo + CRLF;
				CountLine++;
				CRCBuffer += Serialno + ModelNo + MacBuildStd + MacLocation + AssetNo;
				dexBuffer.append(Tofile);
			}

			if (std::stoi(m_DEXConfigParams["ID4"]["enable"]))
			{
				if (std::stoi(m_DEXConfigParams["ID401"]["enable"]))
				{
					CurrencyDecimalPt = VMobj["Currency_Decimal"].toInt();
					CurrDecimalPt = IntToStringConvertion(CurrencyDecimalPt);
				}
				if (std::stoi(m_DEXConfigParams["ID402"]["enable"]))
				{
					CurrencyCode = VMobj["Currency_Code"].toInt();
					CurrCode = IntToStringConvertion(CurrencyCode);
				}
				if (std::stoi(m_DEXConfigParams["ID403"]["enable"]))
				{
					ANCurrencyCode = VMobj["Alphabetic_Currency_Code"].toString();		
				}

				Tofile = "ID4" + delimiter + CurrDecimalPt + delimiter + CurrCode
					+ delimiter + ANCurrencyCode + CRLF;
				CountLine++;
				CRCBuffer += CurrDecimalPt + CurrCode + ANCurrencyCode;
				dexBuffer.append(Tofile);
			}

			if (std::stoi(m_DEXConfigParams["ID5"]["enable"]))
			{
				VMDateTime = VMobj["Timestamp"].toInt();
				getUTCTime(VMDateTime, VMDate, VMTime);
				if (std::stoi(m_DEXConfigParams["ID501"]["enable"]))
				{
					VMReportDate = VMDate;
				}
				if (std::stoi(m_DEXConfigParams["ID502"]["enable"]))
				{
					VMReportTime = VMTime;
				}
				Tofile = "ID5" + delimiter + VMReportDate + delimiter + VMReportTime + CRLF;
				CountLine++;
				CRCBuffer += VMReportDate + VMReportTime;
				dexBuffer.append(Tofile);
			}

			if (std::stoi(m_DEXConfigParams["CA9"]["enable"]))
			{
				if (std::stoi(m_DEXConfigParams["CA901"]["enable"]))
				{
					// # TODO : Shall be added in future
				}
				if (std::stoi(m_DEXConfigParams["CA902"]["enable"]))
				{
					// # TODO : Shall be added in future
				}

				// As of now the CA9 values are hardcoded with default value - 0
				Tofile = "CA9" + delimiter + DEFAULTVALUE + delimiter + DEFAULTVALUE + CRLF;
				CountLine++;
				CRCBuffer += DEFAULTVALUE;
				dexBuffer.append(Tofile);
			}
		}
	}
}

/**
-------------------------------------------------------------------------------
getUTCTime
-------------------------------------------------------------------------------
*   @brief		To convert timestamp from epoch to UTC format
*
*	@param timeToBeConverted	Epoch time to be converted to UTC format
*	@param date					Date in UTC format
*   @param time					Time in UTC format
*
*	@return		None
*
*/
void DexConverter::getUTCTime(time_t timeToBeConverted, QString &date, QString &time)
{
	struct tm convertedTime;
	char buffer[80];

#ifdef _WIN32
	gmtime_s(&convertedTime, &timeToBeConverted);
#elif __linux__
	gmtime_r(&timeToBeConverted, &convertedTime);
#endif 

	strftime(buffer, 80, "%Y%m%d", &convertedTime);
	date = QString::fromUtf8(buffer);

	memset(buffer, 0, sizeof buffer);	
	strftime(buffer, 80, "%H%M", &convertedTime);
	time = QString::fromUtf8(buffer);
}

/**
-------------------------------------------------------------------------------
Clear_Memory
-------------------------------------------------------------------------------
*   @brief		To clear variables
*
*	@param		None
*	
*	@return		None
*
*/

void DexConverter::Clear_Memory()
{
	CoinsToTubes = 0;
	CoinsInCashBox = 0;
	BillsInCashBox = 0;
	CountLine = 0;
	PaidVendCount = 0;
	FreeVendCount = 0;
	TestVendCount = 0;
	PaidVendCountRefill = 0;
	FreeVendCountRefill = 0;
	TestVendCountRefill = 0;
	TCardSalesCountInit = 0;
	TCashSalesCountInit = 0;
	TCoinSalesCountInit = 0;
	TCoinSalesCountRefill = 0;
	TCardSalesCountRefill = 0;
	TCashSalesCountRefill = 0;
	RefillTimeUTC = 0;
	ReadCount = 0;
	AuditTableCoinsInCashBoxInit = 0;
	AuditTableCoinsToTubesInit = 0;
	AuditTableCashSalesInit = 0;
	AuditTableCardSalesInit = 0;
	PaidVendValueRefill = 0;
	FreeVendValueRefill = 0;
	TestVendValueRefill = 0;
	PaidVendValueInit = 0;
	FreeVendValueInit = 0;
	TestVendValueInit = 0;
	TotalBillsValueInit = 0;
	TotalCoinsValueInit = 0;
	TotalCashlessValueInit = 0;
	TotalCoinsValueRefill = 0;
	TotalCashlessValueRefill = 0;
	TotalBillsValueRefill = 0;
	LastSaleProductSelectionCnt = 0;
	RecentSaleTimeStamp = 0;

	TotalTokenValueInit = 0;
	TTokenSalesCountInit = 0;
	TTokenSalesCountRefill = 0;
	TotalTokenValueRefill = 0;
	
	ProductMap1.clear();
}

