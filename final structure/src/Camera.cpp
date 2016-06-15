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
	Mat rot_vec;
	Rodrigues(rvec, rot_vec);
	Mat rotaugtrans = Mat::zeros(3, 4, CV_64F);
	for (int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
			rotaugtrans.at<double>(i, j) = rot_vec.at<double>(i, j);
		rotaugtrans.at<double>(i, 3) = tvec.at<double>(i, 1);
	}
	proj_mat = optCamMat * rotaugtrans;
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
		cap.read(temp);
		medianBlur(temp, temp, kernel_size);
		pMOG2->apply(temp, temp1);
	}
	cap.set(CAP_PROP_POS_FRAMES,0);							//set the pointer back to start to sync later
}


