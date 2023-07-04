import csv
import time

def importTxtToStr(strPath):
    with open(strPath) as txtReader:
        arrImport = (txtReader.readlines())
    strBuild = ""
    for intTemp in range(0,len(arrImport),1):
        strBuild = strBuild + str(arrImport[intTemp])
    return strBuild

def importTxtToArray(strPath):
    with open(strPath) as txtReader:
        arrImport = (txtReader.readlines())
    for intTemp in range(0,len(arrImport),1):
        if str(arrImport[intTemp])[-1] == """
""":
            arrImport[intTemp] = str(arrImport[intTemp])[:-1]
        else:
            arrImport[intTemp] = str(arrImport[intTemp])
    return arrImport

def importCSV(strPath):
    arrStore = []
    with open(strPath, "r") as csvfile:
        arrReader = csv.reader(csvfile,delimiter=",",quotechar="|")
        for arrIterate in arrReader:
            arrStore.append(arrIterate)
        return arrStore

def detectPaths(strInput):
    print("Finding constituencies")
    intConstituencyCount = 0
    
    arrPaths = []
    strBuild = ""
    boolListen = False
    for intIter in range(len(strInput)-5):
        if boolListen:
            strBuild = strBuild + strInput[intIter]
            if strInput[intIter] == ">":
                arrPaths.append(strBuild)
                strBuild = ""
                boolListen = False
                intConstituencyCount = intConstituencyCount + 1
                print(str(intConstituencyCount) + " constituencies found")
        else:
            if strInput[intIter:intIter+5] == "<path":
                boolListen = True
    return arrPaths

def formatPath(strPath):
    arrPath = []
    strBuild = ""
    for charIter in strPath:
        if charIter == "\n":
            arrPath.append(strBuild)
            strBuild = ""
        elif strBuild != "" or charIter != " ":
            strBuild = strBuild + charIter
    arrPath.append(strBuild)
    return arrPath

def readPaths(arrPaths):
    dicPaths = {}
    for arrPath in arrPaths:
        strName = ""
        strData = ""
        for strIter in arrPath:
            if strIter[:4] == 'id="':
                strName = strIter[4:-1]
            elif strIter[:3] == 'd="':
                strData = strIter[3:-1]
        dicPaths[strName] = strData
    return dicPaths

def sortPaths(dicPaths,arrSort):
    arrOutput = []
    for strName in arrSort:
        arrOutput.append(dicPaths[strName])
    return arrOutput

def breakdown(strCoOrds):
    arrOut = []
    strBuild = ""
    if strCoOrds[-2:] == '"/':
        strCoOrds = strCoOrds[:-2]
    elif strCoOrds[-3:] == '" /':
        strCoOrds = strCoOrds[:-3]
    for charTemp in strCoOrds:
        if charTemp == " ":
            arrOut.append(strBuild)
            strBuild = ""
        else:
            strBuild = strBuild + charTemp
    arrOut.append(strBuild)
    return arrOut
            
def convertCoOrds(strCoOrds):
    for intIter in range(len(strCoOrds)):
        if strCoOrds[intIter] == ",":
            break
    return [float(strCoOrds[:intIter]),float(strCoOrds[intIter+1:])]

def vectorSum(arrLeft,arrRight):
    arrSum = []
    for intIter in range(len(arrLeft)):
        arrSum.append(arrLeft[intIter]+arrRight[intIter])
    return arrSum

