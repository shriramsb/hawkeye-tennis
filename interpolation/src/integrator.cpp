#include<auv_drivers/serial_drivers/dvl/explorerDvl/integrator.h>
#include<iostream>
#include<fstream>
SplineFit::SplineFit(int nPoints)
{
    if(nPoints >= 3)
        maxDataPoints = nPoints;
    position = 0;
    n = 0;
    maxReached = false;
}
void SplineFit::reset()
{
    position = 0;
    n = 0;
    maxReached = false;
    _position.resize(0);
    _time.resize(0);
    h.resize(0);
}
void SplineFit::getPosAndTime(double pos, double time)
{
    if(n >= maxDataPoints)
    {
        h.erase(h.begin());
        _position.erase(_position.begin());
        _time.erase(_time.begin());
    }
    
    n++;
    _position.emplace_back(pos);
    _time.emplace_back(time);
    
    if(n>=2)
    {
        int n1 = _time.size();
        double dt = _time[n1-1]-_time[n1-2];
        h.emplace_back(dt);
    }
}
double SplineFit::calculatePosition(int degree)
{
    double dx, dt;
    if(n>=2)
    {
        double ps;
        if(degree==1)
        {    
//            f.open("rectPlot.txt",std::ofstream::out | std::ofstream::trunc);
            ps = rectangular();
        }
        else if(degree==2)
        {    
//            f.open("trapezoidalPlot.txt",std::ofstream::out | std::ofstream::trunc);
            ps = trapezoidal();
        }
        else if(degree==3)
        {    
//            f.open("splinePlot.txt",std::ofstream::out | std::ofstream::trunc);
            ps = fitCubicSplineRel();
        }
/*            for(int j=0;j<_time.size();j++)
                f<<_time[j]<<' ';
            f<<'\n';
            for(int j=0;j<_position.size();j++)
                f<<_position[j]<<' ';
            f<<'\n';
            f.close();
*/
        return ps;
    }
    else
        return 0;
}
std::vector<Pose> SplineFit::trapezoidal()
{
    if(n <= maxDataPoints && n>=2)
    {
        float x1 = _time[n-2], y1 = _position[n-2]; 
        float x2 = _time[n-1], y2 = _position[n-1];
        float m = (y2-y1)/(x2-x1);
        for(int i=0;i*DT<h[n-1];i++)
            path.emplace_back() = y1 + m*i*DT;
        return path;
    }
    else if(n>maxDataPoints)
    {
        n--;
        return rectangular();
    }
}
double SplineFit::fitCubicSplineRel()
{
    if(n==2)
    {
        A.conservativeResize(2,2);
        B.resize(0);
        A(0,0)=1;A(1,0)=0;A(1,1)=1;A(0,1)=0;
        B.emplace_back(0);
        B.emplace_back(0);
        _position.resize(0);
        _position.emplace_back(0);
        return 0;
    }
    else if(n <= maxDataPoints && n>=3)
    {
        //A->n-1 x n-1
    //    std::cout<<"1\n";
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
        B[n-2] = (3.0/h[n-2])*(_position[n-1]-_position[n-2]) -(3.0/h[n-3])*(_velocity[n-2]-_velocity[n-3]);
            B.emplace_back(0);
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
            c.emplace_back(ci);
        }
        //std::cout<<"6\n";
        
        b.resize(0);
        for(int i=0;i<n-1;i++)
            b.emplace_back(((_position[i+1]-_position[i])/h[i])-(h[i]*(2.0*c[i]+c[i+1]))/3.0);
      //  std::cout<<"7\n";
        
        d.resize(0);
        for(int i=0;i<n-1;i++)
            d.emplace_back((c[i+1]-c[i])/(3.0*h[i]));
       // std::cout<<"8\n";
        
        position = _position[0];
       // std::cout<<"Initial Positon: "<<position<<'\n';
        _position.resize(1);
        for(int i=0;i<n-1;i++)
        {
            position += h[i]*(_position[i]+ (b[i]/2.0)*h[i]+ (c[i]/3.0)*h[i]*h[i] + (d[i]/4.0)*h[i]*h[i]*h[i]);
            _position.emplace_back(position);
        }
        
       // std::cout<<"_Positon: "<<'\n';
       /* for(int i=0;i<n;i++)
            std::cout<<_position[i]<<'\n'; */
       // std::cout<<"9\n";
       // std::cout<<"Final Positon: "<<position<<'\n';
    std::vector<double> timeNew,positionNew;
    for(double t=_time[0];t<=_time[n-1];t+=0.01)
    {
        timeNew.emplace_back(t);
        positionNew.emplace_back(f(t));
        if(t+0.01>=_time[n-1] && t<_time[n-1])
        {
            t = _time[n-1];
         //   std::cout<<"\ntime:"<<t;
        }
    }
   /* std::ofstream f("plot.txt",std::ios::app);
    for(int i=0;i<timeNew.size();i++)
        f<<timeNew[i]<<' ';
    f<<'\n';
    for(int i=0;i<positionNew.size();i++)
        f<<positionNew[i]<<' ';
    f<<'\n';
    f.close();
     */   return position;
    }
    else if(n>maxDataPoints)
    {
        n--;
        A(0,0) = 2*h[0];
        A(0,1) = h[0];
        A.block(1,0,n-2,n-1) = A.block(2,1,n-2,n-1);
        double prevSlope = b[1];
        B[0] = (3.0/h[0])*(_position[1]-_position[0])-3*prevSlope;
        B.erase(B.begin()+1);
        _position.erase(_position.begin());
       // std::cout<<"\nin second part:\n";
        double ps = fitCubicSplineRel();
        maxReached = true;
        return ps;
    }
}
double SplineFit::f(double x)
{
    double f;
    for(int i=0;i<n;i++)
    {
        if(x>=_time[i] && x<_time[i+1])
        {
            double xi = _time[i];
            f = _position[i]+b[i]*(x-xi)+c[i]*(x-xi)*(x-xi)+d[i]*(x-xi)*(x-xi)*(x-xi);
            return f;
        }
    }
}
void SplineFit::calculateD()
{
    for(int i=0;i<n-1;i++)
        d.emplace_back((c[i+1]-c[i])/(3.0*h[i]));
    //printD();
}
void SplineFit::calculateB()
{
    for(int i=0;i<n-1;i++)
        b.emplace_back(((_position[i+1]-_position[i])/h[i])-(h[i]*(2.0*c[i]+c[i+1]))/3.0);
    //printb();
}
void SplineFit::fitCubicSpline()
{   
    //printTime();
    //printPosition();
    //printH();
    Eigen::MatrixXd A(n,n);
    for(int i=0;i<n;i++)
    for(int j=0;j<n;j++)
    if(i!=0 && i!=n-1)
    {
    if(i==j)
        A(i,j) = 2.0*(h[i-1]+h[i]);
    else if(j==i-1)
        A(i,j) = h[i-1];
    else if(j==i+1)
        A(i,j) = h[i];
    else
        A(i,j) = 0;
    }
    else
        A(i,j) = 0;
    A(0,0) = 1;
    A(n-1,n-1) = 1;
    B.emplace_back(0);
    for(int i=1;i<n-1;i++)
    {
        B.emplace_back((3.0/h[i])*(_position[i+1]-_position[i]) -(3.0/h[i-1])*(_velocity[i]-_velocity[i-1]));
    }
    B.emplace_back(0);
    //printB();
    Eigen::MatrixXd aInverse(n,n);
    //std::cout<< "\nA\n"<<A;
    aInverse = A.inverse();
    //std::cout<< "\nA inverse\n"<<aInverse;
    double ci=0;
    for(int i=0;i<n;i++)
    {
        ci = 0;
        for(int j=0;j<n;j++)
            ci += ((aInverse(i,j))*B[j]);
        c.emplace_back(ci);
    }
    //printC();
    calculateD();
    calculateB();
    calculateAccuratePosition();
}
void SplineFit::calculateAccuratePosition()
{
    //printTime();
    //printPosition();
    if(_position.size()<maxDataPoints)
        position = 0;
    else
        position = _position[0];
    double t1=_time[0], tn=_time[n-1];
    int nOld = n;
    n = 0;
    std::vector<double> timeNew,timeOld = _time,hOld = h;
    std::vector<double> positionNew,velocityOld = _position;
    for(double t=t1;t<=tn;t+=0.01)
    {
        timeNew.emplace_back(t);
        n++;
        positionNew.emplace_back(f(t));
            if(t+0.01 >= tn)
            {
                t = tn;
               // std::cout<<"\ntime:"<<t;
            }
    }

    _time = timeNew;
    _position = velocityNew;
    std::ofstream f("plot.txt");
    for(int i=0;i<n;i++)    
        f<<_time[i]<<' ';
    f<<'\n';
    for(int i=0;i<n;i++)    
        f<<_position[i]<<' ';
    f<<'\n';
    f.close();
    h.assign(n-1,0.01);
    printTime();
    printPosition();
    //printH();
    int n1=n;
    for(n=2;n<=n1;n++)
       // position+= (_position[n-1]+_position[n-2])*h[n-2]/2.0;
       calculatePosition(2);
       //std::cout<<"\nn="<<n<<" position:"<<calculatePosition(2)<<'\n';
    //std::cout<<"\nn="<<n<<" position:"<<position<<'\n';
    n = nOld;
    _time = timeOld;
    _position = velocityOld;
    h = hOld;
}

