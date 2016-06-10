#include<eigen3/Eigen/Dense>
#include<vector>
#include<fstream>
class Integrator
{
    double position;
    int maxDataPoints;
    std::vector<double> _velocity;
    std::vector<double> _time;
    std::vector<double> _position;
    Eigen::MatrixXd A;
    std::vector<double> b, c, d, h;
    std::vector<double> B;
    std::vector<double>::iterator iter;
    int n;
    double dt0;
    bool maxReached;
    void fitCubicSpline();
    double fitCubicSplineRel();
    void calculateD();
    void calculateB();
    double f(double x);
    double rectangular();
    double trapezoidal();
    void calculateAccuratePosition();
    void printH();
    void printB();
    void printC();
    void printD();
    void printb();
    void printTime();
    void printVelocity();
    public:
    Integrator(int nPoints=10);
    void getVelAndTime(double vel, double time);
    void reset();
    double calculatePosition(int method);
};
