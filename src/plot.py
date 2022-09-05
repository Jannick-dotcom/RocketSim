import matplotlib.pyplot as plt
import numpy as np
import time

file = open("test.txt")
x = []
yalt = []
yspd = []

for line in file:
    x.append(float(line.split(",")[0]))
    yalt.append(float(line.split(",")[1]))
    yspd.append(float(line.split(",")[2]))

# x = np.linspace(0, len(yspd), len(yspd))
# dx = x[1]-x[0]
# dydx = np.gradient(yspd, dx)

plt.figure("landing")
plt.plot(x,yalt, label="alt")
# plt.plot(x, dydx)
plt.plot(x, yspd, label="spd")
plt.legend()
# plt.plot(z)
plt.show()