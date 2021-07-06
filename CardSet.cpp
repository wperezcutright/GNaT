#include "pch.h"
#include "CardSet.h"

using std::string;
using std::cout;
using std::endl;


CardSet::CardSet(CardSource* drawSourcepm, int initialHandSizepm)
{
	source = drawSourcepm;
	currentSize = 0;

	for (int i = 0; i < initialHandSizepm; i++) // initialize first set of cards (according to given hand size)
	{
		this->drawCard();
	}

}


CardSet::~CardSet()
{
}

int CardSet::getCurrentSize() // returns the size of the array
{
	return currentSize;
}

std::string CardSet::getCardType(int indexpm)
{
	return cardList[indexpm].type;
}

bool CardSet::drawCard() // draws single card from source (if not empty) and puts it in the list
{
	bool successfulDraw = false;

	if (!source->isEmpty()) // if there are cards in the source draw pile
	{
		cardList[currentSize] = source->drawCard();;  // puts card in list
		currentSize++; // update size of list
		successfulDraw = true;
	}
	return successfulDraw;
}

bool CardSet::checkExistence(int selectedCardpm) // determines whether a card exists in the set of cards
{
	bool cardExists = false;

	if (selectedCardpm > 0 && selectedCardpm <= currentSize)
	{
		cardExists = true;
	}

	return cardExists;
}

bool CardSet::hasCard(std::string typepm, int& indexpm, int& quantitypm) // tells whether type of card exists, as well
{																		 // as where in the hand the last of that type
	bool success = false;												 // resides and how many there are
	quantitypm = 0;

	for (int i = 0; i < currentSize; i++)
	{
		if (cardList[i].type == typepm)
		{
			quantitypm++;
			indexpm = i;
			success = true;
		}
	}
	return success;
}

int CardSet::mostPopulous() // returns index of most popular card
{
	int rockCounter = 0;
	int paperCounter = 0;
	int scissorsCounter = 0;
	int index = -1;
	int rockIndex;
	int paperIndex;
	int scissorsIndex;
		
	for (int i = 0; i < currentSize; i++)
		{
			if (cardList[i].type == "rock")
			{
				rockCounter++;
				rockIndex = i;
			}
			else if (cardList[i].type == "paper")
			{
				paperCounter++;
				paperIndex = i;
			}
			else
			{
				scissorsCounter++;
				scissorsIndex = i;
			}
		}
	if (rockCounter >= paperCounter && rockCounter >= scissorsCounter)
	{
		index = rockIndex;
	}
	else if (paperCounter >= rockCounter && paperCounter >= scissorsCounter)
	{
		index = paperIndex;
	}
	else
	{
		index = scissorsIndex;
	}

	return index;
}

string CardSet::playCard(int whichCardpm) // removes card from hand, sending its value to the function caller
{
	string cardType;
	whichCardpm--; // prepares for use as index, avoiding off-by-one errors 

	cardType = cardList[whichCardpm].type;

	for (int i = whichCardpm + 1; i < currentSize; i++)
	{
		cardList[i - 1] = cardList[i];
	}

	currentSize--;

	return cardType;
}

void CardSet::printConsole() // displays all the cards in the set of cards
{
	if (currentSize != 0)
	{
		cout << "These are the cards in your hand: " << endl << endl;

		for (int i = 0; i < currentSize; i++)
		{
			cout << i + 1 << ". " << cardList[i].type << endl;
		}
		cout << endl;
	}
	else
	{
		cout << "Your hand is empty!" << endl << endl;
	}
}

void CardSet::printFeed(std::ofstream& fout) // displays all the cards in the set of cards
{
	if (currentSize != 0)
	{
		fout << "These are the cards in your hand: " << endl << endl;

		for (int i = 0; i < currentSize; i++)
		{
			fout << i + 1 << ". " << cardList[i].type << endl;
		}
		fout << endl;
	}
	else
	{
		fout << "Your hand is empty!" << endl << endl;
	}
}