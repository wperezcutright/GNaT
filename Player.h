#pragma once
#include "ProbSet.h"
#include "CardSet.h"
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <fstream>

class Player {
public:

	struct Attack {
		std::string atkType;
		int atkValue;
	};

	Player(int, int, int, CardSource*, std::string, std::string, bool);
	~Player();
	int getHealth(); // get functions - their names are pretty self-explanatory
	int getBank();
	std::string getName();
	int getHandSize();
	bool checkIsAI(); // tells whether Player is an AI or not
	void setHealth(int); // for modifying health after setup
	void setOpponent(Player*); // notifies Player of an opponent Player
	void toFeed(std::string); // sends string to fout
	void printStatus(); // prints pertinent information
	int roll(); // rolls using the ProbSet
	void drawCard(); // take a card from the pile
	Attack visitShop(); // opportunity to purchase attacks
	std::string playCard(int); // give a type to an attack
	void takeDamage(Attack); // mechanism for taking damage from other player's attacks
	void changeDefType(); // player must change def type
	void upBank(); // gain currency
	void upProb(); // change probability, pass in probability
	void select(); // player chooses option
	std::string returnDisVantageType(std::string);
	std::string returnAdvantageType(std::string);
	// AI methods

	void setOppDefType(std::string);
	char purchaseAtkDecision(); // these all make decisions based on logic given for each AItype
	char playCardDecision(int);
	int defTypeDecision();
	int selectActionDecision();
	int determineGameState();

	// the method for probDecision is in the ProbSet class for programming convenience
	

private:
	bool isAI; // whether or not the player is an AI
	std::string AItype; // the type (name) of this particular AI
	ProbSet playerProbSet;
	int bank;
	int health;
	int id;
	std::string defType;
	CardSet hand;
	std::string playerName;
	Player* opponent;
	CardSource* drawDeck;
	std::ofstream fout; // output file stream

	// AI-specific variables
	std::string opponentDefType; // based on guesses
	bool defTypeKnown;
	int gameState;
	int stateWeights[6];
};