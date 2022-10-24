import os
import matplotlib.pyplot as plt

directory = "/home/jannick/Desktop/outputs"

def plot(liste):
    plt.figure("speed")
    if(isinstance(liste[0], list)):
        for i in liste:
            plt.plot(i[1:], label=i[0].split("/")[-1])
    else:
        plt.plot(liste[1:], label=liste[0].split("/")[-1])
        plt.legend()
    plt.show()

def extractVals(filename):
    tempVel = []
    tempVel.append(filename)
    for i in open(filename):
        try:
            tempVel.append(float(i))
        except:
            continue
    return tempVel

globSpd = []
for filename in os.listdir(directory):
    if(os.path.getsize(os.path.join(directory, filename)) == 0):
        continue
    if filename.endswith(".csv"):
        # print(os.path.join(directory, filename))
        spd = extractVals(os.path.join(directory, filename))
        if(spd[-1] > 1):
            globSpd.append(spd)
    else:
        continue
    # plot(spd)

plot (globSpd)
# plt.figure("position")
# for i in globPos:
#     plt.plot(i, label="pos")
# plt.legend()

# while(true):
#     time.sleep(1)