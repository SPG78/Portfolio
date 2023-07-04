## QUESTION 1

def generateInverses(intLength): # Function to generate the inverses of each number mod (intLength), where intLength is presumed to be prime
    arrInverse = []
    for intIter in range(1,intLength,1): # For every a between 1 and p-1 inclusive
        for intSubIter in range(1,intLength,1): # For every b between 1 and p-1 inclusive
            if (intIter * intSubIter) % intLength == 1: # If ab ≡ 1 mod p...
                arrInverse.append(intSubIter) #... set b as the inverse of a
                break # Move onto next a
    if len(arrInverse) == intLength-1: # If all values of a have inverses (i.e. if intLength is prime)
        return arrInverse
    else: # If intLength is not prime
        print(str(intLength) + " is not prime, so some inverses are not defined")


## Q1) Can be modified by setting the inverse of a^(-1) to a
## Q2) Each value appears as an inverse precisely once
##      => number of iterations is p(p-1)/2
##      Each iteration involves multiplication, modulus (3 steps) and evaluation
##      => iterative stages has 5p(p-1)/2
##      Each number begins with incrementing the number and ends with appending the inverse
##      => these includes 2(p-1) steps
##      => Total number = (p-1)(5p/2+2) = (5/2)p^2-p/2-2



## QUESTION 3

def transposeRows(arrMatrix,intFirst,intSecond): # T(i,j) = transposeRows(arrMatrix,i,j)
    arrMatrix[intFirst],arrMatrix[intSecond] = arrMatrix[intSecond],arrMatrix[intFirst]
    return arrMatrix

#def divideRow(arrMatrix,intRow,intDivisor):
#    for intColumn in range(len(arrMatrix[intRow])):
#        arrMatrix[intRow][intColumn] = arrMatrix[intRow][intColumn]/intDivisor
#    return arrMatrix

def divideRow(arrMatrix,intRow,intDivisor,arrInverses): # D(i,a) = divideRow(arrMatrix,i,a,arrInverses)
    for intColumn in range(len(arrMatrix[intRow])):
        arrMatrix[intRow][intColumn] = arrMatrix[intRow][intColumn]*arrInverses[intDivisor%(len(arrInverses)+1)-1]
    return arrMatrix

def subtractRow(arrMatrix,intFirst,intMultiplier,intSecond): # S(i,a,j) = subtractRow(arrMatrix,i,a,j)
    for intColumn in range(len(arrMatrix[intFirst])):
        arrMatrix[intFirst][intColumn] = arrMatrix[intFirst][intColumn] - intMultiplier * arrMatrix[intSecond][intColumn]
    return arrMatrix

def rowEchelon(arrMatrix,intModulus): # Function to convert arrMatrix into row echelon form mod (intModulus)
    arrInverse = generateInverses(intModulus) # Calculate inverses
    intRows = len(arrMatrix) # Get number of rows
    intColumns = len(arrMatrix[0]) # Get number of columns
    intColumnOffset = 0

    for intTarget in range(min(intColumns,intRows)): # min(intColumns,intRows) gives the maximum possible rank of an (intRows x intColumns) matrix
        intCount = 0
        if intTarget+intColumnOffset >= intColumns:
            break
        else:
            boolZero = False
        while arrMatrix[intTarget][intTarget+intColumnOffset] == 0: # If the element in the first unconsidered column of the first unconsidered row is zero,
            intCount = intCount + 1
            for intTrans in range(intTarget,intRows-1,1): # shuffle it to the bottom row
                arrMatrix = transposeRows(arrMatrix,intTrans,intTrans+1)
            if intCount == intRows-intTarget: # If all remaining elements in this column are zero
                if intColumnOffset == intColumns-intTarget-1: # If we are in the final column of the matrix
                    boolZero = True # End the process
                    break
                else:
                    intColumnOffset = intColumnOffset + 1 # Consider the next column
                    intCount = 0
        if boolZero:
            break
        if arrMatrix[intTarget][intTarget+intColumnOffset] != 0:
            arrMatrix = divideRow(arrMatrix,intTarget,arrMatrix[intTarget][intTarget+intColumnOffset],arrInverse) # Divide the first unconsidered row by its element in the first unconsidered column
            for intRow in range(0,intRows,1): # For every other row
                if intRow != intTarget:
                    subtractRow(arrMatrix,intRow,arrMatrix[intRow][intTarget+intColumnOffset],intTarget) # Subtract the fixed row by its element in this column

        for intRow in range(0,intRows,1):
            for intColumn in range(0,intColumns,1):
                arrMatrix[intRow][intColumn] = arrMatrix[intRow][intColumn] % intModulus # Take the modulus of every element in the matrix
    return arrMatrix

