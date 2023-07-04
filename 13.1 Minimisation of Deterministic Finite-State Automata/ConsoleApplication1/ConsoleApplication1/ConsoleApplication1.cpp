// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define intMaxLanguageSize 63
#define intMaxStates 255

using namespace std;

struct state{
	bool accept;
	int transition[intMaxLanguageSize];
};


int intEmpty = -858993460;
int size(int arrInput[],int intNull = intEmpty)
{
	int intCount = 0;
	while (true)
	{
		if (arrInput[intCount] == intNull)
		{
			return intCount;
		}
		intCount = intCount + 1;
	}

}

/*int size(state arrDFA[])
{
	int intCount = 0;
	while (true)
	{
		if (arrDFA[intCount].accept != false)
		{
			if (arrDFA[intCount].accept != true)
			{
				return intCount;
			}
		}
		intCount = intCount + 1;
	}
}*/

int size(state arrDFA[])
{
	int intCount = 0;
	while (true)
	{
		if (arrDFA[intCount].transition[0] < 0)
		{
			return intCount;
		}
		intCount = intCount + 1;
	}
}

bool checkElement(int intElement, int arrTest[])
{
	bool boolOutput = false;
	int intSize = size(arrTest);
	for (int intIter = 0; intIter < intSize; intIter = intIter + 1)
	{
		if (arrTest[intIter] == intElement)
		{
			boolOutput = true;
		}
	}
	return boolOutput;
}

void bubbleSort(int arrList[])
{
	int intSize = size(arrList);
	bool boolCont = true;
	int intStore;
	while (boolCont)
	{
		boolCont = false;
		for (int intIter = 1; intIter < intSize; intIter = intIter + 1)
		{
			if (arrList[intIter - 1] > arrList[intIter])
			{
				intStore = arrList[intIter - 1];
				arrList[intIter - 1] = arrList[intIter];
				arrList[intIter] = intStore;
				boolCont = true;
			}
		}
	}
}

void importCSV(string* pointFirstElement, string strFilename, char charDelim = ',') // Function to import a .csv file to an array of strings
{
	// pointFirstElement = Pointer for the first element of the array
	// strFilename       = Filename of the .csv file to be imported
	// charDelim         = CSV Delimiter

	int intIncrement = 0; // Parameter for increment
	string strLine; // Paramater for reading each line

	ifstream csvFile(strFilename.c_str()); // Generate a stream from the file
	while (getline(csvFile, strLine)) // Iterate through the file, reading each line to strLine
	{
		istringstream issLine(strLine); // Generate a stream from the line
		string strEntry; // Parameter for reading each entry
		while (getline(issLine, strEntry, charDelim)) // Iterate through the line, reading each entry to strEntry
		{
			*(pointFirstElement + intIncrement) = strEntry; // Write the value of strEntry to the appropriate element
			intIncrement = intIncrement + 1; // Increment the second index
		}
	}
}

void buildDFAFromFile(state arrDFA[], string strFilename, int intAlphabetSize, char charDelim)
{
	string arrImport[1023];
	importCSV(&arrImport[0], strFilename, charDelim);
	int intIter = 0;
	while (arrImport[intIter*(intAlphabetSize+1)].length() > 0)
	{
		for (int intCount = 0; intCount < intAlphabetSize; intCount = intCount + 1)
		{
			arrDFA[intIter].transition[intCount] = stoi(arrImport[intIter*(intAlphabetSize + 1) + intCount])-1;
		}
		arrDFA[intIter].accept = stoi(arrImport[intIter*(intAlphabetSize + 1) + intAlphabetSize]);
		intIter = intIter + 1;
	}
}

int pow(int intBase, int intExp)
{
	int intReturn = 1;
	for (int intIter = 0; intIter < intExp; intIter = intIter + 1)
	{
		intReturn = intReturn * intBase;
	}
	return intReturn;
}

