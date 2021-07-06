#include "pch.h"
#include "Player.h"


using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::numeric_limits;
using std::max;
using std::streamsize;

Player::Player(int idpm, int randSeed, int handSize, CardSource* deckpm, std::string namepm, std::string AItypepm, bool isAIpm) 
: fout("feed" + std::to_string(idpm) + ".txt"), hand(deckpm, handSize)
{
	isAI = isAIpm;
	id = idpm;
	bank = 100;
	health = 1300;
	playerName = namepm;
	AItype = AItypepm;
	drawDeck = deckpm;
	opponentDefType = "unknown";
	defTypeKnown = false;
	time_t randSeedt = randSeed; // random seed passed in from main
	time_t* randSeedtptr = &randSeedt;


	srand(time(randSeedtptr)); // randomly determine what deftype the user is
	int type = rand() % 3;

	switch (type)
	{
	case 0: defType = "rock";
		break;
	case 1: defType = "paper";
		break;
	case 2: defType = "scissors";
		break;
	default: std::cout << "Error assigning defType during player construction." << endl << endl;
	}
	
	bool noFileError = fout.is_open();
	if (!noFileError)
	{
		cout << "There was an issue with opening the file for " << playerName << endl << endl;
	}

	stateWeights[0] = 3; // RELATIVE DESIRE TO GET CARDS
	stateWeights[1] = 5; // RELATIVE DESIRE TO ATTACK
	stateWeights[2] = 0; // RELATIVE DESIRE TO CHANGE DEFENSE TYPE
	stateWeights[3] = 5; // RELATIVE DESIRE TO GET MONEY
	stateWeights[4] = 3; // RELATIVE DESIRE TO  CHANGE PROBABILITIES
}

Player::~Player()
{
}

int Player::getHealth() // gets remaining health
{
	return health;
}

int Player::getBank() // gets amount of currency held by player
{
	return bank;
}

std::string Player::getName()
{
	return playerName;
}

int Player::getHandSize()
{
	return hand.getCurrentSize();
}

bool Player::checkIsAI()
{
	return isAI;
}

void Player::setHealth(int healthpm)
{
	health = healthpm;
}

void Player::setOpponent(Player* opponentpm)
{
	opponent = opponentpm;
}

void Player::toFeed(std::string output)
{
	fout << output << endl << endl;
}

void Player::printStatus() // prints information available to player
{
	if (!isAI)
	{
		cout << "Status of " << playerName << ':' << endl << endl;
		cout << "Remaining health: " << health << endl;
		cout << "Available currency: " << bank << endl;
		cout << "Defense type: " << defType << endl;
		cout << "Cards in hand: " << endl;
		hand.printConsole();

		// print available information about opponent
		cout << "Opponent status: " << endl;
		cout << "Remaining health: " << opponent->getHealth() << endl;
		cout << "Available currency: " << opponent->getBank() << endl;
		cout << "Number of cards in hand: " << opponent->getHandSize() << endl << endl;
	}
	else
	{
		fout << "Status of " << playerName << ':' << endl << endl;
		fout << "Remaining health: " << health << endl;
		fout << "Available currency: " << bank << endl;
		fout << "Defense type: " << defType << endl;
		fout << "Cards in hand: " << endl;
		hand.printFeed(fout);

		// print available information about opponent
		fout << "Opponent status: " << endl;
		fout << "Remaining health: " << opponent->getHealth() << endl;
		fout << "Available currency: " << opponent->getBank() << endl;
		fout << "Number of cards in hand: " << opponent->getHandSize() << endl << endl;
	}
}

