import random
import math
import time
start_time = time.time()

def isHit(array, x, startIndex, endIndex):
    if startIndex > endIndex:
        return False

    mid = int((endIndex+startIndex)/2)
    if x >= array[mid] and x < array[mid]+spaceBetweenRows:
        if x+heightDiff <= array[mid] or x+heightDiff > listOfRows[mid]+spaceBetweenRows:
            return 1
        return 0

    if x < array[mid]:
        return isHit(array, x, 0, mid-1)
    if x > array[mid]:
        return isHit(array, x, mid+1, endIndex) 
    return 0

#settings
spaceBetweenRows=200
needleSize = 175
numberOfRows = 10
numberOfThrows = 30000000

hits=0
listOfRows=[]
paperSize = spaceBetweenRows*numberOfRows

for row in range(0,numberOfRows):
    listOfRows.append(row*spaceBetweenRows)

for throw in range(0,numberOfThrows):
    needleStartPoint = random.randrange(0,paperSize)
    angle = random.randrange(1,360)
    heightDiff = needleSize*math.sin(angle) #height difference between starting point and end of needle
    if isHit(listOfRows, needleStartPoint, 0, numberOfRows-1):
        hits=hits+1

res = (2*needleSize*numberOfThrows)/(hits*spaceBetweenRows)

print(f"Calculated pi: {res}")
print(f"Actual pi:     {math.pi}")
print(f"Difference:    {abs(math.pi-res)}")
print(f"Hit rate:      {hits/numberOfThrows}")
print(f"Program time:  {(time.time() - start_time)} seconds")
print()