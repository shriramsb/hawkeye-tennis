#include <detection.h>
#include "point_details.h"
Mat fund_mat;
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768



void syncVideo(CamFrame camera[]){				//manual synchronization of video 
	namedWindow("camera1", WINDOW_NORMAL);
	namedWindow("camera2", WINDOW_NORMAL);
	resizeWindow("camera1", SCREEN_WIDTH/2, SCREEN_HEIGHT);
	resizeWindow("camera2", SCREEN_WIDTH/2, SCREEN_HEIGHT);
	for (int i = 0; i < 2; i++){
		camera[i].getframe();
		char temp[20];
		sprintf(temp, "camera%d", i + 1);
		imshow(temp, camera[i].orig);
	}
	char c;
	c = waitKey(0);
	while(c != 'q'){
		if (c == '1'){
			camera[0].clear();
			camera[0].getframe();
			imshow("camera1", camera[0].orig);
		}
		else if (c == '2'){
			camera[1].clear();
			camera[1].getframe();
			imshow("camera2", camera[1].orig);
		}
		else if (c == '8'){
			camera[0].clear();
			camera[0].nextsecond();
			imshow("camera1", camera[0].orig);
		}
		else if (c == '9'){
			camera[1].clear();
			camera[1].nextsecond();
			imshow("camera2", camera[1].orig);
		}
		else if (c == 'p'){
			cout << camera[0].cam.cap.get(CAP_PROP_POS_FRAMES) << endl << camera[1].cam.cap.get(CAP_PROP_POS_FRAMES);
		}
		else if (c == 'b'){
			for (int i = 0; i < 2; i++){
				camera[i].clear();
				camera[i].cam.cap.set(CAP_PROP_POS_FRAMES, camera[i].cam.cap.get(CAP_PROP_POS_FRAMES) - 2);
				camera[i].getframe();
				char temp[20];
				sprintf(temp, "camera%d", i + 1);
				imshow(temp, camera[i].orig);
			}
		}
		else if (c == 'r'){
			camera[0].cam.cap.set(CAP_PROP_POS_FRAMES, 3334);
			camera[1].cam.cap.set(CAP_PROP_POS_FRAMES, 3268);
		}
		c = waitKey(0);
	}
	destroyWindow("camera1");
	destroyWindow("camera2");
}

