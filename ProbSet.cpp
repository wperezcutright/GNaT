#include "pch.h"
#include "ProbSet.h"
#include <random>
#include <iostream>
#include <limits>
#include <ctime>

using std::cout;
using std::endl;
using std::numeric_limits;
using std::max;
using std::streamsize;

ProbSet::ProbSet()
{
	for (int i = 0; i < 4; i++) // initializes statList to have 2 for every stat bar "select"
	{
		statList[i] = 6;
	}
	statList[4] = 4;
	statList[5] = 2;
}


ProbSet::~ProbSet()
{
}

int ProbSet::getTotalStats()
{
	int statTotal = 0;
	for (int i = 0; i < 6; i++)
	{
		statTotal += statList[i];
	}
	return statTotal;
}

int ProbSet::roll() // determines what the user's action will be (based on probabilities in statList)
{
	int result;

	std::cout << "ROLLING FOR ACTION..." << std::endl << std::endl;

	std::default_random_engine generator((unsigned int)time(0));
	std::uniform_int_distribution<int> distribution(1, getTotalStats());

	int myRandomInt = distribution(generator); // generate random int

	// this next "if-else" construct determines the result

	if (myRandomInt <= statList[0])
	{
		result = 0;
	}
	else if (myRandomInt <= statList[0] + statList[1])
	{
		result = 1;
	}
	else if (myRandomInt <= statList[0] + statList[1] + statList[2])
	{
		result = 2;
	}
	else if (myRandomInt <= statList[0] + statList[1] + statList[2] + statList[3])
	{
		result = 3;
	}
	else if (myRandomInt <= statList[0] + statList[1] + statList[2] + statList[3] + statList[4])
	{
		result = 4;
	}
	else if (myRandomInt <= getTotalStats())
	{
		result = 5;
	}
	else
	{
		cout << "error: random roll machine broke :(" << endl << endl;
		result = 99; // I guess this can be used as a check
	}
	return result;
}

void ProbSet::addPoint(bool isAIpm, std::ofstream& fout, int statepm, std::string aiTypepm) // boosts one stat in statList
{
	bool goodProbInput = false;
	int pointSelection;

	if (isAIpm)
	{
		fout << "You get to increase the likelihood that you will roll a particular action! To which do you choose to add?"
			<< endl << "Pick the probability to increase by pressing its corresponding number." <<
			"(Note: Probability will only increase at half the rate for items 5 and 6)." << endl << endl;
		printFeed(fout);
	}
	else
	{
		cout << "You get to increase the likelihood that you will roll a particular action! To which do you choose to add?"
			<< endl << "Pick the probability to increase by pressing its corresponding number." <<
			"(Note: Probability will only increase at half the rate for items 5 and 6)." << endl << endl;
		printConsole();
	}

	if (isAIpm)
	{
		pointSelection = probDecision(statepm, aiTypepm);
	}
	else
	{
		while (!goodProbInput) // this is the loop for validating input for selection of the stat to increase
		{
			if (std::cin >> pointSelection)
			{
				if (pointSelection > 0 && pointSelection < 7)
				{
					goodProbInput = true;
				}
				else
				{
					cout << "That's not an option, sorry." << endl << endl;
				}
			}
			else
			{
				cout << "Please enter a valid integer." << endl << endl;
				std::cin.clear();
				std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
	}

	pointSelection--; // decrement to avoid one-off error

	if (pointSelection != 5 && pointSelection != 4)
	{
		statList[pointSelection] += 2;
	}
	else 
	{
		statList[pointSelection] += 1;
	}
	if (isAIpm)
	{
		fout << "The stat has been updated. New value: " << statList[pointSelection] << endl << endl;
	}
	else 
	{
		cout << "The stat has been updated. New value: " << statList[pointSelection] << endl << endl;
	}
}

void ProbSet::printConsole() // displays current stat spread in statList
{
	cout << "1. Draw a card. (Current value: " << statList[0] << ')' << endl;
	cout << "2. Purchase an attack. (Current value: " << statList[1] << ')'  << endl;
	cout << "3. Change defense type. (Current value: " << statList[2] << ')' << endl;
	cout << "4. Increase amount of currency held. (Current value: " << statList[3] << ')' << endl;
	cout << "5. Increase probability of a certain action being rolled.  (Current value: " << statList[4] << ')' << endl;
	cout << "6. Select an action. (Current value: " << statList[5] << ')' << endl << endl;
}

void ProbSet::printFeed(std::ofstream &fout) // displays current stat spread in statList
{
	fout << "1. Draw a card. (Current value: " << statList[0] << ')' << endl;
	fout << "2. Purchase an attack. (Current value: " << statList[1] << ')' << endl;
	fout << "3. Change defense type. (Current value: " << statList[2] << ')' << endl;
	fout << "4. Increase amount of currency held. (Current value: " << statList[3] << ')' << endl;
	fout << "5. Increase probability of a certain action being rolled.  (Current value: " << statList[4] << ')' << endl;
	fout << "6. Select an action. (Current value: " << statList[5] << ')' << endl << endl;
}

int ProbSet::probDecision(int statepm, std::string aiTypepm) // determines logic for deciding probability increases
{
	std::default_random_engine generator((unsigned int)time(0));

	int response = -1;

	if (aiTypepm == "type1") // this always choose "select"
	{
		response = 6;
	}
	else if (aiTypepm == "type2") // this chooses between "select", "choose probability" (not weighted as high), and statepm
	{
		std::uniform_int_distribution<int> distribution(1, 5);
		int myRandomInt = distribution(generator); // generate random int
		if (myRandomInt == 1)
		{
			response = 5;
		}
		else if (myRandomInt == 2 || myRandomInt == 3)
		{
			response = 6;
		}
		else if (myRandomInt == 4 || myRandomInt == 5)
		{
			response = statepm;
		}
	}
	return response;
}