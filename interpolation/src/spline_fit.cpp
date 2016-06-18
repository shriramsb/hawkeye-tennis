#include <interpolation/spline_fit.h>
#include <iostream>
#include <fstream>

SplineFit::SplineFit(int deg, int nBefore, int nAfter)
{
    degree = deg;
    n_before = nBefore;
    n_after = nAfter;
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
    x_min = _x[0] - n_before*DX;
    x_max = _x[n-1] + n_after*DX;
}

std::vector<double> SplineFit::interpolate()
{
    if(n>=2)
    {
        if(degree==1)
            linearFit();
        else if(degree==2)
            quadraticFit();
        else if(degree==3)
            cubicFit();
    }
    return _finalY;
}

void SplineFit::linearFit()
{
    double m = (_y[n-1]-_y[n-2])/(_x[n-1]-_x[n-2]);
    for(double x=_x[n-2];x<=_x[n-1];x+=DX)
    {
        double y = _y[n-2] + m*(x-_x[n-2]);
        _finalY.push_back(y);
    }
}

void SplineFit::quadraticFit()
{
    fitYQuad();
    _finalY.resize(0);
    for(double x=x_min;x<x_max;x+=DX)
    {
        double y = fyQuad(x);
        _finalY.push_back(y);
    }
}

void SplineFit::cubicFit()
{
    fitYCubic();
    _finalY.resize(0);
    for(double x=x_min;x<x_max;x+=DX)
    {
        double y = fyCubic(x);
        _finalY.push_back(y);
    }
}

void SplineFit::fitYCubic()
{
    if(n==2)
    {
        A.resize(2,2);
        B.resize(0);
        A(0,0)=1;A(1,0)=0;A(1,1)=1;A(0,1)=0;
        B.push_back(0);
        B.push_back(0);
        b.push_back((_y[1]-_y[0])/h[0]);
        c.push_back(0);
        c.push_back(0);
        d.push_back(0);
    }
    else if(n>=3)
    {
        //A->n-1 x n-1
        //std::cout<<"1\n";
        A.conservativeResize(n,n);
        for(int i=0;i<n;i++)
        {
            A(n-1,i) = 0;
            A(i,n-1) = 0;
        }
        A(n-2,n-3) = h[n-3];
        A(n-2,n-2) = 2.0*(h[n-3]+h[n-2]);
        A(n-2,n-1) = h[n-2];
        A(n-1,n-1) = 1;
        //  std::cout<<"2\n";
        //  std::cout<<"A:\n"<<A<<'\n';
        // std::cout<<"3\n";
        //A->n x n

        //B->n-1
        B[n-2] = (3.0/h[n-2])*(_y[n-1]-_y[n-2]) -(3.0/h[n-3])*(_y[n-2]-_y[n-3]);
        B.push_back(0);
        // std::cout<<"4\n";
        //B->n

        Eigen::MatrixXd aInverse(n,n);
        aInverse = A.inverse();
        // std::cout<<"5\n";
        c.resize(0);
        double ci=0;
        for(int i=0;i<n;i++)
        {
            ci = 0;
            for(int j=0;j<n;j++)
                ci += ((aInverse(i,j))*B[j]);
            c.push_back(ci);
        }
        //std::cout<<"6\n";

        b.resize(0);
        for(int i=0;i<n-1;i++)
            b.push_back(((_y[i+1]-_y[i])/h[i])-(h[i]*(2.0*c[i]+c[i+1]))/3.0);
          //std::cout<<"7\n";

        d.resize(0);
        for(int i=0;i<n-1;i++)
            d.push_back((c[i+1]-c[i])/(3.0*h[i]));
         //std::cout<<"8\n";
    }
}

void SplineFit::fitYQuad()
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

double SplineFit::fyQuad(double x)
{
    double f;
    if(x>=_x[n-1] or x<_x[0])
    {
        int i;
        if(x>=_x[n-1]) i = n-2;
        else if(x<_x[0]) i = 0;
        double xi = _x[i];
        f = _y[i] + b[i]*(x-xi) + c[i]*(x-xi)*(x-xi);
        return f;
    }
    else
    {
        for(int i=0;i<n-1;i++)
        {
            if(x>=_x[i] && x<_x[i+1])
            {
                double xi = _x[i];
                f = _y[i] + b[i]*(x-xi) + c[i]*(x-xi)*(x-xi);
                //std::cout<<"c["<<i<<"]="<<c[i]<<'\n';
                //print();
                return f;
            }
        }
    }
}

double SplineFit::fyCubic(double x)
{
    double f;

    if(x>=_x[n-1] or x<_x[0])
    {
        int i;
        if(x>=_x[n-1]) i = n-2;
        else if(x<_x[0]) i = 0;
        double xi = _x[i];
        f = _y[i] + b[i]*(x-xi) + c[i]*(x-xi)*(x-xi) + d[i]*(x-xi)*(x-xi)*(x-xi);
        return f;
    }
    else
    {
    for(int i=0;i<n-1;i++)
    {
        if(x>=_x[i] && x<_x[i+1])
        {
            double xi = _x[i];
            f = _y[i]+b[i]*(x-xi)+c[i]*(x-xi)*(x-xi)+d[i]*(x-xi)*(x-xi)*(x-xi);
            return f;
        }
    }
    }
}
/*
void SplineFit::extrapolateQuad()
{
    //ae + be*(x-_x[n]) + ce*(x-_x[n])*(x-_x[n])
    //ae = _y[n]
    //be = b[n-1] + 2*c[n-1]*h[n-1]
    //ce = ?????
}

void SplineFit::extrapolateCubic()
{
    //ae + be*(x-_x[n]) + ce*(x-_x[n])*(x-_x[n]) + de*(x-_x[n])*(x-_x[n])*(x-_x[n])
    //ae = _y[n]
    //be = b[n-1] + 2*c[n-1]*h[n-1] +3*d[n-1]*h[n-1]*h[n-1]
    //ce = 2*c[n-1] + 6*d[n-1]*h[n-1]
    //de = ?????
}*/
void SplineFit::print()
{
    for(int i=0;i<c.size();i++)
        std::cout<<"c["<<i<<"]:"<<c[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<b.size();i++)
        std::cout<<"b["<<i<<"]:"<<b[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<d.size();i++)
        std::cout<<"d["<<i<<"]:"<<d[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<h.size();i++)
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
