#include <iostream>
#include <vector>
#include <interpolation/pose.h>

class SplineFit
{
    std::vector<Pose> path;
    int n;
    int degree;
    public:
    SplineFit(int num=20, int degree=3);
    std::vector<Pose> interpolate(std::vector<Pose> pts);
};
