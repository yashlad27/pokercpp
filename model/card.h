#ifndef CARD_H
#define CARD_H

#include <string>

// enum for suit
enum class Suit {
	Hearts, 
	Diamonds,
	Clubs,
	Spades
};

// Enum for Rank
enum class Rank {
	Two = 2, Three, Four, Five, Six, Seven, 
	Eight, Nine, Ten, Jack, Queen, King, Ace
};

// Card class
class Card {
public: 
	Suit suit;
	Rank rank;

	Card(Suit s, Rank r);
	std::string toString() const;
};

#endif 