#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

//This function returns the square of the euclidean distance between 2 points.
double dist(Point x,Point y)
{
	return (x.x-y.x)*(x.x-y.x)+(x.y-y.y)*(x.y-y.y);
}


//This function returns the radius and the center of the circle given 3 points
//If a circle cannot be formed , it returns a zero radius circle centered at (0,0)
pair<Point,double> circleFromPoints(Point p1, Point p2, Point p3)
{
	double offset = pow(p2.x,2) +pow(p2.y,2);
	double bc =   ( pow(p1.x,2) + pow(p1.y,2) - offset )/2.0;
	double cd =   (offset - pow(p3.x, 2) - pow(p3.y, 2))/2.0;
	double det =  (p1.x - p2.x) * (p2.y - p3.y) - (p2.x - p3.x)* (p1.y - p2.y); 
	double TOL = 0.0000001;
	if (abs(det) < TOL) {
		//cout<<"POINTS TOO CLOSE"<<endl;
		return make_pair(Point(0,0),0);
	}

	double idet = 1/det;
	double centerx =  (bc * (p2.y - p3.y) - cd * (p1.y - p2.y)) * idet;
	double centery =  (cd * (p1.x - p2.x) - bc * (p2.x - p3.x)) * idet;
	double radius = sqrt( pow(p2.x - centerx,2) + pow(p2.y-centery,2));

	return make_pair(Point(centerx,centery),radius);
}


