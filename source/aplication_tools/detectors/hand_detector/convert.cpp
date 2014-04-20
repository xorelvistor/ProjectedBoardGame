#include "hand_detector.h"
using namespace cv;
Mat src;
extern VideoCapture cam;
int detect(){
   
	while(waitKey(100) != 27) {
		cam.read(src);
		if (src.empty())
			return -1;
		blur( src, src, Size(3,3) );
		Mat hsv;
		cvtColor(src, hsv, CV_BGR2HSV);
		Mat bw;
		//inRange(hsv, Scalar(0, 40, 60), Scalar(20, 150, 255), bw);
		inRange(hsv, Scalar(0, 10, 60), Scalar(20, 150, 255), bw);
		imshow("src", src);
		imshow("dst", bw);
	}
    return 0;
}