/*void buildDFAFromInt(state arrDFA[], int intInput, int intAlphabetSize, int intStates)
{
	//structure of intInput: each state has k digits, corresponding to the destinations of each character, followed by a binary digit determining accept
	int intRead = 0;
	int intDenom;
	intInput = intInput % (pow((intAlphabetSize*intStates), intStates)*pow(intStates, 2));
	for (int intIter = 0; intIter < intStates; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < intAlphabetSize; intSubIter = intSubIter + 1)
		{
			intDenom = pow((intAlphabetSize*(intStates-intIter)-intSubIter-1), intStates)*pow(intStates-intIter, 2);
			intRead = intInput / intDenom;
			arrDFA[intIter].transition[intSubIter] = intRead;
			intInput = intInput - intRead * intDenom;
		}
		intDenom = pow((intAlphabetSize*(intStates - intIter - 1) - 1), intStates)*pow(intStates - intIter, 2);
		intRead = intInput / intDenom;
		arrDFA[intIter].accept = intRead;
		intInput = intInput - intRead * intDenom;
	}
}*/

void readDFA(state arrDFA[], string strAlphabet)
{
	int intAlphabetSize = strAlphabet.length();
	int intDFASize = size(arrDFA);
	for (int intIter = 0; intIter < intDFASize; intIter = intIter + 1)
	{
		cout << "State " << intIter + 1 << '\n';
		cout << "Accept: " << arrDFA[intIter].accept << '\n';
		for (int intSubIter = 0; intSubIter < intAlphabetSize; intSubIter = intSubIter + 1)
		{
			cout << strAlphabet[intSubIter] << " --> " << arrDFA[intIter].transition[intSubIter] + 1 << '\n';
		}
	}
}


void buildDFAFromInt(state arrDFA[], int intInput, int intAlphabetSize, int intStates)
{
	//structure of intInput: first n digits correspond to accept properties of the n states, followed by nk digits corresponding to the k destinations from each of the n states

	intInput = intInput % (pow(intStates, intStates*intAlphabetSize)*pow(2, intStates));
	int intAccept = intInput / pow(intStates, intStates*intAlphabetSize);
	intInput = intInput % pow(intStates, intStates*intAlphabetSize);

	for (int intIter = 0; intIter < intStates; intIter = intIter + 1)
	{
		arrDFA[intIter].accept = intAccept / pow(2,intStates-intIter-1);
		intAccept = intAccept % pow(2, intStates - intIter - 1);
		for (int intSubIter = 0; intSubIter < intAlphabetSize; intSubIter = intSubIter + 1)
		{
			arrDFA[intIter].transition[intSubIter] = intInput / pow(intStates, (intStates-intIter)*intAlphabetSize-intSubIter-1);
			intInput = intInput % pow(intStates, (intStates - intIter)*intAlphabetSize - intSubIter - 1);
		}
	}

}


int getIndex(int intElement, int arrList[])
{
	int intLength = size(arrList);
	for (int intIter = 0; intIter < intLength; intIter = intIter + 1)
	{
		if (intElement == arrList[intIter])
		{
			return intIter;
		}
	}
}

int dfaToInt(state arrDFA[], int intAlphabetSize, int intStates, bool boolAscend = false)
{
	int intAccept = 0;
	int intOutput = 0;
	int arrOrder[intMaxStates];
	int intIndex = 1;
	arrOrder[0] = 0;
	if (boolAscend)
	{
		for (int intState = 0; intState < intStates; intState = intState + 1)
		{
			for (int intDest = 0; intDest < intAlphabetSize; intDest = intDest + 1)
			{
				if (!checkElement(arrDFA[intState].transition[intDest], arrOrder))
				{
					arrOrder[intIndex] = arrDFA[intState].transition[intDest];
					intIndex = intIndex + 1;
					if (intIndex == intStates)
					{
						goto reorder;
					}
				}
			}
		}
	reorder:
		state arrCopy[intMaxStates];
		int arrInverse[intMaxStates];
		for (int intState = 0; intState < intStates; intState = intState + 1)
		{
			arrCopy[intState] = arrDFA[intState];
			arrInverse[intState] = getIndex(intState, arrOrder);
		}
		for (int intState = 0; intState < intStates; intState = intState + 1)
		{
			arrDFA[intState] = arrCopy[arrOrder[intState]];
			for (int intTrans = 0; intTrans < intAlphabetSize; intTrans = intTrans + 1)
			{
				arrDFA[intState].transition[intTrans] = arrInverse[arrDFA[intState].transition[intTrans]];
			}
		}
	}

	for (int intState = 0; intState < intStates; intState = intState + 1)
	{
		intAccept = intAccept + (arrDFA[intState].accept*pow(2, intStates - intState - 1));
		for (int intIter = 0; intIter < intAlphabetSize; intIter = intIter + 1)
		{
			intOutput = intOutput + (arrDFA[intState].transition[intIter] * pow(intStates, (intStates - intState)*intAlphabetSize - intIter - 1));
		}
	}
	intOutput = intOutput + intAccept * pow(intStates, intStates*intAlphabetSize);
	return intOutput;
}

