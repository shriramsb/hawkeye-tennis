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
	namedWindow("camera1colMask", WINDOW_NORMAL);
	namedWindow("camera2colMask", WINDOW_NORMAL);
	namedWindow("camera1morph_colMask", WINDOW_NORMAL);
	namedWindow("camera2morph_colMask", WINDOW_NORMAL);
	namedWindow("camera1contour", WINDOW_NORMAL);
	namedWindow("camera2contour", WINDOW_NORMAL);
	syncVideo(camera);									//manual synchronization video from both cameras
	char c;
	int t;
	int wait = 30;
	while(true){
		get3dloc(camera, output);						//prints 3d position of ball center in next frame and write to file
		c = waitKey(wait);
		if (c == 'p'){
			c = waitKey(0);
			if (c == 's'){
				save_current(camera);
				c = '0';
			}
			if (c == 'f'){
				camera[0].findcenter();
				camera[1].findcenter();
				Mat pt3d;
				pos3d_solve(camera[0].cam.proj_mat, camera[1].cam.proj_mat, camera[0].center, camera[1].center, pt3d);
				cout << pt3d;
				output << pt3d << endl;
				c = '0';
				cin >> c;
			}
		}
		else if (c == 's'){
			syncVideo(camera);
			c = '0';
		}
		else if (c == '4'){
			wait++;
			c = '0';
		}
		else if (c == '6'){
			wait--;
			wait = wait > 0 ? wait : 0;
			c = '0';
		}
		else if (c == 'f'){
			cin >> t;
			int pos[2];
			for (int i = 0; i < 2; i++){
				pos[i] = camera[i].cam.cap.get(CAP_PROP_POS_MSEC);
				camera[i].cam.cap.set(CAP_PROP_POS_MSEC, pos[i] + t * 1000);
			}
			c = '0';
		}
		else if (c == 'u'){

		}
	}
	output.close();
}