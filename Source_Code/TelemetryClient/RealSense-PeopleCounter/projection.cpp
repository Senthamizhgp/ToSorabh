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
*    @file        Projection.cpp
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
#include "projection.h"
#include "pxcsession.h"
#include "pxccapture.h"
#include "pxcmetadata.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;
//#include "main.h"



/**
-------------------------------------------------------------------------------
Projection
-------------------------------------------------------------------------------
*	@brief This function is constructor for Projection
*
*   @param  PXCImage  *          color
*   @param  PXCSession  *          session
*   @param  PXCCapture::Device  *          device
*   @param  PXCImage::ImageInfo dinfo
*   @param  PXCImage::ImageInfo cinfo
*/
Projection::Projection(PXCImage *color, PXCSession *session, PXCCapture::Device *device, PXCImage::ImageInfo dinfo, PXCImage::ImageInfo cinfo, int depthValue, bool enableGUI) {

/* retrieve the invalid depth pixel values */
invalid_value = device->QueryDepthLowConfidenceValue();
projection = device->CreateProjection();
int size = dinfo.width*dinfo.height;
dcords = new PXCPoint3DF32[size];
ccords = new PXCPointF32[size];
if (dcords) {
	for (int y = 0, k = 0; y < (int)dinfo.height; y++)
		for (int x = 0; x < (int)dinfo.width; x++, k++)
			dcords[k].x = (pxcF32)x, dcords[k].y = (pxcF32)y;
}

uvmap = new PXCPointF32[dinfo.width*dinfo.height];
ij = new PXCPointF32[dinfo.width*dinfo.height];

invuvmap = new PXCPointF32[cinfo.width*cinfo.height];
PXCImage::ImageInfo drawDepthInfo;
memset(&drawDepthInfo, 0, sizeof(drawDepthInfo));
drawDepthInfo.width = dinfo.width;
drawDepthInfo.height = dinfo.height;
drawDepthInfo.format = PXCImage::PIXEL_FORMAT_RGB32;
drawDepth = 0;
drawDepth = session->CreateImage(&drawDepthInfo);
uvMapVec.resize(dinfo.width * dinfo.height);
depthVal = depthValue;
enableWindow = enableGUI;
}

/**
-------------------------------------------------------------------------------
Projection
-------------------------------------------------------------------------------
*	@brief This function is destructor for Projection
*
*/
Projection::~Projection(void) {
if (uvmap) delete[] uvmap;
if (ij) delete[] ij;
if (invuvmap) delete[] invuvmap;
//if (projection) projection->Release();
if (ccords) delete[] ccords;
if (dcords) delete[] dcords;
//if (ColorMat) delete ColorMat;

if (drawDepth) drawDepth->Release();
uvMapVec.clear();
if (projection) projection->Release();

}
/**
-------------------------------------------------------------------------------
PlotXY
-------------------------------------------------------------------------------
*	@brief This is plotting function
*    @param pxcBYTE  *          cpixels
*    @param int xx
*    @param int yy
*    @param int cwidth
*    @param int cheight
*    @param int dots
*    @param int color
*
*/
void Projection::PlotXY(pxcBYTE *cpixels, int xx, int yy, int cwidth, int cheight, int dots, int color) {
//cout << color << endl;
if (xx < 0 || xx >= cwidth || yy < 0 || yy >= cheight)
{
	return;
}
int lyy = yy * cwidth;
int xxm1 = (xx > 0 ? xx - 1 : xx), xxp1 = (xx < (int)cwidth - 1 ? xx + 1 : xx);
int lyym1 = yy > 0 ? lyy - cwidth : lyy, lyyp1 = yy < (int)cheight - 1 ? lyy + cwidth : lyy;

if (dots >= 9)  /* 9 dots */
{
	cpixels[(lyym1 + xxm1) * 4 + color] = 0xFF;
	cpixels[(lyym1 + xxp1) * 4 + color] = 0xFF;
	cpixels[(lyyp1 + xxm1) * 4 + color] = 0xFF;
	cpixels[(lyyp1 + xxp1) * 4 + color] = 0xFF;
}
if (dots >= 5)  /* 5 dots */
{
	cpixels[(lyym1 + xx) * 4 + color] = 0xFF;
	cpixels[(lyy + xxm1) * 4 + color] = 0xFF;
	cpixels[(lyy + xxp1) * 4 + color] = 0xFF;
	cpixels[(lyyp1 + xx) * 4 + color] = 0xFF;
}
cpixels[(lyy + xx) * 4 + color] = 0xFF; /* 1 dot */

}

