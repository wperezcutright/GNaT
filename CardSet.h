#pragma once
#include "CardSource.h"
#include <iostream>
#include <fstream>

class CardSet
{
public:
	CardSet(CardSource*, int);
	~CardSet();
	int getCurrentSize();
	std::string getCardType(int);
	bool drawCard();
	bool checkExistence(int);
	bool hasCard(std::string, int&, int&);
	int mostPopulous();
	std::string playCard(int);
	void printConsole();
	void printFeed(std::ofstream&);

private:
	CardSource* source;
	CardSource::Card cardList[30];
	int currentSize;
};

