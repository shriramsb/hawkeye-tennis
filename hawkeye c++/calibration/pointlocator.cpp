#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>

using namespace cv;
using namespace std;

const int n_cols = 7;
const int n_rows = 7;
const Size board_size = Size(n_cols, n_rows);
const float sqSize = 2.35;

class Camera{
public:
	Mat cam_mat;			//optimal camera matrix
	Mat rvec, tvec;			
	Mat proj_mat;			//M*[R|T]
	Mat orig_cam_mat;		
	Mat dist_coeff;
	void calcproj_mat(){
		Mat rot_mat;
		Rodrigues(rvec, rot_mat);									//rotation matrix R from rvec
		Mat aug_mat(3, 4, CV_64F);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				aug_mat.at<double>(i,j) = rot_mat.at<double>(i,j);
		for (int i = 0; i < 3; i++)
			aug_mat.at<double>(i,3) = tvec.at<double>(i,0);			//aug_mat = [R|T]

		proj_mat = cam_mat * aug_mat;
	}
	void init(FileStorage& f, char* filename){
		read(f);
		Mat chbd = imread(filename);
		vector<Point2f> img_pts;
		bool found = findChessboardCorners(chbd, board_size, img_pts);
		cout << found;
		Mat chbd_gray;
		cvtColor(chbd, chbd_gray, COLOR_BGR2GRAY);
		cornerSubPix(chbd_gray, img_pts, Size(11,11),Size(-1,-1),TermCriteria(TermCriteria::EPS+TermCriteria::COUNT, 30, 0.1));
		vector<Point3f> obj_pts;
		for (int i = 0; i < n_rows; i++)
			for (int j = 0; j < n_cols; j++)
				obj_pts.push_back(Point3d(j * sqSize, i * sqSize, 0));
		
		solvePnP(obj_pts, img_pts, orig_cam_mat, dist_coeff, rvec, tvec);	//gets rvec and tvec
		calcproj_mat();
	
	}
	void read(FileStorage& node){
		node["Optimalcammatrix"] >> cam_mat;
		node["CameraMatrix"] >> orig_cam_mat;
		node["Distortion_Coefficients"] >> dist_coeff;
	}
};
void get3dloc(Mat& m1, Mat& m2, Point2d& pt1, Point2d& pt2, Mat& pt)
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
int main(){
	Camera cam1, cam2;
	Point2d pt1, pt2;
	Mat pt;
	get3dloc(cam1.proj_mat, cam2.proj_mat, pt1, pt2, pt);
	cout << pt;
	cout << cam1.rvec << endl << cam1.tvec; 
}