#include "interpolation/spline.h"

int main(int argc, char** argv)
{
    SplineFit spFit(10);
    std::vector<Pose> pts;
    //Sample points for interpolation
    for(int i=0;i<=20;i++)
    {
        Pose position;
        position.x = i;
        position.y = 0;
        position.z = 100-(i-10)*(i-10);
        pts.push_back(position);
    }
    std::vector<Pose> finalPts;
    finalPts = spFit.interpolate(pts);
    return 0;
}
