#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;


class point_details{
public:
	Point3d pt3d;			//not able to write Mat object directly to file
	Point2d pt[2];
	float frame_no[2];
	float frame_time[2];
	point_details();
};