def buildVertices(arrPath):
    arrBuild = []
    intLen = len(arrPath)
    if arrPath[0] == "m" or arrPath[0] == "M":
        arrBuild.append(convertCoOrds(arrPath[1]))
    else:
        print("ERROR: Path starts with " + arrPath[0] + ". Path must start with m or M")
        raise
    strMode = "l"

    for strIter in arrPath[2:]:
        if len(strIter) == 1 and not strIter in "0123456789":
            strMode = strIter
            if strMode == "Z" or strMode =="z":
                arrBuild.append(arrBuild[0])
        else:
            if strMode == "l":
                arrBuild.append(vectorSum(arrBuild[-1],convertCoOrds(strIter)))
            elif strMode == "L":
                arrBuild.append(convertCoOrds(strIter))
            elif strMode == "h":
                arrBuild.append([arrBuild[-1][0]+float(strIter),arrBuild[-1][1]])
            elif strMode == "H":
                arrBuild.append([float(strIter),arrBuild[-1][1]])
            elif strMode == "v":
                arrBuild.append([arrBuild[-1][0],arrBuild[-1][1]+float(strIter)])
            elif strMode == "V":
                arrBuild.append([arrBuild[-1][0],float(strIter)])
            elif strMode == "Z" or strMode == "z":
                pass
            elif strMode == "m":
                arrBuild.append(vectorSum(arrBuild[-1],convertCoOrds(strIter)))
            elif strMode == "M":
                arrBuild.append(convertCoOrds(strIter))
            else:
                print("ERROR: Invalid mode: " + strMode)
                if strMode == "c":
                    return arrBuild
                else:
                    raise
    return arrBuild

def testVertices(arrVertex,arrBoundary):
    floatTolerance = 0.001
    for arrComp in arrBoundary:
        floatDist = (arrVertex[0]-arrComp[0])**2+(arrVertex[1]-arrComp[1])**2
        if floatDist < floatTolerance:
            return 1
    return 0

def genAdjacency(arrVertices):
    #global arrConstituencies
    #global dicRegions
    arrAdjacency = []
    print("Testing adjacency in " +str(len(arrVertices))+ " constituencies")
    for intX in range(len(arrVertices)):
        print("Testing "+arrConstituencies[intX])
        #arrAdjacency.append([0,0,0,0,0])
        arrAdjacency.append([])
        for intY in range(intX):
            arrAdjacency[-1].append(arrAdjacency[intY][intX])
        for intY in range(intX,len(arrVertices)):
            arrAdjacency[-1].append(0)      
        for intY in range(intX+1,len(arrVertices)):
            for strRegion in dicAdj[arrRegions[intX][1]]:
                if intY in dicRegions[strRegion]:
                    print("Testing " + arrConstituencies[intX] + " and " + arrConstituencies[intY])
                    for arrIter in arrVertices[intX]:
                        arrAdjacency[intX][intY] = arrAdjacency[intX][intY] + testVertices(arrIter,arrVertices[intY])
    return arrAdjacency

def genAdjacencyCities(arrVertices):
    #global arrCities
    arrAdjacency = []
    print("Testing adjacency in " +str(len(arrVertices))+ " constituencies")
    for intX in range(len(arrVertices)):
        print("Testing "+arrCities[intX])
        #arrAdjacency.append([0,0,0,0,0])
        arrAdjacency.append([])
        for intY in range(intX):
            arrAdjacency[-1].append(arrAdjacency[intY][intX])
        for intY in range(intX,len(arrVertices)):
            arrAdjacency[-1].append(0)      
        for intY in range(intX+1,len(arrVertices)):
            if intY in dicMaps[arrRegionsCities[intX][1]]:
                print("Testing " + arrCities[intX] + " and " + arrCities[intY])
                for arrIter in arrVertices[intX]:
                    arrAdjacency[intX][intY] = arrAdjacency[intX][intY] + testVertices(arrIter,arrVertices[intY])
    return arrAdjacency

    
def writeToCSV(strPath,arrWrite):
    strWrite=""
    for arrTemp in arrWrite:
        for strTemp in arrTemp:
            if type(strTemp) == list:
                for strTemp2 in strTemp:
                    strWrite = strWrite + str(strTemp2)
                    strWrite = strWrite + ","
            else:
                strWrite = strWrite + str(strTemp)
                strWrite = strWrite + ","
        strWrite = strWrite[:-1] + """
"""
    with open(strPath, "w") as csvfile:
        csvfile.write(strWrite[:-1])





