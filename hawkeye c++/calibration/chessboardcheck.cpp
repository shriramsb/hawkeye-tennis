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
	char filename[50] = "calibimgs/9.JPG";
	const int n_cols = 7;
	const int n_rows = 7;
	const int n_points = n_cols * n_rows;
	const int n_boards = 8;
	const int sqSize = 1;
	const Size board_size = Size(n_cols, n_rows);
	const Size image_size = Size(3240, 4320);
	Mat img = imread(filename,1);
	cout << img.size();
	vector<Point2f> PointBuf;
	bool found = findChessboardCorners(img, board_size, PointBuf);
	cout<<found;
	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	cornerSubPix(img_gray, PointBuf,Size(11,11),Size(-1,-1),TermCriteria(TermCriteria::EPS+TermCriteria::COUNT,30, 0.1));
	Mat img1 = img.clone();
	drawChessboardCorners(img1, board_size, PointBuf, found);
	imshow("corners",img1);
	imwrite("corners.jpg",img1);
	waitKey(0);
}