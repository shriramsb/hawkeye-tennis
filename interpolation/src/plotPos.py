import matplotlib.pyplot as plt
#from mpl_toolkits.mplot3d import Axes3D
def plotFn(fileName):
    x = []
    y = []
    z = []
    t = []
    vx = []
    vy = []
    vz = []
    f = open(fileName,'r')
    data = f.readlines()
    for line in data:
        lineData = line.split()
        x.append(lineData[0])
        y.append(lineData[1])
        z.append(lineData[2])
        vx.append(lineData[3])
        vy.append(lineData[4])
        vz.append(lineData[5])
        t.append(lineData[6])
    plt.plot(t,x,'bo')
    plt.plot(t,y,'ro')
    plt.plot(t,z,'go')
 #   fig = plt.figure()
  #  ax = plt.axes(projection='3d')
   # ax.scatter(x,y,z,'-b')
    plt.show()

if __name__=="__main__":
    plotFn("../time.txt")
