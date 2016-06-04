#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(){
	vector<Mat> frame;
	Mat frame32f;
	Mat avg, stddev;
	char* filename = "images/1.jpg";
	const int count = 4;
	const int width = 1280;
	const int height = 720;
	avg = Mat::zeros(width,height,CV_32FC1);
	stddev = avg.clone();
	for (int i = 1; i <= count; i++){
		filename[7] = i;
		frame.push_back(imread(filename, IMREAD_GRAYSCALE));
		frame[i - 1].convertTo(frame32f, CV_32FC1);
		avg += frame32f;
	}
	avg = avg * (1.0 / count);
	for (int i = 1; i <= count; i++){
		frame[i - 1].convertTo(frame32f, CV_32FC1);
		pow(frame32f - avg, 2 , frame32f);
		stddev += frame32f;
	}
	stddev = stddev * (1.0/count);
	pow(stddev, 0.5, stddev);
	avg.convertTo(avg, CV_8UC1);
	stddev.convertTo(stddev, CV_8UC1);
	imshow("Average", avg);
	imshow("Standard Deviation", stddev);
	waitKey(0);
}