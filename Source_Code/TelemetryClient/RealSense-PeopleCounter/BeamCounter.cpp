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
* the Materials, either expressly, by implication,  inducement, estoppal or
* otherwise. Any license under such intellectual property rights must be express
* and approved by Intel in writing.
*
******************************************************************************/
/**
*
*    @file        BeamCounter.cpp
*
*    @brief
*
*    par  Software License Agreement:
*
*    The software supplied herewith by Intel's vendor to Intel for Sunset Pass
*    Project for use exclusively on PC. The software is owned by the Intel, and
*    is protected under applicable copyright laws.
*
*/
#include "StdAfx.h"
#include "BeamCounter.h"
#include "pxcsession.h"
#include "pxccapture.h"
#include "pxcmetadata.h"
#include <stdio.h>
#include "ConfigurationReader.h"
/* Qt headers for QtSql Database */
#include "QtSql/qsql.h"
#include "QtSql/qsqldatabase.h"
#include "QtSql/qsqlquery.h"
#include "QtSql/qsqlerror.h"
#include "Qtcore/qdebug.h"
#include "DBHandler.h"
/* Own headers */
//#include "common.h"


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <ctime>
#include <time.h>
using namespace cv;
using namespace std;

#define SNPRINTF	_snprintf_s
#define MAX_BUF_LEN		255
DBHandler *m_DB_Handler;
/**
-------------------------------------------------------------------------------
BeamCounter
-------------------------------------------------------------------------------
*	@brief This function is constructor for BeamCounter
*
*	@param void
*
*/
BeamCounter::BeamCounter(void)
{
	m_beamBrokenLeft = 0;
	m_beamBrokenRight = 0;
	m_beamBreakCountLeft = 0;
	m_beamBreakCountRight = 0;
	m_moveRight = 1;
	m_moveLeft = 1;
	m_moveUnknown = 1;
	m_timeBeamBreakLeft = 0;
	m_timeBeamBreakRight = 0;
	m_frameNum = 0;
	arr = new int[5];
	xPointsLeft = new int[5];
	xPointsRight = new int[5];
	yPointMid = new int[5];
	arrMoments = new double[5];
	arrBeamBrokenLeft = new int[3];
	arrBeamBrokenRight = new int[3];
	arrBeamBreakCountLeft = new int[3];
	arrBeamBreakCountRight = new int[3];
	arrMoveLeft = new int[3];
	arrMoveRight = new int[3];
	arrMoveUnknown = new int[3];
	arrTimeBeamBreakLeft = new int[3];
	arrTimeBeamBreakRight = new int[3];
	// Initialize constants

	m_DB_Handler = DBHandler::getInstance();
	for (int i = 0; i < 3; i++)
	{
		arrBeamBrokenLeft[i] = 0;
		arrBeamBrokenRight[i] = 0;
		arrBeamBreakCountLeft[i] = 0;
		arrBeamBreakCountRight[i] = 0;
		arrMoveLeft[i] = 0;
		arrMoveRight[i] = 0;
		arrMoveUnknown[i] = 0;
		arrTimeBeamBreakLeft[i] = 0;
		arrTimeBeamBreakRight[i] = 0;
	}
}

/**
-------------------------------------------------------------------------------
~BeamCounter
-------------------------------------------------------------------------------
*   @brief  This function is destructor for BeamCounter
*
*	@param void
*
*/
BeamCounter::~BeamCounter(void)
{
	if (arr) delete[] arr;
	if (xPointsLeft) delete[] xPointsLeft;
	if (xPointsRight) delete[] xPointsRight;
	if (yPointMid) delete[] yPointMid;
	if (arrMoments) delete[] arrMoments;

	if (arrBeamBrokenLeft) delete[] arrBeamBrokenLeft;
	if (arrBeamBrokenRight) delete[] arrBeamBrokenRight;
	if (arrBeamBreakCountLeft) delete[] arrBeamBreakCountLeft;
	if (arrBeamBreakCountRight) delete[] arrBeamBreakCountRight;
	if (arrMoveLeft) delete[] arrMoveLeft;
	if (arrMoveRight) delete[] arrMoveRight;
	if (arrMoveUnknown) delete[] arrMoveUnknown;

	if (arrTimeBeamBreakLeft) delete[] arrTimeBeamBreakLeft;
	if (arrTimeBeamBreakRight) delete[] arrTimeBeamBreakRight;
	
	contours.clear();
	hierarchy.clear();
	boundRect.clear();
	delete m_DB_Handler;

}

