#include "pch.h"
#include "CardSource.h"


CardSource::CardSource()
{
	currentSize = 0;
}


CardSource::~CardSource()
{
}

void CardSource::fill(Card cardToAdd)
{
	list[currentSize] = cardToAdd;
	currentSize++;
}

CardSource::Card CardSource::drawCard() // randomly disperses a card from this source
{
	Card temp;
	std::default_random_engine generator((unsigned int)time(0));
	std::uniform_int_distribution<int> distribution(0, (currentSize - 1));

	int myRandomInt = distribution(generator); // generate random int

	temp = list[myRandomInt]; // copy card before removing it

	for (int i = myRandomInt + 1; i < currentSize; i++) // copy over the slot for the card being "removed"
	{
		list[i - 1] = list[i];
	}

	currentSize--; // decrement after removing card

	return temp;
}

bool CardSource::isEmpty() // tells the caller whether or not there are cards in the card source
{
	bool empty = false;

	if (currentSize == 0)
	{
		empty = true;
	}

	return empty;
}