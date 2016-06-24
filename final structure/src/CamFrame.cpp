#include <detection.h>

CamFrame::CamFrame(){

}
CamFrame::CamFrame(char* calib, char* video, Scalar l, Scalar u) : cam(calib, video){
	lb = l;
	ub = u;
}
void CamFrame::clear(){
	orig.release();
	fgMask.release();
	fg.release();
	colMask.release();
	morph_colMask.release();
	canny.release();
	contour_plot.release();
	contours.clear();
	hierarchy.clear();
	minRect.clear();
	minEllipse.clear();
	minCircle.clear();
	constraint_center.clear();
}

void CamFrame::getframe(){
	Mat temp;
	temp.release();
	frame_no = cam.cap.get(CAP_PROP_POS_FRAMES);
	frame_time = cam.cap.get(CAP_PROP_POS_MSEC);
	cam.cap.read(temp);

	remap(temp, orig, cam.mapx, cam.mapy, INTER_LINEAR);
}
void CamFrame::nextsecond(){
	Mat temp;
	int pos = cam.cap.get(CAP_PROP_POS_MSEC);
	cam.cap.set(CAP_PROP_POS_MSEC, pos + 1000);
	cam.cap.read(temp);
	remap(temp, orig, cam.mapx, cam.mapy, INTER_LINEAR);
}

void CamFrame::subtBgColDet(){
	medianBlur(orig, orig, kernel_size);
	
	cam.pMOG2->apply(orig, fgMask, 0);						//without updating background model getting fgMask
	threshold(fgMask, fgMask, 230, 255, THRESH_BINARY);
	orig.copyTo(fg, fgMask);
	Mat fg_hsv;
	fg_hsv.release();
	cvtColor(fg, fg_hsv, COLOR_BGR2HSV);
	
	inRange(fg_hsv, lb, ub, colMask);						//colMask by filtering fg 

}

bool CamFrame::contourDetection(){
	int thresh = 200;										
	int thresh_max_val = 255;
	int n_erode = 1;
	threshold(colMask, morph_colMask, thresh, thresh_max_val, THRESH_BINARY);		//thresholding colMask to remove gray color
	Mat element = getStructuringElement(MORPH_RECT,Size(kernel_size,kernel_size));	//				in some places
	for (int i = 0; i < n_erode; i++)
		erode(morph_colMask, morph_colMask, element);
	for (int i = 0; i < 2*n_erode; i++)
		dilate(morph_colMask, morph_colMask, element);
	for (int i = 0; i < n_erode; i++)
		erode(morph_colMask, morph_colMask, element);
	Canny(morph_colMask, canny, 50, 150, 3);

	findContours(canny, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));

	minRect.resize(contours.size());
	minEllipse.resize(contours.size());
	minCircle.resize(contours.size());
	vector<vector<Point> > contours_poly(contours.size());

	for (int i = 0; i < contours.size(); i++){
		minRect[i] = minAreaRect(Mat(contours[i]));
		if(contours[i].size() > 5)
			minEllipse[i] = fitEllipse(Mat(contours[i]));
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		minEnclosingCircle( contours_poly[i], minCircle[i].centre, minCircle[i].radius);
	}
	orig.copyTo(contour_plot);
	Scalar red = Scalar(0,0,255);
	bool found_contour = false;
	for (int i = 0; i < contours.size(); i++){								//drawing contours which satisfy constraint 
		Point2f center1 = minRect[i].center;
		Size2f s = minRect[i].size;
		if(contours[i].size() > 5 && s.width/s.height > 0.7 && s.width/s.height < 1.4 && s.width > 5 && s.height > 5){
			circle(contour_plot, minCircle[i].centre,minCircle[i].radius, red);
			constraint_center.push_back(minCircle[i].centre);						//assuming it creates a copy of minCircle[i] and 
			circle(contour_plot, minCircle[i].centre, 1, red);				//						stores it
			found_contour = true;
		}
	}
	eliminate_duplication(constraint_center);
	return found_contour;
}


void CamFrame::findcenter(){
	/*if (contours.size() == 1){
		center = minEllipse[0].center;
		return;
	}

	else if (contours.size() == 2){
		Point2f center[2] = {minEllipse[0].center, minEllipse[1].center};
		if (norm(center[0] - center[1]) < 0.5){
			center = (center[0] + center[1])/2;
			return;
		}
	}*/
	for (int i = 0; i < contours.size(); i++){				//prints all selected contours satisfying constraint and asks to
		Point2f center1 = minCircle[i].centre;				//			choose one containing center
		Size2f s = minRect[i].size;
		if(contours[i].size() > 5 && s.width/s.height > 0.7 && s.width/s.height < 1.4 && s.width > 5 && s.height > 5){
			cout << i << endl;
			cout << center1.x << " " << center1.y << endl;
			cout << s.width << " " << s.height << endl << endl;
		}
	}
	cout << "Select contour";
	int sel;
	cin >> sel;
	center = minCircle[sel].centre;
}


