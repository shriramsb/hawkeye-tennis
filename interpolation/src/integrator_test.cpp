#include<auv_drivers/serial_drivers/dvl/explorerDvl/integrator.h>
#include<iostream>
#include<fstream>
int main()
{
    const int size = 20;
    std::ifstream fl("x.txt");
    float time = 5101, vel;
    int i = 0;
    double v[size];
    double t[size];
    char c;
    while(i<size and time<5200)
    {
        fl>>time>>vel;
        if(time<5200 and time>5090)
        {
            v[i] = vel;
            t[i] = time;
            i++;
        }
    }
    for(int i=0;i<size;i++)
        std::cout<<t[i]<<' '<<v[i]<<'\n';
    //double v[size] = {0.1,0.2,0.3,0.25,0.10,0.0,-0.20,-0.32,-0.25,-0.10,0.0,0.4,0.5,0.5,0.5,0.3,0.1,0.4,0.3,0.4};
    //double v[size] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    //double t[size] = {0.12,0.30,0.45,0.62,0.70,0.85,1.05,1.21,1.37,1.53,1.69,1.88,1.99,2.13,2.30,2.35,2.55,2.69,2.87,3.00};
    double position[size];
    std::ofstream h;
    h.open("plot.txt",std::ofstream::out | std::ofstream::trunc);
    h.close();
    h.open("rectPlot.txt",std::ofstream::out | std::ofstream::trunc);
    h.close();
    h.open("trapezoidalPlot.txt",std::ofstream::out | std::ofstream::trunc);
    h.close();
    h.open("splinePlot.txt",std::ofstream::out | std::ofstream::trunc);
    h.close();
    std::ofstream g("raw.txt",std::ofstream::out | std::ofstream::trunc);
    for(int i=0;i<size;i++)
        g<<t[i]<<' ';
    g<<'\n';
    for(int i=0;i<size;i++)
        g<<v[i]<<' ';
    g<<'\n';
    g.close();
    for(int a=1;a<=3;a++)
    {
        Integrator integrator;
        for(int i=0;i<size;i++)
        {
           // for(double k=0;k<100000000;k++);
            integrator.getVelAndTime(v[i],t[i]);
            position[i] = integrator.calculatePosition(a);
            std::cout<<"Calculated position "<<i<<'\n';
            std::ofstream f;
            if(a==1)
                f.open("rectPlot.txt");
            else if(a==2)
                f.open("trapezoidalPlot.txt");
            else if(a==3)
                f.open("splinePlot.txt");

            for(int j=0;j<=i;j++)
                f<<t[j]<<' ';
            f<<'\n';
            for(int j=0;j<=i;j++)
                f<<position[j]<<' ';
            f<<'\n';
            f.close();
        }
    }
    return 0;
}