//The main function :D
bool detectHand(VideoCapture& cap, vector<Point2f>& hits) {
	cout << CV_VERSION << endl;
	if (!cap.isOpened()) {
		cout << "kamera neni online" << endl;
		return false;
	}
	Mat frame;
	Mat back;
	Mat fore;
	vector<pair<Point,double> > palm_centers;
	BackgroundSubtractorMOG2 bg;
	bg.set("nmixtures",3);
	bg.set("detectShadows",false);
	
	
	namedWindow("Frame");
	//namedWindow("Background");
	int backgroundFrame=10;
	double c_max = 6000.0;
	double c_min = 1000.0;
	double obsah;
	int klavesa = 0;
	bool p = false;
	
	cout << "detekce" << endl;
	//while(hits.empty()) {
		//Get the frame
		cap >> frame;
		//cvtColor(frame,frame,CV_BGR2HLS);

		//Update the current background model and get the foreground
		//if(backgroundFrame > 0) { 
		//for(;backgroundFrame > 0;) {
			//cap >> frame;
			bg.operator ()(frame,fore);
		//	backgroundFrame--;
		//}
		//}
		waitKey(100);

		cap >> frame;

		bg.operator ()(frame,fore,0);

		//Find the contours in the foreground
		
		//if (backgroundFrame > 0) {
			//cout << "Zbyva "<< backgroundFrame << endl;
			//putText(frame, "Recording Background", cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
			//imshow("Frame",frame);
		//	continue;
		//}

		//Get background image to display it
		bg.getBackgroundImage(back);

		//Enhance edges in the foreground by applying erosion and dilation
		erode(fore,fore,Mat());
		dilate(fore,fore,Mat());
		
		//imshow("fore",fore);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		vector<vector<Point> > tcontours;
		findContours( fore, contours, hierarchy, RETR_TREE, CV_CHAIN_APPROX_TC89_KCOS);

		if(waitKey(20) == 's') {
			imwrite("hand.jpg",frame);
		}
		for(int i=0;i<contours.size();i++) {
			//Ignore all small insignificant areas
			obsah = contourArea(contours[i]);
			if(obsah >= c_min && obsah < c_max) {
				//Draw contour
				
				tcontours.push_back(contours[i]);
				//drawContours(frame,tcontours,-1,cv::Scalar(255,255,255),2);
				cout << "Obsah kontury (" << i << ") je " << obsah << endl;
				//Detect Hull in current contour
				
				vector<vector<Point> > hulls(1);
				vector<vector<int> > hullsI(1);
				convexHull(Mat(tcontours[0]),hulls[0],false);
				convexHull(Mat(tcontours[0]),hullsI[0],false);
				drawContours(frame,hulls,-1,cv::Scalar(255,0,0),2);
				
				//Find minimum area rectangle to enclose hand
				RotatedRect rect=minAreaRect(Mat(tcontours[0]));

				//Find Convex Defects
				vector<Vec4i> defects;
				
				if(hullsI[0].size()>0) {
					Point2f rect_points[4];
					rect.points( rect_points );
					//for( int j = 0; j < 4; j++ )
						//line( frame, rect_points[j], rect_points[(j+1)%4], Scalar(255,0,0), 1, 8 );
						
					Point rough_palm_center;
					convexityDefects(tcontours[0], hullsI[0], defects);
					if(defects.size()>=3) {
						vector<Point> palm_points;
						for(int j=0;j<defects.size();j++) {
							int startidx=defects[j][0];
							Point ptStart( tcontours[0][startidx] );
							int endidx=defects[j][1];
							Point ptEnd( tcontours[0][endidx] );
							int faridx=defects[j][2];
							Point ptFar( tcontours[0][faridx] );
							//Sum up all the hull and defect points to compute average
							rough_palm_center += ptFar + ptStart + ptEnd;
							//rough_palm_center = rect.center;
							palm_points.push_back(ptFar);
							palm_points.push_back(ptStart);
							palm_points.push_back(ptEnd);
						}

						//Get palm center by 1st getting the average of all defect points, this is the rough palm center,
						//Then U chose the closest 3 points ang get the circle radius and center formed from them which is the palm center.
						rough_palm_center.x/=defects.size()*3;
						rough_palm_center.y/=defects.size()*3;
						Point closest_pt=palm_points[0];
						vector<pair<double,int> > distvec;
						for(int i=0;i<palm_points.size();i++)
							distvec.push_back(make_pair(dist(rough_palm_center,palm_points[i]),i));
						sort(distvec.begin(),distvec.end());

						//Keep choosing 3 points till you find a circle with a valid radius
						//As there is a high chance that the closes points might be in a linear line or too close that it forms a very large circle
						pair<Point,double> soln_circle;
						for(int i=0;i+2<distvec.size();i++) {
							Point p1=palm_points[distvec[i+0].second];
							Point p2=palm_points[distvec[i+1].second];
							Point p3=palm_points[distvec[i+2].second];
							soln_circle=circleFromPoints(p1,p2,p3);//Final palm center,radius
							if(soln_circle.second!=0)
								break;
						}

						//Find avg palm centers for the last few frames to stabilize its centers, also find the avg radius
						palm_centers.push_back(soln_circle);
						if(palm_centers.size()>10)
							palm_centers.erase(palm_centers.begin());
						
						Point palm_center;
						double radius=0;
						for(int i=0;i<palm_centers.size();i++) {
							palm_center+=palm_centers[i].first;
							radius+=palm_centers[i].second;
						}
						palm_center.x/=palm_centers.size();
						palm_center.y/=palm_centers.size();
						radius/=palm_centers.size();

						//Draw the palm center and the palm circle
						//The size of the palm gives the depth of the hand
						circle(frame,palm_center,5,Scalar(144,144,255),3);
						circle(frame,palm_center,radius,Scalar(144,144,255),2);

						//Detect fingers by finding points that form an almost isosceles triangle with certain thesholds
						int no_of_fingers=0;
						for(int j=0;j<defects.size();j++) {
							int startidx=defects[j][0];
							Point ptStart( tcontours[0][startidx] );
							int endidx=defects[j][1];
							Point ptEnd( tcontours[0][endidx] );
							int faridx=defects[j][2];
							Point ptFar( tcontours[0][faridx] );
							//X o--------------------------o Y
							double Xdist=sqrt(dist(palm_center,ptFar));
							double Ydist=sqrt(dist(palm_center,ptStart));
							double length=sqrt(dist(ptFar,ptStart));

							double retLength=sqrt(dist(ptEnd,ptFar));
							//Play with these thresholds to improve performance
							if( length <= 3*radius && Ydist >= 0.4*radius &&
								length >= 10 && retLength >= 10 &&
								max(length,retLength)/min(length,retLength) >= 0.8) {

								if(min(Xdist,Ydist)/max(Xdist,Ydist) <= 0.8) {
									if((Xdist >= 0.1 * radius && Xdist <= 1.3 * radius &&
										Xdist < Ydist) || (Ydist >= 0.1 * radius &&
										Ydist <= 1.3 * radius && Xdist > Ydist)) {
										circle(frame,ptEnd,4,Scalar(0,255,0),-1);
										hits.push_back(ptEnd);
										hits.push_back(palm_center);
										hits.push_back(ptStart);
										circle(frame,ptStart,4,Scalar(20,20,20),-1);
										line( frame, ptEnd, ptFar, Scalar(20,20,20), 1 );
										no_of_fingers++;
										}
								}
							}


						}
						
						no_of_fingers=min(5,no_of_fingers);
						//cout<<"NO OF FINGERS: "<<no_of_fingers<<endl;
						//mouseTo(palm_center.x,palm_center.y);//Move the cursor corresponding to the palm
/*
						if(no_of_fingers<4)//If no of fingers is <4 , click , else release
							mouseClick();
						else
							mouseRelease();
						*/
					}
				}
				

			}
		}
		if (!p)
			cv::imshow("Frame",frame);

		//cv::imshow("Background",back);
		klavesa = waitKey(20);
		if(klavesa == 27)
			return false;
		else if (klavesa == '+') {
			c_max += 1000.0;
			cout << "max = " << c_max << endl;
		} else if (klavesa == '-') {
			c_max -= 1000.0;
			cout << "max = " << c_max << endl;
		} else if (klavesa == '8') {
			c_min += 1000.0;
			cout << "min = " << c_min << endl;
		} else if (klavesa == '2') {
			c_min -= 1000.0;
			cout << "min = " << c_min << endl;
		} else if (klavesa == 'i') {
			cout << "BRIGHTNESS: " << cap.get(CV_CAP_PROP_BRIGHTNESS) << endl;
			cout << "CONTRAST: " << cap.get(CV_CAP_PROP_CONTRAST) << endl;
			cout << "SATURATION: " << cap.get(CV_CAP_PROP_SATURATION) << endl;
			cout << "EXPOSURE: " << cap.get(CV_CAP_PROP_EXPOSURE) << endl;
			cout << "GAIN: " << cap.get(CV_CAP_PROP_GAIN) << endl;
		} else if (klavesa == 'p') {
			p = !p;
			waitKey();
		}
	//}
	cout << hits << endl;
	return true;
}
/*
int main () {
	VideoCapture cap(0);
	Mat frame;
	vector<Point2f> hits;

	if (cap.isOpened()) {
		waitKey(1000);
		cap >> frame;
		waitKey(1000);
		detectHand(cap,hits);
		if(!hits.empty())
			cout << hits;
	}
}
*/
