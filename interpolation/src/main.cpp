#include <interpolation/spline.h>

int main(int argc, char** argv)
{
    SplineFit spFit;
    std::vector<Pose> pts;
    //Sample points for interpolation
    //TODO getPosAandTime
    for(int i=0;i<=20;i++)
    {
        Pose position;
        position.x = i;
        position.y = 0;
        position.z = 100-(i-10)*(i-10);
        pts.push_back(position);
        std::cout<<pts[i].x<<','<<pts[i].y<<','<<pts[i].z<<'\n';
    }
    std::vector<Pose> finalPts;
    finalPts = spFit.interpolate(pts);
    int nMax = finalPts.size();
    for(int i=0;i<nMax;i++)
        std::cout<<finalPts[i].x<<','<<finalPts[i].y<<','<<finalPts[i].z<<'\n';
    return 0;
}
