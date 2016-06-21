

class point_details{
public:
	Point3d pt3d;			//not able to write Mat object directly to file
	Point2d pt[2];
	float frame_no[2];
	float frame_time[2];
	point_details();
	point_details(Mat& a, CamFrame c[]);
};