#!/usr/bin/python

import matplotlib.pyplot as plt
import multiprocessing as mp

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


def savePlot(que: mp.Queue):
    plot = que.get(True, 10)
    plt.figure(plot.name)
    plt.plot(plot.values, label=plot.name)
    plt.savefig(f"outputs/images/{plot.name}.png")
    plt.close(plot.name)
    print(f"Saved {plot.name}")

x = []
for i in lop:
    q = mp.Queue()
    el = mp.Process(target=savePlot, args=(q,))
    q.put(i)
    el.start()
    x.append(el)