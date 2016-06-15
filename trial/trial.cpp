//Program that gives 3D coordinates of the ball after camera 1 and 2 has been calibrated
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include "opencv2/video.hpp"
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace cv;
using namespace std;

//ALL GLOBAL VARIABLES
Ptr<BackgroundSubtractor> pMOG1, pMOG2;
RNG rng(12345);

/*class frame { // class that takes a fgmask as input and returns pixel coordinate for the ball assuming contour detection works
	public:
		Mat frame;
};
*/
void syncvid (VideoCapture cap1, VideoCapture cap2) { //right now manual. have to do something to control cameras
	Mat frame1, frame2;
	char c='g';
	cap1.read(frame1);
	cap2.read(frame2);
	while(c!='q') {
		if(c=='1')
			cap1.read(frame1);
		if(c=='2') 
			cap2.read(frame2);
		imshow("frame1",frame1);
		imshow("frame2",frame2);
		c=waitKey(0);
	}
}
void fgmask(VideoCapture cap1, VideoCapture cap2, Mat fg1, Mat fg2) {
	Mat frame1, frame2;
	cap1.read(frame1);
	cap2.read(frame2);
	medianBlur(frame1, frame1, 5);
	medianBlur(frame2, frame2, 5);
	pMOG1->apply(frame1, fg1,0);
	pMOG2->apply(frame2, fg2,0);
	threshold(fg1, fg1, 0,255,0);
	threshold(fg2, fg2, 0,255,0);
	Mat element = getStructuringElement(MORPH_RECT, Size(3,3),Point(1,1));
	for (int i = 0; i < 2; i++) {
		dilate(fg1, fg1, element);
		dilate(fg2, fg2, element);
	}
	for (int i = 0; i < 2; i++) {
		erode(fg1, fg1, element);
		erode(fg2, fg2, element);
	}
}

void bglearner(VideoCapture cap1, VideoCapture cap2) {//Learns the background for both the videos in first 10secs
	Mat frame1, frame2, fg1, fg2;
	pMOG1 = createBackgroundSubtractorMOG2();
	pMOG2 = createBackgroundSubtractorMOG2();
	for (int i = 0; i < 300; i++)
		{
			cap1.read(frame1);
			medianBlur(frame1, frame1, 5);
			pMOG1->apply(frame1, fg1);
			cap2.read(frame2);
			medianBlur(frame2, frame2, 5);
			pMOG2->apply(frame2, fg2);
		}
}

int main() {
	VideoCapture cap1("link1"), cap2("link2");
	Mat fg1, fg2;
	char c='g';
	bglearner(cap1, cap2);
	syncvid(cap1,cap2);
	while(c!='q'){
		fgmask(cap1, cap2, fg1, fg2);

	}
}
