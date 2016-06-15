/*Detection of 3d position of ball
	Takes 2 calibration output files, corresponding 2 video files destination file name to write coordinates located point 
		in each frame */

#include <detection.h>

int main(int argc, char** argv){
	CamFrame camera[2];
	fstream output;
	output.open(argv[5], ios::app);
	output << endl << endl;
	camera[0] = CamFrame(argv[1], argv[3]);
	camera[1] = CamFrame(argv[2], argv[4]);
	//namedWindow("camera1frame", WINDOW_NORMAL);
	//namedWindow("camera2frame", WINDOW_NORMAL);
	namedWindow("camera1morph_colMask", WINDOW_NORMAL);
	namedWindow("camera2morph_colMask", WINDOW_NORMAL);
	namedWindow("camera1contour", WINDOW_NORMAL);
	namedWindow("camera2contour", WINDOW_NORMAL);
	syncVideo(camera);
	while(true){
		get3dloc(camera, output);
	}
	output.close();
}