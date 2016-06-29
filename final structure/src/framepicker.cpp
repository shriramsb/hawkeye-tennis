/*Loads a video and save that frame when 's' is pressed at some time and displays next frame when 'p' is pressed
	Give name of video file and destination image file as argument
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

void correctFormat();

int main(int argc, char** argv){
	correctFormat();
	cout << "Check whether input is in above format.\nContinue(y/n)?";
	char correct = 'n';
	cin >> correct;
	if (correct != 'y')
		return 1;
	VideoCapture cap(argv[1]);
	char c = 'p';
	Mat frame;
	namedWindow("frame", WINDOW_NORMAL);
	while(c == 'p'){
		cap.read(frame);
		imshow("frame", frame);
		c = waitKey(0);
		if (c == 's'){
			imwrite(argv[2], frame);
			c = 'p';
		}
	}
}

void correctFormat(){
	cout << "First argument: " << "Video file" << endl;
	cout << "Second argument: " << "Destination image file" << endl;
}
