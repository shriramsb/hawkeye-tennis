import matplotlib.pyplot as plt
import time
import math
import numpy as np
def drawCircle(xc,yc,r):
    x = []
    y = []
    R = 0.0
    while(R<=r):
        a = 0.0
        while(a<360.0):
            th = (a*math.pi)/180.0
            x.append(xc + R*math.cos(th))
            y.append(yc + R*math.sin(th))
            a = a+0.1
        R = R+r/2.5;
    plt.plot(x, y, color=(r/8.0,r/9.0,0), marker='o', markerfacecolor=(r/8.0,r/9.0,0), markeredgecolor=(r/5.0,r/9.0,0), markersize=2.0)
    
def plotFn(fileName):
    f = open(fileName,'r')
    data = f.readlines()
    for line in data:
        lineData = line.split()
        x = float(lineData[0])
        y = float(lineData[1])
        r = float(lineData[3])
        drawCircle(x,y,r)
    plt.axis('equal')

if __name__=="__main__":
    plotFn("../area.txt")
    plt.show()
