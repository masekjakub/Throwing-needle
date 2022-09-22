import random
import math
import time
start_time = time.time()

def isHit(array, x, startIndex, endIndex):
    if startIndex > endIndex:
        return False

    mid = int((endIndex+startIndex)/2)
    if x > array[mid] and x < array[mid]+spaceBetweenRows:
        if x+height < array[mid] or x+height > listOfRowsX[mid]+spaceBetweenRows:
            return 1
        return 0

    if x < array[mid]:
        return isHit(array, x, 0, mid-1)
    if x > array[mid]:
        return isHit(array, x, mid+1, endIndex) 
    return 0

#settings
spaceBetweenRows=2000
needleSize = 1999
numberOfRows = 100
numberOfThrows = 5000000

hits=0
listOfRowsX=[]
paperSize = spaceBetweenRows*numberOfRows

for row in range(0,numberOfRows):
    listOfRowsX.append(row*spaceBetweenRows)

for throw in range(0,numberOfThrows):
    needleStartPoint = random.randrange(0,paperSize)
    angle = random.randrange(1,360)
    height = needleSize*math.sin(angle) #height difference between starting point and end of needle
    if isHit(listOfRowsX, needleStartPoint, 0, numberOfRows-1):
        hits=hits+1

res = (2*needleSize*numberOfThrows)/(hits*spaceBetweenRows)
print(f"Calculated pi:{res}")
print(f"Estimated time: {(time.time() - start_time)} seconds")