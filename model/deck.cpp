#include "deck.h"
#include <iostream>

Deck::Deck()
{
	rng.seed(static_cast<unsigned int>(std::time(nullptr)));

	// popularte deck with 52 cards
	for (int s = 0; s < 4; ++s)
	{
		for (int r = 2; r <= 14; ++r)
		{
			cards.emplace_back(
				static_cast<Suit>(s),
				static_cast<Rank>(r));
		}
	}

	shuffle();
}

void Deck::shuffle()
{
	std::shuffle(cards.begin(), cards.end(), rng);
}

Card Deck::dealCard()
{
	if (cards.empty())
	{
		std::cerr << "Error: Tried to deal a card from an empty deck.\n";
		exit(EXIT_FAILURE);
	}

	Card topCard = cards.back();
	cards.pop_back();
	return topCard;
}

bool Deck::isEmpty() const
{
	return cards.empty();
}

int Deck::size() const
{
	return static_cast<int>(cards.size());
}

/**
 * emplace_back = modern method to add elements to a vector, more efficient than
 * push_back() when constructing objects
 **/