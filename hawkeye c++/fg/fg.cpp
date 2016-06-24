#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


int main(){
	Mat fg,edge, fg16,bg,stddev;
	fg = imread("images/test1.jpg",IMREAD_GRAYSCALE);
	bg = imread("images/average.jpg",IMREAD_GRAYSCALE);
	stddev = imread("images/stddev.jpg",IMREAD_GRAYSCALE);
	const int width = 1280;
	const int height = 720;
	const int lowThreshold = 50;
	const int upThreshold = 150;
	const int kernel_size = 3;
	medianBlur(fg,fg,kernel_size);
	Canny(fg,edge,lowThreshold,upThreshold,kernel_size);
	fg.copyTo(fg16,edge);
	imshow("fgcanny",fg16);
	waitKey(0);
	absdiff(fg16,bg,fg16);
	imshow("absfg",fg16);
	imwrite("images/absfg.jpg",fg16);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (fg16.at<uchar>(i,j) < 2*stddev.at<uchar>(i,j))
				fg16.at<uchar>(i,j) = 0;
	imshow("afterthres",fg16);
	imwrite("images/afterthres.jpg",fg16);
	waitKey(0);
}