import math

floatCrit = 3.29*10**(15) #v0
floatCritAbs = 6.3*10**(-22) #av0
c = 299792458
hk = 6.626/1.381 * 10**4


def solveIonisation(nH,T,TSurf,R):
    s=1568435653#=1.568435653x10^9#=2*math.pi*c^(-2)*floatCritAbs*(floatCrit)^3

    dicRecomb = {5000:4.54*10**(-19),10000:2.59*10**(-19),20000:2.52*10**(-19)}
    dicRecombScale = {5000:4.54,10000:2.59,20000:2.52}
    floatSScale =2*math.pi*29.9792458**(-2)*6.3*3.29**3#= 1.568435653
    #print(floatSScale)
    #floatNHScale = nH*10**(-9)
    #floatCritAbsScale = 6.3*10**6
    floatCritScale = 3.29

    arrOutput = [[0,nH]]

    floatDStep = 0.01
    floatThreshold = 2**(-14)
    #floatTStep = 0.01
    floatTStep = R/10
    tv = 0
    rMax = R * 100 # r has arbitrary scaling apparently
    #rMax = 6

    #for r in range(R,rMax,floatTStep):
    #print(R)
    #print(rMax)
    #print(floatTStep)
    for intIter in range(int((rMax-R)/floatTStep)):
        r = intIter*floatTStep + R
        tv = tv + floatTStep*floatCritAbs*(nH-arrOutput[-1][1])

        floatD = 0
        floatHeight = 1
        floatV = floatCritScale
        while floatHeight > floatThreshold:
            floatHeight = dIntegral(floatV,TSurf,tv)*10**13
            floatD = floatD + floatHeight * floatDStep
            floatV = floatV + floatDStep
        floatD =  floatD * (R/r)**2
        floatNP = nP(floatSScale,floatD,dicRecombScale[T],nH)
        arrOutput.append([r,floatNP])
    del arrOutput[0]
    return arrOutput
    
def nP (s,d,aB,nH):
    floatTemp = s*d/aB
    floatRet = -floatTemp/2 + ((floatTemp/2)**2+floatTemp*nH)**(1/2)
    return floatRet

def dIntegral(v,T,tv): # v/10^15
    return (math.e**(-tv*(3.29/v)**3))/(v*(math.e**(hk*v/T)-1))

def interpolateTable(arrTable,intLookup):
    for intIter in range(len(arrTable)):
        if intLookup < arrTable[intIter][0]:
            floatFraction = (intLookup-arrTable[intIter-1][0])/(arrTable[intIter][0]-arrTable[intIter-1][0])
            floatReturn = floatFraction * (arrTable[intIter][1]-arrTable[intIter-1][1]) + arrTable[intIter-1][1]
            break
    return floatReturn        

def Q(TSurf,R):
    floatD = 0
    floatHeight = 1
    floatV = 3.29
    floatDStep = floatV/100
    floatThreshold = 2**(-14)
    while floatHeight > floatThreshold:
        floatHeight = 8*(math.pi*R*floatV*10**15/c)**2/(math.e**(hk*floatV/TSurf)-1)
        floatD = floatD + floatHeight * floatDStep
        floatV = floatV + floatDStep
    return floatD


def solveIonisationQuasar(nH,T,R):
    s=7.566225034*10**34#=1.568435653x10^9#=2*math.pi*c^(-2)*floatCritAbs*(floatCrit)^3

    dicRecomb = {5000:4.54*10**(-19),10000:2.59*10**(-19),20000:2.52*10**(-19)}
    dicRecombScale = {5000:4.54,10000:2.59,20000:2.52}
    floatSScale =7.566225034#= 1.568435653
    #print(floatSScale)
    #floatNHScale = nH*10**(-9)
    #floatCritAbsScale = 6.3*10**6
    #floatCritScale = 3.29

    arrOutput = [[0,nH]]

    floatDStep = 0.01
    floatThreshold = 2**(-14)
    #floatTStep = 0.01
    floatTStep = R * 100
    tv = 0
    rMax = R * 1000000 # r has arbitrary scaling apparently
    #rMax = 6

    #for r in range(R,rMax,floatTStep):
    #print(R)
    #print(rMax)
    #print(floatTStep)
    for intIter in range(int((rMax-R)/floatTStep)):
        r = intIter*floatTStep + R
        tv = tv + floatTStep*floatCritAbs*(nH-arrOutput[-1][1])

        floatD = 0
        floatHeight = 1
        floatV = 1
        while floatHeight > floatThreshold:
            floatHeight = floatV**(-5.4)*math.e**(-floatV/10-tv*floatV**(-3))*10**17
            floatD = floatD + floatHeight * floatDStep
            floatV = floatV + floatDStep
        floatD =  floatD / r**2
        #print(floatD)
        floatNP = nP(floatSScale,floatD,dicRecombScale[T],nH)
        arrOutput.append([r,floatNP])
    del arrOutput[0]
    return arrOutput
