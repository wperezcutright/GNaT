// mygame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "player.h"
#include "windows.h"
#include <limits>
#include <iostream>
#include <chrono>
#undef max;

using std::getline;
using std::cin;
using std::string;
using std::cout;
using std::endl;
using std::numeric_limits;
using std::max;
using std::streamsize;

int whoStarts();
bool checkWin(Player*, Player*, int, int);
bool statPrintOut(Player*, Player*, int, int);

int main()
{
    // the game will run until win condition

	// alternate between turns

	// of what does a turn consist?
	// so on a turn: metaphorical die is rolled. Outcome is determined and player is directed to make choices as necessary.
	// Turn then ends.

	// How about you start with a die roll that determines your action. Your actions can then change the
	// probability of each option
	// These actions can include: gaining a card, gaining currency, gaining an opportunity to spend currency,
	// or having the choice of any of the other actions

	// Cards will be drawn from a deck limited in size and quantity. 

	// The primary objective of the game will be winning combat, i.e. reducing the health of the opponent to zero.

	// Given the above, what procedures will I need? The main engine for the game mechanics will be the probability feature.
	// It would be very helpful if I could give the probability distribution to the procedure and have it output a single
	// result. 

	// Keep in mind the game needs to be fairly simple. By that I mean the number of choices made by the player should
	// be limited - otherwise there will be too many variable factors to evaluate.

	// What cards should there be? How effective should they be, and how should they relate to other game components? If
	// I have a deck of thirty cards, how are the various ones distributed?

	// 16.6% (5) change defense type to rock,  16.6% (5) change defense type to paper,
	// 16.6% (5) change defense type to scissors, 16.6% (5) change attack type to rock, 
	// 16.6% (5) change attack type to paper, 16.6% (5) change attack type to scissors
	
	// How should the start-first advantage be neutralized? One way is to simply limit the number of rounds and determine
	// who has more health. However, since actions of one player nearly directly affect the other, it seems like this would not 
	// make the difference. One way that might make it more even would be giving an extra card to the defending player (this 
	// would be more or less effective depending on how many cards each is given to start). Alternatives include giving
	// the defender more health, giving probability points to the defender (involves choice), or making their first roll
	// a guaranteed "select an option".

	// Initial probability distributions:
	// Mayhaps it would be good to 2 to every option but "select an option" (1). It just seems too strong from a conceptual
	// view. Additionally, two probability points could be required to increase its value by one.

	// Where are the decision points?
	// 1. If "select an option" is rolled. (aggressor)
	// 2. If "add probability points" is rolled. (aggressor)
	// 3. & 4. If "attack" is rolled (first decision is which attack, second is whether to use cards). (aggressor)
	// 5. If "attack" is rolled (play defense change card). (defender)

	// That should be it if we are making defense type changes stochastic. None of these having to do with rock-paper-scissors
	// deals with hidden information.

	// Keep in mind that you're going to have to distinguish between who can see what on their feed. Additionally, I
	// will need a log of moves made.

	bool error = false;
	bool goodPlayerTypeInput = false;
	bool isBot = false;
	int firstPlayer = whoStarts(); // randomly determine who starts
	int currentTurn = firstPlayer;
	int numberOfRounds = 0; // for keeping track of rounds played
	int playerTypesInput;
	string playerName;
	string CPUtype = "";

	srand(time(0)); // seeds rand() using time

	CardSource::Card rockType; // for instantiating deck
	rockType.type = "rock";
	CardSource::Card paperType;
	paperType.type = "paper";
	CardSource::Card scissorsType;
	scissorsType.type = "scissors";

	CardSource theDeck;
	CardSource * deckPointer = &theDeck;

	Player dummy(0, 0, 0, deckPointer, "dummyman", "null", false); // dummy, draws no cards
	Player* currentPlayer = &dummy; // define player dummies (for keeping track of turns)
	Player* otherPlayer = &dummy;

	for (int i = 0; i < 10; i++)
	{
		theDeck.fill(rockType);
		theDeck.fill(paperType);
		theDeck.fill(scissorsType);
	}

	cout << "Greetings. Welcome to the game." << endl << endl;

	cout << "Do you want to play against the computer, or do you want to pit two AIs against each other?" << endl << endl;

	cout << "Press 1 to face the computer" << endl;
	cout << "Press 2 for an AI showdown (the match summary will be autogenerated in \"gamestats.txt\")" << endl << endl;

	while (!goodPlayerTypeInput) // this is the loop for validating input for selection of the types of players
	{
		if (std::cin >> playerTypesInput)
		{
			if (playerTypesInput > 0 && playerTypesInput < 3)
			{
				goodPlayerTypeInput = true;
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

	if (playerTypesInput == 1)
	{
		cout << "Human player: What is your name?" << endl;
		std::cin.clear();
		std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, playerName); // get the player's name
		CPUtype = "null";
		cout << endl << endl;
	}
	else
	{
		CPUtype = "type1";
		isBot = true;
		playerName = "CPU0";
	}
	
	cout << endl << "Getting started..." << endl << endl;

	Player player1(0, rand(), 5, deckPointer, playerName, CPUtype, isBot);
	Sleep(2600); // causes program to wait for at least 2.6 seconds
	Player player2(1, rand(), 5, deckPointer, "CPU1", "type2", true);

	Player* player1ptr = &player1;
	Player* player2ptr = &player2;

	// give compensation to player who plays last

	if (currentTurn == 0)
	{
		player2ptr->setHealth(player1ptr->getHealth() + 100);
	}
	else
	{
		player1ptr->setHealth(player1ptr->getHealth() + 100);
	}

	player1.setOpponent(player2ptr); // allows each player to know their opponent
	player2.setOpponent(player1ptr);

	while (checkWin(player1ptr, player2ptr, numberOfRounds, firstPlayer) == false && error == false)
	{
		// determine which player will take a turn

		if (currentTurn == 0)
		{
			currentPlayer = player1ptr;
			otherPlayer = player2ptr;
			currentTurn = 1; // changes turn; remove if currentTurn is ever used elsewhere
			cout << endl << endl << "<<<<<<<<<<<<<<<<<<<<IT IS " << currentPlayer->getName() << "'S" << " TURN>>>>>>>>>>>>>>>>>>>>" 
				<< endl << endl << endl << endl;
			currentPlayer->toFeed("\n\n<<<<<<<<<<<<<<<<<<<<IT IS " + currentPlayer->getName() + "'S" + " TURN>>>>>>>>>>>>>>>>>>>>\n\n");
			otherPlayer->toFeed("\n\n<<<<<<<<<<<<<<<<<<<<IT IS " + currentPlayer->getName() + "'S" + " TURN>>>>>>>>>>>>>>>>>>>>\n\n");

		}
		else if (currentTurn == 1)
		{
			currentPlayer = player2ptr;
			otherPlayer = player1ptr;
			currentTurn = 0; // changes turn; remove if currentTurn is ever used elsewhere
			cout << endl << endl << ">>>>>>>>>>>>>>>>>>>>IT IS " << currentPlayer->getName() << "'S" << " TURN<<<<<<<<<<<<<<<<<<<<" 
				<< endl << endl << endl << endl;
			currentPlayer->toFeed("\n\n>>>>>>>>>>>>>>>>>>>>IT IS " + currentPlayer->getName() + "'S" + " TURN<<<<<<<<<<<<<<<<<<<<\n\n");
			otherPlayer->toFeed("\n\n>>>>>>>>>>>>>>>>>>>>IT IS " + currentPlayer->getName() + "'S" + " TURN<<<<<<<<<<<<<<<<<<<<\n\n");
		}
		else
		{
			error = true;
			cout << "Don't know how it happened but apparently the turn system is messed up." << endl << endl;
		}

		// turn begins here

		currentPlayer->printStatus();
		if (playerTypesInput == 1)
		{
			Sleep(2670); // pauses for 2 seconds if human is playing
		}
		else
		{
			Sleep(1270);
		}

		if (error == false)
		{
			switch (currentPlayer->roll())
			{
			case 0: currentPlayer->toFeed(currentPlayer->getName() + " gets to draw a card ");
				otherPlayer->toFeed(currentPlayer->getName() + " gets to draw a card ");
				cout << currentPlayer->getName() << " gets to draw a card" << endl << endl;
				currentPlayer->drawCard(); // take a card from the pile
				break;
			case 1: if (currentPlayer->getBank() >= 100) // if the player has enough currency
					{ 
				currentPlayer->toFeed(currentPlayer->getName() + " gets to visit the shop ");
					otherPlayer->toFeed(currentPlayer->getName() + " gets to visit the shop ");
					cout << currentPlayer->getName() << " gets to visit the shop" << endl << endl;
					otherPlayer->takeDamage(currentPlayer->visitShop()); // opportunity to purchase attacks
					}
					else
					{
				currentPlayer->toFeed("Not enough money to make a purchase, though...");
					otherPlayer->toFeed("Not enough money to make a purchase, though...");
						cout << "Not enough money to make a purchase, though..." << endl << endl;
					}
				break;
			case 2: currentPlayer->toFeed(currentPlayer->getName() + " has to change their defense type ");
				otherPlayer->toFeed(currentPlayer->getName() + " has to change their defense type ");
				cout << currentPlayer->getName() << " has to change their defense type" << endl << endl;
				currentPlayer->changeDefType(); // player must change def type
				break;
			case 3: currentPlayer->toFeed(currentPlayer->getName() + " gets some money ");
				otherPlayer->toFeed(currentPlayer->getName() + " gets some money ");
				cout << currentPlayer->getName() << " gets some money" << endl << endl;
				currentPlayer->upBank(); // gain currency
				break;
			case 4: currentPlayer->toFeed(currentPlayer->getName() + " gets to change their probabilities ");
				otherPlayer->toFeed(currentPlayer->getName() + " gets to change their probabilities ");
				cout << currentPlayer->getName() << " gets to change their probabilities" << endl << endl;
				currentPlayer->upProb(); // change probability
				break;
			case 5: currentPlayer->toFeed(currentPlayer->getName() + " gets to choose an option this turn ");
				otherPlayer->toFeed(currentPlayer->getName() + " gets to choose an option this turn ");
				cout << currentPlayer->getName() << " gets to choose an option this turn" << endl << endl;
				currentPlayer->select(); // player chooses option
				break;
			default: error = true;
				cout << "Error: Somehow a number not in the range 0-5 was rolled" << endl << endl;
			}
		}
		numberOfRounds++; // count the round that was just played in the total number of rounds
	}
	system("pause");
	return 0;
}

int whoStarts() // randomly determine who starts
{
	std::default_random_engine generator((unsigned int)time(0));
	std::uniform_int_distribution<int> distribution(0, 1);

	return distribution(generator); // generate random int
}

bool checkWin(Player* p1ptrpm, Player* p2ptrpm, int noRoundspm, int starterpm)
{
	bool win = false;
	if (p1ptrpm->getHealth() <= 0)
	{
		win = true;
		cout << p2ptrpm->getName() << " won!" << endl << endl;
	}
	else if (p2ptrpm->getHealth() <= 0)
	{
		win = true;
		cout << p1ptrpm->getName() << " won!" << endl << endl;
	}
	if (win)
	{
		if (statPrintOut(p1ptrpm, p2ptrpm, noRoundspm, starterpm))
		{
			cout << "File print was good!" << endl << endl;
		}
		else
		{
			cout << "File print not complete." << endl << endl;
		}
	}

	return win;
}

bool statPrintOut(Player* p1ptrpm, Player* p2ptrpm, int noRoundspm, int starterpm)
{
	// auto end = std::chrono::system_clock::now();

	std::ofstream statsFile("gamestats.txt");
	bool success = statsFile.is_open();

	// std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	if (success)
	{
		statsFile << "Game stats" << endl;
		if (starterpm == 0)
		{
			statsFile << "Player 1 started." << endl;
		}
		else
		{
			statsFile << "Player 2 started." << endl;
		}
		// statsFile << "Game ended at " << std::ctime(&end_time) << endl;
		statsFile << "Number of turns played: " << noRoundspm << endl;
		statsFile << "Player 1 (" << p1ptrpm->getName() << ") stats:" << endl;
		statsFile << '\t' << "health: " << p1ptrpm->getHealth() << endl;
		statsFile << '\t' << "currency: " << p1ptrpm->getBank() << endl;
		statsFile << '\t' << "hand size: " << p1ptrpm->getHandSize() << endl << endl;

		statsFile << "Player 2 (" << p2ptrpm->getName() << ") stats:" << endl;
		statsFile << '\t' << "health: " << p2ptrpm->getHealth() << endl;
		statsFile << '\t' << "currency: " << p2ptrpm->getBank() << endl;
		statsFile << '\t' << "hand size: " << p2ptrpm->getHandSize() << endl;
		statsFile.close();
	}
	return success;
}

// ideas scrapped for complexity:
// 16.6% (5) select, 16.6% (5) roll again, increase value of attacks, increase efficacy of attacks, increase efficacy of
// defense
// Rarer cards will have more unique abilities. Actions
// can have probabilities attached for calculating efficacy.
// gaining an opportunity to use expenditures
// reduce probability points if draw pile is empty

