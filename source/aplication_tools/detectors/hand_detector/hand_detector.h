#ifndef HAND_DETECTOR_H
#define HAND_DETECTOR_H

#include <opencv2\opencv.hpp>
#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

using namespace cv;
using namespace std;

double dist(cv::Point x,cv::Point y);
pair<cv::Point,double> circleFromPoints(Point p1, Point p2, Point p3);
bool detectHand(cv::VideoCapture& cap, cv::vector<cv::Point2f>& hits);
#endif
