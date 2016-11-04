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
*	@file		Transaction.cpp
*
*	@brief		This module processes vending database information to generate 
*					- Transaction report and
*					- Summary report.
*
*	\par  Software License Agreement:
*
*	The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*	Project for use exclusively on PC. The software is owned by the Intel, and
*	is protected under applicable copyright laws.
*
*/

#include "Transaction_mock.h"
#include "qvariant.h"

using namespace std;

//QString dexBuffer = "";

/**
-------------------------------------------------------------------------------
Transaction
-------------------------------------------------------------------------------
*   @brief		Constructor initializing the member variables and required inits.
*
*	@param		none
*
*	@return		None
*
*/
Transaction::Transaction()
{

}


/**
-------------------------------------------------------------------------------
~Transaction
-------------------------------------------------------------------------------
*   @brief		Destructor performing the cleanup like deleting allocated memory, dereferencing etc..
*
*	@param		none
*
*	@return		None
*
*/
Transaction::~Transaction()
{
	// Define all the code cleanup and memory delete
}

/**
-------------------------------------------------------------------------------
WriteToFile
-------------------------------------------------------------------------------
*   @brief		This function is for debugging purpose.
*
*	@param		none
*
*	@return		None
*
*/
void Transaction::WriteToFile()
{
	fstream Transactionfile;
	//PublishTableController publishcontroller1;
	Transactionfile.open("D:/Intel/Transaction_Report.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	if (!Transactionfile)
		cout << "error opening file for writing" << endl;
	//	std::cout << " Current Time :: " << currentDateTime() << std::endl;
	//	Dexfile << "#####   Current Time ::  " << currentDateTime() << "     ######" << endl << endl;
//	std::cout << "------------   DEX report !! START !! --------------  " << endl << dexBuffer.toUtf8().constData() << endl;
//	std::cout << "------------   DEX report !! END !! ----------------  " << endl << endl;
//	Transactionfile << dexBuffer.toUtf8().constData();
	Transactionfile.close();
}

/**
-------------------------------------------------------------------------------
Read_Vminfo_Table
-------------------------------------------------------------------------------
*   @brief		This function reads vminfo table to get device ID.
*
*	@param		Json string
*
*	@return		None
*
*/
TRANSACTION_SUMMARY_STATUS_CODE Transaction::Read_Vminfo_Table(string JsonData)
{
	Datatranslator datatranslator;
	QString JsonData1 = JsonData.c_str();    // string to Qstring conversion
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	QJsonObject jsonObject = JsonResponse.object();
	QJsonValue Vminfo = jsonObject.value(QString("vminfo"));
	if (Vminfo.isArray())
	{
		QJsonArray Vminfoarray = jsonObject["vminfo"].toArray();
		if (Vminfoarray.isEmpty())
		{
			cout << "[Transaction::Read_Vminfo_Table] No data - vminfo table" << endl;
			return READ_VMINFO_FAILURE;
		}
		foreach(const QJsonValue & value, Vminfoarray)
		{
			QJsonObject Vminfoobj = value.toObject();
			Comm_ID = Vminfoobj["Comm_Id"].toString();
		}
	}
	else
	{
		cout << "[Transaction::Read_Vminfo_Table] vminfo is not an Array" << endl;
		return READ_VMINFO_FAILURE;
	}
	return READ_VMINFO_SUCCESS;
}

/**
------------------------------------------------------------------------------ -
Transaction_Summary
------------------------------------------------------------------------------ -
*	@brief		This function generates transaction summary.
*
*	@param		Json string
*
*	@return		Timestamp
*
*/
TRANSACTION_SUMMARY_STATUS_CODE Transaction::Transaction_Summary(string JsonData)
{
	int ProductID = 0;
	int PPrice = 0;
	int PPaymentSource = 0;
	int CampaignID = 0;
	float DiscountRatio = 0;
	qlonglong PVTime;
	QString PVendorID;
	QString PShelfID;
	QString PColumnID;
	QString Temp;
	Datatranslator datatranslator;
	string timestamp;



	if (READ_VMINFO_FAILURE == Read_Vminfo_Table(JsonData))
	{
		cout << "[Transaction::Transaction_Summary] Error in Read_Vminfo_Table" << endl;
	}
	
	QString JsonData1 = JsonData.c_str();    // string to Qstring conversion

	QMap <QString, QVariant> innermap;
	QList<QMap <QString, QVariant>> listofmap;

	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());

	QJsonObject jsonObject = JsonResponse.object();

	QJsonValue ProductVend = jsonObject.value(QString("product_vend_info"));
	if (ProductVend.isArray())
	{
		QJsonArray ProductVendArray = jsonObject["product_vend_info"].toArray();
		if (ProductVendArray.isEmpty())
		{
			cout << "[Transaction::Transaction_Summary] No data - product_vend_info table" << endl;
			return TRANSACTION_SUMMARY_FAILURE;
		}
		foreach(const QJsonValue & value, ProductVendArray)
		{
			QJsonObject Productobj = value.toObject();
		//	PTrans_ID = Productobj["Transaction_ID"].toInt();
		//	PVTime = Productobj["Timestamp"].toInt();
			PVTime = Productobj["Timestamp"].toVariant().toLongLong();
			ProductID = Productobj["Product_ID"].toInt();
			PShelfID = Productobj["ShelfID"].toString();
			PColumnID = Productobj["ColumnID"].toString();
			PPrice = Productobj["Product_price"].toInt();
			PPaymentSource = Productobj["PaymentSource"].toInt();
			CampaignID = Productobj["CampaignId"].toInt();
			DiscountRatio = Productobj["Discount_Ratio"].toDouble();
			PVendorID = Productobj["VendorID"].toString();
			
	//		qDebug() << "TimeStamp" << "\t:" << PVTime;
		//	Temp = InttoStringConvertion(PVTime);
		//	dexBuffer.append("TimeStamp :" + QString::number(PVTime) + "\r\n");
			innermap.insert("TimeStamp", PVTime);

	//		qDebug() << "Device ID	: " << Comm_ID;
		//	dexBuffer.append("DeviceID \t: " + Comm_ID + "\n");
			innermap.insert("DeviceID", Comm_ID);

	//		qDebug() << "Vendor ID	: " << PVendorID;
			//dexBuffer.append("VendorID :" + PVendorID + "\r\n");
			innermap.insert("VendorID", PVendorID);

	//		qDebug() << "Product ID" << "\t:" << ProductID;
			Temp = InttoStringConvertion(ProductID);
		//	dexBuffer.append("ProductID :" + Temp + "\r\n");
			innermap.insert("Product_ID", ProductID);

	//		qDebug() << "Shelf ID" << "\t:" << PShelfID.toUtf8().constData();
		//	dexBuffer.append("ShelfID :" + PShelfID + "\r\n");
			innermap.insert("Shelf_ID", PShelfID);

	//		qDebug() << "Column ID" << "\t:" << PColumnID.toUtf8().constData();
		//	dexBuffer.append("ColumnID :" + PColumnID + "\r\n");
			innermap.insert("Column_ID", PColumnID);

	//	    qDebug() << "Product Price" << "\t:" << PPrice;
			Temp = InttoStringConvertion(PPrice);
		//	dexBuffer.append("Product Price :" + Temp + "\r\n");
			innermap.insert("Product_Price", PPrice);

	//		qDebug() << "productPaymentMethod" << "\t:" << PPaymentSource;
			Temp = InttoStringConvertion(PPaymentSource);
		//	dexBuffer.append("productPaymentMethod :" + Temp + "\r\n");
			innermap.insert("productPaymentMethod", PPaymentSource);

	//		qDebug() << "productDiscountPrice" << "\t:" << DiscountRatio;
			Temp = FloattoStringConvertion(DiscountRatio);
		//	dexBuffer.append("productDiscountPrice :" + Temp + "\r\n");
			innermap.insert("productDiscountPrice", DiscountRatio);

	//		qDebug() << "CampaignId" << "\t:" << CampaignID;
			Temp = InttoStringConvertion(CampaignID);
		//	dexBuffer.append("CampaignId :" + Temp + "\r\n");
			innermap.insert("CampaignId", CampaignID);
		//	dexBuffer.append("\n");
	//		qDebug() << endl;

			listofmap.push_back(innermap);
			innermap.clear();
			 datatranslator.converting_sensor_data_JSON_Indvidual(listofmap, "Trans");
			listofmap.clear();
		}
		/*listofmap.push_back(innermap);
		innermap.clear();*/
	}
	else
	{
		cout << "[Transaction::Transaction_Summary] product_vend_info is not an Array" << endl;
		return TRANSACTION_SUMMARY_FAILURE;
	}


	return TRANSACTION_SUMMARY_SUCCESS;
}

