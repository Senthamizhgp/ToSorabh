// RealSensePeopleCounter.h
#pragma once
#ifndef _RSP_H_
#define _RSP_H_
#endif 
/* System headers */
#include <stdio.h>
#include <time.h>
//#include "common.h"
/*opencv header*/
#include <iostream>
#include <sstream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <pxcsensemanager.h>
#include "pxcsession.h"
#include "pxcmetadata.h"
#include "pxcprojection.h"
#include "pxcimage.h"
#include "projection.h"
#include "BeamCounter.h"
//#include "util_cmdline.h"
//#include "util_render.h"
#include <sys/timeb.h>
#include <chrono>

#define SNPRINTF	_snprintf_s
#define MAX_BUF_LEN		255

enum STATUS_CODE {
	SUCCESS = 0,		// /< Success
	RS_FAILURE = 1,	// /< Failed to perform the Scanning
	DB_FAILURE = 2,		// /< DB failure on using QtSql methods
};

#ifdef RSP_EXPORTS
#define RSP_API __declspec(dllexport) 
#else
#define RSP_API __declspec(dllimport) 
#endif

	class   RealSense
	{
	
	
	public:
		  STATUS_CODE RSP_API init_RS_PeopleCounter();
		  STATUS_CODE RSP_API exit_RS_PeopleCounter();
	private:
		   STATUS_CODE init_db();
		   STATUS_CODE execute_query(char *query);
	public:
		PXCSenseManager *pp;
		PXCCaptureManager *cm;	
		QSqlDatabase db;
	private:
		 unsigned short depthValue ;
		 bool enableWindow;
	};
	  
