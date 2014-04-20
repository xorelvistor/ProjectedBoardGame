#include "camera_calibration.h"
#include <list>




#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

using namespace cv;
using namespace std;


static void read(const FileNode& node, Settings& x, const Settings& default_value = Settings()) {
	if(node.empty())
		x = default_value;
	else
		x.read(node);
}

/* mody prubehu aplikace */
enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };


static double computeReprojectionErrors(const vector<vector<Point3f> >& objectPoints,
										const vector<vector<Point2f> >& imagePoints,
										const vector<Mat>& rvecs, const vector<Mat>& tvecs,
										const Mat& cameraMatrix , const Mat& distCoeffs,
										vector<float>& perViewErrors)
{
	vector<Point2f> imagePoints2;
	int i, totalPoints = 0;
	double totalErr = 0, err;
	perViewErrors.resize(objectPoints.size());

	for( i = 0; i < (int)objectPoints.size(); ++i ) {
		projectPoints( Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
		err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);

		int n = (int)objectPoints[i].size();
		perViewErrors[i] = (float) std::sqrt(err*err/n);
		totalErr += err*err;
		totalPoints += n;
    }

	return std::sqrt(totalErr/totalPoints);
}

static void calcBoardCornerPositions(Size boardSize,
									 float squareSize,
									 vector<Point3f>& corners,
                                     Settings::Pattern patternType = Settings::CHESSBOARD)
{
	corners.clear();

	switch(patternType) {
		case Settings::CHESSBOARD:
	    case Settings::CIRCLES_GRID:
			for( int i = 0; i < boardSize.height; ++i )
				for( int j = 0; j < boardSize.width; ++j )
					corners.push_back(Point3f(float( j*squareSize ), float( i*squareSize ), 0));
			break;

		case Settings::ASYMMETRIC_CIRCLES_GRID:
			for( int i = 0; i < boardSize.height; i++ )
				for( int j = 0; j < boardSize.width; j++ )
					corners.push_back(Point3f(float((2*j + i % 2)*squareSize), float(i*squareSize), 0));
			break;
		default:
			break;
    }
}

static bool runCalibration( Settings& s,
							Size& imageSize,
							Mat& cameraMatrix,
							Mat& distCoeffs,
                            vector<vector<Point2f> > imagePoints,
							vector<Mat>& rvecs,
							vector<Mat>& tvecs,
                            vector<float>& reprojErrs,
							double& totalAvgErr)
{

	cameraMatrix = Mat::eye(3, 3, CV_64F);
	if( s.flag & CV_CALIB_FIX_ASPECT_RATIO )
		cameraMatrix.at<double>(0,0) = 1.0;

	distCoeffs = Mat::zeros(8, 1, CV_64F);

	vector<vector<Point3f> > objectPoints(1);
	calcBoardCornerPositions(s.boardSize, s.squareSize, objectPoints[0], s.calibrationPattern);

	objectPoints.resize(imagePoints.size(),objectPoints[0]);

	//Find intrinsic and extrinsic camera parameters
	double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, s.flag|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);

	cout << "Re-projection error reported by calibrateCamera: "<< rms << endl;

	bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

	totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints,
                                             rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);

	return ok;
}

