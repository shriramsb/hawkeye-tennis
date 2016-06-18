#include <interpolation/spline_fit.h>
#include <interpolation/pose.h>
#include <fstream>
int main(int argc, char** argv)
{
    SplineFit spFitX(3,0,0);
    SplineFit spFitZ(3,0,0);
    SplineFit spFitX1(3,0,0);
    SplineFit spFitZ1(3,0,0);
    std::ofstream f("extra.txt");
    std::ofstream g("pts.txt");
    std::vector<Pose> pts;
    std::vector<Pose> finalPts;
    std::vector<double> finalPtsX;
    std::vector<double> finalPtsZ;
    std::vector<double> finalPtsX1;
    std::vector<double> finalPtsZ1;
    //Sample points for interpolation
    for(float i=0.0;i<40.0;i++)
    {
        Pose position;
        position.y = i;
        if(i<=19.0)
        {
            position.x = 100-(i-10)*(i-10);
            position.z = 25-(i/2.0-5)*(i/2.0-5);
            spFitX.getXAndY(position.y,position.x);
            spFitZ.getXAndY(position.y,position.z);
            finalPtsX = spFitX.interpolate();
            finalPtsZ = spFitZ.interpolate();
        }
        else if(i>20.0)
        {
            std::cout<<"in else\n";
            position.x = 49-((i-20)/(10.0/7)-7)*((i-20)/(10.0/7)-7);
            position.z = 9-((i-20)/(10.0/3)-3)*((i-20)/(10.0/3)-3);
            spFitX1.getXAndY(position.y,position.x);
            spFitZ1.getXAndY(position.y,position.z);
            finalPtsX1 = spFitX1.interpolate();
            finalPtsZ1 = spFitZ1.interpolate();
        }
        pts.push_back(position);
    }
    double y_min=spFitX.x_min;
    double y_max=spFitX.x_max;
    int i=0;
    for(double y=y_min;y<y_max;y+=DX,i++)
    {
        Pose position;
        position.y = y;
        position.x = finalPtsX[i];
        position.z = finalPtsZ[i];
        finalPts.push_back(position);
    }
    y_min=spFitX1.x_min;
    y_max=spFitX1.x_max;
    i=0;
    for(double y=y_min;y<y_max;y+=DX,i++)
    {
        Pose position;
        position.y = y;
        position.x = finalPtsX1[i];
        position.z = finalPtsZ1[i];
        finalPts.push_back(position);
    }
    for(i=0;i<finalPts.size();i++)
    {
        f<<finalPts[i].x<<' '<<finalPts[i].y<<' '<<finalPts[i].z<<'\n';
        std::cout<<finalPts[i].x<<' '<<finalPts[i].y<<' '<<finalPts[i].z<<'\n';
    }
    for(i=0;i<pts.size();i++)
        g<<pts[i].x<<' '<<pts[i].y<<' '<<pts[i].z<<'\n';
    
    f.close();
    g.close();
    return 0;
}