## QUESTION 4

##def kernel(arrMatrix,intModulus):
##    strAlphabet = "abcdefghijklmnopqrstuvwxyz"[-len(arrMatrix[0]):]
##    arrRowEchelon = rowEchelon(arrMatrix,intModulus)
##    dicEquations = {}
##    for intRow in range(len(arrRowEchelon)):
##        strElement = ""
##        charParameter = ""
##        for intSkip in range(len(arrRowEchelon[intRow])):
##            if arrRowEchelon[intRow][intSkip]:
##                charParameter = strAlphabet
##                break
##        for intColumn in range(intSkip+1,len(arrRowEchelon[intRow])):
##            if arrRowEchelon[intRow][intColumn] > 0:
##                strElement = strElement + " - " + str(arrRowEchelon[intRow][intColumn]) + strAlphabet[intColumn]
##            elif arrRowEchelon[intRow][intColumn] < 0:
##                strElement = strElement + " + " + str(arrRowEchelon[intRow][intColumn]*(-1)) + strAlphabet[intColumn]
##        if charParameter != "":
##            dicEquations[charParameter] = strElement
##    arrKernel = []
##    for charIter in strAlphabet:
##        if charIter in dicEquations.keys():
##            arrKernel.append(dicEquations[charIter])
##        else:
##            arrKernel.append(charIter)
##    return arrKernel

def kernel(arrMatrix,intModulus): # Function to calculate the kernel of a given matrix with given modulus
    arrRowEchelon = rowEchelon(arrMatrix,intModulus) # Convert the matrix into row echelon form
    #intNullity = len(arrRowEchelon[0])
    #for intRow in range(len(arrRowEchelon)):
    #    for intColumn in range(len(arrRowEchelon[intRow])):
    #        if arrRowEchelon[intRow][intColumn]:
    #            intNullity = intNullity -1
    #            break
    
    arrL = []
    for intRow in range(len(arrRowEchelon)):
        for intColumn in range(len(arrRowEchelon[intRow])):
            if arrRowEchelon[intRow][intColumn]:
                arrL.append(intColumn) # Find the column containing the first non-zero element in each row
                break

    arrKernel = []
    for intColumn in range(len(arrRowEchelon[0])):
        if intColumn not in arrL: # If a column does not contain the first non-zero element any row
            arrKernel.append([])
            for intIter in range(len(arrRowEchelon[0])):
                arrKernel[-1].append(0) # Generate a vector of appropriate length
            for intRow in range(len(arrL)): # For every non-zero row
                arrKernel[-1][arrL[intRow]] = -arrRowEchelon[intRow][intColumn] # Make a negative copy of the column, placing each element so it would be multiplied by its row's corresponding image basis vector.
            arrKernel[-1][intColumn] = 1 # Set the element that would be multiplied by this column to be one.

    for intRow in range(len(arrKernel)):
        for intColumn in range(len(arrKernel[intRow])):
            arrKernel[intRow][intColumn] = arrKernel[intRow][intColumn] % intModulus # Take modulus of each element

    return arrKernel

