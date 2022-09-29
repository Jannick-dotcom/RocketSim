import matplotlib.pyplot as plt
import numpy as np
import time

file = open("outputs/log.csv")

matrix = []

for line in file:
    matrix.append([x for x in line.split(";")])

yalt = []
spd = []
mass = []
angle = []
for i in matrix[1:]:
    try:
        angle.append(float(i[1]))
        spd.append(float(i[0]))
        yalt.append(float(i[20]))
        mass.append(float(i[7]))
    except:
        pass


plt.figure("landing_altitude")
plt.plot(yalt, label="alt")
plt.legend()
plt.figure("landing_speed")
plt.plot(spd, label="spd")
plt.legend()
plt.figure("landing_mass")
plt.plot(mass, label="mass")
plt.legend()
# plt.figure("landing_angle")
# plt.plot(angle, label="angle")
# plt.legend()
plt.show()