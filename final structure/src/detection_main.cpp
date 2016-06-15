/*Detection of 3d position of ball
	Takes 2 calibration output files, corresponding 2 video files destination file name to write coordinates located point 
		in each frame 
argv[1], argv[2] - path to xml file holding camera parameters
argv[3], argv[4] - path to video 
argv[5] - path the output file */
#include <detection.h>

int main(int argc, char** argv){
	CamFrame camera[2];
	fstream output;
	output.open(argv[5], ios::app);
	output << endl << endl;								//to separate from previously written output
	camera[0] = CamFrame(argv[1], argv[3]);				
	camera[1] = CamFrame(argv[2], argv[4]);
	//namedWindow("camera1frame", WINDOW_NORMAL);
	//namedWindow("camera2frame", WINDOW_NORMAL);
	namedWindow("camera1morph_colMask", WINDOW_NORMAL);
	namedWindow("camera2morph_colMask", WINDOW_NORMAL);
	namedWindow("camera1contour", WINDOW_NORMAL);
	namedWindow("camera2contour", WINDOW_NORMAL);
	syncVideo(camera);									//manual synchronization video from both cameras
	while(true){
		get3dloc(camera, output);						//prints 3d position of ball center in next frame and write to file
	}
	output.close();
}