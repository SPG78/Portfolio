def vectorSum(intLeft,arrLeft,intRight,arrRight):
    arrOutput = []
    for intIter in range(len(arrLeft)):
        arrOutput.append(intLeft*arrLeft[intIter]+intRight*arrRight[intIter])
    return arrOutput

def copy(arrInput):
    arrOutput = []
    for intIter in range(len(arrInput)):
        arrOutput.append(arrInput[intIter])
    return arrOutput

def bedpan(x):
    return x[0]+x[1]+x[0]**2/4-x[1]**2+(x[1]**2-x[0]/2)**2

def bedpanPartials(x):
    return [1+x[0]-x[1]**2,1-2*(1+x[0])*x[1]+4*x[1]**3]

def rosenbrock(x):
    return (1-x[0])**2+80*(x[1]-x[0]**2)**2

def rosenbrockPartials(x):
    return [2*x[0]-2+320*(x[0]**3-x[0]*x[1]),160*(x[1]-x[0]**2)]

def quad(x):
    return 0.4*x[0]**2+0.2*x[1]**2+x[2]**2+x[0]*x[2]

def quadPartials(x):
    return [0.8*x[0]+x[2],0.4*x[1],2*x[2]+x[0]]

def identityMatrix(intLength):
    arrOutput = []
    for intIter in range(intLength):
        arrOutput.append([])
        for intSubIter in range(intLength):
            if intIter == intSubIter:
                arrOutput[-1].append(1)
            else:
                arrOutput[-1].append(0)
    return arrOutput

def steepestGradient(f,g,x): ##TBH probably best just to merge s into function instead of using as argument
    arrOutput = []
    boolSkip = False
    while not boolSkip:
    #for intTemp in range(10):
        arrDir = []
        for intIter in range(len(x)):
            arrDir.append(g(x)[intIter]*(-1))
        floatLambda = 0
        floatStep = 0.0001
        boolSkip = True
        while f(vectorSum(1,x,floatStep*10,arrDir))<f(vectorSum(1,x,floatStep,arrDir)):
            boolSkip = False
            floatStep = floatStep * 10
        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
            floatLambda = floatLambda + floatStep
        floatStep = floatStep / 10
        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
            floatLambda = floatLambda + floatStep
        x = vectorSum(1,x,floatLambda,arrDir)
        arrOutput.append([copy(x),f(x)])
    return arrOutput

def conjugateGradient(f,g,x): ##TBH probably best just to merge s into function instead of using as argument
    arrOutput = [[copy(x),f(x)]]
    boolSkip = False
    arrDir = []
    for intIter in range(len(x)):
        arrDir.append(g(x)[intIter]*(-1))
    #while not boolSkip:
    for intTemp in range(10):
        floatLambda = 0
        floatStep = 0.0001
        boolSkip = True
        while f(vectorSum(1,x,floatStep*10,arrDir))<f(vectorSum(1,x,floatStep,arrDir)):
            boolSkip = False
            floatStep = floatStep * 10
        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
            floatLambda = floatLambda + floatStep
        floatStep = floatStep / 10
        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
            floatLambda = floatLambda + floatStep
        x = vectorSum(1,x,floatLambda,arrDir)
        arrOutput.append([copy(x),f(x)])
        
        intNum = 0
        intDen = 0
        gx = g(x)
        gy = g(arrOutput[-2][0])
        for intIter in range(len(gx)):
            intNum = intNum + gx[intIter]**2
            intDen = intDen + gy[intIter]**2
        arrDir = vectorSum(-1,g(x),(intNum/intDen)**(1/2),arrDir)
    return arrOutput

def dfp(f,g,x): ##TBH probably best just to merge s into function instead of using as argument
    arrOutput = []
    boolSkip = False
    H=identityMatrix(len(x))
    arrDir = copy(x)
    #while not boolSkip:
    for intTemp in range(10):
        arrGrad = g(x)
        for intIter in range(len(arrDir)):
            arrDir[intIter]=0
            for intSubIter in range(len(arrDir)):
                arrDir[intIter] = arrDir[intIter] - H[intIter][intSubIter]*arrGrad[intSubIter]
        floatLambda = 0
        floatStep = 0.0001
        boolSkip = True
        while f(vectorSum(1,x,floatStep*10,arrDir))<f(vectorSum(1,x,floatStep,arrDir)):
            boolSkip = False
            floatStep = floatStep * 10
        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
            floatLambda = floatLambda + floatStep
        floatStep = floatStep / 10
        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
            floatLambda = floatLambda + floatStep
        x = vectorSum(1,x,floatLambda,arrDir)
        arrOutput.append([copy(x),f(x)])
        p = vectorSum(1,g(x),-1,g(vectorSum(1,x,-floatLambda,arrDir)))
        q = copy(arrDir)
        for intIter in range(len(q)):
            q[intIter] = q[intIter] * floatLambda
        Hp = []
        for intIter in range(len(q)):
            Hp.append(0)
            for intSubIter in range(len(q)):
                Hp[-1] = Hp[-1] + H[intIter][intSubIter]*p[intSubIter]
        Hstar = []
        for arrIter in H:
            Hstar.append(copy(arrIter))
        for intIter in range(len(q)):
            for intSubIter in range(len(q)):
                intFirst = 0
                intSecond = 0
                intThird = 0
                intFourth = 0
                for intSum in range(len(q)):
                    intFirst = intFirst + H[intIter][intSum]*p[intSum]
                    intSecond = intSecond + p[intSum]*H[intSum][intSubIter]
                    intThird = intThird + p[intSum]*Hp[intSum]
                    intFourth = intFourth + p[intSum]*q[intSum]
                if intThird == 0 or intFourth == 0:
                    boolSkip = True
                else:
                    Hstar[intIter][intSubIter] = H[intIter][intSubIter] - intFirst*intSecond/intThird + q[intIter]*q[intSubIter]/intFourth
        for intIter in range(len(Hstar)):
            H[intIter] = copy(Hstar[intIter])
        #Hpstar = []
        #for intIter in range(len(q)):
        #    Hpstar.append(0)
        #    for intSubIter in range(len(q)):
        #        Hpstar[-1] = Hpstar[-1] + H[intIter][intSubIter]*p[intSubIter]
        #print(vectorSum(1,Hpstar,-1,q))

    return arrOutput