void SplineFit::printTime()
{
    std::cout<<"time";
    for(iter=_time.begin();iter!=_time.end();iter++)
        std::cout<<*iter<<' ';
    std::cout<<'\n';
}
void SplineFit::printPosition()
{
    std::cout<<"Position";
    for(iter=_position.begin();iter!=_position.end();iter++)
        std::cout<<*iter<<' ';
    std::cout<<'\n';
}
void SplineFit::printb()
{
    std::cout<<'b';
    for(iter=b.begin();iter!=b.end();iter++)
        std::cout<<*iter<<' ';
    std::cout<<'\n';
}
void SplineFit::printH()
{
    std::cout<<'h';
    for(iter=h.begin();iter!=h.end();iter++)
        std::cout<<*iter<<' ';
    std::cout<<'\n';
}
void SplineFit::printB()
{
    std::cout<<'B';
    for(iter=B.begin();iter!=B.end();iter++)
        std::cout<<*iter<<' ';
    std::cout<<'\n';
}
void SplineFit::printC()
{
    std::cout<<'c';
    for(iter=c.begin();iter!=c.end();iter++)
        std::cout<<*iter<<' ';
    std::cout<<'\n';
}
void SplineFit::printD()
{
    std::cout<<'D';
    for(iter=d.begin();iter!=d.end();iter++)
        std::cout<<*iter<<' ';
    std::cout<<'\n';
}
