#pragma once

#include <iostream>
#include <random>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>
#include "struct.h"
#include <filesystem>

using namespace std;


// Settings
bool boolNormalise = false;
float floatFearImpairance = 0.15;
int floatAverage = 7;
int intSkills = 10;
int intFailureModel = 2;
bool boolLimitFailures = true;
float floatThreshold = 0.7;
bool boolDetailedResults = false;
default_random_engine generator(time(0));
hero arrHeroes[INT_MAXHEROES];
trial arrTrials[INT_MAXTRIALS];
int intGoldPerLife = 6;
string strHeroDirectory;
string strTrialDirectory;

/*
SKILLS
0 - Height
1 - Weight
2 - Accuracy
3 - Patience
4 - Courage
5 - Stamina
6 - Strength
7 - Agility
8 - Speed
9 - Teamwork
10 - Wisdom
11 - Leadership

FEARS
0 - Dark
1 - Death
2 - Enclosed Spaces
3 - Heights
4 - Water
5 - Snakes
6 - Spiders
*/

float artanh(float x)
{
	float floatArtanh;
	floatArtanh = log((1 + x) / (1 - x)) / 2;
	return floatArtanh;
}

template<typename iterand, size_t intLength>
void bubbleSort(iterand(&arrSort)[intLength], bool boolAscend = true)
{
	bool boolPass = false;
	float floatSave;
	while (!boolPass)
	{
		boolPass = true;
		if (boolAscend)
		{
			for (int intIter = 0; intIter < intLength - 1; intIter = intIter + 1)
			{
				if (arrSort[intIter] > arrSort[intIter + 1])
				{
					floatSave = arrSort[intIter];
					arrSort[intIter] = arrSort[intIter + 1];
					arrSort[intIter + 1] = floatSave;
					boolPass = false;
				}
			}
		}
		else
		{
			for (int intIter = 0; intIter < intLength - 1; intIter = intIter + 1)
			{
				if (arrSort[intIter] < arrSort[intIter + 1])
				{
					floatSave = arrSort[intIter];
					arrSort[intIter] = arrSort[intIter + 1];
					arrSort[intIter + 1] = floatSave;
					boolPass = false;
				}
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

void importHeroes(hero arrHeroes[], string strFilename)
{
	string arrImport[INT_MAXHEROES][25];
	importCSV(&arrImport[0][0], strFilename);

	arrHeroes[0].name = "None";
	arrHeroes[0].emblem = "None";
	arrHeroes[0].age = 0;
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		arrHeroes[0].skills[intIter] = 0;
		arrHeroes[0].fears[intIter] = 0;
	}
	for (int intIter = 7; intIter < 12; intIter = intIter + 1)
	{
		arrHeroes[0].skills[intIter] = 0;
	}

	for (int intIter = 1; intIter < INT_MAXHEROES; intIter = intIter + 1)
	{
		if (arrImport[intIter][0] == "")
		{
			break;
		}
		else
		{
			arrHeroes[intIter].name = arrImport[intIter][0];
		}
		arrHeroes[intIter].emblem = arrImport[intIter][1];
		arrHeroes[intIter].age = stof(arrImport[intIter][3]);
		for (int intVirtue = 0; intVirtue < 12; intVirtue = intVirtue + 1)
		{
			arrHeroes[intIter].skills[intVirtue] = stof(arrImport[intIter][5 + intVirtue]);
		}
		for (int intFear = 0; intFear < 7; intFear = intFear + 1)
		{
			arrHeroes[intIter].fears[intFear] = stof(arrImport[intIter][18 + intFear]);
		}
	}
}

void importWarriorsGUI(hero arrHeroes[], string arrImport[][25], int arrIter[], int intFirst, int intLength)
{
	for (int intIter = 0; intIter < intLength; intIter = intIter + 1)
	{
		if (arrImport[arrIter[intIter]][0] == "")
		{
			break;
		}
		else
		{
			arrHeroes[intFirst + intIter].name = arrImport[arrIter[intIter]][0];
		}
		arrHeroes[intFirst + intIter].emblem = arrImport[arrIter[intIter]][1];
		arrHeroes[intFirst + intIter].age = stof(arrImport[arrIter[intIter]][3]);
		for (int intVirtue = 0; intVirtue < 12; intVirtue = intVirtue + 1)
		{
			arrHeroes[intFirst + intIter].skills[intVirtue] = stof(arrImport[arrIter[intIter]][5 + intVirtue]);
		}
		for (int intFear = 0; intFear < 7; intFear = intFear + 1)
		{
			arrHeroes[intFirst + intIter].fears[intFear] = stof(arrImport[arrIter[intIter]][18 + intFear]);
		}
		arrHeroes[intFirst + intIter].gold = intGoldPerLife * intGoldPerLife;
	}
}

void importTrials(trial arrTrials[], string strFilename)
{
	string arrImport[INT_MAXTRIALS + 1][50];
	importCSV(&arrImport[0][0], strFilename);

	for (int intIter = 0; intIter < INT_MAXTRIALS; intIter = intIter + 1)
	{
		arrTrials[intIter].name = arrImport[intIter + 1][0];
		arrTrials[intIter].numberOfTeams = stoi(arrImport[intIter + 1][1]);
		arrTrials[intIter].heroesPerTeam = stoi(arrImport[intIter + 1][2]);
		arrTrials[intIter].race = stoi(arrImport[intIter + 1][3]);
		arrTrials[intIter].losingCosts = stoi(arrImport[intIter + 1][4]);
		arrTrials[intIter].notFirstCosts = stoi(arrImport[intIter + 1][5]);
		for (int intSkill = 0; intSkill < 12; intSkill = intSkill + 1)
		{
			arrTrials[intIter].skills[intSkill] = stof(arrImport[intIter + 1][7 + intSkill]);
		}
		for (int intFear = 0; intFear < 7; intFear = intFear + 1)
		{
			arrTrials[intIter].fears[intFear] = stof(arrImport[intIter + 1][20 + intFear]);
		}
		arrTrials[intIter].luck = stof(arrImport[intIter + 1][28]);
		arrTrials[intIter].maximumFailures = arrImport[intIter + 1][30];
		arrTrials[intIter].medFailureRisk = stof(arrImport[intIter + 1][31]);
		arrTrials[intIter].badFailureRisk = stof(arrImport[intIter + 1][32]);
		arrTrials[intIter].failureDescription[0] = arrImport[intIter + 1][33];
		arrTrials[intIter].failureDescription[1] = arrImport[intIter + 1][34];
		for (int intReward = 0; intReward < 6; intReward = intReward + 1)
		{
			try
			{
				arrTrials[intIter].rewards[intReward] = stoi(arrImport[intIter + 1][36 + intReward]);
			}
			catch (...)
			{
				arrTrials[intIter].rewardOptions = intReward;
				for (int intLeftover = intReward; intLeftover < 6; intLeftover = intLeftover + 1)
				{
					arrTrials[intIter].rewards[intLeftover] = 0;
				}
				break;
			}
		}
		arrTrials[intIter].winnerBonus = stoi(arrImport[intIter + 1][42]);
		arrTrials[intIter].optionalGold = stoi(arrImport[intIter + 1][43]);
		arrTrials[intIter].optionalGoldBias = stoi(arrImport[intIter + 1][44]);
		arrTrials[intIter].loserKeepsReward = stoi(arrImport[intIter + 1][45]);
		arrTrials[intIter].time[0] = stoi(arrImport[intIter + 1][47]) * 0.8;
		for (int intTime = 1; intTime < 4; intTime = intTime + 1)
		{
			arrTrials[intIter].time[intTime] = stoi(arrImport[intIter + 1][46 + intTime]);
		}
	}
}

void importChallengesGUI(trial arrTrials[], string arrImport[][50], int arrIter[], int intFirst, int intLength)
{

	for (int intIter = 0; intIter < intLength; intIter = intIter + 1)
	{
		if (arrImport[arrIter[intIter]][0] == "")
		{
			break;
		}
		else
		{
			arrTrials[intFirst + intIter].name = arrImport[arrIter[intIter]][0];
			arrTrials[intFirst + intIter].numberOfTeams = stoi(arrImport[arrIter[intIter]][1]);
			arrTrials[intFirst + intIter].heroesPerTeam = stoi(arrImport[arrIter[intIter]][2]);
			arrTrials[intFirst + intIter].race = stoi(arrImport[arrIter[intIter]][3]);
			arrTrials[intFirst + intIter].losingCosts = stoi(arrImport[arrIter[intIter]][4]);
			arrTrials[intFirst + intIter].notFirstCosts = stoi(arrImport[arrIter[intIter]][5]);
			for (int intSkill = 0; intSkill < 12; intSkill = intSkill + 1)
			{
				arrTrials[intFirst + intIter].skills[intSkill] = stof(arrImport[arrIter[intIter]][7 + intSkill]);
			}
			for (int intFear = 0; intFear < 7; intFear = intFear + 1)
			{
				arrTrials[intFirst + intIter].fears[intFear] = stof(arrImport[arrIter[intIter]][20 + intFear]);
			}
			arrTrials[intFirst + intIter].luck = stof(arrImport[arrIter[intIter]][28]);
			arrTrials[intFirst + intIter].maximumFailures = arrImport[arrIter[intIter]][30];
			arrTrials[intFirst + intIter].medFailureRisk = stof(arrImport[arrIter[intIter]][31]);
			arrTrials[intFirst + intIter].badFailureRisk = stof(arrImport[arrIter[intIter]][32]);
			arrTrials[intFirst + intIter].failureDescription[0] = arrImport[arrIter[intIter]][33];
			arrTrials[intFirst + intIter].failureDescription[1] = arrImport[arrIter[intIter]][34];
			for (int intReward = 0; intReward < 6; intReward = intReward + 1)
			{
				try
				{
					arrTrials[intFirst + intIter].rewards[intReward] = stoi(arrImport[arrIter[intIter]][36 + intReward]);
				}
				catch (...)
				{
					arrTrials[intFirst + intIter].rewardOptions = intReward;
					for (int intLeftover = intReward; intLeftover < 6; intLeftover = intLeftover + 1)
					{
						arrTrials[intFirst + intIter].rewards[intLeftover] = 0;
					}
					break;
				}
			}
			arrTrials[intFirst + intIter].winnerBonus = stoi(arrImport[arrIter[intIter]][42]);
			arrTrials[intFirst + intIter].optionalGold = stoi(arrImport[arrIter[intIter]][43]);
			arrTrials[intFirst + intIter].optionalGoldBias = stoi(arrImport[arrIter[intIter]][44]);
			arrTrials[intFirst + intIter].loserKeepsReward = stoi(arrImport[arrIter[intIter]][45]);
			arrTrials[intFirst + intIter].time[0] = stoi(arrImport[arrIter[intIter]][47]) * 0.8;
			for (int intTime = 1; intTime < 4; intTime = intTime + 1)
			{
				arrTrials[intFirst + intIter].time[intTime] = stoi(arrImport[arrIter[intIter]][46 + intTime]);
			}
		}
	}
}

float skillTransform(const hero heroIter, const int intVirtue)
{
	float floatSkills;
	floatSkills = float(heroIter.skills[intVirtue]);

	if (boolNormalise)
	{
		int intSum = 0;
		for (int intIter = 2; intIter < 12; intIter = intIter + 1)
		{
			intSum = intSum + heroIter.skills[intIter];
		}
		float floatAdjust;
		floatAdjust = floatAverage - intSum / intSkills;
		floatSkills = floatSkills + floatAdjust;
	}

	floatSkills = floatSkills * (heroIter.skills[0] * 0.02 + 0.86); // Height adjust
	floatSkills = floatSkills * (heroIter.skills[1] * 0.03 + 0.79); // Weight adjust
	floatSkills = floatSkills * (-0.334+0.147*heroIter.age-0.00519*heroIter.age*heroIter.age+0.0000735* heroIter.age* heroIter.age* heroIter.age-0.000000368* heroIter.age* heroIter.age* heroIter.age* heroIter.age); // Age adjust

	floatSkills = floatSkills - floatAverage;

	return floatSkills;
}

float skills(float arrRelevance[], hero arrTeam[], int intHeroesPerTeam)
{
	float arrSkills[8][12];
	float arrTeamSkills[12];

	// Transforming Skills
	for (int intIter = 0; intIter < 8; intIter = intIter + 1)
	{
		if (arrTeam[intIter].name == "None")
		{
			break;
		}
		else
		{
			arrSkills[intIter][0] = arrTeam[intIter].skills[0] - floatAverage;
			arrSkills[intIter][1] = arrTeam[intIter].skills[1] - floatAverage;
			for (int intSubIter = 2; intSubIter < 12; intSubIter = intSubIter + 1)
			{
				arrSkills[intIter][intSubIter] = skillTransform(arrTeam[intIter], intSubIter);
			}
		}
	}

	// Height, Weight, Accuracy, Calm under Pressure, Courage, Stamina, Strength scores
	for (int intSkill = 0; intSkill < 7; intSkill = intSkill + 1)
	{
		float floatSum = 0;
		for (int intWarrior = 0; intWarrior < intHeroesPerTeam; intWarrior = intWarrior + 1)
		{
			floatSum = floatSum + arrSkills[intWarrior][intSkill];
		}
		floatSum = floatSum / intHeroesPerTeam;
		arrTeamSkills[intSkill] = floatSum;
	}

	// Agility, Strength, Teamwork scores
	for (int intSkill = 7; intSkill < 10; intSkill = intSkill + 1)
	{
		float floatMin = arrSkills[0][intSkill];
		for (int intHero = 0; intHero < intHeroesPerTeam; intHero = intHero + 1)
		{
			floatMin = (floatMin > arrSkills[intHero][intSkill] && arrSkills[intHero][intSkill] > (-1) * floatAverage) ? arrSkills[intHero][intSkill] : floatMin;
		}
		arrTeamSkills[intSkill] = floatMin;
	}

	// Wisdom score
	float floatMax = arrSkills[0][10];
	for (int intHero = 0; intHero < intHeroesPerTeam; intHero = intHero + 1)
	{
		floatMax = (floatMax < arrSkills[intHero][10]) ? arrSkills[intHero][10] : floatMax;
	}
	arrTeamSkills[10] = floatMax;


	// Leadership score
	float arrLeadership[8] = { -10,-10,-10,-10,-10,-10,-10,-10 };
	for (int intHero = 0; intHero < intHeroesPerTeam; intHero = intHero + 1)
	{
		arrLeadership[intHero] = arrSkills[intHero][11];
	}
	bubbleSort(arrLeadership, false);
	int intMedian;
	intMedian = int(intHeroesPerTeam * 0.49);
	if (arrLeadership[intHeroesPerTeam - 1] >= 7 && arrLeadership[intMedian] > 8)
	{
		arrTeamSkills[11] = arrLeadership[intMedian];
	}
	else
	{
		arrTeamSkills[11] = arrLeadership[0];
	}

	/*cout << intMedian << ", " << intWarriorsPerTeam << '\n';
	for (float floatTemp : arrLeadership)
	{
		cout << floatTemp << '\n';
	}*/

	//Total score
	float floatScore = 0;
	for (int intIter = 0; intIter < 12; intIter = intIter + 1)
	{
		floatScore = floatScore + arrTeamSkills[intIter] * arrRelevance[intIter];
		//cout << intIter << ": " << arrTeamVirtues[intIter] << 'x' << arrRelevance[intIter] << " = " << arrTeamVirtues[intIter] * arrRelevance[intIter] << " --> " << floatScore << '\n';
	}
	floatScore = floatScore + floatAverage;
	return floatScore;
}

float fears(float arrRelevance[], hero arrTeam[], int intHeroesPerTeam)
{
	float arrTeamFears[7];
	float floatSum;
	float floatMax;

	for (int intFear = 0; intFear < 5; intFear = intFear + 1)
	{
		floatSum = 0;
		for (int intHero = 0; intHero < intHeroesPerTeam; intHero = intHero + 1)
		{
			floatSum = floatSum + arrTeam[intHero].fears[intFear];
		}
		floatSum = floatSum / intHeroesPerTeam;
		arrTeamFears[intFear] = floatSum;
	}

	for (int intFear = 5; intFear < 7; intFear = intFear + 1)
	{
		floatMax = 0;
		for (int intHero = 0; intHero < intHeroesPerTeam; intHero = intHero + 1)
		{
			floatMax = (floatMax < arrTeam[intHero].fears[intFear]) ? arrTeam[intHero].fears[intFear] : floatMax;
		}
		arrTeamFears[intFear] = floatMax;
	}

	float floatScore = 0;
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		floatScore = floatScore + arrTeamFears[intIter] * arrRelevance[intIter];
	}
	return floatScore;
}

string outcomes(float floatFailure, bool boolEndangered, bool boolRace, bool boolLostRace, string arrFailure[])
{
	string strOutput = "OK";
	if (boolEndangered)
	{
		// default_random_engine generator (time(0)+intIncrement);
		uniform_real_distribution<float> distribution(0, 1);
		float floatRand = distribution(generator);
		cout << "   Dice Roll: " << floatRand << '\n';
		if (floatRand < floatFailure)
		{
			if (arrFailure[0] != "")
			{
				if (arrFailure[1] != "")
				{
					uniform_real_distribution<float> distribution(0, 1);
					float floatRand;
					floatRand = distribution(generator);
					if (floatRand < 0.5)
					{
						strOutput = "LOSE (" + arrFailure[0] + ')';
					}
					else
					{
						strOutput = "LOSE (" + arrFailure[1] + ')';
					}
				}
				else
				{
					strOutput = "LOSE (" + arrFailure[0] + ')';
				}
			}
		}
		else if (floatRand < floatFailure * 1.15)
		{
			strOutput = "JUST OK";
		}
	}
	if (boolRace && boolLostRace && strOutput.substr(0, 4) != "LOSE")
	{
		strOutput = "LOSE";
	}
	return strOutput;
}

bool refusal(float floatFear, float floatCourage, float floatLeadership)
{
	float floatCourPower = 1.5;
	float floatLeadPower = 1;
	float floatRandomMitigation = 0.1;

	float floatCour;
	floatCour = 7 / floatCourage;
	floatCour = pow(floatCour, floatCourPower);

	float floatLead;
	floatLead = (floatLeadership > 7) ? 7 / floatLeadership : 1;
	floatLead = pow(floatLead, floatLeadPower);

	// default_random_engine generator (time(0)+intIncrement);
	uniform_real_distribution<float> distribution(0.00000001, 1);
	float floatRand;
	float floatRandOne;
	float floatRandTwo;
	floatRandOne = distribution(generator);
	floatRandOne = abs(2 * log(floatRandOne));
	floatRandOne = pow(floatRandOne, 0.5);
	floatRandTwo = distribution(generator);
	floatRandTwo = cos(floatRandTwo * 1.570796326);
	floatRand = 1 - floatRandOne * floatRandTwo * floatRandomMitigation;
	cout << "   Random mitigation: " << floatRand << '\n';

	float floatProduct;
	floatProduct = floatFear * floatCour * floatLead * floatRand;
	/*
	cout << "floatFear: " << floatFear << '\n';
	cout << "floatCourage: " << floatCourage << '\n';
	cout << "floatLeadership: " << floatLeadership << '\n';
	cout << "floatCour: " << floatCour << '\n';
	cout << "floatLead: " << floatLead << '\n';
	cout << "floatRand: " << floatRand << '\n';
	cout << "floatProduct: " << floatProduct << '\n';
	*/
	if (floatProduct > floatThreshold)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int optionalRewards(const trial trialSelected, float floatScore)
{
	int intPosition;
	// default_random_engine generator (time(0)+intIncrement);

	// cout << "chalSelected.ringOptions: " << chalSelected.ringOptions << '\n';
	if (trialSelected.optionalGoldBias == 2)
	{
		if (floatScore > 0)
		{
			uniform_int_distribution<int> distribution(0, trialSelected.rewardOptions - 2);
			intPosition = distribution(generator);
			cout << "   Above average index: " << intPosition + 1 << '\n';
		}
		else
		{
			uniform_int_distribution<int> distribution(1, trialSelected.rewardOptions - 1);
			intPosition = distribution(generator);
			cout << "   Below average index: " << intPosition + 1 << '\n';
		}
	}
	else if (trialSelected.optionalGoldBias == 1 || trialSelected.optionalGoldBias == 3)
	{
		uniform_real_distribution<float> distribution(0, 1);
		float floatRand;
		float floatRandOne;
		float floatRandTwo;
		floatRandOne = distribution(generator);
		floatRandOne = abs(2 * log(floatRandOne));
		floatRandOne = pow(floatRandOne, 0.5);
		floatRandTwo = distribution(generator);
		floatRandTwo = cos(floatRandTwo * 1.570796326);
		floatRand = floatRandOne * floatRandTwo * (trialSelected.rewardOptions - 1) / 3;
		cout << "   High bias index: " << floor(floatRand) << '\n';
		if (trialSelected.optionalGoldBias == 1) // LOW BIAS
		{
			intPosition = trialSelected.rewardOptions - int(floor(floatRand));
		}
		else if (trialSelected.optionalGoldBias == 3) // HIGH BIAS
		{
			intPosition = int(floor(floatRand));
		}
	}
	else
	{
		intPosition = 0;
	}
	return trialSelected.rewards[intPosition];
}

int rings(const trial trialSelected, string strOutcome, int intRank, float floatScore)
{
	int intRings;
	if (strOutcome == "REFUSED")
	{
		intRings = 0;
	}
	else if (trialSelected.race)
	{
		if (trialSelected.optionalGold)
		{
			if (!trialSelected.loserKeepsReward && strOutcome.substr(0, 4) == "LOSE")
			{
				intRings = 0;
			}
			else
			{
				intRings = optionalRewards(trialSelected, floatScore);
				if (intRank == 1)
				{
					intRings = intRings + trialSelected.winnerBonus;
				}
			}
		}
		else
			if (strOutcome.substr(0, 4) == "LOSE")
			{
				intRings = 0;
			}
			else
			{
				intRings = trialSelected.rewards[intRank - 1];
			}
	}
	else if (strOutcome.substr(0, 4) == "LOSE")
	{
		intRings = 0;
	}
	else if (trialSelected.optionalGold)
	{
		intRings = optionalRewards(trialSelected, floatScore);
	}
	else
	{
		intRings = trialSelected.rewards[0];
	}
	return intRings;
}

float raceTime(const trial trialSelected, float floatScore)
{
	float floatTime;
	if (floatScore > 10)
	{
		floatTime = ((16 - floatScore) / 6) * (trialSelected.time[1] - trialSelected.time[0]) + trialSelected.time[0];
	}
	else if (floatScore > 7)
	{
		floatTime = ((10 - floatScore) / 3) * (trialSelected.time[2] - trialSelected.time[1]) + trialSelected.time[1];
	}
	else
	{
		floatTime = ((7 - floatScore) / 3) * (trialSelected.time[3] - trialSelected.time[2]) + trialSelected.time[2];
	}
	return floatTime;
}

void runTrial(trial trialSelected, hero arrTeams[][8])
{
	int intTeams = 8;
	int intHeroesPerTeam = 8;
	float arrScores[8][4]; // Overall score, Virtues, Fears, Luck
	float arrRelative[8]; // Scores relative to average
	float arrFailure[8]; // Risks of failure
	string arrStates[8]; // Challenge outcomes (OK, LOSE, etc.)
	int arrRanks[8];
	int intMaxRank; // Number of warriors guarenteed not to lose a life
	int intLastPlace; // Final place in a race that does not lose a life
	int arrGold[8];
	float arrTimes[8];

	cout << "Running challenge: " << trialSelected.name << '\n';

	for (int intIter = 0; intIter < 8; intIter = intIter + 1)
	{
		if (arrTeams[intIter][0].name == "None")
		{
			intTeams = intIter;
			break;
		}
	}

	for (int intIter = 0; intIter < 8; intIter = intIter + 1)
	{
		if (arrTeams[0][intIter].name == "None")
		{
			intHeroesPerTeam = intIter;
			break;
		}
	}

	intTeams = (intTeams > trialSelected.numberOfTeams) ? trialSelected.numberOfTeams : intTeams;
	intHeroesPerTeam = (intHeroesPerTeam > trialSelected.heroesPerTeam) ? trialSelected.heroesPerTeam : intHeroesPerTeam;


	if (trialSelected.notFirstCosts)
	{
		intLastPlace = 1;
	}
	else if (trialSelected.losingCosts)
	{
		intLastPlace = intTeams - 1;
	}
	else
	{
		intLastPlace = intTeams;
	}

	int intFail;
	intFail = int(trialSelected.maximumFailures[0]) - 48;
	if (intFail >= 0 && intFail <= 9) // If the first character of chalSelected.maximumFailures is an integer
	{
		intMaxRank = intTeams - intFail;
	}
	else
	{
		if (trialSelected.maximumFailures == "few")
		{
			intMaxRank = ceil(intTeams * 3 / 4);
		}
		else if (trialSelected.maximumFailures == "many")
		{
			intMaxRank = 1;
		}
		else
		{
			intMaxRank = 0;
		}
	}

	// Calculating proportional virtue relevance
	cout << "Calculating proportional skill relevance\n";
	float arrSkillProp[12];
	float floatSum;
	floatSum = 0;
	for (int intIter = 0; intIter < 12; intIter = intIter + 1)
	{
		floatSum = floatSum + abs(trialSelected.skills[intIter]);
	}
	if (floatSum == 0)
	{
		for (int intIter = 0; intIter < 12; intIter = intIter + 1)
		{
			arrSkillProp[intIter] = 0;
		}
	}
	else
	{
		for (int intIter = 0; intIter < 12; intIter = intIter + 1)
		{
			arrSkillProp[intIter] = trialSelected.skills[intIter] / floatSum;
		}
	}

	// Calculating proportional fears relevance
	cout << "Calculating proportional fears relevance\n";
	float arrFearProp[7];
	floatSum = 0;
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		floatSum = floatSum + trialSelected.fears[intIter];
	}
	if (floatSum == 0)
	{
		for (int intIter = 0; intIter < 7; intIter = intIter + 1)
		{
			arrFearProp[intIter] = 0;
		}
	}
	else
	{
		for (int intIter = 0; intIter < 7; intIter = intIter + 1)
		{
			arrFearProp[intIter] = trialSelected.fears[intIter] / floatSum;
		}
	}

	// Calculating team scores
	cout << "Calculating team scores\n";
	// default_random_engine generator (time(0));
	normal_distribution<float> distribution(1, trialSelected.luck / 2);
	floatSum = 0;

	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		cout << "Calculating skill score for team " << intIter << '\n';
		arrScores[intIter][1] = skills(arrSkillProp, arrTeams[intIter], intHeroesPerTeam);
		cout << "Virtue score: " << arrScores[intIter][1] << '\n';
		cout << "Calculating fear score for team " << intIter << '\n';
		arrScores[intIter][2] = fears(arrFearProp, arrTeams[intIter], intHeroesPerTeam);
		cout << "Fear score: " << arrScores[intIter][2] << '\n';
		cout << "Calculating luck score for team " << intIter << '\n';
		arrScores[intIter][3] = distribution(generator);
		cout << "   Luck changes score: " << arrScores[intIter][3] << '\n';
		cout << "Luck score: " << arrScores[intIter][3] << '\n';
		arrScores[intIter][0] = arrScores[intIter][1] * arrScores[intIter][3] * (1 - arrScores[intIter][2] * floatFearImpairance);
		cout << "Team " << intIter << " score: " << arrScores[intIter][0] << "\n\n";
		floatSum = floatSum + arrScores[intIter][0];
	}
	floatSum = floatSum / intTeams;

	// Failure percentages
	cout << "Calculating failure percentages\n";
	float floatScale = 3 / (artanh(1 - 2 * trialSelected.medFailureRisk) - artanh(1 - 2 * trialSelected.badFailureRisk));
	float floatOffset = 7 - floatScale * artanh(1 - 2 * trialSelected.medFailureRisk);

	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		arrRelative[intIter] = arrScores[intIter][0] / floatSum - 1;
		if (intFailureModel == 1)
		{
			if (arrRelative[intIter] < 0)
			{
				arrFailure[intIter] = trialSelected.medFailureRisk - arrRelative[intIter] * (1 - trialSelected.medFailureRisk);
			}
			else if (arrRelative[intIter] > 0)
			{
				arrFailure[intIter] = trialSelected.medFailureRisk - arrRelative[intIter] * trialSelected.medFailureRisk;
			}
			else
			{
				arrFailure[intIter] = trialSelected.medFailureRisk;
			}
		}
		else if (intFailureModel == 2)
		{
			// cout << arrScores[intIter][0] << '\n' << floatOffset << '\n' << floatScale << '\n' << tanh((arrScores[intIter][0]-floatOffset)/floatScale) << '\n';
			arrFailure[intIter] = (1 - tanh((arrScores[intIter][0] - floatOffset) / floatScale)) / 2;
		}
		else
		{
			arrFailure[intIter] = 0;
		}
		cout << "Team " << intIter << " has a failure probability of " << arrFailure[intIter] << '\n';
	}

	cout << "Ranking teams\n";
	float arrSort[8];
	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		arrSort[intIter] = arrScores[intIter][0];
	}
	bubbleSort(arrSort, false);
	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		for (int intRank = 0; intRank < intTeams; intRank = intRank + 1)
		{
			if (arrScores[intIter][0] == arrSort[intRank])
			{
				arrRanks[intIter] = intRank + 1;
				break;
			}
		}
	}

	bool boolEndangered;

	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		if (boolLimitFailures)
		{
			if (arrFailure[intIter] > 0 && arrRanks[intIter] > intMaxRank)
			{
				boolEndangered = true;
			}
			else
			{
				boolEndangered = false;
			}
		}
		else
		{
			boolEndangered = true;
		}

		float floatMin = 0;
		float floatMax = 0;
		float arrLeadership[8] = { 0,0,0,0,0,0,0,0 };
		floatSum = 0;
		for (int intHero = 0; intHero < intHeroesPerTeam; intHero = intHero + 1)
		{
			floatSum = floatSum + arrTeams[intIter][intHero].skills[4];
			floatMax = (floatMax < arrTeams[intIter][intHero].skills[11]) ? arrTeams[intIter][intHero].skills[11] : floatMax;
			floatMin = (floatMin > arrTeams[intIter][intHero].skills[11]) ? arrTeams[intIter][intHero].skills[11] : floatMin;
			arrLeadership[intHero] = arrTeams[intIter][intHero].skills[11];
		}
		floatSum = floatSum / intHeroesPerTeam;
		bubbleSort(arrLeadership, false);
		int intMedian;
		intMedian = int(intHeroesPerTeam * 0.49);
		if (floatMin >= 7 && arrLeadership[intMedian] > 8)
		{
			floatMax = arrLeadership[intMedian];
		}
		cout << "Simulating challenge for team " << intIter << '\n';
		if (refusal(arrScores[intIter][2], floatSum, floatMax))
		{

			arrStates[intIter] = outcomes(arrFailure[intIter], boolEndangered, trialSelected.race, arrRanks[intIter] > intLastPlace, trialSelected.failureDescription);
		}
		else
		{
			arrStates[intIter] = "REFUSED";
		}

		cout << "Team " << intIter << " outcome: " << arrStates[intIter] << '\n';
		arrGold[intIter] = rings(trialSelected, arrStates[intIter], arrRanks[intIter], arrRelative[intIter]);
		cout << "Team " << intIter << " win " << arrGold[intIter] << " rings\n";
	}
	// Relegating failed teams in ranks
	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		if (arrStates[intIter] == "LOSE" || arrStates[intIter] == "REFUSED")
		{
			for (int intSubIter = 0; intSubIter < intTeams; intSubIter = intSubIter + 1)
			{
				if (arrRanks[intSubIter] > arrRanks[intIter])
				{
					arrRanks[intSubIter] = arrRanks[intSubIter] - 1;
				}
			}
			arrRanks[intIter] = intTeams;
		}
	}
	if (!trialSelected.race && trialSelected.optionalGold)
	{
		float arrGoldSort[8];
		for (int intIter = 0; intIter < 8; intIter = intIter + 1)
		{
			arrGoldSort[intIter] = float(arrGold[intIter]);
		}
		bubbleSort(arrGoldSort, false);
		for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
		{
			arrGold[intIter] = int(arrGoldSort[arrRanks[intIter] - 1]);
		}
	}

	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		arrTimes[intIter] = raceTime(trialSelected, arrScores[intIter][0]);
		cout << "Team " << intIter << " time: " << arrTimes[intIter] << '\n';
	}

	cout << trialSelected.name << '\n';
	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < intHeroesPerTeam; intSubIter = intSubIter + 1)
		{
			cout << arrTeams[intIter][intSubIter].name << ' ';
		}
		cout << '\n';
		cout << "OUTCOME: " << arrStates[intIter] << '\n';
		cout << "RINGS: " << arrGold[intIter] << '\n';
		cout << "TIME: " << arrTimes[intIter] << "\n\n";
	}
}


