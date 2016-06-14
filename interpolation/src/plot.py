import matplotlib.pyplot as plt
#from mpl_toolkits.mplot3d import Axes3D
def plotFn(fileName):
    x = []
    y = []
    z = []
    f = open(fileName,'r')
    data = f.readlines()
    for line in data:
        lineData = line.split()
        x.append(lineData[0])
        y.append(lineData[1])
        z.append(lineData[2])
    plt.plot(y,z,'bo')
 #   fig = plt.figure()
  #  ax = plt.axes(projection='3d')
   # ax.scatter(x,y,z,'-b')
    plt.show()

if __name__=="__main__":
    plotFn("../cubic.txt")
