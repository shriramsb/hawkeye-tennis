#include<eigen3/Eigen/Dense>
#include<vector>
#include<fstream>
#include<interpolation/pose.h>

class SplineFit
{
    std::vector<Pose> _position;
    std::vector<Pose> _path;
    std::vector<double> _time;
    std::vector<Pose> h;
    int degree;
    int n;
    int nPoints;
    Eigen::MatrixXd A;
    std::vector<double> B;
    
    void getPosAndTime(Pose pos, double time);
    void linearFit();
    void quadraticFit();
    void calculateB();
    public:
    SplineFit(int deg=10, int np=10);
    std::vector<Pose> interpolate(std::vector<Pose> pts);
};