int lookup(char charInput, string strReference)
{
	for (int intLocation = 0; intLocation < strReference.length(); intLocation = intLocation + 1)
	{
		if (strReference[intLocation] == charInput)
		{
			return intLocation;
		}
	}
	return 0;
}

bool emulateDFA(state arrDFA[], string strInput, string strAlphabet)
{
	int intState = 0;
	for (int intIter = 0; intIter < strInput.length(); intIter = intIter + 1)
	{
		intState = arrDFA[intState].transition[lookup(strInput[intIter],strAlphabet)];
	}
	return arrDFA[intState].accept;
}

// QUESTION 2

void findAccessibles(state arrDFA[], int arrOutput[])
{
	int intSize = size(arrDFA);
	int intIndex = 1;
	arrOutput[0] = 0;
	for (int intIter = 1; intIter < intSize; intIter = intIter + 1)
	{
		arrOutput[intIter] = intEmpty;
	}
	int intIter = 0;
	while (intIter < intIndex)
	{
		for (int intDest = 0; intDest < size(arrDFA[arrOutput[intIter]].transition); intDest = intDest + 1)
		{
			if (!checkElement(arrDFA[arrOutput[intIter]].transition[intDest], arrOutput))
			{
				arrOutput[intIndex] = arrDFA[arrOutput[intIter]].transition[intDest];
				intIndex = intIndex + 1;
			}
		}
		intIter = intIter + 1;
	}
	bubbleSort(arrOutput);
}

// QUESTION 3
void removeInaccessibles(state arrDFA[], int intAlphabetSize)
{
	int intStates = size(arrDFA);
	int arrAccessibles[intMaxStates];
	findAccessibles(arrDFA, arrAccessibles);
	int intLength;
	// SORT arrAccessibles URGENTLY OTHERWISE THIS ALL BREAKS
	intLength = size(arrAccessibles);
	
	for (int intIter = 0; intIter < intLength; intIter = intIter + 1)
	{
		arrDFA[intIter] = arrDFA[arrAccessibles[intIter]];
		for (int intSubIter = 0; intSubIter < intAlphabetSize; intSubIter = intSubIter + 1)
		{
			arrDFA[intIter].transition[intSubIter] = getIndex(arrDFA[intIter].transition[intSubIter],arrAccessibles);
		}
		for (int intSubIter = intAlphabetSize; intSubIter < intStates; intSubIter = intSubIter + 1)
		{
			arrDFA[intIter].transition[intSubIter] = intEmpty;
		}

	}
	for (int intIter = intLength; intIter < intStates; intIter = intIter + 1)
	{
		arrDFA[intIter].transition[0] = -1;
	}
}

