#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
 
const double BALL_RAD = 4.5;

void readFile(std::ifstream& fl,double *var)
{
    for(int i=0;i<3;i++)
        fl>>var[i];
}

std::vector<double> interpolate(double pos0, double vel0, double t0, double pos1, double vel1, double t1)
{
    double a,b,c,d,h; 
    a = pos0;
    b = vel0;
    h = t1-t0;
    d = (vel1 - b)/(h*h) - 2*(pos1 - a - b*h)/(h*h*h);
    c = (pos1 - a - b*h)/(h*h) - d*h;

    std::vector<double> finalPos;
    double DX = 0.01;
    std::cout<<"t0"<<t0<<" t1"<<t1<<'\n';
    for(double t=t0;t<=t1;t+=DX)
    {
        std::cout<<"t:"<<t<<"\n";
        double f = a + b*(t-t0) + c*(t-t0)*(t-t0) + d*(t-t0)*(t-t0)*(t-t0);
        finalPos.emplace_back(f);
    }
    return finalPos;
}

int main(int argc, char** argv)
{
    std::ifstream f("bounce.txt");
    std::ofstream g("area.txt");
    double v[2][3], pt[2][3];
    double t[2];
    std::vector< std::vector<double> > finalPt;
    readFile(f,pt[0]);
    f>>t[0];
    readFile(f,pt[1]);
    f>>t[1];
    readFile(f,v[0]);
    readFile(f,v[1]);
    std::cout<<pt[0][0]<<' '<<pt[0][1]<<' '<<pt[0][2]<<'\n';
    std::cout<<pt[1][0]<<' '<<pt[1][1]<<' '<<pt[1][2]<<'\n';
    for(int i=0;i<3;i++)
        finalPt.emplace_back(interpolate(pt[0][i],v[0][i],t[0],pt[1][i],v[1][i],t[1]));
    
    std::cout<<finalPt.size()<<'\n';
    std::cout<<finalPt[0].size()<<'\n';
    std::cout<<finalPt[1].size()<<'\n';
    std::cout<<finalPt[2].size()<<'\n';
    
    std::cout<<t[0]<<' '<<t[1]<<'\n';
    for(int i=0;i<finalPt[0].size();i++)
    {
        double x=finalPt[0][i];
        double y=finalPt[1][i];
        double z=finalPt[2][i];
        double r = sqrt(BALL_RAD*BALL_RAD - z*z);
        double time = t[0]+i*0.01;
        std::cout<<x<<' '<<y<<' '<<z<<' '<<r<<' '<<time<<'\n';
        g<<x<<' '<<y<<' '<<r<<' '<<time<<'\n';
    }
    return 0;
}
