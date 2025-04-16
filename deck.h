#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>

class Deck {
private:
	std::vector<Card> cards;
	std::mt19937 rng; // Mersenne Twister random number generator

public:
	Deck(); 				// Constructor: builds and shuffles the deck
	void shuffle();			// Reshuffles the deck
	Card dealCard();		// Deals one card
	bool isEmpty() const; 	// Checks if the deck is empty
	int size() const;		// Number of cards remaining
};

#endif