void show(CamFrame& camera, int window){					//display frame
	stringstream ss;
	ss << "Frame number: " << camera.frame_no;
	putText(camera.contour_plot, ss.str(), Point(50,50),FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,255,0));
	char windowname[50];
	//sprintf(windowname, "camera%dframe",window);
	//imshow(windowname, camera.orig);
	sprintf(windowname,"camera%dcolMask", window);
	imshow(windowname, camera.colMask);
	sprintf(windowname, "camera%dmorph_colMask",window);
	imshow(windowname, camera.colMask);
	sprintf(windowname, "camera%dcontour",window);
	imshow(windowname, camera.contour_plot);

}
void get3dloc(CamFrame camera[], fstream& output, bool epipolar){			
	
	for (int i = 0; i < 2; i++){
		camera[i].clear();
		camera[i].getframe();
		camera[i].subtBgColDet();
		camera[i].contourDetection();
		//show(camera[i], i + 1);
		//camera[i].findcenter();
	}
	if (epipolar && camera[0].constraint_center.size() >= 1 && camera[1].constraint_center.size() >= 1){
		epipolar_constraint(camera[0].constraint_center, camera[1].constraint_center);
	}
	vector<Point3f> cam_lines1;
	if (camera[0].constraint_center.size() >= 1){				//drawing epilines
		computeCorrespondEpilines(camera[0].constraint_center, 1, fund_mat, cam_lines1);
		Point2f end_pts[2] = {Point2f(-cam_lines1[0].z/cam_lines1[0].x,0), Point2f(0, -cam_lines1[0].z/cam_lines1[0].y)};
		line(camera[1].contour_plot, end_pts[0], end_pts[1] , Scalar(0,0,255));
		cout << "End_pts" << end_pts[0] << " " << end_pts[1] << endl;
		cout << "a = " << cam_lines1[0].x << "b = " << cam_lines1[0].y << "c = " << cam_lines1[0].z << endl;
	}
	bool can_find3d = true;
	Mat pt3d = Mat::zeros(3,1,CV_64F);
	for (int i = 0; i < 2; i++){
		if (camera[i].constraint_center.size() == 1)
			camera[i].center = camera[i].constraint_center[0];
		else
			can_find3d = false;
	}
	if (can_find3d){
		/*vector<Point2f> t1, t2;
		correctMatches(fund_mat, camera[0].constraint_center, camera[1].constraint_center, t1, t2);
		camera[0].center = t1[0];
		camera[1].center = t2[0];*/
		for (int i = 0; i < 2; i++){
			circle(camera[i].contour_plot, camera[i].center, 2, Scalar(255,0,0));
			char text[80];
			sprintf(text, "Pixel coordinates of center: (%.1f, %.1f)", camera[i].center.x, camera[i].center.y);
			putText(camera[i].contour_plot, text, Point(50, 70), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,255,0));
			show(camera[i], i + 1);
		}
		pos3d_solve(camera[0].cam.proj_mat, camera[1].cam.proj_mat, camera[0].center, camera[1].center, pt3d);	
		point_details temp(pt3d, camera);
		cout << "3d coordinates of ball's center: " << temp.pt3d << endl << endl;
		output.seekg(camera[0].frame_no * sizeof(temp));			//will take a lot of space but easy to read
		output.write((char*)&temp, sizeof(temp));
	}
	else{
		if ( camera[0].constraint_center.size() > 1 || camera[1].constraint_center.size() > 1)
			cout << "Multiple contours" << endl << endl;
		else 
			cout << "Ball not found" << endl << endl;
		for (int i = 0; i < 2; i++)
			show(camera[i], i + 1);
		point_details temp(pt3d, camera);
		output.seekg(camera[0].frame_no * sizeof(temp));
		output.write((char*)&temp, sizeof(temp));
	}
	output.close();


	
	
	
	/*Mat pt3d;
	pos3d_solve(camera[0].cam.proj_mat, camera[1].cam.proj_mat, camera[0].center, camera[1].center, pt3d);
	cout << pt3d;
	output << pt3d << endl;
	int n;
	cin >> n;
	if (n == 100)
		syncVideo(camera);
	else if (n != 0){
		int pos[2];
		for (int i = 0; i < 2; i++){
			pos[i] = camera[i].cam.cap.get(CAP_PROP_POS_MSEC);
			camera[i].cam.cap.set(CAP_PROP_POS_MSEC, pos[i] + n * 1000);
		}
	}
*/
}


void pos3d_solve(Mat& m1, Mat& m2, Point2d& pt1, Point2d& pt2, Mat& pt)
{
	Mat A(4, 3, CV_64F);
	Mat B(4, 1, CV_64F);
	for (int i = 0; i < 3; i++)
	{
		A.at<double>(0, i) = m1.at<double>(0, i) - pt1.x * m1.at<double>(2, i);
		A.at<double>(1, i) = m1.at<double>(1, i) - pt1.y * m1.at<double>(2, i);
		A.at<double>(2, i) = m2.at<double>(0, i) - pt2.x * m2.at<double>(2, i);
		A.at<double>(3, i) = m2.at<double>(1, i) - pt2.y * m2.at<double>(2, i);
	}
	B.at<double>(0, 0) = -(m1.at<double>(0, 3) - pt1.x * m1.at<double>(2, 3));
	B.at<double>(1, 0) = -(m1.at<double>(1, 3) - pt1.y * m1.at<double>(2, 3));
	B.at<double>(2, 0) = -(m2.at<double>(0, 3) - pt2.x * m2.at<double>(2, 3));
	B.at<double>(3, 0) = -(m2.at<double>(1, 3) - pt2.y * m2.at<double>(2, 3));	

	solve(A, B, pt, DECOMP_QR);
}

