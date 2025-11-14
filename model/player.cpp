#include "player.h"
#include <iostream>

Player::Player(const std::string &name, int startingChips)
	: name(name), chips(startingChips), folded(false) {}

// Adds one of two hole cards
void Player::recieveCard(const Card &card)
{
	if (hand.size() < 2)
	{
		hand.push_back(card);
	}
	else
	{
		std::cerr << name << " already has 2 cards.\n";
	}
}

// clears hand between rounds
void Player::clearHand()
{
	hand.clear();
}

// deducts chips from player's stack
void Player::bet(int amount)
{
	if (amount > chips)
	{
		std::cerr << name << " does not have enough chips to bet $$$";
		return;
	}
	chips -= amount;
	
	// Color code based on player name
	const char* color = (name == "You") ? "\033[32m" : "\033[36m"; // Green for You, Cyan for Bot
	const char* reset = "\033[0m";
	const char* bold = "\033[1m";
	const char* yellow = "\033[33m";
	
	std::cout << bold << color << name << reset << " bets " << yellow << bold << amount << " chips" << reset << ". 💵\n";
}

// adds chips to player's stack (for winnings)
void Player::addChips(int amount)
{
	chips += amount;
}

// Marks player as folded (out of round)
void Player::fold()
{
	folded = true;
	std::cout << name << " folds.\n";
}

// unfolds player at start of new round
void Player::resetStatus()
{
	folded = false;
}

bool Player::isFolded() const
{
	return folded;
}

int Player::getChipCount() const
{
	return chips;
}

std::string Player::getName() const
{
	return name;
}

// shows cards during a showdown
void Player::showHand(bool showCards) const
{
	std::cout << name << "'s hand: ";
	if (showCards)
	{
		for (const Card &card : hand)
		{
			std::cout << card.toString() << " ";
		}
	}
	else
	{
		for (size_t i = 0; i < hand.size(); ++i)
		{
			std::cout << "[hidden]";
		}
	}
	std::cout << std::endl;
}

std::vector<Card> Player::getHand() const
{
	return hand;
}