#include "hand_detector.h"
#include "..\hand_detector\camera_calibration.h"
using namespace cv;
using namespace std;  

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

 int main () {    
	cv::Mat camera, coeff, homography;
	/*
	if(!systemCalibration(camera,coeff,homography)) {
		cout << "chyba kalibrace" << endl;
		return -1;
	}
	*/
    cv::namedWindow( "Final", 1 );  
    cv::Mat img,hsv,binary;  
    
    VideoCapture cap(0);    
    while(true) {   
        //grab frame from camera  
        cap >> img;  
  
        //convert RGB image into HSV image  
        cvtColor(img, hsv, CV_BGR2HSV);  
      
        //get binary image 
		imshow("HSV",hsv); 
        inRange(hsv, Scalar(0, 80, 100), Scalar(20, 150, 255), binary);  
      
        //find contours from binary image  
        int i;  
        vector< vector<Point> > contours;  
        findContours(binary, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE); //find contours  
        vector<double> areas(contours.size());  
        //find largest contour area  
        for(i = 0; i < contours.size(); i++) {  
            areas[i] = contourArea(Mat(contours[i]));  
        }  
        //get index of largest contour  
        double max;  
        Point maxPosition;  
        minMaxLoc(Mat(areas),0,&max,0,&maxPosition);  
        //draw largest contour.  
        drawContours(binary, contours, maxPosition.y, Scalar(255), CV_FILLED);  
      
		imshow("Binary",binary);
        //draw bounding rectangle around largest contour  
        Point center;  
        Rect r;  
        if (contours.size() >= 1) {  
            r = boundingRect(contours[maxPosition.y]);  
            rectangle(img, r.tl(),r.br(), CV_RGB(255, 0, 0), 3, 8, 0); //draw rectangle  
        }  
        //get centroid  
        center.x = r.x + (r.width/2);  
        center.y= r.y + (r.height/2);  
        //print x and y co-ordinates on image  
        char x[15],y[6];  
        itoa(center.x,x,10);  
        itoa(center.y,y,10);  
        strcat(x,",");  
        putText(img, strcat(x,y), center, FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(255,0,0), 1, CV_AA);  
        imshow("Final",img);  
  
        //wait for key press  
        int c = cvWaitKey(10);    
        if((char)c==27 ) break;  

		circle(binary,center,10,cvScalar(255),-1);
		imshow("Binary",binary);
    }    
    cv::destroyAllWindows();  
    VideoCapture release;    
    return 0;    
 }  