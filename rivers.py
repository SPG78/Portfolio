import csv

intSeaLevel = 92
strImportPath = "topology.bmp"
strExportPath = "rivers.bmp"
strSources = "sources.csv"
floatWidthIncrement = 0.5

arrKey = [
[255,0,128],# 0 = SEA
[255,255,255],# 1 = LAND
[0,255,0], # 2 = SOURCE
[255,0,0], # 3 = TRIBUTARY
[255,252,0]# 4 = SPLIT
[[0,0,100],[0,0,150],[0,0,200],[0,0,225],[0,0,255],[0,100,255],[0,150,255],[0,200,255],[0,225,255]], # 5+ = RIVER (Decreasing width)
]

def exportBMP(arrOut,strFilename):
    intX = len(arrOut[0])
    intY = len(arrOut)
    bstrOut = b"\x42\x4D" # BM
    bstrOut = bstrOut + (3*intX*intY+54).to_bytes(4,byteorder="little") # File size, include rest
    bstrOut = bstrOut + b"\x00\x00\x00\x00" # Reserved bytes, may need replacing
    bstrOut = bstrOut + (54).to_bytes(4,byteorder="little") # Pixel offset
    bstrOut = bstrOut + (40).to_bytes(4,byteorder="little") # DIB header size (BITMAPINFOHEADER)
    bstrOut = bstrOut + (intX).to_bytes(4,byteorder="little") # Image width
    bstrOut = bstrOut + (intY).to_bytes(4,byteorder="little") # Image height
    bstrOut = bstrOut + (1).to_bytes(2,byteorder="little") # Number of colour planes
    bstrOut = bstrOut + (24).to_bytes(2,byteorder="little") # Bits per pixel
    bstrOut = bstrOut + (0).to_bytes(4,byteorder="little") # Compression (none)
    bstrOut = bstrOut + (3*intX*intY).to_bytes(4,byteorder="little") # Image size
    bstrOut = bstrOut + (0).to_bytes(4,byteorder="little") # Horizontal resolution, ignore
    bstrOut = bstrOut + (0).to_bytes(4,byteorder="little") # Vertical resolution, ignore
    bstrOut = bstrOut + (0).to_bytes(4,byteorder="little") # Default colour palette
    bstrOut = bstrOut + (0).to_bytes(4,byteorder="little") # Important colours, ignore

    for intIter in range(intY-1,-1,-1):
        for intSubIter in range(0,intX,1):
            bstrOut = bstrOut + (arrOut[intIter][intSubIter][2]).to_bytes(1,byteorder="little")
            bstrOut = bstrOut + (arrOut[intIter][intSubIter][1]).to_bytes(1,byteorder="little")
            bstrOut = bstrOut + (arrOut[intIter][intSubIter][0]).to_bytes(1,byteorder="little")
    with open(strFilename,"wb") as bmpWrite:
        bmpWrite.write(bstrOut)

def importBMP(strPath):
    with open(strPath,"rb") as bmpReader:
        bmpReader.read(10)
        intOffset = int.from_bytes(bmpReader.read(4),"little")
        bstrDIB = bmpReader.read(4)
        if bstrDIB != (40).to_bytes(4,byteorder="little"):
            raise
        intWidth = int.from_bytes(bmpReader.read(4),"little")
        intHeight = int.from_bytes(bmpReader.read(4),"little")
        bmpReader.read(intOffset-26)
        arrRead = []
        for intIter in range(intHeight):
            arrRead.append([])
            for intSubIter in range(intWidth):
                arrRead[-1].append([0,0,0])
        for intIter in range(intHeight-1,-1,-1):
            for intSubIter in range(0,intWidth,1):
                arrRead[intIter][intSubIter][2] = int.from_bytes(bmpReader.read(1),"little")
                arrRead[intIter][intSubIter][1] = int.from_bytes(bmpReader.read(1),"little")
                arrRead[intIter][intSubIter][0] = int.from_bytes(bmpReader.read(1),"little")
        return arrRead

def importCSV(strPath):
    arrStore = []
    with open(strPath, "r") as csvfile:
        arrReader = csv.reader(csvfile,delimiter=",",quotechar="|")
        for arrIterate in arrReader:
            arrStore.append(arrIterate)
        return arrStore

def findLand(arrTop):
    arrLand = []
    for intIter in range(len(arrTop)):
        arrLand.append([])
        for intSubIter in range(len(arrTop[0])):
            if arrTop[intIter][intSubIter] > intSeaLevel:
                arrLand[-1].append(arrKey[1])
            else:
                arrLand[-1].append(arrKey[0])
    return arrLand

def addRiver(arrTop,arrRivers,intSourceX,intSourceY):
    arrRivers[intSourceX][intSourceY] = arrKey[2]
    intX = intSourceX
    intY = intSourceY
    while arrTop[intX][intY] >= intSeaLevel:
        intDirection = 0
        intMin = arrTop[intX][intY]
        if arrTop[intX+1][intY] <= intMin:
            intDirection = 1
            intMin = arrTop[intX+1][intY]
        if arrTop[intX][intY-1] <= intMin:
            intDirection = 2
            intMin = arrTop[intX][intY-1]
        if arrTop[intX-1][intY] <= intMin:
            intDirection = 3
            intMin = arrTop[intX-1][intY]
        if arrTop[intX][intY+1] <= intMin:
            intDirection = 4
            intMin = arrTop[intX][intY+1]
        if intDirection == 0:
            return 0
        if intDirection == 1:
            intX = intX+1
        elif intDirection == 2:
            intY = intY-1
        elif intDirection == 3:
            intX = intX-1
        elif intDirection == 4:
            intY = intY+1
        if arrRivers[intX][intY] != arrKey[0] and arrRivers[intX][intY] != arrKey[1]:
            if arrRivers[intX][intY] == arrKey[2]:
                arrRivers[intX][intY] = arrKey[5]#MAX WIDTH
                return 0
            elif arrRivers[intX][intY] == arrKey[3]:
                print("WARNING: three-way tribute at " + str(intX) + ", " + str(intY))
                return 0
            else:
                arrRivers[intSourceX][intSourceY] = arrKey[5]
                arrRivers[intX][intY] = arrKey[3]
                return 0

        else:
            if intDirection % 2 == 0:
                floatAverage = arrTop[intX][intY+1] + arrTop[intX][intY-1] - 2*arrTop[intX][intY]
            else:
                floatAverage = arrTop[intX+1][intY] + arrTop[intX-1][intY] - 2*arrTop[intX][intY]
            if floatAverage/floatIncrement >= 9: #min width
                arrRivers[intX][intY] = arrKey[14]
            else:
                arrRivers[intX][intY] = arrKey[5+int(floatAverage/floatIncrement)]
            
arrTop = importBMP(strImportPath)
arrRivers = findLand(arrTop)
arrSources = importCSV(strSources)
for arrRiver in arrSources:
    addRiver(arrTop,arrRivers,arrRiver[0],arrRiver[1])
exportBMP(arrRivers,strExportPath)
