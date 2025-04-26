#include "card.h"
#include <sstream>

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define BLACK "\033[30m"

Card::Card(Suit s, Rank r) : suit(s), rank(r) {}

static std::string suitToSymbol(Suit suit)
{
	switch (suit)
	{
	case Suit::Hearts:
		return "❤️ " RESET;
	case Suit::Diamonds:
		return "♦️ " RESET;
	case Suit::Clubs:
		return "♣️ " RESET;
	case Suit::Spades:
		return "♠️ " RESET;
	default:
		return "?";
	}
}

static std::string rankToString(Rank rank)
{
	int r = static_cast<int>(rank);
	if (r >= 2 && r <= 10)
	{
		return std::to_string(r);
	}
	switch (rank)
	{
	case Rank::Jack:
		return "J";
	case Rank::Queen:
		return "Q";
	case Rank::King:
		return "K";
	case Rank::Ace:
		return "A";
	default:
		return "?";
	}
}

std::string Card::toString() const
{
	return rankToString(rank) + suitToSymbol(suit);
}

/**
 * Card class - Represents a single playing card (e.g., Ace of Spades)
 * Suit enum - Defines the four suits: H, D, C, S
 * Rank enum - Defines card values from Two (2) through Ace (14)
 * toString() - Returns a string like "10H", "KS", "AD" for display in CLI
 * rankToString() - Converts Rank enums into strings like "10", "J", "Q", etc.
 * suitToSymbol() - Converts Suit enums into single-character symbols: H, D, C, S
 */