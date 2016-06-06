#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include "opencv2/video.hpp"

#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

Mat frame;
Mat fg;
Mat fgMaskMOG;
Ptr<BackgroundSubtractor> pMOG;
int keyboard;

int main(){
	char filename[40] = "videos/7.MOV";
	const int count = 300;
	const int kernel_size = 3;
	const int width = 1280;
	const int height = 720;
	char c = 'p';
	pMOG = createBackgroundSubtractorMOG2();
	VideoCapture capture(filename);
	for (int i = 0; i < 100; i++)
		capture.read(frame);
	for (int i = 100; i < 100 + count; i++)
	{
		capture.read(frame);
		medianBlur(frame, frame, kernel_size);
		pMOG->apply(frame, fgMaskMOG);
		imshow("frame",frame);
		imshow("fg",fgMaskMOG);
		waitKey(30);
	}
	capture.set(CAP_PROP_POS_FRAMES, 1200);
	while(c != 'q'){
		capture.read(frame);
		medianBlur(frame,frame,kernel_size);
		pMOG->apply(frame, fgMaskMOG,0);
		imshow("frame", frame);
		imshow("fg",fgMaskMOG);
		c = waitKey(30);
		if(c=='q')
		{
			imwrite("images/frame.png",frame);
			imwrite("images/fgmask.png",fgMaskMOG);
			fg = Mat::zeros(height, width, CV_8UC3);
			frame.copyTo(fg,fgMaskMOG);
			imwrite("images/fg.png",fg);
			c = waitKey(0);
			c = 'p';

		}
	}
	waitKey(0);
}