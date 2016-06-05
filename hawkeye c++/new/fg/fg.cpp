#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv){
	Mat fg;
	Mat bg;
	fg = imread(argv[1], IMREAD_COLOR);
	bg = imread("images/average.jpg",IMREAD_COLOR);
	const int width = 1280;
	const int height = 720;
	const int lowThreshold = 40;
	const int upThreshold = 120;
	const int kernel_size = 3;
	medianBlur(fg,fg,kernel_size);
	medianBlur(bg, bg, kernel_size);
	fg = fg - bg;
	imshow("diff",fg);
	imwrite("images/diff.png",fg);
	Mat fgthres;
	threshold(fg, fgthres, 50, 255, 0);
	imshow("diffthres",fgthres);
	imwrite("images/diffthres.png", fgthres);
	waitKey(0);
}
