#include <detection.h>

Camera::Camera(){}													//default constructor
	
Camera::Camera(char* calib, char* video){							//calib - path to xml file
	Mat rvec, tvec;													//video - path to video file
	pMOG2 = createBackgroundSubtractorMOG2();
	cap.open(video);												
	FileStorage param(calib, FileStorage::READ);
	param["OptimalCamMatrix"] >> optCamMat;
	param["undist_mapx"] >> mapx;									
	param["undist_mapy"] >> mapy;
	param["rvec"] >> rvec;
	param["tvec"] >> tvec;
	param["proj_mat"] >> proj_mat;
	learnBg();
}


void Camera::learnBg(){
	int start, end;
	Mat temp;
	Mat temp1;
	cout << "Enter start postion(in sec):";				
	cin >> start;
	cout << "Enter end position(in sec):";					
	cin >> end;
	start *= 1000;											// *1000 to convert to milliseconds
	end *= 1000;
	cap.set(CAP_PROP_POS_MSEC,start);				
	while(end - cap.get(CAP_PROP_POS_MSEC) > 1000){
		cap.read(temp1);
		remap(temp1, temp, mapx, mapy, INTER_LINEAR);
		medianBlur(temp, temp, kernel_size);
		pMOG2->apply(temp, temp1);							//updating background model
		temp.release();
		temp1.release();
	}
	//cap.set(CAP_PROP_POS_FRAMES,0);							//set the pointer back to start to sync later
}


