#ifndef HAND_EVALUATOR_H
#define HAND_EVALUATOR_H

#include "card.h"
#include <vector>

// enum for hand strength
enum class HandRank
{
	HighCard = 0,
	OnePair,
	TwoPair,
	ThreeOfAKind,
	Straight,
	Flush,
	FullHouse,
	FourOfAKind,
	StraightFlush,
	RoyalFlush
};

// structure to store the evaluated hand value
struct HandValue
{
	HandRank rank;
	std::vector<int> kickers; // type of hand

	bool operator>(const HandValue &other) const;
	bool operator==(const HandValue &other) const;
};

// main hand evaluation function
HandValue evaluateHand(const std::vector<Card> &cards);

#endif