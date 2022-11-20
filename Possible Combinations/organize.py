import re
import os

file = open("output.txt", "r")
outputFile = open("organized.txt", "w")

solution = int(input("Enter the sample concentration : "))
buffer = int(input("Enter the buffer concentration : "))
for i in range(1, solution):
    for line in file.readlines():
        file.seek(0)
        pattern = f"'{i}'"
        if (len(re.findall(pattern, line)) == 1):
            # outputFile.write(line)
            totBuffer = len(re.findall(r"\W%d" % buffer, line))
            totSol = len(re.findall(r"\W%d" % solution, line))

            lineSplit = line.split()

            target = lineSplit[1]
            target = target[14:len(target)-2]

            steps = lineSplit[2]
            steps = steps[6:-1]

            #For calculating waste
            arr = []
            for x in range(4, len(lineSplit)):
                arr.append(lineSplit[x][1:-1])

            temp = []
            for x in range(len(arr)):
                temp.append(arr[x].split(','))

            num = []
            for x in range(len(temp)):
                for y in range(3):
                    num.append(temp[x][y])

            
            hashWaste = [0]*(solution)

            for x in range(len(num)):
                if int(num[x]) != solution and int(num[x]) != 0:
                    hashWaste[int(num[x])] += 1

            waste = 0
            for x in range(len(hashWaste)):
                if x != target and hashWaste[x] != 0:
                    hashWaste[x] %= 2
                    waste += hashWaste[x]

            #Now modifying line split to display creation history
            lineSplit=lineSplit[::-1]
            outputFile.write(f'Target : {target},   Steps : {steps},   Buffer : {totBuffer},   Solution : {totSol},   Waste : {waste}   ,   Creation :  ')

            for x in range(int(steps)):
                outputFile.write(f"{lineSplit[x]}   ")
            outputFile.write('\n')

    outputFile.write('\n')

file.close()
outputFile.close()
os.remove("./output.txt")
