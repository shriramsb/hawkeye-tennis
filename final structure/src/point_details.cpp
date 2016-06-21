#include "detection.h"
#include "point_details.h"

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