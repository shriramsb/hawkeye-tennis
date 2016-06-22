#include <interpolation/spline_fit.h>
#include <interpolation/pose.h>
#include <fstream>
int main(int argc, char** argv)
{
    enum {x,y,z};
    SplineFit spFit1[3]={SplineFit(3,0,10),SplineFit(3,0,10),SplineFit(3,0,10)};
    SplineFit spFit2[3]={SplineFit(3,10,0),SplineFit(3,10,0),SplineFit(3,10,0)};
    std::ofstream f("time.txt");
    std::ofstream g("pts.txt");
    std::vector<Pose> pts;
    std::vector<Pose> finalPts;
    std::vector<double> extraPtsX1, extraPtsX2;
    std::vector<double> extraPtsY1, extraPtsY2;
    std::vector<double> extraPtsZ1, extraPtsZ2;
    std::vector<double> _time, time;
    //Sample points for interpolation
    for(float t=0.0;t<40.0;t++)
    {
        Pose position;
        if(t<=29.0)
        {
            position.x = 25-(t/2.85-5)*(t/2.85-5);
            position.y = 10*t;
            position.z = 225-(t/0.95-15)*(t/0.95-15);
            spFit1[x].getXAndY(t,position.x);
            spFit1[y].getXAndY(t,position.y);
            spFit1[z].getXAndY(t,position.z);
            extraPtsX1 = spFit1[x].interpolate();
            extraPtsY1 = spFit1[y].interpolate();
            extraPtsZ1 = spFit1[z].interpolate();
        }
        else if(t>30.0)
        {
            std::cout<<"in else\n";
            position.x = 9-9*(t-35.5)*(t-35.5)/19.5;
            position.y = 10*t;
            position.z = 49-49*(t-35.5)*(t-35.5)/19.5;
            spFit2[x].getXAndY(t,position.x);
            spFit2[y].getXAndY(t,position.y);
            spFit2[z].getXAndY(t,position.z);
            extraPtsX2 = spFit2[x].interpolate();
            extraPtsY2 = spFit2[y].interpolate();
            extraPtsZ2 = spFit2[z].interpolate();
        }
        pts.emplace_back(position);
        _time.emplace_back(t);
    }
    double t_min=spFit1[x].x_min;
    double t_max=spFit1[x].x_max;
    int i=0;
    for(double t=t_min;t<t_max;t+=DX,i++)
    {
        Pose position;
        position.x = extraPtsX1[i];
        position.y = extraPtsY1[i];
        position.z = extraPtsZ1[i];
        finalPts.emplace_back(position);
        time.emplace_back(t);
    }
    t_min=spFit2[x].x_min;
    t_max=spFit2[x].x_max;
    i=0;
    for(double t=t_min;t<t_max;t+=DX,i++)
    {
        Pose position;
        position.x = extraPtsX2[i];
        position.y = extraPtsY2[i];
        position.z = extraPtsZ2[i];
        finalPts.emplace_back(position);
        time.emplace_back(t);
    }
    for(i=0;i<finalPts.size();i++)
    {
        f<<finalPts[i].x<<' '<<finalPts[i].y<<' '<<finalPts[i].z<<' '<<time[i]<<'\n';
        std::cout<<finalPts[i].x<<' '<<finalPts[i].y<<' '<<finalPts[i].z<<' '<<time[i]<<'\n';
    }
    for(i=0;i<pts.size();i++)
        g<<pts[i].x<<' '<<pts[i].y<<' '<<pts[i].z<<' '<<_time[i]<<'\n';
    
    f.close();
    g.close();
    return 0;
}
