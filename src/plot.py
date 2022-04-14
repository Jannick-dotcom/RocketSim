import matplotlib.pyplot as plt
import numpy as np

file = open("test.txt")
x = []
yalt = []
yspd = []

for line in file:
    yalt.append(float(line.split(",")[0]))
    yspd.append(float(line.split(",")[1])*-1)

x = np.linspace(0, len(yspd) * 0.001, len(yspd))
dx = x[1]-x[0]
dydx = np.gradient(yspd, dx)

plt.figure("landing")
# plt.plot(x,yalt)
# plt.plot(x, dydx)
plt.plot(x, yspd)
# plt.plot(z)
plt.show()