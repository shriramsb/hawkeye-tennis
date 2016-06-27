#include <detection/locator.h>

Locator::Locator(VideoCapture v1, VideoCapture v2):d1(v1),d2(v2)
{
    getMatrices();
}
void Locator::getMatrices()
{
}
void get3dloc()
{
    pt1 = d1.getXAndY();
    pt2 = d2.getXAndY();
}
