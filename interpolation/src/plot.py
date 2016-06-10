import matplotlib.pyplot as plt
def plotFn(fileName,color):
    t1 = []
    v1 = []
    f = open(fileName,'r')
    data = f.readlines()
    for line in data:
        lineData = line.split()
        t1.append(lineData[0])
        v1.append(lineData[1])
    plt.axis([4900,5400,-40,40])
    plt.plot(t1,v1,color)
    plt.show()
    i = 1
    f = open("raw.txt",'r')
    data = f.readlines()
    for line in data:
        if i is 1:
            t3 = line.split()
            i = i+1
        elif i is 2:
            v3 = line.split()
    i = 1
    g = open("plot.txt",'r')
    data = g.readlines()
    for line in data:
        if i is 1:
            t2 = line.split()
            i = i+1
        elif i is 2:
            v2 = line.split()
            i = 1
            plt.axis([5090,5100,20,40])
            plt.plot(t3, v3, 'r', t2, v2)
            plt.show()
def plotPos():
    i = 1
    f = open("rectPlot.txt",'r')
    data = f.readlines()
    for line in data:
        if i is 1:
            t1 = line.split()
            i = i+1
        elif i is 2:
            v1 = line.split()
    i = 1
    f = open("trapezoidalPlot.txt",'r')
    data = f.readlines()
    for line in data:
        if i is 1:
            t2 = line.split()
            i = i+1
        elif i is 2:
            v2 = line.split()
    i = 1
    f = open("splinePlot.txt",'r')
    data = f.readlines()
    for line in data:
        if i is 1:
            t3 = line.split()
            i = i+1
        elif i is 2:
            v3 = line.split()
    plt.plot(t1, v1,'r', t2, v2,'g', t3, v3,'b')
    plt.show()
    
if __name__=="__main__":
    plotFn("x.txt",'b')
    plotFn("y.txt",'g')
    plotFn("z.txt",'r')
#    plotPos()
