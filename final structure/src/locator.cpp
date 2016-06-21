#include <detection.h>

Mat fund_mat;

point_details::point_details(){
	Mat temp = Mat::zeros(3,1,CV_64F);
	pt3d = Point3d(temp);
	for (int i = 0; i < 2; i++){
		pt[i] = Point2d(-1,-1);
		frame_no[i] = -1;
		frame_time[i] = -1;
	}
}
point_details::point_details(Mat& pt_3d, CamFrame c[]){
	pt3d = Point3d(pt_3d.clone());
	for (int i = 0; i < 2; i++){
		pt[i] = c[i].center;
		frame_no[i] = c[i].frame_no;
		frame_time[i] = c[i].frame_time;
	}
}
void syncVideo(CamFrame camera[]){				//manual synchronization of video 
	namedWindow("camera1", WINDOW_NORMAL);
	namedWindow("camera2", WINDOW_NORMAL);
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
			camera[0].getframe();
			imshow("camera1", camera[0].orig);
		}
		else if (c == '2'){
			camera[1].getframe();
			imshow("camera2", camera[1].orig);
		}
		else if (c == '8'){
			camera[0].nextsecond();
			imshow("camera1", camera[0].orig);
		}
		else if (c == '9'){
			camera[1].nextsecond();
			imshow("camera2", camera[1].orig);
		}
		else if (c == 'p'){
			cout << camera[0].cam.cap.get(CAP_PROP_POS_FRAMES) << endl << camera[1].cam.cap.get(CAP_PROP_POS_FRAMES);
		}
		else if (c == 'r'){
			camera[0].cam.cap.set(CAP_PROP_POS_FRAMES, 1935);
			camera[1].cam.cap.set(CAP_PROP_POS_FRAMES, 1921);
		}
		c = waitKey(0);
	}
	destroyWindow("camera1");
	destroyWindow("camera2");
}

void show(CamFrame& camera, int window){					//display frame
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
		show(camera[i], i + 1);
		//camera[i].findcenter();
	}
	if (epipolar && camera[0].constraint_center.size() >= 1 && camera[1].constraint_center.size() >= 1){
		epipolar_constraint(camera[0].constraint_center, camera[1].constraint_center);
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
		pos3d_solve(camera[0].cam.proj_mat, camera[1].cam.proj_mat, camera[0].center, camera[1].center, pt3d);	
		cout << camera[0].center << camera[1].center << pt3d << endl << endl;
		point_details temp(pt3d, camera);
		output.seekg(camera[0].frame_no * sizeof(temp));			//will take a lot of space but easy to read
		output.write((char*)&temp, sizeof(temp));
	}
	else{
		point_details temp;
		cout << pt3d;
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
	float dist_threshold = 10;
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
	return d;
}