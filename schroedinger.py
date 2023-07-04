def returnZero(x): # Dummy function to use if solution is unknown
    return [0,0]

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

def schroedinger(V,E): # Defines d2Y/dX2 for the Schroedinger equation to be used in Runge-Kutte Method. V is the potential function, E is the energy eigenvalue.
    def f(x,y):
        return [y[1],(V(x)-E)*y[0]]
    return f

def harmonic(x): # Defines the potential for the harmonic oscillator
    return x**2

def nearlySquare(delta): # Defines the potential for a nearly square well with parameter delta
    def V(x):
        return -delta*(1+x**2)**(-2)
    return V

def vectorSum(floatLeft,arrLeft,floatRight,arrRight): # Function to give a linear combination of two vectors
    arrSum = []
    for intIter in range(len(arrLeft)):
        arrSum.append(floatLeft*arrLeft[intIter]+floatRight*arrRight[intIter])
    return arrSum


def intervalBisection(funcValue,floatLower,floatUpper,floatTol): # Function to find zeroes of function funcValue on range [floatLower,floatUpper], with precision floatTol, using interval bisection. 
    arrLimits = [floatLower,floatUpper] # Set the initial limits as given in parameters
    arrValues = [funcValue(floatLower),funcValue(floatUpper)] # Calculate initial values
    arrLog = [] # Array to record each step of method
    while abs(arrLimits[1]-arrLimits[0])>floatTol: # Repeat until within required precision
        floatMid = (arrLimits[0]+arrLimits[1])/2 # Calculate middle of interval
        floatValue = funcValue(floatMid) # Evaluate function at midpoint
        arrLog.append([arrLimits[0],floatMid,arrLimits[1],arrValues[0],floatValue,arrValues[1]]) # Record this step
        arrLimits[int(floatValue>0)] = floatMid # Replace relevant limit with the old midpoint
        arrValues[int(floatValue>0)] = floatValue # Replace relevant value of function with the newly evaluated value of the function
    return arrLog

def callSchroe(funcV,arrYo,floatStep,floatXMax): # Defines a function that integrates the Schrodinger equation with a given eigenvalue
    def func(x):
        return rungeKuttaVector(schroedinger(funcV,x),returnZero,0,arrYo,floatStep,int(floatXMax/floatStep))[-1][1][0]
    return func
