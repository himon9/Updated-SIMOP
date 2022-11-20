import numpy as np
import matplotlib
import matplotlib.pyplot as plt


f = open('barGraphData.txt', 'r')
SeqNo = []
Output = []
Sample = []
Buffer = []
Waste = []

for line in f:
    columns = line.split()
    SeqNo.append(int(columns[0]))
    Output.append(int(columns[1]))
    Sample.append(int(columns[2]))
    Buffer.append(int(columns[3]))
    Waste.append(int(columns[4]))

x = np.arange(len(SeqNo))


width = 0.1
  
# plot data in grouped manner of bar type
plt.bar(x-0.2, Output, width, color='blue')
plt.bar(x-0.1,Sample,width, color='silver')
plt.bar(x, Buffer, width, color='red')
plt.bar(x+0.1, Waste, width, color='grey')

plt.xticks(x, SeqNo)

plt.xlabel("SeqNo")
plt.ylabel("Volume")
plt.legend(["Output", "Sample", "Buffer","Waste"])
plt.savefig("./BarGraphImage", format="PNG")
plt.show()

