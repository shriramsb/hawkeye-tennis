#include <interpolation/spline.h>
#include <iostream>
#include <fstream>

SplineFit::SplineFit(int deg, int np)
{
    degree = deg;
    nPoints = np;
    n = 0;
}

void SplineFit::getPosAndTime(Pose pos, double time)
{
    _position.push_back(pos);
    _time.push_back(time);

    if(n>=1)
    {
        Pose ps;
        ps.x = _position[n].x-_position[n-1].x;
        ps.y = _position[n].y-_position[n-1].y;
        ps.z = _position[n].z-_position[n-1].z;
        h.push_back(ps);
    }
    n++;
}

std::vector<Pose> SplineFit::interpolate(std::vector<Pose> pts)
{
    _position = pts;
    if(n>=2)
    {
        if(degree==1)
            linearFit();
        else if(degree==2)
            quadraticFit();
    }
    return _path;
}

void SplineFit::linearFit()
{
    float m1 = (_position[n-1].x-_position[n-2].x)/nPoints;
    float m2 = (_position[n-1].y-_position[n-2].y)/nPoints;
    float m3 = (_position[n-1].z-_position[n-2].z)/nPoints;

    for(int j=0;j<nPoints;j++)
    {
        Pose ps;
        ps.x = _position[j].x + j*m1;
        ps.y = _position[j].y + j*m2;
        ps.z = _position[j].z + j*m3;
        _path.push_back(ps);
    }
}

void SplineFit::quadraticFit()
{
    fitX();
    fitZ();

    float m2 = (_position[n-1].y-_position[n-2].y)/nPoints;
    for(int j=0;j<nPoints;j++)
    {
        Pose ps;
        ps.y = _position[j].y + j*m2;
        ps.x = fx(ps.y);
        ps.z = fz(ps.y);
        _path.push_back(ps);
    }
}

void SplineFit::fitX()
{
    if(n==1)
    {
        Ax.conservativeResize(1,1);
        Ax(0,0)=1;
        Bx.push_back(0);
    }
    else if(n>=2)
    {
        //Ax->n-1 x n-1
        //    std::cout<<"1\n";
        Ax.conservativeResize(n,n);
        for(int i=0;i<n;i++)
        {
            Ax(n-1,i) = 0;
            Ax(i,n-1) = 0;
        }
        Ax(n-1,n-2) = h[n-2].y;
        Ax(n-1,n-1) = h[n-1].y;
        //  std::cout<<"2\n";
        //  std::cout<<"Ax:\n"<<Ax<<'\n';
        // std::cout<<"3\n";
        //Ax->n x n

        //Bx->n-1
        float bx1 = (1.0/h[n-1].y)*(_position[n].x-_position[n-1].x)-(1.0/h[n-2].y)*(_position[n-1].x-_position[n-2].x);
        Bx.push_back(bx1);
        // std::cout<<"4\n";
        //Bx->n

        Eigen::MatrixXd aInverse(n,n);
        aInverse = Ax.inverse();
        // std::cout<<"5\n";

        cx.resize(0);
        double cxi=0;
        for(int i=0;i<n;i++)
        {
            cxi = 0;
            for(int j=0;j<n;j++)
                cxi += ((aInverse(i,j))*Bx[j]);
            cx.push_back(cxi);
        }
        //std::cout<<"6\n";

        bx.resize(0);
        for(int i=0;i<n;i++)
            bx.push_back(((_position[i+1].x-_position[i].x)/h[i].y)-(h[i].y*cx[i]));
        //  std::cout<<"7\n";

    }
}

void SplineFit::fitZ()
{
    if(n==1)
    {
        Az.conservativeResize(1,1);
        Az(0,0)=1;
        Bz.push_back(0);
    }
    else if(n>=2)
    {
        //Az->n-1 x n-1
        //    std::cout<<"1\n";
        Az.conservativeResize(n,n);
        for(int i=0;i<n;i++)
        {
            Az(n-1,i) = 0;
            Az(i,n-1) = 0;
        }
        Az(n-1,n-2) = h[n-2].y;
        Az(n-1,n-1) = h[n-1].y;
        //  std::cout<<"2\n";
        //  std::cout<<"Az:\n"<<Az<<'\n';
        // std::cout<<"3\n";
        //Az->n x n

        //Bz->n-1
        float bz1 = (1.0/h[n-1].y)*(_position[n].z-_position[n-1].z)-(1.0/h[n-2].y)*(_position[n-1].z-_position[n-2].z);
        Bz.push_back(bz1);
        // std::cout<<"4\n";
        //Bz->n

        Eigen::MatrixXd aInverse(n,n);
        aInverse = Az.inverse();
        // std::cout<<"5\n";

        cz.resize(0);
        double czi=0;
        for(int i=0;i<n;i++)
        {
            czi = 0;
            for(int j=0;j<n;j++)
                czi += ((aInverse(i,j))*Bz[j]);
            cz.push_back(czi);
        }
        //std::cout<<"6\n";

        bz.resize(0);
        for(int i=0;i<n;i++)
            bz.push_back(((_position[i+1].z-_position[i].z)/h[i].y)-(h[i].y*cz[i]));
        //  std::cout<<"7\n";

    }
}

double SplineFit::fx(double y)
{
    double f;
    for(int i=0;i<n;i++)
    {
        if(y>=_position[i].y && y<_position[i+1].y)
        {
            double yi = _position[i].y;
            f = _position[i].y+bx[i]*(y-yi)+cx[i]*(y-yi)*(y-yi);
            return f;
        }
    }
}

double SplineFit::fz(double y)
{
    double f;
    for(int i=0;i<n;i++)
    {
        if(y>=_position[i].y && y<_position[i+1].y)
        {
            double yi = _position[i].y;
            f = _position[i].y+bz[i]*(y-yi)+cz[i]*(y-yi)*(y-yi);
            return f;
        }
    }
}
