#include <detection/detector.h>

class Locator
{
    Detector d1;
    Detector d2;
    Mat m1;
    Mat m2;
    Point3D pt3D;
    public:
    Locator(VideoCapture v1, VideoCapture v2):d1(v1),d2(v2)
    void getMatrices();
    void get3dloc();
};
