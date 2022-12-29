#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np
file = open("outputs/log.csv", "r")

class parameter:
    def __init__(self, name, index):
        self.name = name
        self.index = index
        self.values = []


lop = []
for i, line in enumerate(file):
    if i == 0:
        params = line.strip().split(";")
        for p, param in enumerate(params):
            lop.append(parameter(param, p))

    else:
        values = line.strip().split(";")
        for p, param in enumerate(lop):
            param.values.append(values[param.index])

file.close()


def savePlot(plot):
    plt.figure(plot.name)
    plt.plot(plot.values)
    plt.savefig(f"outputs/{plot.name}.png", dpi=100)
    print(f"Saved {plot.name}")

for i in lop:
    savePlot(i)