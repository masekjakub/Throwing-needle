import random
import math

def isHit():
    for index in range(0,numberOfRows):
        #check if starting point of needle is between rows
        if needleStartPoint > listOfRowsX[index] and needleStartPoint < listOfRowsX[index]+spaceBetweenRows:
            #check if end of needle is in space between different rows
            if needleStartPoint+height < listOfRowsX[index] or needleStartPoint+height > listOfRowsX[index]+spaceBetweenRows:
                return 1
    return 0

spaceBetweenRows=6000
needleSize = 1000
numberOfRows = 666
numberOfThrows = 2000000

hits=0
listOfRowsX=[]
paperSize = spaceBetweenRows*numberOfRows

for row in range(0,numberOfRows):
    listOfRowsX.append(row*spaceBetweenRows)

for throw in range(0,numberOfThrows):
    needleStartPoint = random.randrange(0,paperSize)
    angle = random.randrange(1,360)
    height = needleSize*math.sin(angle) #difference between starting point and end of needle
    if isHit():
        hits=hits+1

res = (2*needleSize*numberOfThrows)/(hits*spaceBetweenRows)
print(f"Calculated pi:{res}")
print()