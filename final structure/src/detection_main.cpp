/*Detection of 3d position of ball
	Takes 2 calibration output files, corresponding 2 video files destination file name to write coordinates located point 
		in each frame 
argv[1], argv[2] - path to xml file holding camera parameters
argv[3], argv[4] - path to video 
argv[5] - path the output file */
#include <detection.h>
void correctFormat();
int main(int argc, char** argv){
	correctFormat();
	cout << "Check whether input is in above format.\nContinue(y/n)?";
	char correct = 'n';
	cin >> correct;
	if (correct != 'y')
		return 1;
	bool epipolar = ask_fund_mat();
	CamFrame camera[2];
	fstream output;
	output.open(argv[5], ios::out);
	init_camera(camera, argv);	
	//namedWindow("camera1frame", WINDOW_NORMAL);
	//namedWindow("camera2frame", WINDOW_NORMAL);
	createWindow();
	syncVideo(camera);									//manual synchronization video from both cameras
	while(true){
		get3dloc(camera, output, epipolar);						//prints 3d position of ball center in next frame and write to file
		output.open(argv[5], ios::out | ios::in);
		video_controls(camera);
	}
	output.close();
}

void correctFormat(){
	cout << "First argument: " << "calibration data file for 1st camera" << endl;
	cout << "Second argument: " << "calibration data file for 2nd camera" << endl;
	cout << "Third argument: " << "Video from 1st camera" << endl;
	cout << "Fourth argument: " << "Video from 2nd camera" << endl;
	cout << "Fifth argument" << "Destination of output file containing calculated 3d coordinates" << endl;
}