wstring strConvert;

LPCTSTR strToLPCTSTR(string strInput)
{
	//string strInput = "Ascent";
	wstring strTemp(strInput.begin(), strInput.end());
	strConvert = strTemp;
	LPCTSTR strOut = strConvert.c_str();
	return strOut;
}

string CStringToStr(CString strInput)
{
	CT2CA strInt(strInput);
	string strConv(strInt);
	return strConv;
}

void writeToCSV(string* pointFirstElement, int intColumns, int intRows, string strFilename, char charDelim = ',')
{
	// pointFirstElement = Pointer for the first element of the array
	// intColumns		 = Number of columns in csv
	// intRows			 = Number of rows in csv
	// strFilename       = Filename of the .csv file to be written
	// charDelim         = CSV Delimiter

	string strOut = ""; // String to record output
	for (int intIter = 0; intIter < intRows; intIter = intIter + 1) // For each row
	{
		for (int intSubIter = 0; intSubIter < intColumns; intSubIter = intSubIter + 1) // For each column
		{
			strOut = strOut + *(pointFirstElement + intColumns * intIter + intSubIter); // Add the cell value
			if (intSubIter != intColumns - 1) // If not the last column
			{
				strOut = strOut + charDelim; // Add the delimiter
			}
		}
		if (intIter != intRows - 1) // If not the last row
		{
			strOut = strOut + '\n'; // Start a new row
		}
	}

	ofstream csvFile;
	csvFile.open(strFilename); // Open the file to be written
	csvFile << strOut; // Write to file
	csvFile.close(); // Close the file
}

