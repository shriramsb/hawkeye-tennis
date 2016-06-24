#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; 
Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

int main(int argc, char** argv)
{
	src = imread(argv[1],0);

	
	Canny(src,src, 50, 150, 3);
	imshow("src", src);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));
	vector<RotatedRect> minRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	for( size_t i = 0; i < contours.size(); i++)
	{
		if(contours[i].size() > 5)
			minRect[i] = fitEllipse(Mat(contours[i]));
		minEnclosingCircle(contours[i],center[i],radius[i]);
	}
	Mat drawing = Mat::zeros(src.size(),CV_8UC3);
	for( size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
		Size2f s = minRect[i].size;
		if(contours[i].size() > 5 && s.width/s.height > 0.7 && s.width/s.height < 1.4 && s.width > 3 && s.height > 3 && contourArea(contours[i])/(s.width*s.height) > 0.6 && contourArea(contours[i])/(s.width*s.height) < 0.8 && arcLength(contours[i],true)/(2*CV_PI*radius[i])>0.8 && arcLength(contours[i],true)/(2*CV_PI*radius[i]) < 1.2)
			drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
		
	
	}
	imshow("contours",drawing);
	waitKey(0);

}