void save_current(CamFrame camera[]){
	imwrite("temp/camera1/frame1.jpg",camera[0].orig);
	imwrite("temp/camera1/fgMask1.jpg",camera[0].fgMask);
	imwrite("temp/camera1/fg1.jpg",camera[0].fg);
	imwrite("temp/camera1/colMask1.jpg",camera[0].colMask);
	imwrite("temp/camera1/morph_colMask1.jpg",camera[0].morph_colMask);
	imwrite("temp/camera1/canny1.jpg",camera[0].canny);
	imwrite("temp/camera1/contour_plot1.jpg",camera[0].contour_plot);
	imwrite("temp/camera2/frame2.jpg",camera[1].orig);
	imwrite("temp/camera2/fgMask2.jpg",camera[1].fgMask);
	imwrite("temp/camera2/fg2.jpg",camera[1].fg);
	imwrite("temp/camera2/colMask2.jpg",camera[1].colMask);
	imwrite("temp/camera2/morph_colMask2.jpg",camera[1].morph_colMask);
	imwrite("temp/camera2/canny2.jpg",camera[1].canny);
	imwrite("temp/camera2/contour_plot2.jpg",camera[1].contour_plot);
}

void eliminate_duplication(vector<Point2f>& c){
	vector<Point2f> temp;
	float dist_threshold = 2;
	vector<bool> copied(c.size(), false);
	for(int i = 0; i < c.size(); i++){
		if (copied[i])
			continue;
		int count = 0;
		int pos;
		for(int j = i + 1; j < c.size(); j++){
			if (copied[j])
				continue;
			if (norm(c[i] - c[j]) < dist_threshold){
				count++;
				pos = j;
			}
		}
		if (count != 1){
			temp.push_back(c[i]);
			copied[i] = true;
		}
		else if (count == 1){
			temp.push_back((c[i] + c[pos]) / 2);
			copied[i] = copied[pos] = true;

		}
	}
	c.clear();
	c = temp;
}


void getFundMat(char f[]){
	FileStorage g(f, FileStorage::READ);
	g["fund_mat"] >> fund_mat;
}

void epipolar_constraint(vector<Point2f>& cam1pts, vector<Point2f>& cam2pts){
	float dist_threshold = 200;
	vector<Point3f> cam_lines1;
	vector<bool> to_remove[2];
	computeCorrespondEpilines(cam1pts, 1, fund_mat, cam_lines1);

	to_remove[0].resize(cam1pts.size(), true);
	to_remove[1].resize(cam2pts.size(), true);
	for (int i = 0; i < cam2pts.size(); i++)
		for (int j = 0; j < cam1pts.size(); j++)
			if (dist(cam2pts[i], cam_lines1[j]) < dist_threshold){
				to_remove[0][j] = false;
				to_remove[1][i] = false;
			}
		
	
	for (int i = to_remove[0].size() - 1; i >= 0; i--)
		if (to_remove[0][i] == true)
			cam1pts.erase(cam1pts.begin() + i);

	
	for (int i = to_remove[1].size() - 1; i >= 0; i--)
		if (to_remove[1][i] == true)
			cam2pts.erase(cam2pts.begin() + i);	
}

float dist(Point2f pt, Point3f line){
	float d;
	d = abs(line.x * pt.x + line.y * pt.y + line.z);
	cout << d;
	return d;
}

bool ask_fund_mat(){
	cout << "Want to apply epipolar constraint?(y/n)";
	char ch;
	cin >> ch;
	bool epipolar = false;
	if (ch == 'y'){
		epipolar = true;
		cout << "Source file for fundamental matrix?";
		char f[80];
		cin >> f;
		getFundMat(f);
	}
	return epipolar;
}

