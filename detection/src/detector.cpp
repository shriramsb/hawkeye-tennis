#include <detection/ball_detector.h>

BallDetector::BallDetector(string fileName)
{
    capture.open(fileName);
    pMOG = createBackgroundSubtractorMOG2();
    for (int i = 0; i < 100; i++)
        capture.read(frame);
}
void BallDetector::run()
{
    learnBg();
    subtractBg();
}
void BallDetector::learnBg() 
{
    pMOG = createBackgroundSubtractorMOG2();
    for (int i = 0; i < COUNT; i++)
    {
        cap.read(frame);
        medianBlur(frame, frame, 5);
        pMOG->apply(frame, fg);
    }
}

void BallDetector::subtractBg()
{
    capture.set(CAP_PROP_POS_FRAMES, 900);
    while(c != 'q')
    {
        capture.read(frame);
        medianBlur(frame,frame,KERNEL_SIZE);
        pMOG->apply(frame, fgMaskMOG);
        imshow("frame", frame);
        imshow("fg",fgMaskMOG);
        c = waitKey(30);
        if(c=='q')
        {
            imwrite("images/frame.png",frame);
            imwrite("images/fgmask.png",fgMaskMOG);
            fg = Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
            frame.copyTo(fg,fgMaskMOG);
            imwrite("images/fg.png",fg);
            c = waitKey(0);
            c = 'p';
        }
    }
}

void BallDetector::thresholding() 
{
    threshold(fg, thrs, 0,255,0);
    Mat element = getStructuringElement(MORPH_RECT, Size(3,3),Point(1,1));
    for (int i = 0; i < N_DILATE; i++) 
        dilate(thrs, thrs, element);
    for (int i = 0; i < N_ERODE; i++) 
        erode(thrs, thrs, element);
}

void BallDetector::contourDetection()
{
    Canny(thrs,cannyImg, 50, 150, 3);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(cannyImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));
    vector<RotatedRect> minRect(contours.size());
    vector<Point2f> center(contours.size());
    vector<float> radius(contours.size());
    for( size_t i = 0; i < contours.size(); i++)
    {
        if(contours[i].size() > 5)
            minRect[i] = fitEllipse(Mat(contours[i]));
        minEnclosingCircle(contours[i],center[i],radius[i]);
    }
    
    RNG rng(12345);
    Mat drawing = Mat::zeros(cannyImg.size(),CV_8UC3);
    for( size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        Size2f s = minRect[i].size;
        Point2f c = minRect[i].center;
        if(contours[i].size() > 5 && s.width/s.height > 0.7 && s.width/s.height < 1.4 && s.width > 5 && s.height > 5)
            // && arcLength(contours[i],true)/(2*CV_PI*radius[i])>0.8 && arcLength(contours[i],true)/(2*CV_PI*radius[i]) < 1.2 )
        {
            drawContours(drawing, contours, (int)i, color, -1, 8, hierarchy, 0, Point());
            cout<<c.x<<" "<<c.y<<endl;
            cout<<s.width<<" "<<s.height<<endl<<endl;
        }
    }
    imshow("contours",drawing);
    imwrite("images/contours.png",drawing);
}
