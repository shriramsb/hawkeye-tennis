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

const int kernel_size = 3;
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
	void learnBg();						//to create background model
	Camera();	
	Camera(char *calib, char* video);


};
class CamFrame{
public:	
	Camera cam;							//everything except cam changes in program
	Mat orig;							//original frame read from video each time
	Mat fgMask;							//foreground mask
	Mat fg;								//fgMask applied to orig
	Mat colMask;						//maks to take out ball by color detection
	Mat morph_colMask;					//after morphological operations(erode, dilate...)
	Mat canny;							//canny edge detector applied to morph_colMask
	Mat contour_plot;					//contour plot of detected contours
	float frame_no, frame_time;
	vector<vector<Point> > contours;	
	vector<Vec4i> hierarchy;
	vector<RotatedRect> minRect;		//min. bounding rect. of contour
	vector<RotatedRect> minEllipse;		//min. ellipse of contour
	vector<Circle> minCircle;			//min. bounding circle of contour
	vector<Point2f> constraint_center;	//stores centres of bdd.circles of contours satisfying the constraint
	Point2d center; 					//center of ball in the frame
	Scalar lb, ub;
	CamFrame();
	CamFrame(char* calib, char* video, Scalar l, Scalar u);
	void getframe();					//gets a frame from video and stores in orig
	void nextsecond();
	void subtBgColDet();				//background subtractor and color detector
	bool contourDetection();			//contour detector
	void findcenter();					//find center of ball after bgsub and col det. and contour det.
	void clear();						//clear memory allocated to member Mat object and vector
};



void syncVideo(CamFrame camera[]);
void show(CamFrame& camera, int window);				//displaying each frame
void get3dloc(CamFrame camera[], fstream& output, bool epipolar);		//gives 3d location of ball in each frame
void pos3d_solve(Mat& m1, Mat& m2, Point2d& pt1, Point2d& pt2, Mat& pt);	//gives 3d location gives projection matrix and image
																			//						coordinates of pts. in 2 images
void save_current(CamFrame camera[]);

void eliminate_duplication(vector<Point2f>& c);								//to eliminate two closely detected contours

void getFundMat(char f[]);
void epipolar_constraint(vector<Point2f>& cam1pts, vector<Point2f>& cam2pts);
float dist(Point2f pt, Point3f line);