/**
-------------------------------------------------------------------------------
calculateDirection
-------------------------------------------------------------------------------
*          @brief   This method implements  a basic form of break-beam people counting technology
*					using either a single or dual infra-red beams and a reflector or receiver to monitor visitor counts
*					as people break the infra-red beam(s).
*          @param Mat frame
*          @param Mat colorFrame
*          @param double beamWidth
*          @param double leftBeamPoint
*          @param double rightBeamPoint
*          @param string name
*          @param int beamNum
*		   @return void
*/
void BeamCounter::calculateDirection(Mat frame, Mat colorFrame, double beamWidth, double leftBeamPoint, double rightBeamPoint, string name, int beamNum)
{
	m_frameNum++;

	int blobCount = 0;

	bool insert = 0;

	cv::Mat frameDuplicate(colorFrame.rows, colorFrame.cols, CV_8UC1);
	cv::Mat contourImage(colorFrame.rows, colorFrame.cols, CV_8UC3);
	frame.copyTo(frameDuplicate);
	findContours(frameDuplicate, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	cv::cvtColor(frame, contourImage, CV_GRAY2BGR);
	for (int i = 0; i < contours.size(); i++)
	{
		Moments moment = moments((Mat)contours[i]);
		if ((moment.m00 > 5000) && (blobCount < 5))
		{
			arr[blobCount] = i;
			blobCount++;
		}
	}
	for (int i = 0; i < blobCount; i++)
	{
		drawContours(contourImage, contours, arr[i], Scalar(255, 0, 0), CV_FILLED, 8);
		Rect boundRect = boundingRect(Mat(contours[arr[i]]));
		cv::rectangle(contourImage, boundRect, Scalar(0, 255, 0), 10, 0);
		Moments moment = moments((Mat)contours[i]);
		arrMoments[i] = moment.m00;
		xPointsLeft[i] = boundRect.x;
		xPointsRight[i] = boundRect.x + boundRect.width;
		yPointMid[i] = boundRect.y + boundRect.height / 2;
	}
	cv::rectangle(contourImage, Rect((leftBeamPoint - beamWidth)*contourImage.cols, 0, (leftBeamPoint * contourImage.cols - (leftBeamPoint - beamWidth) * contourImage.cols), contourImage.rows), Scalar(0, 0, 255), CV_FILLED);
	cv::rectangle(contourImage, Rect((rightBeamPoint - beamWidth)*contourImage.cols, 0, (rightBeamPoint * contourImage.cols - (rightBeamPoint - beamWidth) * contourImage.cols), contourImage.rows), Scalar(0, 0, 255), CV_FILLED);
	/*Check for Right Beam Break*/
	for (int i = 0; i < blobCount; i++)
	{
		Moments moment = moments((Mat)contours[arr[i]]);
		int xPointLeft = xPointsLeft[i];
		int xPointRight = xPointsRight[i];

		if ((xPointLeft < frame.cols * (rightBeamPoint - beamWidth)) && (xPointRight > frame.cols*rightBeamPoint))
		{
			if (m_beamBrokenRight == 0)
			{
				m_beamBrokenRight = 1;
				m_beamBreakCountRight++;
				break;
			}
			else if (m_beamBrokenRight == 1)
			{
				break;
			}
		}

		if ((((xPointLeft > frame.cols*rightBeamPoint) && (xPointRight > frame.cols * rightBeamPoint)) || (xPointLeft < frame.cols * (rightBeamPoint - beamWidth)) && (xPointRight < frame.cols * (rightBeamPoint - beamWidth))))
		{
			if ((i == blobCount - 1) && (m_beamBrokenRight == 1))
			{
				m_beamBrokenRight = 0;
				m_timeBeamBreakRight = m_frameNum;
			}
		}
	}
	/*Check for Left Beam Break*/
	for (int i = 0; i < blobCount; i++)
	{
		Moments moment = moments((Mat)contours[arr[i]]);
		int xPointLeft = xPointsLeft[i];
		int xPointRight = xPointsRight[i];
		if ((xPointLeft < frame.cols * (leftBeamPoint - beamWidth)) && (xPointRight > frame.cols*leftBeamPoint))
		{
			if (m_beamBrokenLeft == 0)
			{
				m_beamBrokenLeft = 1;
				m_beamBreakCountLeft++;
				break;
			}
			else if (m_beamBrokenLeft == 1)
			{
				break;
			}
		}

		if ((((xPointLeft > frame.cols*leftBeamPoint) && (xPointRight > frame.cols * leftBeamPoint)) || (xPointLeft < frame.cols * (leftBeamPoint - beamWidth)) && (xPointRight < frame.cols * (leftBeamPoint - beamWidth))))
		{
			if ((i == blobCount - 1) && (m_beamBrokenLeft == 1))
			{
				m_beamBrokenLeft = 0;
				m_timeBeamBreakLeft = m_frameNum;
			}
		}
	}
	if ((m_timeBeamBreakRight != 0) && (m_timeBeamBreakLeft != 0))
	{
		if (m_timeBeamBreakRight > m_timeBeamBreakLeft)
		{
			
			m_moveRight++;
			cout << "[RealSense] Info: Moved right: " << m_moveRight << endl;
			insert = 1;

		}
		else if (m_timeBeamBreakRight < m_timeBeamBreakLeft)
		{
				
			m_moveLeft++;
			cout << "[RealSense] Info: Moved left: " << m_moveLeft << endl;
			insert = 1;
		}
		else
		{
			m_moveUnknown++;
			cout << "[RealSense] Info:Moved Unknown: " << m_moveUnknown << endl;
			insert = 1;

		}
		m_timeBeamBreakRight = 0;
		m_timeBeamBreakLeft = 0;
	}
	if (insert == 1) {
		char* strQuery = "INSERT into people_counter (Timestamp,  right_count,left_count,unknown_count) VALUES (:Timestamp, :count_left_to_right, :count_right_to_left,:count_unknown_count)";
		QString tim;
		tim.setNum((long long)time(NULL));
		std::map<std::string, std::string> args_dataUpdation;
		args_dataUpdation[":Timestamp"] = tim.toStdString();
		args_dataUpdation[":count_left_to_right"] = to_string(m_moveRight - 1);
		args_dataUpdation[":count_right_to_left"] = to_string(m_moveLeft - 1);
		args_dataUpdation[":count_unknown_count"] = to_string(m_moveUnknown - 1);	
		m_DB_Handler->execUpdateQuery(QSqlDatabase::database(QString::fromStdString("RS_CONNECTION_NAME"), true), strQuery, args_dataUpdation);

		
	}
	if (enableWindow) {
		putText(colorFrame, "Left: " + to_string(m_moveLeft - 1), Point(25, 25 + 25 * beamNum), FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(155, 250, 120), 0);
		putText(colorFrame, "Right: " + to_string(m_moveRight - 1), Point(colorFrame.cols - 75, 25 + 25 * beamNum), FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(155, 250, 120), 0);
		putText(colorFrame, "Unknown: " + to_string(m_moveUnknown - 1), Point(colorFrame.cols / 2 - 30, 25 + 25 * beamNum), FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(155, 250, 120), 0);
		imshow("contour" + name, contourImage);
		imshow("contour", colorFrame);
	}
	waitKey(30);
}
/**
-------------------------------------------------------------------------------
retrieveString
-------------------------------------------------------------------------------
*          @brief This method char Array to std String.
*          @param int max
*          @param const char * buf
*		   @return void
*
*/
string BeamCounter::retrieveString(const char* buf, int max) {

	size_t len = 0;
	while ((len < max) && (buf[len] != '\0')) {
		len++;
	}

	return string(buf, len);

}
