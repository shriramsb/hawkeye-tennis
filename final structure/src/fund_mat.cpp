/*Calculates fundamental matrix 
	Takes two undistorted chessboard images as arguments and destination file as argument*/

#include <opencv2/calib3d.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <iostream>

using namespace cv;
using namespace std;

const int n_cols = 7;
const int n_rows = 8;
const Size board_size = Size(n_cols, n_rows);

void correctFormat();

int main(int argc, char** argv){
	correctFormat();
	cout << "Check whether input is in above format.\nContinue(y/n)?";
	char correct = 'n';
	cin >> correct;
	if (correct != 'y')
		return 1;
	Mat img[2], img_gray[2];
	vector<Point2f> img_pts[2];
	for (int i = 0; i < 2; i++){
		img[i] = imread(argv[i + 1], IMREAD_COLOR);
		cvtColor(img[i], img_gray[i], COLOR_BGR2GRAY);
		findChessboardCorners(img[i], board_size, img_pts[i]);
		cornerSubPix(img_gray[i], img_pts[i], Size(11,11), Size(-1,-1), TermCriteria(TermCriteria::EPS+TermCriteria::COUNT, 30, 0.1));
	}
	Mat fund_mat;
	Mat taken;
	fund_mat = findFundamentalMat(img_pts[0], img_pts[1], FM_RANSAC, 1, 0.99, taken);
	cout << taken;
	FileStorage f(argv[3], FileStorage::WRITE);
	f << "fund_mat" << fund_mat;
	f.release();


}

void correctFormat(){
	cout << "First argument: " << "Undistorted chessboard image from 1st camera" << endl;
	cout << "Second argument: " << "Undistorted chessboard image from 2nd camera" << endl;
	cout << "Third argument: " << "Destination file containing fundamental matrix" << endl;
}