#include <interpolation/spline.h>
#include <fstream>
int main(int argc, char** argv)
{
    SplineFit spFit(2,10);
    std::ofstream f("linear.txt");
    std::vector<Pose> pts;
    std::vector<Pose> finalPts;
    //Sample points for interpolation
    for(float i=0.0;i<=20.0;i++)
    {
        Pose position;
        position.y = i;
        position.x = 100-(i-10)*(i-10);
        position.z = 5-(i/5.0-2)*(i/5.0-2);
        pts.push_back(position);
//        std::cout<<pts[i].x<<','<<pts[i].y<<','<<pts[i].z<<'\n';
        spFit.getPosAndTime(position,i/10.0);
        finalPts = spFit.interpolate();
    }
    int nMax = finalPts.size();
    for(int i=0;i<nMax;i++)
    {
        std::cout<<finalPts[i].x<<','<<finalPts[i].y<<','<<finalPts[i].z<<'\n';
        f<<finalPts[i].x<<','<<finalPts[i].y<<','<<finalPts[i].z<<'\n';
    }
    return 0;
}
