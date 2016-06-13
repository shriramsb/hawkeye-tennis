#include <interpolation/spline_fit.h>
#include <iostream>
#include <fstream>

SplineFit::SplineFit(int deg, int np)
{
    degree = deg;
    nPoints = np;
    n = 0;
}

void SplineFit::getXAndY(double x, double y)
{
    _x.push_back(x);
    _y.push_back(y);
    n++;

    if(n>=2)
    {
        double dx = _x[n-1] - _x[n-2];
        h.push_back(dx);
    }
}

std::vector<double> SplineFit::interpolate()
{
    if(n>=2)
    {
        if(degree==1)
            linearFit();
        else if(degree==2)
            quadraticFit();
//        else if(degree==3)
  //          cubicFit();
    }
    return _finalY;
}

void SplineFit::linearFit()
{
    double my = (_y[n-1] - _y[n-2])/nPoints;
    for(int j=0;j<nPoints;j++)
    {
        double yj;
        yj = _y[n-2] + j*my;
        _finalY.push_back(yj);
    }
}

void SplineFit::quadraticFit()
{
    fitY();
    _finalY.resize(0);
    for(int i=2;i<=n;i++)
    {
        float mx = (_x[i-1]-_x[i-2])/nPoints;
        for(int j=0;j<nPoints;j++)
        {
            double x;
            x = _x[i-2] + j*mx;
            double y = fy(x);
            _finalY.push_back(y);
        }
    }
}

void SplineFit::fitY()
{
    if(n==2)
    {
        A.resize(1,1);
        A(0,0)=1;
        B.push_back(0);
        b.push_back((_y[1]-_y[0])/h[0]);
        c.push_back(0);
    }
    else if(n>=3)
    {
        //A->n-2 x n-2
        A.conservativeResize(n-1,n-1);
        for(int i=0;i<n-1;i++)
        {
            A(n-2,i) = 0;
            A(i,n-2) = 0;
        }
        A(n-2,n-3) = h[n-3];
        A(n-2,n-2) = h[n-2];
        //std::cout<<"A:\n"<<A<<'\n';
        //A->n-1 x n-1

        //B->n-2
        float b1 = (_y[n-1]-_y[n-2])/h[n-2]-(_y[n-2]-_y[n-3])/h[n-3];
        B.push_back(b1);
//        std::cout<<"----------------------b1"<<b1;
        //B->n-1

        Eigen::MatrixXd aInverse(n-1,n-1);
        aInverse = A.inverse();
        // std::cout<<"5\n";

        c.resize(0);
        double ci=0;
        for(int i=0;i<n-1;i++)
        {
            ci = 0;
            for(int j=0;j<n-1;j++)
                ci += ((aInverse(i,j))*B[j]);
            c.push_back(ci);
        }
        //std::cout<<"6\n";

        b.resize(0);
        for(int i=0;i<n-1;i++)
            b.push_back(((_y[i+1]-_y[i])/h[i])-(h[i]*c[i]));
        //  std::cout<<"7\n";

    }
}

double SplineFit::fy(double x)
{
    double f;

    for(int i=0;i<n-1;i++)
    {
        if(x>=_x[i] && x<_x[i+1])
        {
            double xi = _x[i];
            f = _y[i] + b[i]*(x-xi) + c[i]*(x-xi)*(x-xi);
            return f;
        }
    }
}

void SplineFit::print()
{
    for(int i=0;i<n-2;i++)
        std::cout<<"c["<<i<<"]:"<<c[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<n-2;i++)
        std::cout<<"b["<<i<<"]:"<<b[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<n-2;i++)
        std::cout<<"h["<<i<<"]:"<<h[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<B.size();i++)
        std::cout<<B[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<_x.size();i++)
        std::cout<<_x[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<_y.size();i++)
        std::cout<<_y[i]<<' ';
    std::cout<<'\n';
    std::cout<<'\n';
}
