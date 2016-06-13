#include <eigen3/Eigen/Dense>
#include <vector>
#include <fstream>
#include <iostream>

class SplineFit
{
    std::vector<double> _x;
    std::vector<double> _y;
    std::vector<double> h;
    std::vector<double> _finalY;
    int degree;
    int n;
    int nPoints;
    Eigen::MatrixXd A;
    std::vector<double> B;
    std::vector<double> b,c;
    
    void linearFit();
    void quadraticFit();
    void cubicFit();
    void fitY();
    double fy(double x);
    public:
    SplineFit(int deg=2, int np=10);
    std::vector<double> interpolate();
    void getXAndY(double x, double y);
    void print();
};
