import matplotlib.pyplot as plt
import numpy as np
import time

file = open("outputs/test.txt")

matrix = []

for line in file:
    matrix.append([float(x) for x in line.split(",")])

def getSortingItem(element):
    return element[0]

matrix.sort(key=getSortingItem)

x = [row[0] for row in matrix]
yalt = [row[1] for row in matrix]
yspd = [row[2] for row in matrix]
mass = [row[3] for row in matrix]

# plt.figure("landing_altitude")
# plt.plot(x,yalt, label="alt")
# plt.legend()
plt.figure("landing_speed")
plt.plot(x, yspd, label="spd")
plt.legend()
plt.figure("landing_mass")
plt.plot(x, mass, label="mass")
plt.legend()
plt.show()