import math

## Pre-defining functions to be used later

def fTwo(x,y):
    return -4*y+3*math.e**(-x)

def yTwo(x):
    return math.e**(-x)-math.e**(-4*x)

def fThree(x,y):
    return [y[1],-floatGamma*y[1]-floatDelta**3*y[0]**2*y[1]-floatOmega**2*y[0]+floatA*math.sin(floatW*x)]

def yThree(y):
    A = floatA*(floatW*floatGamma**2/2-floatW*(floatOmega**2-floatW**2))/(((floatGamma*floatW)**2+(floatOmega**2-floatW**2)**2)*(floatOmega**2-(floatGamma/2)**2)**(1/2))
    B = floatA*floatGamma*floatW/((floatGamma*floatW)**2+(floatOmega**2-floatW**2)**2)
    return [floatA*((floatOmega**2-floatW**2)*math.sin(floatW*y)-floatGamma*floatW*math.cos(floatW*y))/((floatGamma*floatW)**2+(floatOmega**2-floatW**2)**2) + math.e**(-floatGamma*y/2)*(A*math.sin((floatOmega**2-(floatGamma/2)**2)**(1/2)*y)+B*math.cos((floatOmega**2-(floatGamma/2)**2)**(1/2)*y)),
            floatA*((floatOmega**2-floatW**2)*floatW*math.cos(floatW*y)+floatGamma*floatW**2*math.sin(floatW*y))/((floatGamma*floatW)**2+(floatOmega**2-floatW**2)**2) + math.e**(-floatGamma*y/2)*(floatOmega**2-(floatGamma/2)**2)**(1/2)*(A*math.cos((floatOmega**2-(floatGamma/2)**2)**(1/2)*y)-B*math.sin((floatOmega**2-(floatGamma/2)**2)**(1/2)*y))-floatGamma/2*((A*math.sin((floatOmega**2-(floatGamma/2)**2)**(1/2)*y)+B*math.cos((floatOmega**2-(floatGamma/2)**2)**(1/2)*y)))]

def vectorSum(floatLeft,arrLeft,floatRight,arrRight): # Function to give a linear combination of two vectors
    arrSum = []
    for intIter in range(len(arrLeft)):
        arrSum.append(floatLeft*arrLeft[intIter]+floatRight*arrRight[intIter])
    return arrSum

def returnZero(x): # Dummy function to use if solution is unknown
    return [0,0]

## Arguments for functions:
# f = dy/dx
# funcSolution = actual solution of y
# y(floatX) = floatY from initial conditions
# floatStep = h
# intIterations = number of times method is iterated

def eulerMethod(f,funcSolution,floatX,floatY,floatStep,intIterations): # Euler method
    arrValues = [[floatX,floatY,funcSolution(floatX),0,0]]
    for intIter in range(intIterations): # For each iteration
        floatY = floatY + floatStep*f(floatX,floatY) # Apply term to term rule to y
        floatLocal = funcSolution(floatX) + floatStep*f(floatX,funcSolution(floatX)) - funcSolution(floatX+floatStep) # Calculate local error
        floatX = floatX + floatStep # Increment x
        arrValues.append([floatX,floatY,funcSolution(floatX),floatY-funcSolution(floatX),floatLocal]) # Append to table of values
    return arrValues


def leapfrogMethod(f,funcSolution,floatX,floatY,floatStep,intIterations): # Leapfrog (LF) method
    arrValues = [[floatX,floatY,funcSolution(floatX),0,0]]
    floatY = floatY + floatStep*f(floatX,floatY)
    floatLocal = funcSolution(floatX) + floatStep*f(floatX,funcSolution(floatX)) - funcSolution(floatX+floatStep)
    floatX = floatX + floatStep
    arrValues.append([floatX,floatY,funcSolution(floatX),floatY-funcSolution(floatX),floatLocal])
    
    for intIter in range(intIterations-1): # For each iteration
        floatY = arrValues[-2][1] + 2*floatStep*f(floatX,floatY) # Apply term to term rule to y
        floatLocal = funcSolution(floatX-floatStep) + 2*floatStep*f(floatX,funcSolution(floatX))-funcSolution(floatX+floatStep)
        floatX = floatX + floatStep # Increment x
        arrValues.append([floatX,floatY,funcSolution(floatX),floatY-funcSolution(floatX),floatLocal])
    return arrValues

