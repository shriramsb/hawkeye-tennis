#include <detection.h>

CamFrame::CamFrame(){

}
CamFrame::CamFrame(char* calib, char* video) : cam(calib, video){

}

void CamFrame::getframe(){
	Mat temp;
	cam.cap.read(temp);
	remap(temp, orig, cam.map1, cam.mapx, cam.mapy, INTER_LINEAR);
}

void CamFrame::subtBgColDet(){
	medianBlur(orig, orig, kernel_size);
	cam.pMOG2->apply(orig, fgMask, 0);						//without updating background model getting fgMask
	orig.copyTo(fg, fgMask);
	Scalar lb = (51/2, 59, 150);							//hsv values to detect tennis ball
	Scalar ub = (81/2, 255, 255);
	Mat fg_hsv;
	cvtColor(fg, fg_hsv, COLOR_BGR2HSV);
	inRange(fg_hsv, lb, ub, colMask);						//colMask by filtering fg 
}

void CamFrame::contourDetection(){
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
	for (int i = 0; i < contours.size(); i++){								//drawing contours which satisfy constraint 
		Point2f center1 = minEllipse[i].center;
		Size2f s = minEllipse[i].size;
		if(contours[i].size() > 5 && s.width/s.height > 0.7 && s.width/s.height < 1.4 && s.width > 5 && s.height > 5){
			ellipse(contour_plot, minEllipse[i], red);
			circle(contour_plot, center1, 1, red);
		}
	}
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
		Point2f center1 = minEllipse[i].center;				//			choose one containing center
		Size2f s = minEllipse[i].size;
		if(contours[i].size() > 5 && s.width/s.height > 0.7 && s.width/s.height < 1.4 && s.width > 5 && s.height > 5){
			cout << i << endl;
			cout << center1.x << " " << center1.y << endl;
			cout << s.width << " " << s.height << endl << endl;
		}
	}
	cout << "Select contour";
	int sel;
	cin >> sel;
	center = minEllipse[sel].center;
}