/**
------------------------------------------------------------------------------ -
Process_All_Transaction
------------------------------------------------------------------------------ -
*	@brief		This function processes product_vend_info table information.
*
*	@param		Json string
*
*	@return		None
*
*/
TRANSACTION_SUMMARY_STATUS_CODE Transaction::Process_All_Transaction(string JsonData)
{
	QVariant TProdSold;
	int LcardCount = 0;
	int ProductID = 0;
	int PPsource = 0;
	int ProductSold = 0;
	int PSaleprice = 0;
	int CardSalesCount = 0;
	int CashSalesCount = 0;
	int CoinSalesCount = 0;

	QString PShelfID;
	QString PColumnID;
	QString VendorID;

	QMap <QString, QVariant> innermap;
	QList<QMap <QString, QVariant>> listofmap;

	QString JsonData1 = JsonData.c_str();
	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());
	QJsonObject jsonObject = JsonResponse.object();
	QJsonValue ProductVend = jsonObject.value(QString("product_vend_info"));
	if (ProductVend.isArray())
	{
		QJsonArray ProductVendArray = jsonObject["product_vend_info"].toArray();
		if (ProductVendArray.isEmpty())
		{
			cout << "[Transaction::Process_All_Transaction] No data - product_vend_info table" << endl;
			return PROCESS_ALL_TRANSACTION_FAILURE;
		}
		foreach(const QJsonValue & value, ProductVendArray)
		{
			QJsonObject Productobj = value.toObject();
			ProductID = Productobj["Product_ID"].toInt();
			ProductinfoIter1 = ProductMap1.find(ProductID);
			if (ProductinfoIter1 != ProductMap1.end())
			{
				ProductSold = Productobj["ProductSold"].toInt();
				ProdInnerMapIter1 = ProductinfoIter1->second.find("ProductSold");
				TProdSold = ProdInnerMapIter1->second.toInt() + ProductSold;
				ProductMap1[ProductID]["ProductSold"] = TProdSold;
				
				PPsource = Productobj["PaymentSource"].toInt();
				switch (PPsource)
				{
				case 0:
					CoinSalesCount += 1;
					break;
				case 1:
					CardSalesCount += 1;
					break;
				case 2:
					CashSalesCount += 1;
					break;
				default:
					break;
				}
				ProdInnerMapIter1 = ProductinfoIter1->second.find("CardSalesCount");
				LcardCount = ProdInnerMapIter1->second.toInt() + CardSalesCount;
				ProductMap1[ProductID]["CardSalesCount"] = LcardCount;
				CardSalesCount = 0;
			}
			else
			{
				ProductSold = Productobj["ProductSold"].toInt();
				PSaleprice = Productobj["SalePrice"].toInt();
				PShelfID = Productobj["ShelfID"].toString(); 
				PColumnID = Productobj["ColumnID"].toString();
				VendorID = Productobj["VendorID"].toString();
				PPsource = Productobj["PaymentSource"].toInt();
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("ProductSold", ProductSold));
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("SalePrice", PSaleprice));
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("ShelfID", PShelfID));
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("ColumnID", PColumnID));
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("VendorId", VendorID));
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("PaymentSource", PPsource));
				switch (PPsource)
				{
				case 0:
					CoinSalesCount += 1;
					break;
				case 1:
					CardSalesCount += 1;
					break;
				case 2:
					CashSalesCount += 1;
					break;
				default:
					break;
				}
				ProdInnerMap1.insert(map<std::string, QVariant>::value_type("CardSalesCount", CardSalesCount));
				ProductMap1.insert(map<int, ProdInnerInfo1>::value_type(ProductID, ProdInnerMap1));
				ProdInnerMap1.clear();
				CardSalesCount = 0;
			}
		}
	}
	else
	{
		cout << "[Transaction::Process_All_Transaction] product_vend_info is not an Array" << endl;
		return PROCESS_ALL_TRANSACTION_FAILURE;
	}
	return PROCESS_ALL_TRANSACTION_SUCCESS;
}

