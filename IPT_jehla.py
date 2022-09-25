import random
import math
import time
start_time = time.time()

def calculateRows():
    for row in range(0,numberOfRows):
        listOfRows.append(row*spaceBetweenRows+spaceBetweenRows/2)

def throwNeedles(numberOfThrows):
    hits=0
    angle=0
    paperSize = spaceBetweenRows*numberOfRows
    print(paperSize)
    for throw in range(1,numberOfThrows):
        endPointFound=0
        while not endPointFound:
            random.seed(time.time()+throw+angle)
            needleStartPoint = random.randrange(0,paperSize)
            angle = random.randrange(1,360)
            heightDiff = needleSize*math.sin(angle) #height difference between starting point and end of needle
            endPointFound = isOnPaper(needleStartPoint+heightDiff, paperSize)
        if isHit(listOfRows, needleStartPoint, 0, numberOfRows-1, heightDiff, paperSize):
            hits+=1
    return hits

def isOnPaper(x, paperSize):
    if x > paperSize or x < 0:
        return 0
    return 1

def isHit(array, x, startIndex, endIndex, heightDiff, paperSize):
    
    mid = int((endIndex+startIndex)/2)
    if endIndex == -1:
        if x < array[0] and x+heightDiff > array[0]:
            return 1
        return 0

    if startIndex > endIndex:
        print("Error: Binary search failed")
        return 0


    if x >= array[mid] and x < array[mid]+spaceBetweenRows:
        if x + heightDiff < listOfRows[mid] or x + heightDiff >= listOfRows[mid]+spaceBetweenRows:   #end of needle is not between these rows
            if x + heightDiff >= paperSize:
                print("bum")
            return 1
        return 0

    if x < array[mid]:
        return isHit(array, x, 0, mid-1, heightDiff, paperSize)
    if x > array[mid]:
        return isHit(array, x, mid+1, endIndex, heightDiff, paperSize) 

    print("not found")
    return 0

#settings
spaceBetweenRows=30000
needleSize = 20000
numberOfRows = 1000
numberOfThrows = 1500000

listOfRows=[]
calculateRows()
hits = throwNeedles(numberOfThrows)
res = (2*needleSize*numberOfThrows)/(hits*spaceBetweenRows)

print(listOfRows[numberOfRows-1]+spaceBetweenRows)
print(f"Calculated pi: {res}")
print(f"Actual pi:     {math.pi}")
print(f"Difference:    {math.pi-res}")
print(f"Hit rate:      {hits/numberOfThrows}")
print(f"Hits:          {hits}")
print(f"Expected hits: {(2*needleSize*numberOfThrows)/(math.pi*spaceBetweenRows)}")
print(f"Program time:  {(time.time() - start_time)} seconds")
print()
input()