void hopcroft(state arrDFA[], string strAlphabet) // Might be worth rewriting with deques instead. May be worth using checkElement too.
{
	int arrP[intMaxStates][intMaxStates];
	int arrW[intMaxStates][intMaxStates];
	int arrA[intMaxStates];
	int arrX[intMaxStates];
	int arrY[intMaxStates];
	int arrIntersect[intMaxStates];
	int arrDistinct[intMaxStates];

	int intStates = size(arrDFA);

	for (int intIter = 0; intIter < intStates; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < intMaxStates; intSubIter = intSubIter + 1)
		{
			arrP[intIter][intSubIter] = -1;
			arrW[intIter][intSubIter] = -1;
		}
	}
	bool boolEmpty = true;
	for (int intIter = 0; intIter < intStates; intIter = intIter + 1)
	{
		if (arrDFA[intIter].accept)
		{
			boolEmpty = false;
		}
	}

	for (int intIter = 0; intIter < intStates; intIter = intIter + 1)
	{
		if (arrDFA[intIter].accept||boolEmpty)
		{
			for (int intCount = 0; intCount < intMaxStates; intCount = intCount + 1)
			{
				if (arrP[0][intCount] < 0)
				{
					arrP[0][intCount] = intIter;
					arrW[0][intCount] = intIter;
					break;
				}
			}
		}
		else
		{
			for (int intCount = 0; intCount < intMaxStates; intCount = intCount + 1)
			{
				if (arrP[1][intCount] < 0)
				{
					arrP[1][intCount] = intIter;
					arrW[1][intCount] = intIter;
					break;
				}
			}
		}
	}

	while (arrW[0][0] != -1) // While W is not empty do
	{

		for (int intIter = 0; intIter < intMaxStates; intIter = intIter + 1)
		{
			arrA[intIter] = -1;
			arrX[intIter] = -1;
			arrY[intIter] = -1;
			arrIntersect[intIter] = -1;
			arrDistinct[intIter] = -1;
		}
		for (int intIter = intStates - 1; intIter >= 0; intIter = intIter - 1) // choose and remove a set A from W
		{
			if (arrW[intIter][0] != -1)
			{
				for (int intSubIter = 0; intSubIter < intMaxStates; intSubIter = intSubIter + 1)
				{
					arrA[intSubIter] = arrW[intIter][intSubIter];
					arrW[intIter][intSubIter] = -1;
				}
				break;
			}
		}

		//for (char charIter : strAlphabet) // For each c in sigma do
		for (int intChar = 0; intChar < strAlphabet.length(); intChar = intChar + 1)
		{
			// let X be the set of states for which a transition on c leads to a state in A
			int intXIndex = 0;
			for (int intIter = 0; intIter < intStates; intIter = intIter + 1)
			{
				bool boolFind = false;
				for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1)
				{
					if (arrDFA[intIter].transition[intChar] == arrA[intSubIter])
					{
						boolFind = true;
						break;
					}
				}
				if (boolFind)
				{
					arrX[intXIndex] = intIter;
					intXIndex = intXIndex + 1;
				}
			}


			for (int intIter = 0; intIter < intStates; intIter = intIter + 1) // for each set Y in P...
			{
				int intIIndex = 0;
				int intDIndex = 0;
				for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1)
				{
					arrIntersect[intSubIter] = -1;
					arrDistinct[intSubIter] = -1;
				}
				bool boolFound = false;
				for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1)
				{
					arrY[intSubIter] = arrP[intIter][intSubIter];
					boolFound = false;
					for (int intTriIter = 0; intTriIter < intStates; intTriIter = intTriIter + 1)
					{
						
						if (arrY[intSubIter] != -1)
						{
							if (arrY[intSubIter] == arrX[intTriIter])
							{
								arrIntersect[intIIndex] = arrY[intSubIter];
								intIIndex = intIIndex + 1;
								boolFound = true;
								break;
							}
						}
					}
					if (!boolFound && arrY[intSubIter] != -1)
					{
						arrDistinct[intDIndex] = arrY[intSubIter];
						intDIndex = intDIndex + 1;
						
					}
				}
				if (intIIndex != 0 && intDIndex != 0) //... for which XnY is nonempty and Y \ X is nonempty do
				{
					int intCount = 0;
					for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1)
					{
						if (arrP[intSubIter][0] == -1)
						{
							intCount = intSubIter;
							break;
						}
					}

					for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1) // Replace Y in P by the two sets XnY and Y \ X
					{
						arrP[intIter][intSubIter] = arrDistinct[intSubIter];
						arrP[intCount][intSubIter] = arrIntersect[intSubIter];
					}
					for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1)
					{
						if (arrW[intSubIter][0] == -1)
						{
							intCount = intSubIter;
							break;
						}
					}
					int intW = -1;
					for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1)
					{
						bool boolEqual = true;
						for (int intTriIter = 0; intTriIter < intStates; intTriIter = intTriIter + 1)
						{
							if (arrW[intSubIter][intTriIter] != arrY[intTriIter])
							{
								boolEqual = false;
								break;
							}
						}
						if (boolEqual)
						{
							intW = intSubIter;
							break;
						}
					}
					if (intW >= 0) // if Y is in W
					{
						for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1) // Replace Y in W by the same two sets
						{
							arrW[intW][intSubIter] = arrDistinct[intSubIter];
							arrW[intCount][intSubIter] = arrIntersect[intSubIter];
						}
					}
					else // else
					{
						if (intDIndex >= intIIndex) // If |XnY| <= |Y\X|
						{
							for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1) // Add XnY to W
							{
								arrW[intCount][intSubIter] = arrIntersect[intSubIter];
							}
						}
						else // else
						{
							for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1) // Add Y\X to W
							{
								arrW[intCount][intSubIter] = arrDistinct[intSubIter];
							}
						}
					}
				}
				for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1)
				{
					arrY[intSubIter] = -1;
				}
			}
		}
	}
	

	int arrIndex[intMaxStates];
	for (int intIter = 0; intIter < intStates; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < intStates; intSubIter = intSubIter + 1)
		{
			if (arrP[intIter][intSubIter] == -1)
			{
				break;
			}
			else
			{
				arrIndex[arrP[intIter][intSubIter]] = arrP[intIter][0];
			}
		}
	}
	//Iterate through arrDFA[].transition, replacing all elements with arrIndex values.
	for (int intIter = 0; intIter < intStates; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < strAlphabet.length(); intSubIter = intSubIter + 1)
		{
			arrDFA[intIter].transition[intSubIter] = arrIndex[arrDFA[intIter].transition[intSubIter]];
		}
	}
	removeInaccessibles(arrDFA,strAlphabet.length());
}

