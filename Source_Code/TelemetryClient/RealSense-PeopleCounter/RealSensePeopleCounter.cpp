/******************************************************************************
*
* INTEL CONFIDENTIAL
* Copyright 2016 Intel Corporation All Rights Reserved.
*
* The source code contained or described herein and all documents related to the
* source code ("Material") are  owned by Intel Corporation or its suppliers or
* licensor. Title to the Material remains with Intel Corporation or its suppliers
* and licensor. The Material contains trade secrets and proprietary and
* confidential information of Intel or its suppliers and licensor. The Material
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
*    @file        RealSensePeopleCounter.cpp
*
*    @brief        This file contains definition of all APIs which will be exported
*                by the RealSense module.
*
*/
#include "stdafx.h"
#include "ConfigurationReader.h"
#include "RealSensePeopleCounter.h"
/* Qt headers for QtSql Database */
#include "QtSql/qsql.h"
#include "QtSql/qsqldatabase.h"
#include "QtSql/qsqlquery.h"
#include "QtSql/qsqlerror.h"
#include "Qtcore/qdebug.h"
#include "DBHandler.h"
using namespace cv;
using namespace std;

BeamCounter beamCounter;
DBHandler *m_DBHandler;
/* Create an instance with a connection name for DB */



/**
-------------------------------------------------------------------------------
Method: init_RS_PeopleCounter
-------------------------------------------------------------------------------
*   @brief      This function is Entry point for Real sense Module.It
*               initialize RealSense Module  by creating PXCSenseManager  and
*               Acquire Frames using  PXC Capture Manager to process left / right movement.
*    @return       int
*/
STATUS_CODE RealSense::init_RS_PeopleCounter()
{
	cout << "[RealSense] Info:init is started" << endl;
	STATUS_CODE code = init_db();
	if (code != SUCCESS) {
		return code;
	}

	pp = PXCSenseManager::CreateInstance();
	cout << pp << endl;
	if (!pp)
	{
		cout << "[RealSense] ERROR:Unable to create Sense Manager" << endl;
		return RS_FAILURE;
	}
	pp->EnableStream(PXCCapture::STREAM_TYPE_COLOR);
	pp->EnableStream(PXCCapture::STREAM_TYPE_DEPTH);
	//pp->EnableStream(PXCCapture::STREAM_TYPE_IR);
	cm = pp->QueryCaptureManager();
	pxcStatus sts;
	do
	{
		PXCVideoModule::DataDesc desc =	{};
		desc.deviceInfo.streams = PXCCapture::STREAM_TYPE_COLOR;
		pp->EnableStreams(&desc);
		sts = pp->Init();
		if (sts < PXC_STATUS_NO_ERROR)
		{
			cout << "[RealSense] ERROR:Unable to initialize pipeline" << endl;
			return RS_FAILURE;
		}
		for (int n_frames = 0;
			;
			n_frames++)
		{
			sts = pp->AcquireFrame();
			if (sts < PXC_STATUS_NO_ERROR)
			{
				if (sts == PXC_STATUS_STREAM_CONFIG_CHANGED)
				{
					cout << "[RealSense] ERROR:Stream config changed" << endl;
					pp->Close();
				}
			}
			PXCCapture::Sample *sample = pp->QuerySample();
			pp->QueryCaptureManager()->QueryDevice()->SetMirrorMode(PXCCapture::Device::MirrorMode::MIRROR_MODE_HORIZONTAL);
			if (sample)
			{
				int depthHeight = sample->depth->QueryInfo().height;
				int depthWidth = sample->depth->QueryInfo().width;
				int colorHeight = sample->color->QueryInfo().height;
				int colorWidth = sample->color->QueryInfo().width;
				cv::Mat DepthMat(depthHeight, depthWidth, CV_16U);
				cv::Mat InfraredMat(depthHeight, depthWidth, CV_8UC1);
				cv::Mat RGB_IR(depthHeight, depthWidth, CV_8UC3);
				cv::Mat segmentedImg(colorHeight, colorWidth, CV_8UC1);
				cv::Mat erodeKernel = getStructuringElement(MORPH_RECT, Size(9, 9));
				Projection projection(sample->color, pp->QuerySession(), pp->QueryCaptureManager()->QueryDevice(), sample->depth->QueryInfo(), sample->color->QueryInfo(), depthValue, enableWindow);
				RGB_IR = projection.DepthToColorByUVMAP(sample->color, sample->depth, 1);
				inRange(RGB_IR, Scalar(255, 0, 0), Scalar(255, 0, 0), segmentedImg);
				//Threshold the image
				cv::dilate(segmentedImg, segmentedImg, erodeKernel);
				if(enableWindow){
				imshow("Segmented Image", segmentedImg);
				} 
				waitKey(30);
				beamCounter.calculateDirection(segmentedImg, RGB_IR, 0.01, 0.45, 0.55, "Middle", 1);
			}
			pp->ReleaseFrame();
		}
	} while (sts == PXC_STATUS_STREAM_CONFIG_CHANGED);
	return SUCCESS;
}
/**
-------------------------------------------------------------------------------
 Init Method
-------------------------------------------------------------------------------
* @brief This function initialize real sense  module by creating table in DB if not exist
*
*	@return			Refer this Enum @ref STATUS_CODE
*
*/
STATUS_CODE RealSense::init_db()
{

	ConfigurationReader *configReader = ConfigurationReader::getInstance();
	QSqlDatabase db = QSqlDatabase::addDatabase(QString::fromStdString("QMYSQL"), QString::fromStdString("RS_CONNECTION"));
	/* Connect to db and create table, if doesn't exists*/
	db.setDatabaseName(QString::fromStdString(configReader->m_DatabaseName));
	db.setHostName(QString::fromStdString(configReader->m_DatabaseHostName));
	db.setUserName(QString::fromStdString(configReader->m_DatabaseUserName));
	db.setPassword(QString::fromStdString(configReader->m_DatabasePassword));
	db.setPort(configReader->m_DatabasePort);
	cout << configReader->m_DatabasePassword;
	enableWindow = false;
	depthValue =  configReader->m_RealSenseDepthVal;
	if(configReader->m_RealSenseEnableWindow==1){
		enableWindow = true;
	}
	char buf[MAX_BUF_LEN];
	m_DBHandler = DBHandler::getInstance();
	if (!db.open())
	{
#ifndef UNIT_TEST
		qDebug() << "[RealSense] ERROR:Failed to open database: " + db.lastError().text();
#endif
		return DB_FAILURE;	/* Err - failed to open db */
	}
	else
	{
	
		SNPRINTF(buf, MAX_BUF_LEN, "CREATE TABLE IF NOT EXISTS people_counter (    left_count int(50) NOT NULL DEFAULT 0,   right_count int(50) NOT NULL DEFAULT 0,   unknown_count int(50) NOT NULL DEFAULT 0,   Timestamp bigint(20) DEFAULT NULL)");
		if (DB_FAILURE == execute_query(buf))
		{
#ifndef UNIT_TEST
			qDebug() << "[RealSense] ERROR: Failed to create table\n";
#endif
			return DB_FAILURE;
		}
	}
	return SUCCESS;
}

/**
* @brief This function execute the query from calling funcion.
*
*	@param query	Query to execute on DB
*
*	@return			Refer this Enum @ref STATUS_CODE
*
*/
 STATUS_CODE RealSense::execute_query(char *query)
{
	 if (query != NULL)
	 {
		 map<string, string> dummy;
		 if (m_DBHandler->execUpdateQuery(QSqlDatabase::database(QString::fromStdString("RS_CONNECTION"), true), query, dummy) == UPDATE_QUERY_FAILURE)
			 return DB_FAILURE;
	 }
	 else
	 {
		 return DB_FAILURE;		/* NULL argument check */
	 }
	 return SUCCESS;
	
}
 
/**
-------------------------------------------------------------------------------
Method: exit_RS_PeopleCounter
-------------------------------------------------------------------------------
*    @brief      This function is Exit point for Real sense Module.It
*                deletes Object created for RealSense Module .
*    @return       int
*/
STATUS_CODE  RealSense::exit_RS_PeopleCounter()
{
	qDebug() << "[RealSense] Info:Exit RealSense Module";
	if (db.isOpen())
	{
		db.close();
	}
	QSqlDatabase::removeDatabase("RS_CONNECTION");
	beamCounter.~BeamCounter();
	return SUCCESS;
}
