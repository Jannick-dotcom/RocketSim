import matplotlib.pyplot as plt
import numpy as np
import time

file = open("/home/jannick/Desktop/outputs/test.txt")

def getDuplicates(list):
    '''return all duplicates in list'''
    duplicates = []
    for i in range(len(list)):
        for j in range(i+1, len(list)):
            if list[i] == list[j]:
                duplicates.append(list[i])
    return duplicates

def getMissing(list):
    '''return all missing elements in list'''
    missing = []
    for i in range(int(list[0]), int(list[0]) + len(list)):
        if i not in list:
            missing.append(i)
    return missing

matrix = []

for line in file:
    matrix.append([float(x) for x in line.split(",")])

file.close()

def getSortingItem(element):
    return element[0]

matrix.sort(key=getSortingItem)

x = [row[0] for row in matrix]
yalt = [row[1] for row in matrix]
yspd = [row[2] for row in matrix]
mass = [row[3] for row in matrix]

numberOfCrashes = 0
for i in range(len(yspd)):
    if yspd[i] > 1:
        numberOfCrashes += 1
        print("Crash at: " + str(x[i]) + " with speed: " + str(yspd[i]) + " and mass: " + str(mass[i]))

print("number of crashes: " + str(numberOfCrashes) + " out of " + str(len(yspd)))
print(getDuplicates(x))
print(getMissing(x))