def rungeKutta(f,funcSolution,floatX,floatY,floatStep,intIterations): # Runge-Kutta (RK4) method for scalar y
    arrValues = [[floatX,floatY,funcSolution(floatX),0,0]]
    for intIter in range(intIterations): # For each iteration
        floatK1 = floatStep*f(floatX,floatY) # Calculate scalar k1,k2,k3,k4
        floatK2 = floatStep*f(floatX+floatStep/2,floatY+floatK1/2)
        floatK3 = floatStep*f(floatX+floatStep/2,floatY+floatK2/2)
        floatK4 = floatStep*f(floatX+floatStep,floatY+floatK3)
        floatY = floatY + (floatK1+2*floatK2+2*floatK3+floatK4)/6 # Apply term to term rule to y
        floatLocal = funcSolution(floatX) # Recalculate scalar k1,k2,k3,k4 for local error
        floatK1 = floatStep*f(floatX,floatLocal)
        floatK2 = floatStep*f(floatX+floatStep/2,floatLocal+floatK1/2)
        floatK3 = floatStep*f(floatX+floatStep/2,floatLocal+floatK2/2)
        floatK4 = floatStep*f(floatX+floatStep,floatLocal+floatK3)
        floatLocal = floatLocal + (floatK1+2*floatK2+2*floatK3+floatK4)/6 - funcSolution(floatX+floatStep) # Apply term to term rule to y(xn)
        floatX = floatX + floatStep # Increment x
        arrValues.append([floatX,floatY,funcSolution(floatX),floatY-funcSolution(floatX),floatLocal])
    return arrValues

def rungeKuttaVector(f,funcSolution,floatX,arrY,floatStep,intIterations): # Runge-Kutta (RK4) method for vector y
    arrValues = [[floatX,[arrY[0],arrY[1]],funcSolution(floatX),[0,0],[0,0]]]
    arrK1 = []
    arrK2 = []
    arrK3 = []
    arrK4 = []
    for intIter in range(len(arrY)): # Define vector k1,k2,k3,k4 in correct size
        arrK1.append(0)
        arrK2.append(0)
        arrK3.append(0)
        arrK4.append(0)
    for intIter in range(intIterations): # For each iteration
        for intSubIter in range(len(arrY)): # Calculate vector k1,k2,k3,k4
            arrK1[intSubIter] = floatStep*f(floatX,arrY)[intSubIter]
            arrK2[intSubIter] = floatStep*f(floatX+floatStep/2,vectorSum(1,arrY,1/2,arrK1))[intSubIter]
            arrK3[intSubIter] = floatStep*f(floatX+floatStep/2,vectorSum(1,arrY,1/2,arrK2))[intSubIter]
            arrK4[intSubIter] = floatStep*f(floatX+floatStep,vectorSum(1,arrY,1,arrK3))[intSubIter]
            arrY[intSubIter] = arrY[intSubIter] + (arrK1[intSubIter]+2*arrK2[intSubIter]+2*arrK3[intSubIter]+arrK4[intSubIter])/6 # Apply term to term rule to each element of y
        arrLocal = funcSolution(floatX)
        for intSubIter in range(len(arrY)): # Recalculating vector k1,k2,k3,k4 for local error
            arrK1[intSubIter] = floatStep*f(floatX,arrLocal)[intSubIter]
            arrK2[intSubIter] = floatStep*f(floatX+floatStep/2,vectorSum(1,arrLocal,1/2,arrK1))[intSubIter]
            arrK3[intSubIter] = floatStep*f(floatX+floatStep/2,vectorSum(1,arrLocal,1/2,arrK2))[intSubIter]
            arrK4[intSubIter] = floatStep*f(floatX+floatStep,vectorSum(1,arrLocal,1,arrK3))[intSubIter]
            arrLocal[intSubIter] = arrLocal[intSubIter] + (arrK1[intSubIter]+2*arrK2[intSubIter]+2*arrK3[intSubIter]+arrK4[intSubIter])/6 # Apply term to term rule to each element of arrLocal
        floatX = floatX + floatStep # Increment x
        arrValues.append([floatX,[arrY[0],arrY[1]],funcSolution(floatX),vectorSum(1,arrY,-1,funcSolution(floatX)),arrLocal])

    return arrValues

def trapeziumRule(arrValues): # Function to integrate using the trapezium rule
    floatArea = 0
    for intIter in range(1,len(arrValues),1):
        floatArea = floatArea+(arrValues[intIter][1]+arrValues[intIter-1][1])*(arrValues[intIter][0]-arrValues[intIter-1][0])/2
    return floatArea

## Constants used in questions 5-8
floatGamma = 1
floatDelta = 0
floatOmega = 1
floatA = 1
floatW = 3**(1/2)
