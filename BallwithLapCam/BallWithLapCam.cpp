#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
using namespace std;
using namespace cv;
int main(){
	int argv = 5;
	char argc[][100] = {"/home/jeyasoorya/Pictures/Webcam/1.jpg","/home/jeyasoorya/Pictures/Webcam/2.jpg","/home/jeyasoorya/Pictures/Webcam/3.jpg","/home/jeyasoorya/Pictures/Webcam/4.jpg"};
	vector<Mat> src;
	for(int i = 1;i<=argv-1;i++){
		Mat temp = imread(argc[i-1]);
		Mat grey;
		cvtColor(temp,grey,CV_BGR2GRAY);
		medianBlur(temp,temp,3);
		Canny(temp,temp,50,150,3);
		src.push_back(temp);
	}
	
	Mat avgImg = src[0];
//	Mat stdDevImg = Mat::zeros(src[0].rows,src[0].cols,src[0].type());
//	namedWindow("src",WINDOW_NORMAL);
	for(int i = 2;i<=argv-1;i++){
		//imshow("src",avgImg);
		
		avgImg = (avgImg*(i-1) + src[i-1])/i;	
		

	}
	//avgImg = avgImg;
	/*for(int i = 1;i<=argv-1;i++){
		Mat temp;
		subtract(avgImg,src[i-1],temp);
		stdDevImg = stdDevImg + temp.mul(temp);
	}*/
	//stdDevImg = stdDevImg;
	namedWindow("DisplayImg",WINDOW_NORMAL|WINDOW_KEEPRATIO);
	imshow("DisplayImg",avgImg);
	waitKey(0);
	Mat test = imread("/home/jeyasoorya/Pictures/Webcam/test1.jpg");
	cvtColor(test,test,CV_BGR2GRAY);
	medianBlur(test,test,3);
	Canny(test,test,50,150,3);
	Mat dst,dst1;
	subtract(test,avgImg,dst);
	imshow("DisplayImg",dst);
	waitKey(0);
	
	for(int i = 0;i<5;i++){
		Mat elem = getStructuringElement(MORPH_RECT,Size(3,3));
		dilate(dst,dst,elem);
	}
	dst.convertTo(dst1,CV_8U);
	imshow("DisplayImg",dst1);
	waitKey(0);
	//dst.convertTo(dst1,CV_8U);
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
	findContours(dst1,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE,Point(0,0));
	//cout<<contours.size();
	for(int i = 0;i<contours.size();i++){
		Mat dst2 = Mat::zeros(dst1.rows,dst1.cols,CV_8U);
		drawContours(dst2,contours,i,Scalar(100,100,100),CV_FILLED);
		Rect bound = boundingRect(Mat(contours[i]));
		rectangle(dst2,bound,Scalar(150,150,150));
		imshow("DisplayImg",dst2);
		waitKey(0);
	}
} 