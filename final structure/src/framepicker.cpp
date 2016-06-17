/*Loads a video and save that frame when 's' is pressed at some time and displays next frame when 'p' is pressed
	Give name of video file and destination image file as argument
*/

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
	VideoCapture cap(argv[1]);
	char c = 'p';
	Mat frame;
	namedWindow("frame", WINDOW_NORMAL);
	while(c == 'p'){
		cap.read(frame);
		imshow("frame", frame);
		c = waitKey(0);
		if (c == 's')
			imwrite(argv[2], frame);
	}
}
