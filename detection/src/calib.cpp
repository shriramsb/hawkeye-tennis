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
	char filename[50] = "calibimgs/1.JPG";
	const int n_cols = 7;
	const int n_rows = 7;
	const int n_points = n_cols * n_rows;
	const int n_boards = 8;
	const float sqSize = 2.35;
	const Size board_size = Size(n_cols, n_rows);
	int flag = 0;
	//to fix any value
	flag |= CALIB_FIX_K3;
	//flag |= CALIB_ZERO_TANGENT_DIST;
	Size image_size = Size(2160, 1620);
	vector<vector<Point3f> > object_points(1);		//coordinates of chessboard corners wrt its coordinate system attached to it
	vector<Point2f> PointBuf;						//temp vector to store pixel coordinates of chessboard corners
	vector<vector<Point2f> > image_points;			//pixel coordinates of chessboard corners
	for (int i = 0; i < n_cols; i++)				
		for (int j = 0; j < n_rows; j++)
			object_points[0].push_back(Point3f(j * sqSize, i * sqSize, 0));
	object_points.resize(n_boards, object_points[0]);
	int n_read = 0, i = 1;
	bool found;
	Mat img;
	Mat img1;
	Mat img_gray;
	Mat camera_matrix = Mat::eye(3,3, CV_32FC1);
	Mat distCoeff = Mat::zeros(5,1,CV_32FC1);
	vector<Mat> rvecs;
	vector<Mat> tvecs;
	while(n_read != n_boards){
		filename[10] = i + 48;
		i++;
		img = imread(filename,1);
		found = findChessboardCorners(img, board_size, PointBuf);	
		cout << found;
		
		if (found)
		{
			cvtColor(img, img_gray, COLOR_BGR2GRAY);
			//finds pixel coordinates of chessboard corners to subpixel accuracy
			cornerSubPix(img_gray, PointBuf,Size(11,11),Size(-1,-1),TermCriteria(TermCriteria::EPS+TermCriteria::COUNT,30, 0.1));
			image_points.push_back(PointBuf);
		}
		img1 = img.clone();
		drawChessboardCorners(img1, board_size, PointBuf, found);
		imshow("chessboard",img);
		imshow("corners",img1);
		waitKey(0);
		n_read++;
	}
	//rms is reprojection error
	double rms = calibrateCamera(object_points, image_points, image_size, camera_matrix, distCoeff, rvecs, tvecs,flag);
	vector<Point2f> imagepoints2;
	vector<double> perviewerr;
	double err,tot_err = 0;
	//tot_err is reprojection error calculated by me(just to understand it)
	for (int i = 0; i < n_boards; i++)
	{
		projectPoints(object_points[i],rvecs[i],tvecs[i],camera_matrix,distCoeff,imagepoints2);
		err = norm(image_points[i], imagepoints2, NORM_L2);
		perviewerr.push_back(sqrt(pow(err,2)/n_points));
		tot_err += err*err;
	}
	tot_err /= (n_points * n_boards);
	tot_err = sqrt(tot_err);
	//optCamMat - camera matrix of the camera which gives undistorted images
	Mat optCamMat = getOptimalNewCameraMatrix(camera_matrix,distCoeff,image_size,1,image_size);
	FileStorage f("calib.xml", FileStorage::WRITE);
	f << "CameraMatrix" << camera_matrix;
	f << "Optimalcammatrix" << optCamMat;
	f << "distortion_Coefficients" <<distCoeff;
	f << "rms_error" << rms;
	f << "total_error" << tot_err;
	f << "rotationvector1" << rvecs[1];
	f << "translationvector1" << tvecs[1];
	f.release();
	cout << camera_matrix<<endl;
	cout << optCamMat;
	cout << distCoeff<<endl;
	cout << rms<<endl;
	cout << tot_err <<endl;
	Mat map1, map2;
	//undistortion of images
	char destfile[20] = "1.JPG";
	initUndistortRectifyMap(camera_matrix, distCoeff, Mat(),optCamMat,image_size,CV_16SC2,map1,map2);
	for (int i = 0; i < n_boards; i++)
	{
		filename[10] = i + 49;
		destfile[0] = i + 49;
		img = imread(filename,1);
		remap(img, img1, map1, map2,INTER_LINEAR);
		imshow("original",img);
		imshow("undistorted", img1);
		imwrite(destfile,img1);
		waitKey(0);
	}
	
}