// Print camera parameters to the output file
static void saveCameraParams(Settings& s,
							 Size& imageSize,
							 Mat& cameraMatrix,
							 Mat& distCoeffs,
							 const vector<Mat>& rvecs,
							 const vector<Mat>& tvecs,
							 const vector<float>& reprojErrs,
							 const vector<vector<Point2f> >& imagePoints,
							 double totalAvgErr)
{
	FileStorage fs( s.outputFileName, FileStorage::WRITE );

    //time_t tm;
    //time( &tm );
    //struct tm *t2 = localtime( &tm );
	char buf[1024];
    //strftime( buf, sizeof(buf)-1, "%c", t2 );

    //fs << "calibration_Time" << buf;

	if( !rvecs.empty() || !reprojErrs.empty() )
		fs << "nrOfFrames" << (int)std::max(rvecs.size(), reprojErrs.size());
	fs << "image_Width" << imageSize.width;
	fs << "image_Height" << imageSize.height;
	fs << "board_Width" << s.boardSize.width;
	fs << "board_Height" << s.boardSize.height;
	fs << "square_Size" << s.squareSize;

	if( s.flag & CV_CALIB_FIX_ASPECT_RATIO )
		fs << "FixAspectRatio" << s.aspectRatio;

	if( s.flag ) {
		sprintf( buf, "flags: %s%s%s%s",
			s.flag & CV_CALIB_USE_INTRINSIC_GUESS ? " +use_intrinsic_guess" : "",
			s.flag & CV_CALIB_FIX_ASPECT_RATIO ? " +fix_aspectRatio" : "",
			s.flag & CV_CALIB_FIX_PRINCIPAL_POINT ? " +fix_principal_point" : "",
			s.flag & CV_CALIB_ZERO_TANGENT_DIST ? " +zero_tangent_dist" : "" );
		cvWriteComment( *fs, buf, 0 );
	}

	fs << "flagValue" << s.flag;

	fs << "Camera_Matrix" << cameraMatrix;
	fs << "Distortion_Coefficients" << distCoeffs;

	fs << "Avg_Reprojection_Error" << totalAvgErr;
	if( !reprojErrs.empty() )
		fs << "Per_View_Reprojection_Errors" << Mat(reprojErrs);

	if( !rvecs.empty() && !tvecs.empty() ) {
		CV_Assert(rvecs[0].type() == tvecs[0].type());
		Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());
		for( int i = 0; i < (int)rvecs.size(); i++ ) {
			Mat r = bigmat(Range(i, i+1), Range(0,3));
			Mat t = bigmat(Range(i, i+1), Range(3,6));

			CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
			CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
            //*.t() is MatExpr (not Mat) so we can use assignment operator
			r = rvecs[i].t();
			t = tvecs[i].t();
        }
		cvWriteComment( *fs, "a set of 6-tuples (rotation vector + translation vector) for each view", 0 );
		fs << "Extrinsic_Parameters" << bigmat;
	}

	if( !imagePoints.empty() ) {
		Mat imagePtMat((int)imagePoints.size(), (int)imagePoints[0].size(), CV_32FC2);
		for( int i = 0; i < (int)imagePoints.size(); i++ ) {
			Mat r = imagePtMat.row(i).reshape(2, imagePtMat.cols);
			Mat imgpti(imagePoints[i]);
			imgpti.copyTo(r);
		}
		fs << "Image_points" << imagePtMat;
	}
}

bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,vector<vector<Point2f> > imagePoints ) {
	vector<Mat> rvecs, tvecs;
	vector<float> reprojErrs;
	double totalAvgErr = 0;

	bool ok = runCalibration(s,imageSize, cameraMatrix, distCoeffs, imagePoints, rvecs, tvecs,
                             reprojErrs, totalAvgErr);
	cout << (ok ? "Calibration succeeded" : "Calibration failed")
		<< ". avg re projection error = "  << totalAvgErr ;

	if( ok )
		saveCameraParams( s, imageSize, cameraMatrix, distCoeffs, rvecs ,tvecs, reprojErrs, imagePoints, totalAvgErr);
	return ok;
}

