#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(){
	Mat frame;
	Mat frame32f;
	Mat edge;
	Mat avg, stddev;
	char filename[40] = "images/1.jpg";
	const int count = 9;
	const int width = 1280;
	const int height = 720;
	const int lowThreshold = 50;
	const int upThreshold = 150;
	const int kernel_size = 3;
	avg = Mat::zeros(height,width,CV_32FC1);
	stddev = avg.clone();
	for (int i = 1; i <= count; i++){
		filename[7] = i + 48;
		frame = imread(filename, IMREAD_GRAYSCALE);
		medianBlur(frame, frame, kernel_size);
		Canny(frame, edge,lowThreshold,upThreshold,kernel_size);
		frame.copyTo(frame32f, edge);
		frame32f.convertTo(frame32f, CV_32FC1);
		avg += frame32f;
	}
	avg = avg * (1.0 / count);
	waitKey(0);
	for (int i = 1; i <= count; i++){
		filename[7] = i + 48;
		frame = imread(filename, IMREAD_GRAYSCALE);
		medianBlur(frame, frame, kernel_size);
		Canny(frame, edge,lowThreshold,upThreshold,kernel_size);
		imshow("edge",edge);
		frame.copyTo(frame32f, edge);
		imshow("frame",frame32f);
		waitKey(0);
		frame32f.convertTo(frame32f, CV_32FC1);
		pow(frame32f - avg, 2 , frame32f);
		stddev += frame32f;
	}
	stddev = stddev * (1.0/count);
	pow(stddev, 0.5, stddev);
	avg.convertTo(avg, CV_8UC1);
	stddev.convertTo(stddev, CV_8UC1);
	imshow("Average", avg);
	imshow("Standard Deviation", stddev);
	imwrite("images/average.jpg",avg);
	imwrite("images/stddev.jpg",stddev);
	waitKey(0);
}