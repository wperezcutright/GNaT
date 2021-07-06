#pragma once
#include <random>
#include <string>
#include <ctime>

class CardSource
{
public:

	struct Card {
		std::string type;
	};

	CardSource();
	~CardSource();
	void fill(Card);
	Card drawCard();
	bool isEmpty();
	
private:
	Card list[30];
	int currentSize;
};