cv::Mat startCameraCalibration(cv::Mat& cameraMatrix,cv::Size& boardSize) {
	cout << CV_VERSION << endl;

	/** zpracovani konfiguracnoho souboru **/

	Settings s;
	const string inputSettingsFile = "in_VID5.xml"; // konfiguracni soubor
	FileStorage fs(inputSettingsFile, FileStorage::READ); // nacteni dat z konf. souboru
    
	if (!fs.isOpened()) {
		cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
		return cv::Mat();
	} else {
		cout << "open the configuration file: \"" << inputSettingsFile << "\"" << endl;
	}
	fs["Settings"] >> s;
	fs.release();                                         // close Settings file

	if (!s.goodInput) {
		cout << "Invalid input detected. Application stopping. " << endl;
		return cv::Mat();
	}

	vector<vector<Point2f> > imagePoints;
	cv::Mat distCoeffs;
	Size imageSize;
	int mode = s.inputType == Settings::IMAGE_LIST ? CAPTURING : DETECTION;
	clock_t prevTimestamp = 0;
	const Scalar RED(0,0,255), GREEN(0,255,0);
	const char ESC_KEY = 27;

	cout << s.boardSize << endl <<s.delay << endl << s.squareSize << endl;
	cout << "BRIGHTNESS: " << s.inputCapture.get(CV_CAP_PROP_BRIGHTNESS) << endl;
	cout << "CONTRAST: " << s.inputCapture.get(CV_CAP_PROP_CONTRAST) << endl;
	cout << "SATURATION: " << s.inputCapture.get(CV_CAP_PROP_SATURATION) << endl;
	cout << "FOCUS: " << s.inputCapture.get(CV_CAP_PROP_FOCUS) << endl;
	cout << "EXPOSURE" << s.inputCapture.get(CV_CAP_PROP_EXPOSURE) << endl;
	cout << "GAIN: " << s.inputCapture.get(CV_CAP_PROP_GAIN) << endl;
	
	boardSize = s.boardSize;

	if(getchar()== 's') {
		cout << "skip camera calibration" << endl;
		return cv::Mat();
	}
	/* kalibrace dle nastaveni */

	bool found = false;
	mode = CAPTURING;
	for(int i = 0;;++i) {
		Mat view;
		bool blinkOutput = false;
		view = s.nextImage(); // ziskani snimku z kamery

		//-----  If no more image, or got enough, then stop calibration and show result -------------
		if( mode == CAPTURING && imagePoints.size() >= (unsigned)s.nrFrames ) {
			if( runCalibrationAndSave(s, imageSize,  cameraMatrix, distCoeffs, imagePoints))
				mode = CALIBRATED;
			else
				mode = DETECTION;
		}

		if(view.empty()) {         // If no more images then run calibration, save and stop loop.
			if( imagePoints.size() > 0 )
				runCalibrationAndSave(s, imageSize,  cameraMatrix, distCoeffs, imagePoints);
			break;
		}

		imageSize = view.size();  // Format input image.
		if( s.flipVertical )
			flip( view, view, 0 );

		vector<Point2f> pointBuf;

		switch( s.calibrationPattern ) { // Find feature points on the input format
			case Settings::CHESSBOARD:
				found = findChessboardCorners( view, s.boardSize, pointBuf,CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
				break;
			case Settings::CIRCLES_GRID:
				found = findCirclesGrid( view, s.boardSize, pointBuf );
				break;
			case Settings::ASYMMETRIC_CIRCLES_GRID:
				found = findCirclesGrid( view, s.boardSize, pointBuf, CALIB_CB_ASYMMETRIC_GRID );
				break;
			default:
				found = false;
				break;
        }

		if ( found) {                // If done with success,
			// improve the found corners' coordinate accuracy for chessboard
			if( s.calibrationPattern == Settings::CHESSBOARD) {
				Mat viewGray;
				cvtColor(view, viewGray, COLOR_BGR2GRAY);
				cornerSubPix( viewGray, pointBuf, Size(11,11),Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
			}

			// For camera only take new samples after delay time
			if( mode == CAPTURING && (!s.inputCapture.isOpened() || clock() - prevTimestamp > s.delay*1e-9*CLOCKS_PER_SEC) ) {
				imagePoints.push_back(pointBuf);
				prevTimestamp = clock();
				blinkOutput = s.inputCapture.isOpened();
			}

			// Draw the corners.
			drawChessboardCorners( view, s.boardSize, Mat(pointBuf), found );
		}

		//----------------------------- Output Text ------------------------------------------------
		string msg = (mode == CAPTURING) ? "100/100" : mode == CALIBRATED ? "Calibrated" : "Press 'g' to start";
		int baseLine = 0;
		Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
		Point textOrigin(view.cols - 2*textSize.width - 10, view.rows - 2*baseLine - 10);

		if( mode == CAPTURING ) {
			if(s.showUndistorsed)
				msg = format( "%d/%d Undist", (int)imagePoints.size(), s.nrFrames );
			else
				msg = format( "%d/%d", (int)imagePoints.size(), s.nrFrames );
		}

		putText( view, msg, textOrigin, 1, 1, mode == CALIBRATED ?  GREEN : RED);

		if( blinkOutput )
			bitwise_not(view, view);

		//------------------------- Video capture  output  undistorted ------------------------------
		/*if( mode == CALIBRATED && s.showUndistorsed ) {
			Mat temp = view.clone();
			undistort(temp, view, cameraMatrix, distCoeffs);
		}*/

		//------------------------------ Show image and check for input commands -------------------
		imshow("Image View", view);
		char key = (char)waitKey(s.inputCapture.isOpened() ? 50 : s.delay);

		if( key  == ESC_KEY )
			break;

		if( key == 'u' && mode == CALIBRATED ) {
			s.showUndistorsed = !s.showUndistorsed;
			cout << "showUndistorsed: " << s.showUndistorsed << endl; 
		}
		if( s.inputCapture.isOpened() && key == 'g' ) {
			mode = CAPTURING;
			imagePoints.clear();
		}
		if (mode == CALIBRATED)
			break;
		

	}

	// -----------------------Show the undistorted image for the image list ------------------------
	/*if( s.inputType == Settings::IMAGE_LIST && s.showUndistorsed ) {
		Mat view, rview, map1, map2;
		initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),imageSize, CV_16SC2, map1, map2);

		for(int i = 0; i < (int)s.imageList.size(); i++ ) {
			view = imread(s.imageList[i], 1);
			if(view.empty())
				continue;
			remap(view, rview, map1, map2, INTER_LINEAR);
			char c = (char)waitKey();
			if( c  == ESC_KEY || c == 'q' || c == 'Q' )
				break;
		}
	}*/
	cv::destroyAllWindows();
	return distCoeffs;
}

cv::Mat projectorCalibration(cv::Mat& view, cv::Size boardSize, float squareSize) {
	vector<Point2f> camKeyPoints;
	vector<Point2f> originalKeyPoints;
	bool found;
	found = findChessboardCorners( view, boardSize, camKeyPoints,CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
	float okraj = 20;

	if(found) {
		for(int i = 1; i <= boardSize.height ; i++)
			for (int j = 1; j <= boardSize.width; j++)
				originalKeyPoints.push_back(cv::Point2f(j*squareSize+okraj,i*squareSize+okraj));
		//cout << endl << "originalKeyPoints -> " << originalKeyPoints.size()<< endl;
		//cout << originalKeyPoints << endl;
		//cout << endl << "camKeyPoints -> " << camKeyPoints.size() << endl;
		//cout << camKeyPoints << endl;
		cv::Mat homography;
		
		
		homography = findHomography(camKeyPoints,originalKeyPoints);
		if(!homography.empty()) {
			if(homography.cols != 3 && homography.rows != 3) {
				cout << "nepovedlo se najit homografii" <<endl;
				return cv::Mat();
			}
			return homography;
		} else {
			return cv::Mat();
		}
	} else {
		cout <<"nenalezena sachovnice na projektoru" << endl;
		return cv::Mat();
	}
	cout << "neco je divne" << endl;
	return cv::Mat();
}

/** MAIIN **/
cv::Mat copie;
/*
void onMouse(int event, int x, int y, int flags, void* userdata) {
	vector<Point2f> venku;
	vector<Point2f> uvnitr;
	if (event == CV_EVENT_LBUTTONDOWN) {
		cv::Point2f bod_v_kamere(x,y);
		venku.push_back(bod_v_kamere);
		uvnitr.push_back(Point());

		cv::perspectiveTransform(venku,uvnitr,homography);
		circle(copie,uvnitr[0],10,Scalar(250,250,250),-1);
		circle(copie,uvnitr[0],7,Scalar(0.0,0.0,0.0),-1);
		imshow("Kalibrace",copie);
		waitKey(100);
	}
}
*/

bool systemCalibration(cv::Mat& camera, cv::Mat& distorsionCoeff, cv::Mat& homography) {
	cv::Mat view, temp;
	cv::Size boardSize;
	float squarePixSize = 76.0;

	VideoCapture cam;
	cout << "***** Kalibrace kamery *****" <<endl;
	distorsionCoeff = startCameraCalibration(camera,boardSize);
	if(!camera.empty()) {
		cout << endl << camera << endl;
		cout << endl << distorsionCoeff << endl << endl;
	}
	cout << "***** Nacteni a zobrazeni testovaci sachovnice *****" <<endl;
	cv::Mat projection = imread("board.jpg",CV_LOAD_IMAGE_COLOR);
	if (projection.empty()) {
		cout <<"nepovedlo se nacist testovaci  sachovnici" << endl;
		return false;
	}

	/* okno na projektoru */
	namedWindow("Kalibrace",CV_WINDOW_FULLSCREEN);
	resizeWindow("Kalibrace",800,600);
	moveWindow("Kalibrace",PROJECTOR_X, PROJECTOR_Y);
	imshow("Kalibrace", projection); 
	cvWaitKey(100);

	cam.open(0);
	
	cam.set(CV_CAP_PROP_SATURATION,32.0);
	cam.set(CV_CAP_PROP_CONTRAST,32.0);
	cam.set(CV_CAP_PROP_BRIGHTNESS,128.0);
	//cam.set(CV_CAP_PROP_EXPOSURE,0.0);
	//cam.set(CV_CAP_PROP_GAIN,128.0);
while(true) {	
	cout << "***** Porizeni obrazu sachovnice kamerou *****" <<endl;

	cout << "BRIGHTNESS: " << cam.get(CV_CAP_PROP_BRIGHTNESS) << endl;
	cout << "CONTRAST: " << cam.get(CV_CAP_PROP_CONTRAST) << endl;
	cout << "SATURATION: " << cam.get(CV_CAP_PROP_SATURATION) << endl;
	cout << "FOCUS: " << cam.get(CV_CAP_PROP_FOCUS) << endl;
	cout << "EXPOSURE" << cam.get(CV_CAP_PROP_EXPOSURE) << endl;
	cout << "GAIN: " << cam.get(CV_CAP_PROP_GAIN) << endl;
	
	if (cam.isOpened()) {
		cvWaitKey(100); // cekame na ustaleni obrazu
		cam.read(temp);
	}
	cout << cam.get(CV_CAP_PROP_FRAME_WIDTH) << " x " << cam.get(CV_CAP_PROP_FRAME_HEIGHT) << endl; 
	temp.copyTo(view);
	/*
	if(!camera.empty() && !distorsionCoeff.empty())
		undistort(temp,view,camera,distorsionCoeff);
	*/
	
	namedWindow("Undistort",CV_WINDOW_AUTOSIZE);
	moveWindow("Undistort", 400, 100);
	imshow("Undistort",view);
	cvWaitKey(100);
	
	/*
	if (waitKey() == 27) {
		view = projection; //DEBUG
	}
	*/

	/* nalezeni homografie mezi kamerou a projektorem */
	cout << "***** Nalezeni klicovych bodu v obrazech => homografie *****" << endl;
	cv::Mat in_homography = projectorCalibration(view,boardSize,squarePixSize);
	if(in_homography.cols != 3) {
		cout << "nenalezena homografie" << endl;
		//return false;
	} else {
		homography = in_homography;
		cout << endl << in_homography << endl << endl;
		break;
	}

	
	//return true;
}	
	/*
	cout << endl << "***** Reakce na mys *****" <<endl;
	getchar();
	cout << endl << "***** Pro ukonceni ESC *****" <<endl;
	projection.copyTo(copie);
	cv::setMouseCallback("Undistort", onMouse);
	while(waitKey(100) != 27) {
		projection.copyTo(copie);
	}
	return EXIT_SUCCESS;
	*/
	cv::destroyAllWindows();
	return true;
}
