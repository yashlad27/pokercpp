#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <vector>
#include <string>

/**
 * Each player needs:
 * 1. A name (or ID)
 * 2. Hole cards (2 cards in Texas Hold'em)
 * 3. A chip count (how many chips/ money they have)
 * 4. A flag for whether they have folded
 * 5. Methods to recieve cards, bet, fold etc.
**/
class Player {
private:
	std::string name;
	std::vector<Card> hand;
	int chips;
	bool folded;

public:
	Player(const std::string& name, int startingChips);

	void recieveCard(const Card& card);		// Add a card to the hand
	void clearHand();						// Reset hand (for new round)

	void bet(int amount);			// deduct chips
	void fold();					// set folded=true
	void resetStatus();				// unfold for next round

	bool isFolded() const;
	int getChipCount() const;
	std::string getName() const;
	std::vector<Card> getHand() const;

	void showHand(bool showCards=true) const; 	// Print cards (face down if false)
};

#endif

/**
 * hand -> Stores the player's 2 private (hole) cards
 * chips -> represents their current stack for betting
 * folded -> tracks whether player is out of round 
 * recieveCard() -> called twice to deal the hole cards
 * bet() -> removes chips when the player bets
 * showHand() -> used in showdown or debug mode
 **/ 