void createWindow(){
	namedWindow("camera1colMask", WINDOW_NORMAL);
	namedWindow("camera2colMask", WINDOW_NORMAL);
	namedWindow("camera1morph_colMask", WINDOW_NORMAL);
	namedWindow("camera2morph_colMask", WINDOW_NORMAL);
	namedWindow("camera1contour", WINDOW_NORMAL);
	namedWindow("camera2contour", WINDOW_NORMAL);
	resizeWindow("camera1colMask", SCREEN_WIDTH/2, SCREEN_HEIGHT);
	resizeWindow("camera2colMask", SCREEN_WIDTH/2, SCREEN_HEIGHT);
	resizeWindow("camera1morph_colMask", SCREEN_WIDTH/2, SCREEN_HEIGHT);
	resizeWindow("camera2morph_colMask", SCREEN_WIDTH/2, SCREEN_HEIGHT);
	resizeWindow("camera1contour", SCREEN_WIDTH/2, SCREEN_HEIGHT);
	resizeWindow("camera2contour", SCREEN_WIDTH/2, SCREEN_HEIGHT);
}

void video_controls(CamFrame camera[]){
	static char c = 'q';
	static int wait = 1;
	if (c !='p')
		c = waitKey(wait);
	if (c == 'p'){
		c = waitKey(0);
		if (c == 's'){
			save_current(camera);
			c = 'p';
		}
		/*else if (c == 'f'){
			camera[0].findcenter();
			camera[1].findcenter();
			Mat pt3d;
			pos3d_solve(camera[0].cam.proj_mat, camera[1].cam.proj_mat, camera[0].center, camera[1].center, pt3d);
			cout << pt3d;
			output << pt3d << endl;
			c = '0';
			cin >> c;
		}*/
		else if (c =='n'){
			c = 'p';
		}
		else if (c == 'h'){
			videoControls_help();
			c = 'p';
		}
		else
			c = '0';
	}
	else if (c == 's'){
		syncVideo(camera);
		c = '0';
	}
	/*else if (c == '4'){
		wait++;
		c = '0';
	}
	else if (c == '6'){
		wait--;
		wait = wait > 1 ? wait : 1;
		c = '0';
	}*/
	else if (c == 'f'){
		double t;
		cin >> t;
		double pos[2];
		for (int i = 0; i < 2; i++){
			pos[i] = camera[i].cam.cap.get(CAP_PROP_POS_FRAMES);
			camera[i].cam.cap.set(CAP_PROP_POS_FRAMES, pos[i] + t);
		}
		c = '0';
	}
	else
		c = '0';
}

void init_camera(CamFrame camera[], char** argv){
	cout << endl << "Background learning" << endl;
	cout << "Camera 1:" << endl;
	camera[0] = CamFrame(argv[1], argv[3], Scalar(66/2, 59, 40), Scalar(118/2, 255, 255));		//dslr2 - Scalar(66/2, 59, 40), Scalar(118/2, 255, 255) //basktest1 - Scalar(51/2, 59, 40), Scalar(85/2, 255, 255) //dslr - Scalar(51/2, 59, 20), Scalar(118/2, 255, 255)
	cout << "Camera 2:" << endl;
	camera[1] = CamFrame(argv[2], argv[4], Scalar(51/2, 59, 40), Scalar(85/2, 255, 255));		//dslr2 - Scalar(51/2, 59, 40), Scalar(85/2, 255, 255)  //basktest1 - Scalar(51/2, 59, 58), Scalar(85/2, 255, 255) //dslr - Scalar(51/2, 59, 20), Scalar(118/2, 255, 255)
}

void videoControls_help(){
	namedWindow("video help", WINDOW_NORMAL);
	resizeWindow("video help", SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	Mat img = imread("images/video_help.jpg");
	imshow("video help", img);
	char quit = 'r';
	while (quit != 'q'){
		quit = waitKey(0);
	}
	destroyWindow("video help");
}