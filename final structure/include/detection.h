#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include "opencv2/video.hpp"
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;

const int kernel_size = 5;
struct Circle{
	Point2f centre;
	float radius;
};

class Camera{
public:	
	Mat optCamMat;						//optimal camera matrix
	Mat proj_mat;						//projection matrix (= M * [R|T])
	Mat mapx, mapy;						//map for undistortion
	Ptr<BackgroundSubtractor> pMOG2;	//background subtractor
	VideoCapture cap;					//reads video file
	void learnBg();
	Camera();
	Camera(char *calib, char* video);


};
class CamFrame{
public:	
	Camera cam;
	Mat orig;
	Mat fgMask;
	Mat fg;
	Mat colMask;
	Mat morph_colMask;
	Mat canny;
	Mat contour_plot;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<RotatedRect> minRect;
	vector<RotatedRect> minEllipse;
	vector<Circle> minCircle;
	Point2d center; 
	CamFrame();
	CamFrame(char* calib, char* video);
	void getframe();
	void subtBgColDet();
	void contourDetection();
	void findcenter();
};

void syncVideo(CamFrame camera[]);
void show(CamFrame& camera, int window);
void get3dloc(CamFrame camera[], fstream& output);
void pos3d_solve(Mat& m1, Mat& m2, Point2d& pt1, Point2d& pt2, Mat& pt);


