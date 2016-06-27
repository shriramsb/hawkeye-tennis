#include <interpolation/spline_fit.h>
#include <interpolation/pose.h>
#include <interpolation/point_details.h>
#include <fstream>
using namespace cv;

int main(int argc, char** argv)
{
    int start_n = atoi(argv[1]);
    int end_n = atoi(argv[2]);
    int split_n = atoi(argv[3]);
    enum {x,y,z};
    SplineFit spFit1[3]={SplineFit(3,0,200),SplineFit(3,0,200),SplineFit(3,0,200)};
    SplineFit spFit2[3]={SplineFit(3,200,0),SplineFit(3,200,0),SplineFit(3,200,0)};
    std::ofstream f("time.txt");
    std::ofstream g("pts.txt");
    std::ofstream h("bounce.txt");
    std::vector<Pose> pts;
    std::vector<Pose> finalPts;
    std::vector<double> extraPtsX1, extraPtsX2, vx1, vx2;
    std::vector<double> extraPtsY1, extraPtsY2, vy1, vy2;
    std::vector<double> extraPtsZ1, extraPtsZ2, vz1, vz2;
    std::vector<double> _time, time;
    //Sample points for interpolation
    std::fstream fl("output.txt",std::ios::binary|std::ios::in);
    point_details pt;
    fl.seekg(start_n*sizeof(pt),std::ios::beg);
    int current_n = start_n;
	fl.read((char*)&pt,sizeof(pt));
    double start_time = pt.frame_time[0];
    while(current_n < end_n)
    {
    current_n = fl.tellg()/sizeof(pt);    
	fl.read((char*)&pt,sizeof(pt));
	double t = pt.frame_time[0] - start_time;	//0 or 1 whichever is better 
        Pose position;
	position.x = pt.pt3d.x;
	position.y = pt.pt3d.y;
	position.z = -pt.pt3d.z;
        if(current_n<split_n)
        {
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
        else
        {
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
    DX = spFit2[x].DX;
    i=0;
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
    //std::cout<<"\nz:"<<BALL_RAD<<", t1:"<<t1<<", y1:"<<spFit1[y].fy(t1)<<", x1:"<<spFit1[x].fy(t1);
    //std::cout<<"\nz:"<<BALL_RAD<<", t2:"<<t2<<", y2:"<<spFit2[y].fy(t2)<<", x2:"<<spFit2[x].fy(t2);
    //std::cout<<"\nvx1:"<<spFit1[x].fyDeriv(t1)<<", vy1:"<<spFit1[y].fyDeriv(t1)<<", vz1:"<<spFit1[z].fyDeriv(t1);
    //std::cout<<"\nvx2:"<<spFit2[x].fyDeriv(t2)<<", vy2:"<<spFit2[y].fyDeriv(t2)<<", vz2:"<<spFit2[z].fyDeriv(t2);
    h<<spFit1[x].fy(t1)<<' '<<spFit1[y].fy(t1)<<' '<<spFit1[z].fy(t1)<<' '<<t1<<'\n';
    h<<spFit2[x].fy(t2)<<' '<<spFit2[y].fy(t2)<<' '<<spFit2[z].fy(t2)<<' '<<t2<<'\n';
    h<<spFit1[x].fyDeriv(t1)<<' '<<spFit1[y].fyDeriv(t1)<<' '<<spFit1[z].fyDeriv(t1)<<'\n';
    h<<spFit2[x].fyDeriv(t2)<<' '<<spFit2[y].fyDeriv(t2)<<' '<<spFit2[z].fyDeriv(t2)<<'\n';
    f.close();
    g.close();
    h.close();
    return 0;
}
