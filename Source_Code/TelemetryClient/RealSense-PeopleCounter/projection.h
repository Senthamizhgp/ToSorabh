#include "pxcprojection.h"
#include "pxccapture.h"
#include "pxcsession.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

//using namespace cv;
//using namespace std;

class Projection {
public:

	Projection(PXCImage *color, PXCSession *session, PXCCapture::Device *device, PXCImage::ImageInfo dinfo, PXCImage::ImageInfo cinfo,int depthValue,bool enableGUI);
	~Projection(void);
	cv::Mat DepthToColorByUVMAP(PXCImage *color, PXCImage *depth, int dots);
	void ColorToDepthByInvUVMAP(PXCImage *color, PXCImage *depth, int dots);
	PXCImage* ColorToDepthByQueryUVMap( PXCImage *color, PXCImage *depth );
	
private:

    PXCProjection	*projection;
    pxcU16			invalid_value; /* invalid depth values */
	PXCPoint3DF32	*dcords;
	PXCPointF32		*ccords;
    PXCPointF32		*uvmap;
	PXCPointF32     *ij;
	PXCPointF32		*invuvmap;
	std::vector<PXCPointF32>	uvMapVec;
	PXCImage		*drawDepth;
	int depthVal = 2;//This value is used to filter the stream by depth value
	bool enableWindow = false;
	void PlotXY(pxcBYTE *cpixels, int xx, int yy, int cwidth, int cheight, int dots, int color);

    /* prohibit using copy & assignment constructors */
    Projection(Projection&) {}
    Projection& operator= (Projection&) { return *this; }
};
