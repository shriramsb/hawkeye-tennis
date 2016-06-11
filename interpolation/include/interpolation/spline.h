#include <interpolation/pose.h>

#include <eigen3/Eigen/Dense>
#include <vector>
#include <fstream>
#include <iostream>

class SplineFit
{
    std::vector<Pose> _position;
    std::vector<Pose> _path;
    std::vector<double> _time;
    std::vector<Pose> h;
    int degree;
    int n;
    int nPoints;
    Eigen::MatrixXd Ax,Az;
    std::vector<double> Bx,Bz;
    std::vector<double> bx,bz,cx,cz;
    
    void linearFit();
    void quadraticFit();
    void fitX();
    void fitZ();
    double fx(double y);
    double fz(double y);
    public:
    SplineFit(int deg=2, int np=10);
    std::vector<Pose> interpolate();
    void getPosAndTime(Pose pos, double time);
    void print();
};
