#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include "opencv2/video.hpp"
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>

using namespace cv;
using namespace std;

const int COUNT = 300;
const int KERNEL_SIZE = 5;
const int WIDTH = 1280;
const int HEIGHT = 720;
const int N_DILATE = 2;
const int N_ERODE = 2;

class BallDetector
{
    VideoCapture capture;
    Mat frame;
    Mat fg;
    Mat fgMaskMOG;
    Mat thrs;
    Ptr<BackgroundSubtractor> pMOG;
    int keyboard;
    char c;
    Point2d pt;
    void learnBg();
    void subtractBg();
    void thresholding();     
    void contourDetection();
    public:
    BallDetector(string fileName);
    void run();
};
