#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include <cv.h>

using namespace cv;

int main(){
	IplImage* image = 0;
	CvCapture* capture = cvCaptureFromCAM(1);

	while(1){
		cvGrabFrame(capture);
		image = cvRetrieveFrame(capture);
		cvSaveImage("/srv/http/monitor/origin2.jpg", image);
	}
	return 0;
}