// QUESTION 4
string generateWord(int intSeed, string strAlphabet, int intLength)
{
	string strOutput = "";
	int intRead;
	for (int intDigit = 0; intDigit < intLength; intDigit = intDigit + 1)
	{
		intRead = intSeed%strAlphabet.length();
		strOutput = strAlphabet[intRead] + strOutput;
		intSeed = intSeed / strAlphabet.length();
	}
	return strOutput;
}

bool testFinite(state arrDFA[], string strAlphabet)
{
	int intSize = size(arrDFA);
	int intAlphabetSize = strAlphabet.length();
	for (int intLength = intSize; intLength < 2 * intSize; intLength = intLength + 1)
	{
		for (int intWord = 0; intWord < pow(intAlphabetSize, intLength); intWord = intWord + 1)
		{
			if (emulateDFA(arrDFA, generateWord(intWord, strAlphabet, intLength), strAlphabet))
			{
				return false;
			}
		}
	}
	return true;
}
int countLanguages(int intStates, string strAlphabet, bool boolSizes = false, int* ptrSizes = nullptr)
{


	int intLanguages = 0;
	int intAlphabetSize = strAlphabet.length();
	int intMax = pow(2, intStates)*pow(intStates, intStates*intAlphabetSize);
	int *arrLanguages = new int[intMax];
	for (int intIter = 0; intIter < intMax; intIter = intIter + 1)
	{
		arrLanguages[intIter] = intEmpty;
	}
	state arrDFA[intMaxStates];
	int intTemp;
	if (boolSizes)
	{
		for (int intCount = 0; intCount < intMaxStates; intCount = intCount + 1)
		{
			*(ptrSizes + intCount) = 0;
		}
	}
	for (int intDFA = 0; intDFA < intMax; intDFA = intDFA + 1)
	{
		buildDFAFromInt(arrDFA, intDFA, intAlphabetSize, intStates);
		removeInaccessibles(arrDFA,intAlphabetSize);
		hopcroft(arrDFA, strAlphabet);
		if (intStates == size(arrDFA))
		{
			intTemp = dfaToInt(arrDFA, intAlphabetSize, size(arrDFA), true);
			if (!checkElement(intTemp, arrLanguages))
			{
				arrLanguages[intLanguages] = intTemp;
				intLanguages = intLanguages + 1;
				if (boolSizes) //  if boolSizes = true, ptrSizes points to a matrix that stores the sizes of each language generated
				{
					int intCount = arrDFA[0].accept;
					if (testFinite(arrDFA, strAlphabet))
					{
						for (int intSize = 1; intSize < intStates; intSize = intSize + 1)
						{
							for (int intWord = 0; intWord < pow(intAlphabetSize, intSize); intWord = intWord + 1)
							{
								//string strWord = "";
								//for (int intChar = intSize; intChar > 0; intChar = intChar - 1)
								//{
								//	strWord = strWord + strAlphabet[(intWord / (pow(intAlphabetSize, intChar) - 1)) % intAlphabetSize];
								//}
								string strWord = generateWord(intWord, strAlphabet, intSize);
								if (emulateDFA(arrDFA, strWord, strAlphabet))
								{
									//*(ptrSizes + intLanguages - 1) = *(ptrSizes + intLanguages - 1) + 1;
									intCount = intCount + 1;
								}
							}
						}
						*(ptrSizes + intCount) = *(ptrSizes + intCount) + 1;
					}
					else
					{
						*(ptrSizes + pow(intAlphabetSize, intStates) + 1) = *(ptrSizes + pow(intAlphabetSize, intStates) + 1) + 1;
					}
				}
			}
		}
	}
	return intLanguages;
}



