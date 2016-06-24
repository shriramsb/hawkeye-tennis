#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(){
	Mat frame, frame32f;
	char filename[40] = "images/1.jpg";
	Mat mean;
	const int count = 4;
	const int width = 1280;
	const int height = 720;
	Mat resultframe = Mat::zeros(height, width, CV_32FC3);
	for (int i = 1; i <=count; i++){
		filename[7] = i + 48;
		frame = imread(filename);
		frame.convertTo(frame32f, CV_32FC3);
		resultframe +=frame32f;
	}
	resultframe = resultframe * (1.0/count);
	imshow("Average",resultframe);
	imwrite("images/average.jpg",resultframe);
	waitKey(0);
	resultframe.convertTo(resultframe,CV_8UC3);
	imshow("Average1",resultframe);
	imwrite("images/average1.jpg",resultframe);
	waitKey(0);
	return 0;
}