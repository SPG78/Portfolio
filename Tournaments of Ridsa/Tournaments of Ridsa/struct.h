#pragma once

#include <string>

using namespace std;

#define INT_MAXHEROES	64
#define INT_MAXTRIALS 82
#define INT_HEROWIDTH 700
#define INT_HEROHEIGHT 400
#define INT_TRIALWIDTH 700
#define INT_TRIALHEIGHT 700
#define INT_SIMWIDTH 700
#define INT_SIMHEIGHT 500


struct hero {
	string name;
	string emblem;
	float age;
	float skills[12];
	float fears[7];
	int gold;
};

struct trial {
	string name;
	int numberOfTeams;
	int heroesPerTeam;
	bool race;
	bool losingCosts;
	bool notFirstCosts;
	float skills[12];
	float fears[7];
	float luck;
	string maximumFailures; // Probably not appropriate data type, needs changing
	float medFailureRisk;
	float badFailureRisk;
	string failureDescription[2];
	int rewards[6];
	int rewardOptions;
	int winnerBonus;
	bool optionalGold;
	int optionalGoldBias;
	bool loserKeepsReward;
	int time[4];
};