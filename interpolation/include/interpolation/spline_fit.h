#include <eigen3/Eigen/Dense>
#include <vector>
#include <fstream>
#include <iostream>

const double BALL_RAD = 4.5;
class SplineFit
{
    std::vector<double> _x;
    std::vector<double> _y;
    std::vector<double> h;
    std::vector<double> _finalY;
    std::vector<double> _derivY;
    std::vector<double> _intgrY;
    int degree;
    int n;
    int n_before,n_after;
    Eigen::MatrixXd A;
    std::vector<double> B;
    std::vector<double> b,c,d;
    
    void linearFit();
    void quadraticFit();
    void cubicFit();
    void fitYQuad();
    void fitYCubic();
    double fyQuad(double x);
    double fyCubic(double x);
    double fyQuadDeriv(double x);
    double fyCubicDeriv(double x);
    double fyQuadIntgr(double x);
    double fyCubicIntgr(double x);
    public:
    SplineFit(int deg=2, int nBefore=10, int nAfter=10, double dx=0.1);
    std::vector<double> interpolate();
    std::vector<double> differentiate();
    std::vector<double> integrate();
    void getXAndY(double x, double y);
    double getXFromY(double y, bool firstX=true);
    double fy(double x);
    double fyDeriv(double x);
    double fyIntgr(double x);
    void print();
    double x_min, x_max;
    double DX;
};
