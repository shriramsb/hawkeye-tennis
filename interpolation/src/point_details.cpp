#include <interpolation/point_details.h>

point_details::point_details(){
	Mat temp = Mat::zeros(3,1,CV_64F);
	pt3d = Point3d(temp);
	for (int i = 0; i < 2; i++){
		pt[i] = Point2d(-1,-1);
		frame_no[i] = -1;
		frame_time[i] = -1;
	}
}
