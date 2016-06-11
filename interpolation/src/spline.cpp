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
    n++;

    if(n>=2)
    {
        Pose ps;
        ps.x = _position[n-1].x-_position[n-2].x;
        ps.y = _position[n-1].y-_position[n-2].y;
        ps.z = _position[n-1].z-_position[n-2].z;
        h.push_back(ps);
    }
}

std::vector<Pose> SplineFit::interpolate()
{
    if(n>=2)
    {
        if(degree==1)
            linearFit();
        else if(degree==2)
        {
            quadraticFit();
            print();
        }
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
        ps.x = _position[n-2].x + j*m1;
        ps.y = _position[n-2].y + j*m2;
        ps.z = _position[n-2].z + j*m3;
        _path.push_back(ps);
    }
}

void SplineFit::quadraticFit()
{
    fitX();
    //fitZ();

    float m2 = (_position[n-1].y-_position[n-2].y)/nPoints;
    for(int j=0;j<nPoints;j++)
    {
        Pose ps;
        ps.y = _position[n-2].y + j*m2;
        ps.x = fx(ps.y);
       // ps.z = fz(ps.y);
        _path.push_back(ps);
    }
}

void SplineFit::fitX()
{
    if(n==2)
    {
        Ax.resize(1,1);
        Ax(0,0)=1;
        std::cout<<"n="<<n<<'\n';
        std::cout<<"Ax:\n"<<Ax<<'\n';
        Bx.push_back(0);
        bx.push_back((_position[1].x-_position[0].x)/h[0].y);
        cx.push_back(0);
    }
    else if(n>=3)
    {
        //Ax->n-2 x n-2
        Ax.conservativeResize(n-1,n-1);
        for(int i=0;i<n-1;i++)
        {
            Ax(n-2,i) = 0;
            Ax(i,n-2) = 0;
        }
        Ax(n-2,n-3) = h[n-3].y;
        Ax(n-2,n-2) = h[n-2].y;
        //std::cout<<"2\n";
        //std::cout<<"Ax:\n"<<Ax<<'\n';
        //std::cout<<"3\n";
        //Ax->n-1 x n-1

        //Bx->n-2
        float bx1 = (_position[n-1].x-_position[n-2].x)/h[n-2].y-(_position[n-2].x-_position[n-3].x)/h[n-3].y;
        Bx.push_back(bx1);
//        std::cout<<"----------------------bx1"<<bx1;
        //Bx->n-1

        Eigen::MatrixXd aInverse(n-1,n-1);
        aInverse = Ax.inverse();
        // std::cout<<"5\n";

        cx.resize(0);
        double cxi=0;
        for(int i=0;i<n-1;i++)
        {
            cxi = 0;
            for(int j=0;j<n-1;j++)
                cxi += ((aInverse(i,j))*Bx[j]);
            cx.push_back(cxi);
        }
        //std::cout<<"6\n";

        bx.resize(0);
        for(int i=0;i<n-1;i++)
            bx.push_back(((_position[i+1].x-_position[i].x)/h[i].y)-(h[i].y*cx[i]));
        //  std::cout<<"7\n";

    }
}

double SplineFit::fx(double y)
{
    double f;

    for(int i=0;i<n-1;i++)
    {

        std::cout<<'y'<<y<<'a'<<_position[i].y<<'b'<<_position[i+1].y<<'\n';
        std::cout<<'n'<<n<<"size"<<_position.size()<<'\n';
        if(y>=_position[i].y && y<_position[i+1].y)
        {
            std::cout<<"i"<<i<<'\n';
            std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<f"<<f<<'\n';
            double yi = _position[i].y;
            std::cout<<"bxSize"<<bx.size()<<'\n';
            std::cout<<"cxSize"<<cx.size()<<'\n';
            f = _position[i].x + bx[i]*(y-yi) + cx[i]*(y-yi)*(y-yi);
            std::cout<<">>>.................................>>>>>>>>>>>>>>>>f"<<f<<'\n';
            return f;
        }
    }
}

void SplineFit::print()
{
    for(int i=0;i<n-2;i++)
        std::cout<<"cx["<<i<<"]:"<<cx[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<n-2;i++)
        std::cout<<"bx["<<i<<"]:"<<bx[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<n-2;i++)
        std::cout<<"h["<<i<<"].y:"<<h[i].y<<' ';
    std::cout<<'\n';
    for(int i=0;i<Bx.size();i++)
        std::cout<<Bx[i]<<' ';
    std::cout<<'\n';
    for(int i=0;i<_position.size();i++)
        std::cout<<_position[i].y<<' ';
    std::cout<<'\n';
    for(int i=0;i<_position.size();i++)
        std::cout<<_position[i].x<<' ';
    std::cout<<'\n';
    std::cout<<'\n';
}