void writeToTxt(string strOut, string strFilename)
{
	ofstream txtOut;
	txtOut.open(strFilename); // Open the file to be written
	txtOut << strOut; // Write to file
	txtOut.close(); // Close the file
}

void updateScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO sInfo;
	pScrollBar->GetScrollInfo(&sInfo, SIF_ALL);

	int intPos = pScrollBar->GetScrollPos();
	switch (nSBCode)
	{
	case SB_LEFT: intPos = sInfo.nMin; break; // max left/up
	case SB_RIGHT: intPos = sInfo.nMax; break; // max right/down
	case SB_ENDSCROLL: break; // end scroll
	case SB_LINELEFT: if (intPos > sInfo.nMin) { intPos = intPos - 1; } break; // scroll left/up
	case SB_LINERIGHT: if (intPos < sInfo.nMax) { intPos = intPos + 1; } break; // scroll right/down)
	case SB_PAGELEFT: if (intPos > sInfo.nMin) { intPos = intPos - sInfo.nPage > sInfo.nMin ? intPos - sInfo.nPage : sInfo.nMin; } break; // page left/up)
	case SB_PAGERIGHT: if (intPos < sInfo.nMax) { intPos = intPos + sInfo.nPage < sInfo.nMax ? intPos + sInfo.nPage : sInfo.nMax; } break; // page left/up)
	case SB_THUMBPOSITION: case SB_THUMBTRACK: intPos = nPos; break; // drag
	}
	pScrollBar->SetScrollPos(intPos);
}