floatStartTime = time.time()

print("Loading files")
arrConstituencies = importTxtToArray("constituencies.txt")
arrCities = importTxtToArray("cities.txt")
strSVG = importTxtToStr("constituencies.svg")
arrRegions = importCSV("regions.csv")
arrAdj = importCSV("adjacencies.csv")
arrTranslations = importCSV("translations.csv")
#arrRegionsCities("regionscities.csv")

print("Processing regions")
dicRegions = {}
for intIter in range(len(arrRegions)):
    if arrRegions[intIter][1] in dicRegions.keys():
        dicRegions[arrRegions[intIter][1]].append(intIter)
    else:
        dicRegions[arrRegions[intIter][1]]=[intIter]
dicAdj = {}
for arrIter in arrAdj:
    dicAdj[arrIter[0]] = []
    for strIter in arrIter:
        if strIter == "":
            break
        else:
            dicAdj[arrIter[0]].append(strIter)
##dicMaps = {}
##for intIter in range(len(arrRegionsCities)):
##    if arrRegionsCities[intIter][1] in dicMaps.keys():
##        dicMaps[arrRegionsCities[intIter][1]].append(intIter)
##    else:
##        dicMaps[arrRegionsCities[intIter][1]]=[intIter]

        
arrPaths = detectPaths(strSVG)
arrFormat = []
print("Formatting constituencies")
for strIter in arrPaths:
    arrFormat.append(formatPath(strIter))
print("Sorting constituencies")
dicPaths = readPaths(arrFormat)
arrRaw = sortPaths(dicPaths,arrConstituencies)

print("Building constituencies")
arrVertices = []
for intTemp in range(len(arrRaw)):
    print("Building "+arrConstituencies[intTemp])
    arrVertices.append(buildVertices(breakdown(arrRaw[intTemp])))

#writeToCSV("Edgbaston.csv",arrVertices[46])
#writeToCSV("Bromsgrove.csv",arrVertices[94])

print("Translating constituencies")
for arrIter in arrTranslations:
    for arrSubIter in arrVertices[int(arrIter[0])]:
        arrSubIter[0] = arrSubIter[0] + float(arrIter[1])
        arrSubIter[1] = arrSubIter[1] + float(arrIter[2])

print("Testing adjacencies")
arrMatrix = genAdjacency(arrVertices)

##print("Testing cities")
##arrRawCities = sortPaths(dicPaths,arrCities)
##print("Building city constituencies")
##arrVerticiesCities  = []
##for intTemp in range(len(arrRawCities)):
##    print("Building "+arrCities[intTemp])
##    arrVerticiesCities.append(buildVertices(breakdown(arrRawCities[intTemp])))
##print("Testing adjacencies in cities")
##arrAdjCities = genAdjacencyCities(arrVerticiesCities)
##
##print("Overwriting city data")
##arrTranslate = []
##for strCity in arrCities:
##    for intIter in range(len(arrConstituencies)):
##        if strCity[:-2] == arrConstituencies[intIter]:
##            arrTranslate.append(intIter)
##            break
##
##for intX in range(len(arrAdjCities)):
##    for intY in range(len(arrAdjCities[intX])):
##        arrMatrix[arrTranslate[intX]][arrTranslate[intY]] = arrAdjCities[intX][intY]

for intX in range(len(arrMatrix)):
    for intY in range(len(arrMatrix)):
        arrMatrix[intX][intY] = int(bool(arrMatrix[intX][intY]))

print("Writing matrix to file")
writeToCSV("mattMatrix.csv",arrMatrix)
print("Program complete")
floatEndTime = time.time()
floatRuntime = floatEndTime-floatStartTime
print("Total runtime: "+str(int(floatRuntime/3600))+":"+str(int((floatRuntime%3600)/60))+":"+str((floatRuntime%3600)%60))
input()