##for arrRow in rowEchelon([
##    [ 6,16,11,14, 1, 4],
##    [ 7, 9, 1, 1,21, 0],
##    [ 8, 2, 9,12,17, 7],
##    [ 2,19, 2,19, 7,12]],23):
##    print(arrRow)
##
##print()
##print()
##print()
##
##for arrRow in kernel([
##    [ 6,16,11,14, 1, 4],
##    [ 7, 9, 1, 1,21, 0],
##    [ 8, 2, 9,12,17, 7],
##    [ 2,19, 2,19, 7,12]],23):
##    print(arrRow)
##
##print()
##print()
##print()
##
##for arrRow in rowEchelon(kernel([
##    [ 6,16,11,14, 1, 4],
##    [ 7, 9, 1, 1,21, 0],
##    [ 8, 2, 9,12,17, 7],
##    [ 2,19, 2,19, 7,12]],23),23):
##    print(arrRow)
##
##print()
##print()
##print()
##
##for arrRow in kernel(kernel([
##    [ 6,16,11,14, 1, 4],
##    [ 7, 9, 1, 1,21, 0],
##    [ 8, 2, 9,12,17, 7],
##    [ 2,19, 2,19, 7,12]],23),23):
##    print(arrRow)

def copy(arrInput): # Function to copy a nested list with a distinct location in memory.
    arrOutput = []
    for intRow in range(len(arrInput)):
        arrOutput.append([])
        for intColumn in range(len(arrInput[intRow])):
            arrOutput[-1].append(arrInput[intRow][intColumn])
    return arrOutput

def printMatrix(arrMatrix): # Function to print matrices in a readable form
    for arrTemp in arrMatrix:
        print(arrTemp)
    print()

def questionSeven(arrU,arrW,intModulus): # Function to compute bases for U, W, U+W and U∩W given U and W. 
    arrU = rowEchelon(arrU,intModulus) # Convert U, W to row ecehlon form
    arrW = rowEchelon(arrW,intModulus)

    while not True in arrU[-1]: # Remove empty rows from U, W
        del arrU[-1]
    while not True in arrW[-1]:
        del arrW[-1]

    arrUuW = copy(arrU)
    for arrTemp in copy(arrW):
        arrUuW.append(arrTemp) # Concatenate bases of U and W to form matrix with row space U+W
    arrUuW = rowEchelon(arrUuW,intModulus) # Convert to row echelon form
    while not True in arrUuW[-1]:
        del arrUuW[-1] # Remove empty rows

    arrUnW = kernel(copy(arrW),intModulus)
    for arrTemp in kernel(copy(arrU),intModulus):
        arrUnW.append(arrTemp) # Concatenate kernels of U and W to form matrix with row space U'+W'
    arrUnW = rowEchelon(arrUnW,intModulus) # Convert to row echelon form
    while not True in arrUnW[-1]: # Remove empty rows
        del arrUnW[-1]
    arrUnW = kernel(arrUnW,intModulus) # Find kernel of sum of kernels, giving intersection

    print("Basis of U")
    printMatrix(arrU)
    print("Basis of W")
    printMatrix(arrW)
    print("Basis of U+W")
    printMatrix(arrUuW)
    print("Basis of U∩W")
    printMatrix(arrUnW)


## questionSeven([[1,0,0,0,3,0,0],[0,5,0,1,6,3,0],[0,0,5,0,2,0,0],[2,4,0,0,0,5,1],[4,3,0,0,6,2,6]],kernel([[[1,0,0,0,3,0,0],[0,5,0,1,6,3,0],[0,0,5,0,2,0,0],[2,4,0,0,0,5,1],[4,3,0,0,6,2,6]]],23),23)

    
## Q3) Rank of matrix = number of non-zero rows
## Q4) Basis of row space = non-zero row vectors
## Q5) dim(U)+dim(U^o) = n
## Q6) U^o = kernel of A1
## Q6) U^o^o = kernel of kernel of A1

## Q7) Row-echelon A, B and remove empty rows to get bases of U, W.
    # Concatenate A and B, row-echelon, remove empty rows to get UuW.
    # Concatenate kernels of A and B, row-echelon, remove empty rows and find kernel of that to get UnW 

## Q8) U, W partition R^7
