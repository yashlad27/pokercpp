#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "../model/card.h"
#include "../model/hand_evaluator.h"

TEST_CASE("HandEvaluator detects one pair") {
    std::vector<Card> hand = {
        Card(Rank::Ten, Suit::Hearts),
        Card(Rank::Ten, Suit::Clubs),
        Card(Rank::Five, Suit::Spades),
        Card(Rank::Two, Suit::Hearts),
        Card(Rank::Nine, Suit::Diamonds),
        Card(Rank::Ace, Suit::Hearts),
        Card(Rank::King, Suit::Spades)
    };

    HandValue value = evaluateHand(hand);
    REQUIRE(value.rank == HandRank::OnePair);
}