void accessibleStates(string strPath, int intAlphabetSize)
{
	state arrDFA[intMaxStates];
	buildDFAFromFile(arrDFA, strPath, intAlphabetSize, ' ');

	//readDFA(arrDFA, "abc");

	int arrAccessibles[intMaxStates];
	findAccessibles(arrDFA, arrAccessibles);
	int intSize = size(arrAccessibles);
	cout << "Accessible states are: ";
	for (int intIter = 0; intIter < intSize; intIter = intIter + 1)
	{
		cout << arrAccessibles[intIter]+1 << ' ';
	}
	cout << "\n\n\n";
}

void countAllAccessibles(int intSize, int intAlphabetSize)
{
	state arrDFA[intMaxStates];
	int arrAccessibles[intMaxStates];
	int intCount = 0;
	for (int intDFA = 0; intDFA < pow(intSize,intSize*intAlphabetSize); intDFA = intDFA + 1)
	{
		buildDFAFromInt(arrDFA, intDFA, intAlphabetSize, intSize);
		findAccessibles(arrDFA, arrAccessibles);
		if (intSize == size(arrAccessibles))
		{
			intCount = intCount + pow(2, intSize);
		}
	}
	cout << intCount << "\n\n\n";
}

void outputTransitionTable(state arrDFA[], string strAlphabet)
{
	int intAlphabetSize = strAlphabet.length();
	int intDFASize = size(arrDFA);
	for (int intIter = 0; intIter < intDFASize; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < intAlphabetSize; intSubIter = intSubIter + 1)
		{
			cout << arrDFA[intIter].transition[intSubIter] + 1 << ' ';
		}
		cout << arrDFA[intIter].accept << '\n';
	}
}


void applyHopcroft(string strPath, string strAlphabet)
{
	state arrDFA[intMaxStates];
	buildDFAFromFile(arrDFA, strPath, strAlphabet.length(), ' ');
	removeInaccessibles(arrDFA,strAlphabet.length());
	hopcroft(arrDFA, strAlphabet);
	outputTransitionTable(arrDFA, strAlphabet);
	cout << "\n\n";
}

void languageSize(string strPath, string strAlphabet)
{
	state arrDFA[intMaxStates];
	buildDFAFromFile(arrDFA, strPath, strAlphabet.length(), ' ');
	removeInaccessibles(arrDFA, strAlphabet.length());
	hopcroft(arrDFA, strAlphabet);
	if (testFinite(arrDFA, strAlphabet))
	{
		int intSize = arrDFA[0].accept;
		//int intMax = pow(strAlphabet.length(), size(arrDFA) - 1);
			//for (int intWord = 0; intWord < intMax; intWord = intWord + 1)
			//{
			//	intSize = intSize + emulateDFA(arrDFA, generateWord(intWord, strAlphabet), strAlphabet);
		//	}
		for (int intLength = 0; intLength < size(arrDFA); intLength = intLength + 1)
		{
			for (int intWord = 0; intWord < pow(strAlphabet.length(), intLength); intWord = intWord + 1) {
				intSize = intSize + emulateDFA(arrDFA, generateWord(intWord, strAlphabet, intLength), strAlphabet);
			}
		}

			cout << intSize << "\n\n\n";
	}
	else
	{
		cout << "infinite\n\n\n";
	}
}


void fns(int intN, string strAlphabet)
{
	int arrOutput[intMaxStates];
	countLanguages(intN, strAlphabet, true, &arrOutput[0]);
	for (int intS = 0; intS < pow(2, intN); intS = intS + 1)
	{
		cout << "f(" << intN << ','<<2 <<','<< intS << ") = " << arrOutput[intS] << '\n';
	}
	cout << "\n\n\n";
}

