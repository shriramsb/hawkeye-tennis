#include <interpolation/spline_fit.h>
#include <interpolation/pose.h>
#include <fstream>

void getImpactArea()
{
    
}

int main(int argc, char** argv)
{
    enum {x,y,z};
    SplineFit spFit1[3]={SplineFit(3,0,0),SplineFit(3,0,0),SplineFit(3,0,0)};
    SplineFit spFit2[3]={SplineFit(3,0,0),SplineFit(3,0,0),SplineFit(3,0,0)};
    std::ofstream f("time.txt");
    std::ofstream g("pts.txt");
    std::vector<Pose> pts;
    std::vector<Pose> finalPts;
    std::vector<double> extraPtsX1, extraPtsX2, vx1, vx2;
    std::vector<double> extraPtsY1, extraPtsY2, vy1, vy2;
    std::vector<double> extraPtsZ1, extraPtsZ2, vz1, vz2;
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
            vx1 = spFit1[x].differentiate();
            extraPtsY1 = spFit1[y].interpolate();
            vy1 = spFit1[y].differentiate();
            extraPtsZ1 = spFit1[z].interpolate();
            vz1 = spFit1[z].differentiate();
        }
        else if(t>30.0)
        {
            position.x = 9-9*(t-35.5)*(t-35.5)/19.5;
            position.y = 10*t;
            position.z = 49-49*(t-35.5)*(t-35.5)/19.5;
            spFit2[x].getXAndY(t,position.x);
            spFit2[y].getXAndY(t,position.y);
            spFit2[z].getXAndY(t,position.z);
            extraPtsX2 = spFit2[x].interpolate();
            vx2 = spFit2[x].differentiate();
            extraPtsY2 = spFit2[y].interpolate();
            vy2 = spFit2[y].differentiate();
            extraPtsZ2 = spFit2[z].interpolate();
            vz2 = spFit2[z].differentiate();
        }
        pts.emplace_back(position);
        _time.emplace_back(t);
    }
    double t_min=spFit1[x].x_min;
    double t_max=spFit1[x].x_max;
    double DX = spFit1[x].DX;
    int i=0;
    for(double t=t_min;t<t_max;t+=DX,i++)
    {
        Pose position;
        position.x = extraPtsX1[i];
        position.y = extraPtsY1[i];
        position.z = extraPtsZ1[i];
        position.vx = vx1[i];
        position.vy = vy1[i];
        position.vz = vz1[i];
        finalPts.emplace_back(position);
        time.emplace_back(t);
    }
    t_min=spFit2[x].x_min;
    t_max=spFit2[x].x_max;
    i=0;
    std::cout<<"MMMMMMMMMMM";
    for(double t=t_min;t<t_max;t+=DX,i++)
    {
        Pose position;
        position.x = extraPtsX2[i];
        position.y = extraPtsY2[i];
        position.z = extraPtsZ2[i];
        position.vx = vx2[i];
        position.vy = vy2[i];
        position.vz = vz2[i];
        finalPts.emplace_back(position);
        time.emplace_back(t);
    }
    for(i=0;i<finalPts.size();i++)
    {
     //   if(finalPts[i].z<=2*BALL_RAD)
       //     std::cout<<"z:"<<finalPts[i].z<<", t:"<<time[i]<<'\n';
        f<<finalPts[i].x<<' '<<finalPts[i].y<<' '<<finalPts[i].z<<' ';
        f<<finalPts[i].vx<<' '<<finalPts[i].vy<<' '<<finalPts[i].vz<<' '<<time[i]<<'\n';
        //std::cout<<finalPts[i].x<<' '<<finalPts[i].y<<' '<<finalPts[i].z<<' '<<time[i]<<'\n';
    }
    for(i=0;i<pts.size();i++)
        g<<pts[i].x<<' '<<pts[i].y<<' '<<pts[i].z<<' '<<_time[i]<<'\n';
    
    double t1 = spFit1[z].getXFromY(BALL_RAD,false);
    double t2 = spFit2[z].getXFromY(BALL_RAD,true);
    std::cout<<"\nz:"<<BALL_RAD<<", t1:"<<t1<<", y:"<<spFit1[y].fy(t1)<<", x:"<<spFit1[x].fy(t1);
    std::cout<<"\nz:"<<BALL_RAD<<", t2:"<<t2<<", y:"<<spFit2[y].fy(t2)<<", x:"<<spFit2[x].fy(t2);
    f.close();
    g.close();
    getImpactArea();
    return 0;
}
void getMaxRec()
{
            
}