/**
-------------------------------------------------------------------------------
DepthToColorByUVMAP
-------------------------------------------------------------------------------
*	@brief This is Depth/Color Coordinates Conversion  function
*    @param PXCImage  *          color
*    @param PXCImage  *          depth
*    @param int dots
*
*/
Mat Projection::DepthToColorByUVMAP(PXCImage *color, PXCImage *depth, int dots) {
PXCImage::ImageInfo cinfo = color->QueryInfo();

cv::Mat ColorMat(cinfo.height, cinfo.width, CV_8UC3);
cv::Mat FilteredDepthMat(depth->QueryInfo().height, depth->QueryInfo().width, CV_8UC3);
cv::Mat DepthMat(depth->QueryInfo().height, depth->QueryInfo().width, CV_8UC3);
cv::Mat SegmentedMat(depth->QueryInfo().height, depth->QueryInfo().width, CV_8UC1);

/* Retrieve the color pixels */
PXCImage::ImageData cdata;
pxcStatus sts = color->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB24, &cdata);
if (sts >= PXC_STATUS_NO_ERROR) {
	//int cwidth = cdata.pitches[0] / sizeof(int); /* aligned width */

	pxcBYTE *cpixels = cdata.planes[0];

	PXCImage::ImageInfo dinfo = depth->QueryInfo();

	/* Retrieve the depth pixels and uvmap */
	PXCImage::ImageData ddata;
	if (depth->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH, &ddata) >= PXC_STATUS_NO_ERROR) {
		pxcU16 *dpixels = (pxcU16*)ddata.planes[0];
		int   dpitch = ddata.pitches[0] / sizeof(pxcU16); /* aligned width */
		if (enableWindow) {
			namedWindow("Depth Value", CV_WINDOW_NORMAL);
			createTrackbar("Dst (ft)", "Depth Value", &depthVal, 8);
		}

		short *depthPixels = (short*)ddata.planes[0];
		for (int y = 0; y < dinfo.height; y++)
		{
			for (int x = 0; x < dinfo.width; x++)
			{
				short d = depthPixels[y*dpitch + x];
				DepthMat.at<cv::Vec3b>(y, x) = d;
				//if(d < 2750)
				if (d < depthVal * 305)
					FilteredDepthMat.at<cv::Vec3b>(y, x) = d;
			}
		}

		if (enableWindow) {
			imshow("Unfiltered Depth Image", DepthMat);
			imshow("Filtered Depth Image", FilteredDepthMat);
			imshow("Segmented Image", SegmentedMat);
		}
		inRange(FilteredDepthMat, Scalar(1, 0, 0), Scalar(255, 0, 0), SegmentedMat);
		cvWaitKey(30);

		sts = projection->QueryUVMap(depth, uvmap);

		if (sts >= PXC_STATUS_NO_ERROR) {
			int uvpitch = depth->QueryInfo().width;
			memcpy(ColorMat.data, cpixels, sizeof(unsigned char) * 3 * cinfo.width * cinfo.height);

			/* Draw dots onto the color pixels */
			for (int y = 0; y < (int)dinfo.height; y++) {
				for (int x = 0; x < (int)dinfo.width; x++) {
					pxcU16 d = dpixels[y*dpitch + x];
					if (d == invalid_value) continue; // no mapping based on unreliable depth values

					float uvx = uvmap[y*uvpitch + x].x;
					float uvy = uvmap[y*uvpitch + x].y;
					int xx = (int)(uvx * cinfo.width + 0.5f), yy = (int)(uvy * cinfo.height + 0.5f);
					if ((xx) >= 0 && (xx) < cinfo.width && yy >= 0 && yy < cinfo.height)
					{
						ColorMat.at<cv::Vec3b>(yy, xx)[0] = 255;
						ColorMat.at<cv::Vec3b>(yy, xx)[1] = 0;
						ColorMat.at<cv::Vec3b>(yy, xx)[2] = 0;
					}
				}
			}
		}
		depth->ReleaseAccess(&ddata);
	}
	color->ReleaseAccess(&cdata);
}

//return ColorMat;
return SegmentedMat;
}

/**
-------------------------------------------------------------------------------
ColorToDepthByQueryUVMap
-------------------------------------------------------------------------------
*	@brief This is Color/Depth Coordinates Conversion  function
*    @param PXCImage  *          color
*    @param PXCImage  *          depth
*    @param int dots
*
*/
PXCImage* Projection::ColorToDepthByQueryUVMap(PXCImage *color, PXCImage *depth)
{
//if ( !drawDepth || !projection ) return 0;

PXCImage::ImageInfo drawDepthInfo = drawDepth->QueryInfo();
PXCImage::ImageData drawDepthDat;
if (PXC_STATUS_NO_ERROR > drawDepth->AcquireAccess(PXCImage::ACCESS_READ_WRITE, drawDepthInfo.format, &drawDepthDat))
	return 0;

/* Retrieve the color pixels */
PXCImage::ImageData cdata;
pxcStatus sts = color->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB32, &cdata);
if (sts >= PXC_STATUS_NO_ERROR) {
	sts = projection->QueryUVMap(depth, &uvMapVec[0]);
	if (sts >= PXC_STATUS_NO_ERROR) {
		PXCImage::ImageInfo cinfo = color->QueryInfo();
		PXCImage::ImageInfo dinfo = depth->QueryInfo();
		for (pxcI32 y = 0; y < dinfo.height; y++) {
			pxcI32* pDrawDepthDat = (pxcI32*)(drawDepthDat.planes[0] + y * drawDepthDat.pitches[0]);
			PXCPointF32 *uvTest = &uvMapVec[y * dinfo.width];
			for (pxcI32 x = 0; x < dinfo.width; x++) {
				pDrawDepthDat[x] = 0;
				if (uvTest[x].x >= 0 && uvTest[x].x < 1 && uvTest[x].y >= 0 && uvTest[x].y < 1) {
					pxcI32 tmpColor = *(pxcI32*)(cdata.planes[0] + (int)(uvTest[x].y * cinfo.height) * cdata.pitches[0] + 4 * (int)(uvTest[x].x * cinfo.width));
					pDrawDepthDat[x] = tmpColor;
				}
			}
		}
	}
	color->ReleaseAccess(&cdata);
}
drawDepth->ReleaseAccess(&drawDepthDat);

return drawDepth;
}
