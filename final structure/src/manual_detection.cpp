/*Selection of center of ball in images manually
	Gets 2 video file and 2 calibration data files and output file as input*/

#include <detection.h>
#include <point_details.h>

using namespace cv;
using namespace std;

Mat img;
Mat img_detected;
float radius = 1;
Size image_size;
Point2f temp;
point_details p;
Mat proj_mat[2];

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

void draw_show(){
	img_detected.release();
	img_detected = img.clone();
	circle(img_detected, temp, radius, Scalar(0,0,255));
	imshow("Manual Detection", img_detected);
}
void mouse_click(int event, int x , int y , int, void*){
	if (event == EVENT_LBUTTONDOWN){
		temp.x = x;
		temp.y = y;
	}
	draw_show();
}

void correctFormat();
int main(int argc, char** argv){
	correctFormat();
	cout << "Check whether input is in above format.\nContinue(y/n)?";
	char correct = 'n';
	cin >> correct;
	if (correct != 'y')
		return 1;
	for (int i = 0; i < 2; i++){
		cout << "Enter frame_no";
		cin >> p.frame_no[i];
		VideoCapture cap(argv[i + 1]);
		cap.set(CV_CAP_PROP_POS_FRAMES, p.frame_no[i]);
		p.frame_time[i] = cap.get(CV_CAP_PROP_POS_MSEC);
		cap.read(img);
		
		img_detected = img.clone();
		image_size = img.size();
		namedWindow("Manual Detection", WINDOW_NORMAL);
		imshow("Manual Detection", img_detected);
		setMouseCallback("Manual Detection", mouse_click);
		char c = 'p';
		while (c != 'q'){
			c = waitKey(30);
			if (c == 'w'){
				temp.y-=0.5;
				temp.y = temp.y > 0 ? temp.y : 0;
				draw_show();
				c = 'p';
			}
			else if (c == 'a'){
				temp.x-=0.5;
				temp.x = temp.x > 0 ? temp.x : 0;
				draw_show();
				c = 'p';
			}
			else if (c == 's'){
				temp.y+=0.5;
				temp.y = temp.y < image_size.height ? temp.y : image_size.height;
				draw_show();
				c = 'p';
			}
			else if (c == 'd'){
				temp.x+=0.5;
				temp.x = temp.x < image_size.width ? temp.x : image_size.width;
				draw_show();
				c = 'p';
			}
			else if (c == '+'){
				radius+=0.5;
				draw_show();
				c = 'p';
			}
			else if (c == '-'){
				radius-=0.5;
				radius = radius > 1 ? radius : 1;
				draw_show();
				c = 'p';
			}
		}
		char dest[50];
		sprintf(dest, "manual/camera%d/%lf.jpg", i + 1, p.frame_no[i]);
		imwrite(dest, img_detected);
		p.pt[i] = temp;
	}
	for (int i = 0; i < 2; i++){
		FileStorage f(argv[i + 3], FileStorage::READ);
		f["proj_mat"] >> proj_mat[i];
		f.release();
	}
	Mat pt3d;
	pos3d_solve(proj_mat[0], proj_mat[1], p.pt[0], p.pt[1], pt3d);
	fstream f(argv[5], ios::out | ios::in);
	p.pt3d = Point3d(pt3d);
	f.seekg(p.frame_no[0] * sizeof(p));
	f.write((char*)&p ,sizeof(p));
	f.close();
} 

void correctFormat(){
	cout << "First argument: " << "Video from camera1" << endl;
	cout << "Second argument: " << "Video from camera2" << endl;
	cout << "Third argument: " << "calibration data file for camera1" << endl;
	cout << "Fourth argument: " << "calibration data file for camera2" << endl;
	cout << "Fifth argument: " << "output file containing calculated 3d location" << endl;
}