/**
------------------------------------------------------------------------------ -
Stock_Summary
------------------------------------------------------------------------------ -
*	@brief		This function generates stock summary.
*
*	@param		Json string
*
*	@return		None
*
*/
TRANSACTION_SUMMARY_STATUS_CODE Transaction::Stock_Summary(string JsonData)
{
	int PStockID = 0;
	int PStock = 0;
	int localProductStock = 0;
	int localProductPrice = 0;
	int localpaymentsource = 0;
	int CurrentStock = 0;
	float TotalCardSales = 0;
	float TotalCoinSales = 0;
	float TotalCashSales = 0;
	QString TValue;
	string timestamp;
	QVariant localProductID;
	QMap <QString, QVariant> innermap;
	QList<QMap <QString, QVariant>> listofmap;
	innermap.clear();

	if (READ_VMINFO_FAILURE == Read_Vminfo_Table(JsonData))
	{
		cout << "[Transaction_Summary] Error in Read_Vminfo_Table - vminfo" << endl;
	}
	//Read_Vminfo_Table(JsonData);
//	Process_All_Transaction(JsonData);
	if (PROCESS_ALL_TRANSACTION_FAILURE == Process_All_Transaction(JsonData))
	{
		cout << "[Transaction_Summary::Stock_Summary] Error in Process_All_Transaction - product_vend_info Table" << endl;
	}
	Datatranslator datatranslator;
	QString JsonData1 = JsonData.c_str();

	QJsonDocument JsonResponse = QJsonDocument::fromJson(JsonData1.toUtf8());

	QJsonObject jsonObject = JsonResponse.object();
	QJsonValue StockObj = jsonObject.value(QString("stockinfo"));

	if (StockObj.isArray())
	{
		QJsonArray StockArray = jsonObject["stockinfo"].toArray();
		if (StockArray.isEmpty())
		{
			cout << "[Transaction::Stock_Summary] No data - stockinfo Table" << endl;
			return STOCK_SUMMARY_FAILURE;
		}
		//dexBuffer.append("******** Stock Report ********* \n\n");
		foreach(const QJsonValue & value, StockArray)
		{
			QJsonObject PStockObj = value.toObject();
			PStockID = PStockObj["Product_ID"].toInt();
			PStock = PStockObj["Stock"].toInt();
#if 0
		//	for (ProductinfoIter1 = ProductMap1.begin(); ProductinfoIter1 != ProductMap1.end(); ProductinfoIter1++)
			{
		//		if (PStockID == ProductinfoIter1->first)
				{
					localProductID = ProductinfoIter1->first;
					time_t now = time(NULL);
			//		cout << "Timestamp	: " << now << endl;
				//	dexBuffer.append("Timestamp \t: " + QString::number(now) + "\n");
					innermap.insert("Timestamp", static_cast<qlonglong> (now));

			//		cout << "Device ID	: " << Comm_ID.toStdString() <<endl;
				//	dexBuffer.append("Device ID \t: " + Comm_ID + "\n");
					innermap.insert("Device ID", Comm_ID);

					ProdInnerMapIter1 = ProductinfoIter1->second.find("VendorId");
			//		cout << "Vendor Id	: " << ProdInnerMapIter1->second.toString().toUtf8().constData() << endl;
				//	dexBuffer.append("Vendor Id \t: " + ProdInnerMapIter1->second.toString() + "\n");
					innermap.insert("Vendor Id", ProdInnerMapIter1->second.toString());

			//		cout << "Product ID	: " << localProductID.toString().toStdString() << endl;
				//	dexBuffer.append("Product ID \t: " + localProductID.toString() + "\n");
					innermap.insert("Product ID", ProductinfoIter1->first);

					ProdInnerMapIter1 = ProductinfoIter1->second.find("ShelfID");
			//		cout << "ShelfId		: " << ProdInnerMapIter1->second.toString().toUtf8().constData() << endl;
				//	dexBuffer.append("ShelfID \t: " + ProdInnerMapIter1->second.toString() + "\n");
					innermap.insert("ShelfID", ProdInnerMapIter1->second.toString());

					ProdInnerMapIter1 = ProductinfoIter1->second.find("ColumnID");
			//		cout << "ColumnId	: " << ProdInnerMapIter1->second.toString().toUtf8().constData() << endl;
					//dexBuffer.append("ColumnID \t: " + ProdInnerMapIter1->second.toString() + "\n");
					innermap.insert("ColumnID", ProdInnerMapIter1->second.toString());

					ProdInnerMapIter1 = ProductinfoIter1->second.find("ProductSold");
					localProductStock = ProdInnerMapIter1->second.toInt();
			//		cout << "productSalesCount : " << ProdInnerMapIter1->second.toString().toUtf8().constData() << endl;
				//	dexBuffer.append("productSalesCount : " + ProdInnerMapIter1->second.toString() + "\n");
					innermap.insert("productSalesCount", ProdInnerMapIter1->second.toString());

					ProdInnerMapIter1 = ProductinfoIter1->second.find("SalePrice");
					localProductPrice = ProdInnerMapIter1->second.toInt();
			//		cout << "productPrice	: " << ProdInnerMapIter1->second.toString().toUtf8().constData() << endl;
					//dexBuffer.append("productPrice \t: " + ProdInnerMapIter1->second.toString() + "\n");
					innermap.insert("productPrice", ProdInnerMapIter1->second.toString());

					ProdInnerMapIter1 = ProductinfoIter1->second.find("PaymentSource");
					localpaymentsource = ProdInnerMapIter1->second.toInt();

					switch (localpaymentsource)
					{
					case 0:
				//		CoinSalesCount += 1;
						TotalCoinSales = (localProductPrice*localProductStock);
						break;
					case 1:
				//		CardSalesCount += 1;
						TotalCardSales = (localProductPrice*localProductStock);
						break;
					case 2:
				//		CashSalesCount += 1;
						TotalCashSales = (localProductPrice*localProductStock);
						break;
					default:
						break;
					}
			//		cout << "productCashlessSales : " << TotalCardSales << endl;
					//dexBuffer.append("productCashlessSales : " + QString::number(TotalCardSales) + "\n");
					innermap.insert("productCashlessSales", TotalCardSales);

					ProdInnerMapIter1 = ProductinfoIter1->second.find("CardSalesCount");
			//		cout << "productCashlessSalesCount : " << ProdInnerMapIter1->second.toInt() << endl;
				//	dexBuffer.append("productCashlessSalesCount : " + QString::number(ProdInnerMapIter1->second.toInt()) + "\n");
					innermap.insert("productCashlessSalesCount", ProdInnerMapIter1->second.toString());
#endif
					CurrentStock = PStock - localProductStock;
			//		cout << "ProductCurrentLevel : " << CurrentStock << endl;
					TValue = InttoStringConvertion(CurrentStock);
				//	dexBuffer.append("ProductCurrentLevel: " + TValue + "\n");
					innermap.insert("ProductCurrentLevel", TValue);
				//	dexBuffer.append("\n");
					TotalCardSales = 0;
					cout << endl;

				

			
			listofmap.push_back(innermap);
			innermap.clear();
			datatranslator.converting_sensor_data_JSON_Indvidual(listofmap, "stock");
		//	innermap.clear();
			listofmap.clear();
		}
	}
	
	else
	{
		cout << "[Transaction::Stock_Summary] stockinfo is not an Array" << endl;
		return STOCK_SUMMARY_FAILURE;
	}
	//Transaction::WriteToFile();		//For debugging purpose
	return STOCK_SUMMARY_SUCCESS;
	
}
		

/**
------------------------------------------------------------------------------ -
InttoStringConvertion
------------------------------------------------------------------------------ -
*	@brief		This function converts integer format to string.
*
*	@param		integer value
*
*	@return		char_string
*
*/
			QString Transaction::InttoStringConvertion(int Value)
			{
				std::string ToString;
				QString Tochar;
				ToString = std::to_string(Value);				// to convert int to string
				Tochar = ToString.c_str();
			
				return Tochar;
			}



QString Transaction::FloattoStringConvertion(float Value)
{
	std::string ToString;
	QString Tochar;
	ToString = std::to_string(Value);				// to convert int to string
	Tochar = ToString.c_str();
	return Tochar;
}




/* eof */