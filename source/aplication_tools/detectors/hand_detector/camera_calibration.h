
#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "settings.h"

#define PROJECTOR_Y -30
#define PROJECTOR_X 1346

static void read(const FileNode& node, Settings& x, const Settings&);

bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,
                           vector<vector<Point2f> > imagePoints );

static double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                         const vector<vector<Point2f> >& imagePoints,
                                         const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                         const Mat& cameraMatrix , const Mat& distCoeffs,
                                         vector<float>& perViewErrors);

static void calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point3f>& corners,
                                     Settings::Pattern);

static bool runCalibration( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                            vector<vector<Point2f> > imagePoints, vector<Mat>& rvecs, vector<Mat>& tvecs,
                            vector<float>& reprojErrs,  double& totalAvgErr);

static void saveCameraParams( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                              const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                              const vector<float>& reprojErrs, const vector<vector<Point2f> >& imagePoints,
                              double totalAvgErr );

cv::Mat startCameraCalibration(cv::Mat& cameraMatrix,cv::Size& boardSize);

cv::Mat projectorCalibration(cv::Mat& view, cv::Size boardSize, float squareSize);

bool systemCalibration(cv::Mat& camera, cv::Mat& distorsionCoeff, cv::Mat& homography);
#endif