int Player::roll() // calls the player's ProbSet to roll, then returns the result
{
	fout << "ROLLING FOR ACTION..." << endl << endl;
	return playerProbSet.roll();
}

 void Player::drawCard() // calls the player's CardSet to add on another card
{
	hand.drawCard();
}

 Player::Attack Player::visitShop() // Sizeable function that both offers the player a chance to play an attack
{								   // (given they have enough funds) and play a card with it (provided they have one)
	char atkSelection; // variables for user input
	char cardSelection;
	int atkPower = 0;
	int cardSelectionNo;
	string playedType = "null";
	bool atkPurchased = false;
	bool goodInput = false;
	bool goodInput2 = false;

	Attack fullAtk;
	CardSource::Card tempCard;

	// The more you pay, the more you can earn. The prime strategy I can see is just purchasing the most valuable one

	do
	{
		if (!isAI) // console output
		{
			cout << "Welcome to the weapon shop. What would you like to purchase?" << endl << endl;

			cout << "100 power (costs 100 coins): press 1" << endl;
			cout << "250 power (costs 200 coins): press 2" << endl;
			cout << "400 power (costs 300 coins): press 3" << endl;
			cout << "Leave the shop: press q" << endl << endl;
		}
		else
		{
			fout << "Welcome to the weapon shop. What would you like to purchase?" << endl << endl;

			fout << "100 power (costs 100 coins): press 1" << endl;
			fout << "250 power (costs 200 coins): press 2" << endl;
			fout << "400 power (costs 300 coins): press 3" << endl;
			fout << "Leave the shop: press q" << endl << endl;
		}
		if (isAI)
		{
			atkSelection = purchaseAtkDecision(); // calls AI function
		}
		else
		{
			while (!goodInput) // this is the loop for validating input for selection at the shop
			{
				if (std::cin >> atkSelection)
				{
					if (atkSelection == '1' || atkSelection == '2' || atkSelection == '3' || atkSelection == 'q')
					{
						goodInput = true;
					}
					else
					{
						cout << "That's not an option, sorry." << endl << endl;
					}
				}
				else
				{
					cout << "Please enter a valid character" << endl << endl;
					std::cin.clear();
					std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
			}
		}
		if (atkSelection != 'q')
		{
			switch (atkSelection - '0')
			{
			case 1:
				if (bank >= 100)
				{
					atkPower = 100;
					bank -= 100;
					atkPurchased = true;
				}
				else
				{
					cout << endl << "Sorry, not enough money to make that purchase. You only have " << bank <<
						" coins." << endl << endl;
					atkSelection = 'q';
				}
				break;
			case 2:
				if (bank >= 200)
				{
					atkPower = 250;
					bank -= 200;
					atkPurchased = true;
				}
				else
				{
					cout << endl << "Sorry, not enough money to make that purchase. You only have " << bank <<
						" coins." << endl << endl;
					goodInput = false;
				}
				break;
			case 3:
				if (bank >= 300)
				{
					atkPower = 400;
					bank -= 300;
					atkPurchased = true;
				}
				else
				{
					cout << endl << "Sorry, not enough money to make that purchase. You only have " << bank <<
						" coins." << endl << endl;
					goodInput = false;
				}
				break;
			default: cout << endl << "Yeah something's broken" << endl << endl;
			}
		}
	} while (atkPurchased == false && atkSelection != 'q');

	if (atkPurchased) // offer user opportunity to play a card
	{
		if (!isAI)
		{
			cout << "You purchased an attack. Would you like to play a card to pair with your attack?" << endl;
			cout << "Press the corresponding number to select that card from your hand to play:" << endl << endl;
			hand.printConsole(); // print out what cards, if any, are in the player's hand
			cout << "Press q to continue without playing a card." << endl << endl;
		}
		else
		{
			fout << "You purchased an attack. Would you like to play a card to pair with your attack?" << endl;
			fout << "Press the corresponding number to select that card from your hand to play:" << endl << endl;
			hand.printFeed(fout); // print out what cards, if any, are in the player's hand
			fout << "Press q to continue without playing a card." << endl << endl;
		}
		
		if (isAI)
		{
			cardSelection = playCardDecision(atkPower); // calls AI logic for playing cards
			cardSelectionNo = cardSelection - '0';
		}
		else
		{
			while (!goodInput2)
			{ // this is the loop for validating input with respect to selection of a card
				if (std::cin >> cardSelection)
				{
					if (cardSelection == 'q') // if the user input a 'q' character
					{
						goodInput2 = true;
					}
					else
					{
						cardSelectionNo = cardSelection - '0'; // convert char input to int

						if (hand.checkExistence(cardSelectionNo)) // test whether selection corresponds to an existing card
						{
							goodInput2 = true;
						}
						else
						{
							cout << "That's not an option, sorry." << endl << endl;
						}
					}
				}
				else
				{
					cout << "Please enter a valid character" << endl;
					std::cin.clear();
					std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
			}
		}

		if (cardSelection != 'q')
		{
			playedType = playCard(cardSelectionNo); // determine the type of card played
			tempCard.type = playedType;
			drawDeck->fill(tempCard);	// return discarded card to draw pile
		}
		else
		{
			atkPower -= 50; // if no card is played, attack power still suffers from defense
		}
	}

	fullAtk.atkType = playedType;
	fullAtk.atkValue = atkPower;

	return fullAtk;
}

 void Player::takeDamage(Player::Attack atkpm) // function responsible for dealing damage to recipient of attack
{
	int dmg = atkpm.atkValue;
	bool atkVantage = false;
	bool atkDisVantage = false;

	// the following construct of nested if statements determines the efficacy of the attack

	if (atkpm.atkType != "null" && (atkpm.atkType != defType)) // as long as an attack type was played and the attacker's
	{														   // and defender's types are not the same
		if (atkpm.atkType == "rock")
		{
			if (defType == "paper") // attacker's disadvantage
			{
				dmg /= 2;
				atkDisVantage = true;
			}
			else if (defType == "scissors") // attacker's advantage
			{
				dmg *= 2;
				atkVantage = true;
			}
		}
		else if (atkpm.atkType == "paper")
		{
			if (defType == "rock") // attacker's advantage
			{
				dmg *= 2;
				atkVantage = true;
			}
			else if (defType == "scissors") // attacker's disadvantage
			{
				dmg /= 2;
				atkDisVantage = true;
			}
		}
		else if (atkpm.atkType == "scissors") 
		{
			if (defType == "rock") // attacker's disadvantage
			{
				dmg /= 2;
				atkDisVantage = true;
			}
			else if (defType == "paper") // attacker's advantage
			{
				dmg *= 2;
				atkVantage = true;
			}
		}
	}

	health -= dmg; // actually does damage and reduces health

	if (health < 0) // prevents a negative health value by replacing it with zero
	{
		health = 0;
	}

	if (dmg == 0) // if there was no atkPower and thus no damage done
	{
		if (opponent->checkIsAI())
		{
			opponent->toFeed("No attack takes place. " + playerName + " takes no damage and has " + std::to_string(health) + " health remaining.");
		}
		else
		{
		cout << "No attack takes place. " << playerName << " takes no damage and has " << health << " health remaining."
			<< endl << endl;
		}

		if (isAI)
		{
			fout << "No attack takes place. " << playerName << " takes no damage and has " << health << " health remaining."
				<< endl << endl;
		}
		else
		{
			cout << "No attack takes place. " << playerName << " takes no damage and has " << health << " health remaining."
				<< endl << endl;
		}
	}
	else if (dmg > 0) // this is responsible for determining whether to reveal the defense type to the attacker or not
	{
		if (atkVantage == true)	// attack has advantage, so reveal defense type
		{
			opponent->setOppDefType(defType);
			defTypeKnown = true;

			if (opponent->checkIsAI())
			{
				opponent->toFeed("The attack hits for " + std::to_string(dmg) + " damage as the attack was at an advantage against the defense type " +
					defType + ". ");
				opponent->toFeed(playerName + " now has " + std::to_string(health) + " health remaining.");
			}
			else
			{
				cout << "The attack hits for " << dmg << " damage as the attack was at an advantage against the defense type "
					<< defType << ". " << playerName << " now has " << health << " health remaining." << endl << endl;
			}

			if (isAI)
			{
				fout << "The attack hits for " << dmg << " damage as the attack was at an advantage against the defense type "
					<< defType << ". " << playerName << " now has " << health << " health remaining." << endl << endl;
			}
			else
			{
				cout << "The attack hits for " << dmg << " damage as the attack was at an advantage against the defense type "
					<< defType << ". " << playerName << " now has " << health << " health remaining." << endl << endl;
			}
		}
		else if (atkDisVantage == true) // attack has disadvantage, so reveal defense type
		{
			opponent->setOppDefType(defType);
			defTypeKnown = true;

			if (opponent->checkIsAI())
			{
				opponent->toFeed("The attack hits for " + std::to_string(dmg) + " damage as the attack was at a disadvantage against the defense type " +
					defType + ". ");
				opponent->toFeed(playerName + " now has " + std::to_string(health) + " health remaining.");
			}
			else
			{
				cout << "The attack hits for " << dmg << " damage as the attack was at a disadvantage against the defense type "
					<< defType << ". " << playerName << " now has " << health << " health remaining." << endl << endl;
			}

			if (isAI)
			{
				fout << "The attack hits for " << dmg << " damage as the attack was at a disadvantage against the defense type "
					<< defType << ". " << playerName << " now has " << health << " health remaining." << endl << endl;
			}
			else
			{
				cout << "The attack hits for " << dmg << " damage as the attack was at a disadvantage against the defense type "
					<< defType << ". " << playerName << " now has " << health << " health remaining." << endl << endl;
			}
		}
		else if (atkpm.atkType == "null") // attack has no type, defense type is not revealed
		{
			if (opponent->checkIsAI())
			{
				opponent->toFeed("The attack hits for " + std::to_string(dmg) + " . The attack had no associated type. ");
				opponent->toFeed(playerName + " now has " + std::to_string(health) + " health remaining.");
			}
			else
			{
				cout << "The attack hits for " << dmg << " damage. The attack had no associated type. "
					<< playerName << " now has " << health << " health remaining." << endl << endl;
			}

			if (isAI)
			{
				fout << "The attack hits for " << dmg << " damage. The attack had no associated type. "
					<< playerName << " now has " << health << " health remaining." << endl << endl;
			}
			else
			{
				cout << "The attack hits for " << dmg << " damage. The attack had no associated type. "
					<< playerName << " now has " << health << " health remaining." << endl << endl;
			}
		}
		else // attack has same type as defense (neither advantage nor disadvantage), so reveal defense type
		{
			opponent->setOppDefType(defType);
			defTypeKnown = true;

			if (opponent->checkIsAI())
			{
				opponent->toFeed("The attack hits for " + std::to_string(dmg) + " damage as the attack was of the same type as the defense type: " +
					defType + ". ");
				opponent->toFeed(playerName + " now has " + std::to_string(health) + " health remaining.");
			}
			else
			{
				cout << "The attack hits for " << dmg << " damage as the attack was of the same type as the defense type: "
					<< defType << ". " << playerName << " now has " << health << " health remaining." << endl << endl;
			}

			if (isAI)
			{
				fout << "The attack hits for " << dmg << " damage as the attack was of the same type as the defense type: "
					<< defType << ". " << playerName << " now has " << health << " health remaining." << endl << endl;
			}
			else
			{
				cout << "The attack hits for " << dmg << " damage as the attack was of the same type as the defense type: "
					<< defType << ". " << playerName << " now has " << health << " health remaining." << endl << endl;
			}
		}
	}
}

 string Player::playCard(int cardNopm) // hand plays a card and supplies the type of the played card to the caller
{
	return hand.playCard(cardNopm);
}

 void Player::changeDefType() // forces player to choose another defense type
{
	int userInput;
	bool goodTypeInput = false;

	if (isAI)
	{
		fout << "You must choose a defense type. Rock is your current type. What type do you choose?" << endl;
		fout << "Rock: press 1" << endl;
		fout << "Paper: press 2" << endl;
		fout << "Scissors: press 3" << endl << endl;
		userInput = defTypeDecision();
	}
	else
	{
		cout << "You must choose a defense type. Rock is your current type. What type do you choose?" << endl;
		cout << "Rock: press 1" << endl;
		cout << "Paper: press 2" << endl;
		cout << "Scissors: press 3" << endl << endl;

		while (!goodTypeInput) // this is the loop for validating input for selection of new defType
		{
			if (std::cin >> userInput) {
				if (userInput > 0 && userInput < 4)
				{
					goodTypeInput = true;
				}
				else
				{
					cout << "That's not an option, sorry." << endl << endl;
				}
			}
			else {
				cout << "Please enter a valid character" << endl << endl;
				std::cin.clear();
				std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
	}

	switch (userInput)
	{
	case 1:
		defType = "rock";
		break;
	case 2:
		defType = "paper";
		break;
	case 3:
		defType = "scissors";
	}

	opponent->setOppDefType("unknown"); // since you changed your type, your opponent doesn't know what yours is
	defTypeKnown = false;

	if (isAI)
	{
		fout << "Defense type changed to " << defType << endl << endl;
	}
	else
	{
		cout << "Defense type changed to " << defType << endl << endl;
	}
}

void Player::upBank() // adds currency 
{
	bank += 90;
}

void Player::upProb() // calls the player's ProbSet to change the points distribution
{
	playerProbSet.addPoint(isAI, fout, determineGameState(), AItype);
}

void Player::select() // allows user to choose one of the other options
{
	bool goodActionSelection = false;
	int actionSelection;

	if (isAI)
	{
		fout << "You get to choose an action! Here are the available actions: " << endl << endl;
		fout << "1. Draw a card." << endl;
		fout << "2. Purchase an attack." << endl;
		fout << "3. Change defense type." << endl;
		fout << "4. Increase amount of currency held." << endl;
		fout << "5. Increase probability of a certain action being rolled." << endl << endl;
		actionSelection = selectActionDecision(); // AI logic to decide which action to select

	}
	else
	{
		cout << "You get to choose an action! Here are the available actions: " << endl << endl;
		cout << "1. Draw a card." << endl;
		cout << "2. Purchase an attack." << endl;
		cout << "3. Change defense type." << endl;
		cout << "4. Increase amount of currency held." << endl;
		cout << "5. Increase probability of a certain action being rolled." << endl << endl;

		while (!goodActionSelection) // this is the loop for validating input for selection of the action to do that turn
		{
			if (std::cin >> actionSelection)
			{
				if (actionSelection > 0 && actionSelection < 6)
				{
					if ((actionSelection == 2 && bank >= 100) || (actionSelection != 2)) // they need to have a certain amount of 
					{																	 // currency to choose to visit the attack shop
						goodActionSelection = true;
					}
					else
					{
						cout << "Not enough money to make a purchase, though... Pick something else." << endl << endl;
					}
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

	switch (actionSelection)
	{
	case 1: fout << playerName << " chose to draw a card" << endl << endl;
		opponent->toFeed(playerName + "chose to draw a card\n\n");
		cout << playerName << " chose to draw a card" << endl << endl; 
		drawCard(); // take a card from the pile
		break;
	case 2:  fout << playerName << " chose to purchase an attack" << endl << endl;
		opponent->toFeed(playerName + "chose to purchase an attack\n\n");
		cout << playerName << " chose to purchase an attack" << endl << endl;
		opponent->takeDamage(visitShop()); // opportunity to purchase attacks
		break;
	case 3:  fout << playerName << " chose to change defense type" << endl << endl;
		opponent->toFeed(playerName + "chose to change defense type\n\n");
		cout << playerName << " chose to change defense type" << endl << endl;
		changeDefType(); // player must change def type
		break;
	case 4:  fout << playerName << " chose to gain currency" << endl << endl;
		opponent->toFeed(playerName + "chose to gain currency\n\n");
		cout << playerName << " chose to gain currency" << endl << endl; 
		upBank(); // gain currency
		break;
	case 5:  fout << playerName << " chose to change probability" << endl << endl;
		opponent->toFeed(playerName + "chose to change probability\n\n");
		cout << playerName << " chose to change probability" << endl << endl; 
		upProb(); // change probability
		break;
	}
}

string Player::returnDisVantageType(std::string advType)
{
	string disType = "";

	if (advType == "rock")
	{
		disType = "scissors";
	}
	else if (advType == "paper")
	{
		disType = "rock";
	}
	else if (advType == "scissors")
	{
		disType = "paper";
	}
	return disType;
}

string Player::returnAdvantageType(std::string disType)
{
	string advType = "";

	if (disType == "rock")
	{
		advType = "paper";
	}
	else if (disType == "paper")
	{
		advType = "scissors";
	}
	else if (disType == "scissors")
	{
		disType = "rock";
	}
	return advType;
}

void Player::setOppDefType(std::string theType)
{
	opponentDefType = theType;
}

char Player::purchaseAtkDecision() // makes decision for purchasing attacks from shop
{
	char response = 'q';
	std::default_random_engine generator((unsigned int)time(0));
	std::uniform_int_distribution<int> distribution(1, 3);

	int myRandomInt = distribution(generator);

	// STRATEGY: "Spend!"

	if (AItype == "type2") // always buys if it can, purchasing the most expensive available (the most
	{											// efficient use of funds with respect to power obtained)
		if (bank >= 300)
		{
			response = '3';
		}
		else if (bank >= 200)
		{
			response = '2';
		}
		else if (bank >= 100)
		{
			response = '1';
		}
		else
		{
			response = 'q';
		}
	}

	 // STRATEGY: "Save up until kill"

	if (AItype == "type1") // this one uses what I call "critical points" - that is, measures of the likeliness of 
	{					   // an attack to eliminate the opponent. If it is not likely, the AI saves up to buy the
						   // most expensive attack
		if (bank >= 600)
		{
			response = '3';
		}
		else if (bank >= 500)
		{
			if (opponentDefType != "unknown")
			{
				response = '3'; // will always buy most expensive attack given it knows the type
			}
			else
			{
				response = '2'; // this is an attempt to find out the opponentDefType
			}
		}
		else if (bank >= 300)
		{
			if (opponentDefType != "unknown")
			{
				response = '3'; // will always buy most expensive attack given it knows the type
			}
			else
			{
				response = '1'; // this is an attempt to find out the opponentDefType
			}
		}
		else if (bank >= 200)
		{
			if (opponent->getHealth() > 500) // if opponent has more health than is possible to eliminate
			{
				response = 'q';
			}
			else if (opponent->getHealth() > 200)
			{
				if (opponentDefType != "unknown" || myRandomInt == 1) // if opponent's deftype known, attack
				{													  // otherwise, 1/3 chance of attack
					response = '2';
				}
				else if (opponentDefType == "unknown" && myRandomInt != 1)
				{
					response = 'q';
				}
			}
			else
			{
				response = '2';
			}
		}
		else if (bank >= 100)
		{
			if (opponent->getHealth() > 200) // if opponent has more health than is possible to eliminate
			{
				response = 'q';
			}
			else if (opponent->getHealth() > 50)
			{
				if (opponentDefType != "unknown" || myRandomInt == 1) // if opponent's deftype known, attack
				{													  // otherwise, 1/3 chance of attack
					response = '1';
				}
				else if (opponentDefType == "unknown" && myRandomInt != 1)
				{
					response = 'q';
				}
			}
			else
			{
				response = '1';
			}
		}
		else
		{
			response = 'q';
		}
	}

	// STRATEGY: Go for the double!
	//if (AItype == "type2")
	//{
	//	if (opponentDefType != "unknown") // if the type is known
	//	{
	//		int index;
	//		int quantity;
	//		if (hand.hasCard(returnAdvantageType(opponentDefType), index, quantity))
	//		{
	//			if (bank >= 300)
	//			{
	//				response = '3';
	//			}
	//			else if (bank >= 200)
	//			{
	//				response = '2';
	//			}
	//			else if (bank >= 100)
	//			{
	//				response = '1';
	//			}
	//			else
	//			{
	//				response = 'q';
	//			}
	//		}
	//		else
	//		{
	//			response = 'q';
	//		}
	//	}
	//	else // if the type is not known, find out what the type is
	//	{
	//		if (bank >= 200)
	//		{
	//			response = '1'; // leave some money for a follow-up attack
	//		}
	//		else
	//		{
	//			response = 'q';
	//		}
	//	}
	//}
	return response;
}


char Player::playCardDecision(int dmg)
{
	char response = 'q';
	int index;
	int quantity;


	if (AItype == "type1" || AItype == "type2") // always maximize damage done, guess unless guessing may fail to secure the kill
	{
		if (hand.getCurrentSize() != 0) // if the hand is not empty
		{
			if (opponent->getHealth() > (dmg - 50)) // cannot eliminate or elimination is not guaranteed
			{

				if (opponentDefType != "unknown")
				{
					if (hand.hasCard(returnAdvantageType(opponentDefType), index, quantity)) // if the hand has a type of
					{																		 // card with advantage against
																							 // opponent's defType
						response = '1' + index; // adding to '1' instead of '0' to avoid off-by-one issues
					}
					else if (hand.hasCard(opponentDefType, index, quantity)) // if the hand has a type of card that is the
					{														 // same as the opponent's defType
						response = '1' + index; // adding to '1' instead of '0' to avoid off-by-one issues
					}
					else
					{
						response = 'q'; // because there's never any point to playing a disadvantage if you know the opponent's
					}					// type already
				}
				else // if the opponent's type is unknown
				{
					if (hand.mostPopulous() != (-1))
					{
						response = '1' + hand.mostPopulous(); 
					}
				}
			}
			else // opponent's health less than or equal to (dmg - 50): elimination nearly guaranteed (just don't play disadvantage)
			{
				if (opponentDefType != "unknown")
				{
					if (hand.hasCard(returnAdvantageType(opponentDefType), index, quantity)) // if the hand has a type of
					{																		 // card with advantage against
																							 // opponent's defType
						response = '1' + index; // adding to '1' instead of '0' to avoid off-by-one issues
					}
					else if (hand.hasCard(opponentDefType, index, quantity)) // if the hand has a type of card that is the
					{														 // same as the opponent's defType
						response = '1' + index; // adding to '1' instead of '0' to avoid off-by-one issues
					}
					else
					{
						response = 'q'; // because there's never any point to playing a disadvantage if you know the opponent's
					}					// type already
				}
				else // if the opponent's type is unknown
				{
					response = 'q'; // avoid any chance of playing disadvantage (because that's the only way you could potentially
				}					// mess this up)
			}
		}
		else // if hand is empty
		{
			response = 'q';
		}
	}
	return response;
}

int Player::defTypeDecision() // either random or calculated
{
	int response = -1;
	string responseString;
	std::default_random_engine generator((unsigned int)time(0));
	std::uniform_int_distribution<int> distribution(1, 3);

	response = distribution(generator); // generate random int

	// STRATEGY: "Random defense!"

	if (AItype == "type2") // completely random choice
	{
		response = distribution(generator); // generate random int
	}
	else if (AItype == "type1") // STRATEGY: "Pick defense least likely to be doubled"
	{
		if (hand.getCurrentSize() > 7) // if they have a significant number of cards
		{
			int popularIndex = hand.mostPopulous(); // get the index of one of most common card type
			responseString = returnDisVantageType(hand.getCardType(popularIndex)); // get the type of that card and
			if (responseString == "rock")										   // determine what is weak to that type
			{
				response = 1;
			}
			else if (responseString == "paper")
			{
				response = 2;
			}
			else
			{
				response = 3;
			}
		}
		else
		{
			response = distribution(generator);
		}
	}

	return response;
}

int Player::selectActionDecision()
{
	int response = -1;
	if (AItype == "type1" || AItype == "type2")
	{
		response = determineGameState();	
	}
	return response;
}

int Player::determineGameState()
{

	int weightTotal = 0;
	int result;

	// tests to modify weights

	if (getHandSize() < 4) // few cards left in hand
	{
		stateWeights[0] += 7;
	}

	if (getHandSize() < 2)
	{
		stateWeights[0] += 5;
	}
	

			if (opponentDefType != "unknown")
			{
				int i;
				int quantity;
				if (!hand.hasCard(returnAdvantageType(opponentDefType), i, quantity))
				{
					stateWeights[0] += 15;
				}
			}

	if (opponentDefType != "unknown") // all these are geared to encourage attack
	{
		stateWeights[1] += 10;
	}

	if (opponent->getHealth() <= 50)
	{
		stateWeights[1] += 50;
	}

	if (opponent->getHealth() <= 100)
	{
		stateWeights[1] += 10;
	}

	if (bank > 200)
	{
		stateWeights[1] += 3;	
	}

	if (bank > 300)
	{
		stateWeights[1] += 10;
	}

	if (health <= 50)
	{
		stateWeights[1] += 50;
	}

	if (bank < 100)
	{
		stateWeights[1] = 0;
	}

	// defType tests

	if (defTypeKnown)
	{
		stateWeights[2] += 10;
		if ((health <= 200 && opponent->getBank() >= 100) || (health <= 500 && opponent->getBank() >= 200) || (health <= 800 && opponent->getBank() >= 400))
		{		// if in danger of elimination
			stateWeights[2] += 25;
		}
	}	

	// get money tests

	if (bank < 100)
	{
		stateWeights[3] += 15;
	}

	if (bank < 200)
	{
		stateWeights[3] += 10;
	}

	if (bank < 300)
	{
		stateWeights[3] += 5;
	}

	// tests for changing probabilities
	
	if (playerProbSet.getTotalStats() < 40)
	{
		stateWeights[4] += 10;
	}

	if (playerProbSet.getTotalStats() < 50)
	{
		stateWeights[4] += 7;
	}

	if (playerProbSet.getTotalStats() < 55)
	{
		stateWeights[4] += 3;
	}

	for (int i = 0; i < 5; i++)
	{
		weightTotal += stateWeights[i];
	}

	std::default_random_engine generator((unsigned int)time(0));
	std::uniform_int_distribution<int> distribution(1, weightTotal);

	int myRandomInt = distribution(generator); // generate random int

	// this next "if-else" construct determines the result

	if (myRandomInt <= stateWeights[0])
	{
		result = 0;
	}
	else if (myRandomInt <= stateWeights[0] + stateWeights[1])
	{
		result = 1;
	}
	else if (myRandomInt <= stateWeights[0] + stateWeights[1] + stateWeights[2])
	{
		result = 2;
	}
	else if (myRandomInt <= stateWeights[0] + stateWeights[1] + stateWeights[2] + stateWeights[3])
	{
		result = 3;
	}
	else if (myRandomInt <= weightTotal)
	{
		result = 4;
	}
	else
	{
		cout << "error: random roll machine broke :(" << endl << endl;
		result = 99; // I guess this can be used as a check
	}
	return result;
}