#include "camera_calibration.h"

int main () {
	cv::Mat camera, distorsionCoeff, homography;
	cv::Mat picture, view;
	picture = imread("clov4.png");
	moveWindow("Projector",PROJECTOR_X,PROJECTOR_Y);
	resizeWindow("Projector",800,600);
	namedWindow("Projector",CV_WINDOW_KEEPRATIO);

	namedWindow("Kamera");
	moveWindow("Kamera", 100, 100);
	cout << "***** Main *****" << endl;
	if(systemCalibration(camera, distorsionCoeff, homography)) {
		cout << "\n\nkalibrace probehla ok\n\n";
	
	/** Pouziti homografie **/
	VideoCapture cam(0);
	
	vector<Point2f> venku;
	vector<Point2f> uvnitr;
	cv::Point2f bod_v_kamere(320,240);
	venku.push_back(bod_v_kamere);

	uvnitr.push_back(Point());

	cout << "***** Transformace bodu z kamery do okna *****" <<endl << endl;
	
	perspectiveTransform(venku,uvnitr,homography);
	
	cout << "bod v kamere: " << bod_v_kamere <<endl;
	cout << "bod uvnitr: " << uvnitr[0] << endl;

	circle(picture,uvnitr[0],10,Scalar(250,250,250),-1);
	circle(picture,uvnitr[0],7,Scalar(0.0,0.0,0.0),-1);
	imshow("Projector",picture);
	cvWaitKey(100);

	cam.read(view);
	circle(view,venku[0],10,Scalar(250,250,250),-1);
	circle(view,venku[0],7,Scalar(0.0,0.0,0.0),-1);
	imshow("Kamera",view);
	cvWaitKey(100);
	
	} else {
		cout << "\n\nchyba pri kalibraci\n\n";
	}
	getchar();
	cv::destroyAllWindows();
	return 0;
}