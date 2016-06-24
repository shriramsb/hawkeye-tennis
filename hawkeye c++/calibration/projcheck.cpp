//projects a point wrt to some coordinate system to camera coordinates(pixel coordinates)
#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main(){
	vector<Point3f> obj_pts;
	FileStorage f("calib.xml", FileStorage::READ);
	Mat cam_mat;
	f["Optimalcammatrix"] >> cam_mat;
	Mat rvec;
	Mat tvec;
	FileStorage g("ca.xml",FileStorage::READ);
	g["rotationvector1"] >> rvec;
	g["translationvector1"] >> tvec;
	f.release();
	Mat rot_mat = Mat::zeros(3,3,CV_32FC1);
	Rodrigues(rvec, rot_mat);
	//cout<<rot_mat.type()<<CV_64F;
	Mat rotaugtran = Mat::zeros(4,4,CV_64F);
	for (int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			rotaugtran.at<double>(i,j) = rot_mat.at<double>(i,j);
	for(int i = 0; i < 3; i++)
		rotaugtran.at<double>(i,3) = tvec.at<double>(i,0);
	
	rotaugtran.at<double>(3,3) = 1;
	//cout << rotaugtran;
	//cout<<cam_mat.type();
	Mat pt = Mat::zeros(4,1,CV_64F);
	pt.at<double>(3,0) = 1.0;
	pt.at<double>(0,0) = 6*2.35;
	pt.at<double>(1,0) = 0*2.35;
	Mat ptcam = rotaugtran*pt;
	
	Mat ptcam1 = Mat::zeros(3,1,CV_64F);
	for (int i = 0; i < 3; i++)
		ptcam1.at<double>(i,0) = ptcam.at<double>(i,0);
	
	Mat planepix = cam_mat*ptcam1;
	cout << planepix;
	Mat pixcor = Mat::zeros(2,1,CV_64F);
	for (int i = 0; i < 2; i++)
		pixcor.at<double>(i,0) = planepix.at<double>(i,0)/planepix.at<double>(2,0);
	cout<<pixcor.at<double>(0,0)<<" "<<pixcor.at<double>(1,0);

}