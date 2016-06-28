import matplotlib.pyplot as plt
def plotFn(fileName):
    t = []
    vx = []
    vy = []
    vz = []
    f = open(fileName,'r')
    data = f.readlines()
    for line in data:
        lineData = line.split()
        vx.append(lineData[3])
        vy.append(lineData[4])
        vz.append(lineData[5])
        t.append(lineData[6])
    plt.plot(t,vx,'bo')
    plt.plot(t,vy,'ro')
    plt.plot(t,vz,'go')
    plt.show()

if __name__=="__main__":
    plotFn("../data/time.txt")