def dfpManual(f,g,x,arrLambda): ##TBH probably best just to merge s into function instead of using as argument
    arrOutput = []
    boolSkip = False
    H=identityMatrix(len(x))
    arrDir = copy(x)
    #while not boolSkip:
    for intTemp in range(len(arrLambda)):
        arrGrad = g(x)
        for intIter in range(len(arrDir)):
            arrDir[intIter]=0
            for intSubIter in range(len(arrDir)):
                arrDir[intIter] = arrDir[intIter] - H[intIter][intSubIter]*arrGrad[intSubIter]
##        floatLambda = 0
##        floatStep = 0.0001
        boolSkip = True
##        while f(vectorSum(1,x,floatStep*10,arrDir))<f(vectorSum(1,x,floatStep,arrDir)):
##            boolSkip = False
##            floatStep = floatStep * 10
##        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
##            floatLambda = floatLambda + floatStep
##        floatStep = floatStep / 10
##        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
##            floatLambda = floatLambda + floatStep
        floatLambda = arrLambda[intTemp]
        x = vectorSum(1,x,floatLambda,arrDir)
        arrOutput.append([copy(x),f(x)])
        p = vectorSum(1,g(x),-1,g(vectorSum(1,x,-floatLambda,arrDir)))
        q = copy(arrDir)
        for intIter in range(len(q)):
            q[intIter] = q[intIter] * floatLambda
        Hp = []
        for intIter in range(len(q)):
            Hp.append(0)
            for intSubIter in range(len(q)):
                Hp[-1] = Hp[-1] + H[intIter][intSubIter]*p[intSubIter]
        Hstar = []
        for arrIter in H:
            Hstar.append(copy(arrIter))
        for intIter in range(len(q)):
            for intSubIter in range(len(q)):
                intFirst = 0
                intSecond = 0
                intThird = 0
                intFourth = 0
                for intSum in range(len(q)):
                    intFirst = intFirst + H[intIter][intSum]*p[intSum]
                    intSecond = intSecond + p[intSum]*H[intSum][intSubIter]
                    intThird = intThird + p[intSum]*Hp[intSum]
                    intFourth = intFourth + p[intSum]*q[intSum]
                if intThird == 0 or intFourth == 0:
                    boolSkip = True
                else:
                    Hstar[intIter][intSubIter] = H[intIter][intSubIter] - intFirst*intSecond/intThird + q[intIter]*q[intSubIter]/intFourth
        for intIter in range(len(Hstar)):
            H[intIter] = copy(Hstar[intIter])
        #Hpstar = []
        #for intIter in range(len(q)):
        #    Hpstar.append(0)
        #    for intSubIter in range(len(q)):
        #        Hpstar[-1] = Hpstar[-1] + H[intIter][intSubIter]*p[intSubIter]
        #print(vectorSum(1,Hpstar,-1,q))

    for y in H:
        print(y)
    return arrOutput



def minimise(f,g,s,x): ##TBH probably best just to merge s into function instead of using as argument
    arrOutput = []
    boolSkip = False
    while not boolSkip:
    #for intTemp in range(10):
        arrDir = s(f,g,arrDir,x,arrOutput[-2][0])
        floatLambda = 0
        floatStep = 0.0001
        boolSkip = True
        while f(vectorSum(1,x,floatStep*10,arrDir))<f(vectorSum(1,x,floatStep,arrDir)):
            boolSkip = False
            floatStep = floatStep * 10
        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
            floatLambda = floatLambda + floatStep
        floatStep = floatStep / 10
        while f(vectorSum(1,x,floatLambda+floatStep,arrDir))<f(vectorSum(1,x,floatLambda,arrDir)):
            floatLambda = floatLambda + floatStep
        x = vectorSum(1,x,floatLambda,arrDir)
        arrOutput.append([copy(x),f(x)])
    return arrOutput
