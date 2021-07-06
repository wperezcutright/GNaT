#pragma once
#include <fstream>
class ProbSet
{
public:
	ProbSet();
	~ProbSet();
	int getTotalStats();
	int roll();
	void addPoint(bool, std::ofstream&, int, std::string);
	void printConsole();
	void printFeed(std::ofstream&);

	// AI method



	int probDecision(int, std::string);

private:
	int statList[6];
};

