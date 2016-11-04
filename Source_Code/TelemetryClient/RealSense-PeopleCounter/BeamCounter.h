#pragma once
#ifndef _BR_H_
#define _BR_H_
#endif 
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
#include "pxcprojection.h"
#include "pxccapture.h"
#include "pxcsession.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <iostream>
#include "ConfigurationReader.h"
/* Qt headers for QtSql Database */
#include "QtSql/qsql.h"
#include "QtSql/qsqldatabase.h"
#include "QtSql/qsqlquery.h"
#include "QtSql/qsqlerror.h"
#include "Qtcore/qdebug.h"
#include "DBHandler.h"

//using namespace cv;

#define MAX_BUF_LEN		255

class BeamCounter {
public:
	/**
	-------------------------------------------------------------------------------
	BeamCounter
	-------------------------------------------------------------------------------
	*	@brief This function is constructor for BeamCounter
	*
	*	@param void
	*
	*/
	BeamCounter();

	/**
	-------------------------------------------------------------------------------
	~BeamCounter
	-------------------------------------------------------------------------------
	*   @brief  This function is destructor for BeamCounter
	*
	*	@param void
	*
	*/
	~BeamCounter(void);
	
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
	*
	*/
	void calculateDirection(cv::Mat frame, cv::Mat colorFrame, double beamWidth, double leftBeamPoint, double rightBeamPoint, string name, int beamNum);
	
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
	string retrieveString(const char* buf, int max );
	private:

	int				m_frameNum;
	double			m_timeBeamBreakLeft;
	double			m_timeBeamBreakRight;
	int				m_moveRight;
	int				m_moveLeft;
	int				m_moveUnknown;
	int				m_beamBrokenLeft;
	int				m_beamBrokenRight;
	int				m_beamBreakCountLeft;
	int				m_beamBreakCountRight;
	int				*arr;
	
	int				*xPointsLeft;
	int				*xPointsRight;
	int				*yPointMid;
	double			*arrMoments;
	
	int				*arrBeamBrokenLeft;
	int				*arrBeamBrokenRight;
	int				*arrBeamBreakCountLeft;
	int				*arrBeamBreakCountRight;
	int				*arrMoveRight;
	int				*arrMoveLeft;
	int				*arrMoveUnknown;

	int				*arrTimeBeamBreakLeft;
	int				*arrTimeBeamBreakRight;
	bool			enableWindow = false;
	QSqlDatabase	 db;
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i>		  hierarchy;
	vector<cv::Rect>		  boundRect;

};