void genTransitions(int intStates, string strAlphabet, int intSize)
{


	int intLanguages = 0;
	int intAlphabetSize = strAlphabet.length();
	int intMax = pow(2, intStates)*pow(intStates, intStates*intAlphabetSize);
	int *arrLanguages = new int[intMax];
	string arrLanguage[16];
	for (int intIter = 0; intIter < intMax; intIter = intIter + 1)
	{
		arrLanguages[intIter] = intEmpty;
	}
	state arrDFA[intMaxStates];
	int intTemp;
	for (int intDFA = 0; intDFA < intMax; intDFA = intDFA + 1)
	{
		buildDFAFromInt(arrDFA, intDFA, intAlphabetSize, intStates);
		removeInaccessibles(arrDFA, intAlphabetSize);
		hopcroft(arrDFA, strAlphabet);
		if (intStates == size(arrDFA))
		{
		intTemp = dfaToInt(arrDFA, intAlphabetSize, size(arrDFA), true);
		if (!checkElement(intTemp, arrLanguages))
		{
			arrLanguages[intLanguages] = intTemp;
			intLanguages = intLanguages + 1;
			int intCount = arrDFA[0].accept;
			if (testFinite(arrDFA, strAlphabet))

			{

				for (int intSize = 1; intSize < intStates; intSize = intSize + 1)
				{
					for (int intWord = 0; intWord < pow(intAlphabetSize, intSize); intWord = intWord + 1)
					{
						//string strWord = "";
						//for (int intChar = intSize; intChar > 0; intChar = intChar - 1)
						//{
						//	strWord = strWord + strAlphabet[(intWord / (pow(intAlphabetSize, intChar) - 1)) % intAlphabetSize];
						//}
						string strWord = generateWord(intWord, strAlphabet, intSize);
						if (emulateDFA(arrDFA, strWord, strAlphabet))
						{
							arrLanguage[intCount] = strWord;
							intCount = intCount + 1;
						}
					}

				}

				if (intCount == intSize)
				{
					cout << "\nTransition Table:\n";
					outputTransitionTable(arrDFA, strAlphabet);
					cout << "Language:\n";
					if (arrDFA[0].accept)
					{
						arrLanguage[0] = "empty word";
					}
					for (int intWord = 0; intWord < intCount; intWord = intWord + 1)
					{
						cout << arrLanguage[intWord] << '\n';
						arrLanguage[intWord] = "";
					}
				}
			}
		}
		}
	}
	cout << "\n\n\n";
}



int main()
{
	string strAlphabet = "abcdefghijklmnopqrstuvwxyz";

	// To run code for a specific question, uncomment the specific line below
	//accessibleStates("Table1.txt","Table2.txt","Table3.txt","Table4.txt"); // . Note that arguments give paths to a transition table

	int intInput;
	string strPath;
	int intArg;
	int intArgTwo;
	string strArg;
	
	while (true)
	{
		cout << "Menu:\n";
		cout << "1. Determine accessible states from DFA specified by table\n";
		cout << "2. Compute the number of (n,k)-DFAs with no inaccessible states\n";
		cout << "3. Apply Hopcroft's table-filling algorithm to transition table\n";
		cout << "4. Output number of (n,2)-DFA languages\n";
		cout << "5. Compute language size from transition table\n";
		cout << "6. Compute f(n,2,s)\n";
		cout << "7. Generate minimal (n,2) transition tables for languages of size s\n";
		cin >> intInput;
		switch (intInput)
		{
		case 1:
			cout << "Input path: ";
			cin >> strPath;
			cout << "Input alphabet size: ";
			cin >> intArg;
			accessibleStates(strPath, intArg);
			break;
		case 2:
			cout << "Input n: ";
			cin >> intArg;
			cout << "Input k: ";
			cin >> intArgTwo;
			countAllAccessibles(intArg, intArgTwo);
			break;
		case 3:
			cout << "Input path: ";
			cin >> strPath;
			cout << "Input alphabet: ";
			cin >> strArg;
			applyHopcroft(strPath, strArg);
			break;
		case 4:
			cout << "Input n: ";
			cin >> intArg;
			cout << countLanguages(intArg, "ab") << "\n\n\n";
			break;
		case 5:
			cout << "Input path: ";
			cin >> strPath;
			cout << "Input alphabet: ";
			cin >> strArg;
			languageSize(strPath, strArg);
			break;
		case 6:
			cout << "Input n: ";
			cin >> intArg;
			fns(intArg, "ab");
			break;
		case 7:
			cout << "Input n: ";
			cin >> intArg;
			cout << "Input s: ";
			cin >> intArgTwo;
			genTransitions(intArg, "ab", intArgTwo);
			break;

		default:
			cout << "Command not recognised. Please enter a number above.\n\n\n";
		}
	}
	



	return 0;
}