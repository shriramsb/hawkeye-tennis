#include <interpolation/spline_fit.h>
#include <iostream>
#include <fstream>
#include <math.h>

void readFile(std::ifstream& fl,double *var)
{
    for(int i=0;i<3;i++)
        fl>>var[i];
}

int main(int argc, char** argv)
{
    std::ifstream f("bounce.txt");
    std::ofstream g("area.txt");
    double v[2][3], pt[2][3];
    double t[2];
    std::vector< std::vector<double> > finalPt;
    std::vector< std::vector<double> > finalVel;
    readFile(f,pt[0]);
    f>>t[0];
    readFile(f,pt[1]);
    f>>t[1];
    readFile(f,v[0]);
    readFile(f,v[1]);
    std::cout<<v[0][0]<<' '<<v[0][1]<<' '<<v[0][2]<<'\n';
    std::cout<<v[1][0]<<' '<<v[1][1]<<' '<<v[1][2]<<'\n';
    SplineFit spFit[3] = {SplineFit(1,0,0,0.01), SplineFit(1,0,0,0.01), SplineFit(1,0,0,0.01)};
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<3;j++)
        {
            spFit[j].getXAndY(t[i],v[i][j]);
            spFit[j].interpolate();
            if(i==1)
            {
                finalPt.emplace_back(spFit[j].integrate());
                finalVel.emplace_back(spFit[j].interpolate());
            }
        }
    }
    std::cout<<finalPt.size()<<'\n';
    std::cout<<finalPt[0].size()<<'\n';
    std::cout<<finalPt[1].size()<<'\n';
    std::cout<<finalPt[2].size()<<'\n';
    
    std::cout<<pt[0][0]<<' '<<pt[0][1]<<' '<<pt[0][2]<<'\n';
    std::cout<<pt[1][0]<<' '<<pt[1][1]<<' '<<pt[1][2]<<'\n';
    std::cout<<t[0]<<' '<<t[1]<<'\n';
    finalPt[0][0] = 0;
    finalPt[1][0] = 0;
    finalPt[2][0] = 0;
    for(int i=0;i<finalPt[0].size();i++)
    {
        double x=pt[0][0]+finalPt[0][i], vx=finalVel[0][i];
        double y=pt[0][1]+finalPt[1][i], vy=finalVel[1][i];
        double z=pt[0][2]+finalPt[2][i], vz=finalVel[2][i];
        double r = sqrt(BALL_RAD*BALL_RAD - z*z);
        double time = t[0]+i*0.01;
        std::cout<<x<<' '<<y<<' '<<z<<' '<<r<<' '<<time<<'\n';
        std::cout<<"Velocity: "<<vx<<' '<<vy<<' '<<vz<<"\n\n";
        g<<x<<' '<<y<<' '<<r<<' '<<time<<'\n';
    }
    return 0;
}
