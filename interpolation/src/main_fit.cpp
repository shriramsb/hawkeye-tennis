#include <interpolation/spline_fit.h>
#include <interpolation/pose.h>
#include <fstream>
int main(int argc, char** argv)
{
    const int NPOINTS = 10;
    SplineFit spFitX(3,NPOINTS);
    SplineFit spFitZ(3,NPOINTS);
    std::ofstream f("cubic.txt");
    std::vector<Pose> pts;
    std::vector<Pose> finalPts;
    std::vector<double> finalPtsX;
    std::vector<double> finalPtsZ;
    //Sample points for interpolation
    std::cout<<"Q\n";
    for(float i=0.0;i<=20.0;i++)
    {
        Pose position;
        position.y = i;
        position.x = 100-(i-10)*(i-10);
        position.z = 5-(i/5.0-2)*(i/5.0-2);
        pts.push_back(position);
        spFitX.getXAndY(position.y,position.x);
        std::cout<<"W\n";
        spFitZ.getXAndY(position.y,position.z);
        std::cout<<"E\n";
        finalPtsX = spFitX.interpolate();
        std::cout<<"R\n";
        finalPtsZ = spFitZ.interpolate();
        std::cout<<"T\n";
    }
    int nMax = pts.size();
    for(int i=0;i<nMax-1;i++)
    {
        double my = (pts[i+1].y - pts[i].y)/NPOINTS;
        std::cout<<"Y\n";
        for(int j=0;j<NPOINTS;j++)
        {
            double yj = pts[i].y + j*my;
            Pose position;
            position.y = yj;
            position.x = finalPtsX[i*NPOINTS+j];
            position.z = finalPtsZ[i*NPOINTS+j];
            finalPts.push_back(position);
        }
    }
    nMax = finalPts.size();
    for(int i=0;i<nMax;i++)
    {
        f<<finalPts[i].x<<' '<<finalPts[i].y<<' '<<finalPts[i].z<<'\n';
        std::cout<<finalPts[i].x<<' '<<finalPts[i].y<<' '<<finalPts[i].z<<'\n';
    }
    f.close();
    return 0;
}
