#include <detection.h>


void syncVideo(CamFrame camera[]){
	namedWindow("camera1", WINDOW_NORMAL);
	namedWindow("camera2", WINDOW_NORMAL);
	for (int i = 0; i < 2; i++){
		camera[i].getframe();
		char temp[20];
		sprintf(temp, "camera%d", i);
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
		c = waitKey(0);
	}
	destroyWindow("camera1");
	destroyWindow("camera2");
}

void show(CamFrame& camera, int window){
	char windowname[50];
	//sprintf(windowname, "camera%dframe",window);
	//imshow(windowname, camera.orig);
	sprintf(windowname, "camera%dmorph_colMask",window);
	imshow(windowname, camera.morph_colMask);
	sprintf(windowname, "camera%dcontour",window);
	imshow(windowname, camera.contour_plot);
	waitKey(0);
}
void get3dloc(CamFrame camera[], fstream& output){
	for (int i = 0; i < 2; i++){
		camera[i].getframe();
		camera[i].subtBgColDet();
		camera[i].contourDetection();
		show(camera[i], i + 1);
		camera[i].findcenter();
	}
	Mat pt3d;
	pos3d_solve(camera[0].cam.proj_mat, camera[1].cam.proj_mat, camera[0].center, camera[1].center, pt3d);
	cout << pt3d;
	output << pt3d << endl;
	waitKey(0);
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

	solve(A, B, pt, DECOMP_SVD);
}