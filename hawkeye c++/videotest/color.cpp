#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;
int lowH_slider = 0;
int lowS_slider = 0;
int lowV_slider = 0;
int highH_slider = 180;
int highS_slider = 255;
int highV_slider = 255;
const int Hmax = 360;
const int SVmax = 255;
Mat img;
Mat mask;
Mat fg;
Scalar lb = Scalar(lowH_slider/2, lowS_slider, lowV_slider);
Scalar ub = Scalar(highH_slider/2, highS_slider, highV_slider);
void disp(){
	inRange(img, lb, ub, mask);
	imshow("mask", mask);
	imwrite("color1/mask.png", mask);
	img.copyTo(fg, mask);
	cvtColor(fg, fg, COLOR_HSV2BGR);
	imshow("fg", fg);
	imwrite("color1/copy.png", fg);
}
void lowhue(int, void*){
	lb = Scalar(lowH_slider/2, lowS_slider, lowV_slider);
	disp();
}
void highhue(int, void*){
	ub[0] = highH_slider/2;
	disp();
}
void lowsat(int, void*){
	lb[1] = lowS_slider;
	disp();
}
void highsat(int, void*){
	ub[1] = highS_slider;
	disp();
}
void lowval(int, void*){
	lb[2] = lowV_slider;
	disp();
}
void highval(int, void*){
	ub[2] = highV_slider;
	disp();
}



int main(int argc, char** argv){
	img = imread(argv[1],1);
	imshow("orig", img);
	cvtColor(img, img, COLOR_BGR2HSV);
	namedWindow("mask");
	namedWindow("fg");
	createTrackbar("lowhue", "fg", &lowH_slider, Hmax, lowhue);
	createTrackbar("highhue", "fg", &highH_slider, Hmax, highhue);
	createTrackbar("lowsat", "fg", &lowS_slider, SVmax, lowsat);
	createTrackbar("highsat", "fg", &highS_slider, SVmax, highsat);
	createTrackbar("lowval", "fg", &lowV_slider, SVmax, lowval);
	createTrackbar("highval", "fg", &highV_slider, SVmax, highval);
	
	lowhue(lowH_slider, 0);
	highhue(highH_slider, 0);
	lowsat(lowS_slider, 0);
	highsat(highS_slider, 0);
	lowval(lowV_slider, 0);
	highval(highV_slider, 0);
	
	waitKey(0);
}	