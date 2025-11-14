/**
 * Unit Tests for Hand Evaluator
 * 
 * Validates poker hand ranking and comparison logic
 */

#include "../model/advanced_hand_evaluator.h"
#include "../model/card.h"
#include <iostream>
#include <vector>
#include <cassert>

#define TEST(name) void test_##name()
#define RUN_TEST(name) do { \
    std::cout << "Running " << #name << "... "; \
    test_##name(); \
    std::cout << "PASSED\n"; \
} while(0)

#define ASSERT_TRUE(expr) do { \
    if (!(expr)) { \
        std::cerr << "FAILED: " << #expr << " at line " << __LINE__ << "\n"; \
        exit(1); \
    } \
} while(0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "FAILED: " << #a << " != " << #b << " at line " << __LINE__ << "\n"; \
        exit(1); \
    } \
} while(0)

// Test: Royal Flush detection
TEST(royal_flush_detection) {
    std::vector<Card> hand = {
        Card(Rank::Ace, Suit::Hearts),
        Card(Rank::King, Suit::Hearts),
        Card(Rank::Queen, Suit::Hearts),
        Card(Rank::Jack, Suit::Hearts),
        Card(Rank::Ten, Suit::Hearts)
    };
    
    HandValue hv = AdvancedHandEvaluator::evaluate(hand);
    ASSERT_EQ(hv.rank, HandRank::RoyalFlush);
}

// Test: Straight Flush detection
TEST(straight_flush_detection) {
    std::vector<Card> hand = {
        Card(Rank::Nine, Suit::Diamonds),
        Card(Rank::Eight, Suit::Diamonds),
        Card(Rank::Seven, Suit::Diamonds),
        Card(Rank::Six, Suit::Diamonds),
        Card(Rank::Five, Suit::Diamonds)
    };
    
    HandValue hv = AdvancedHandEvaluator::evaluate(hand);
    ASSERT_EQ(hv.rank, HandRank::StraightFlush);
}

// Test: Four of a Kind detection
TEST(four_of_a_kind_detection) {
    std::vector<Card> hand = {
        Card(Rank::King, Suit::Hearts),
        Card(Rank::King, Suit::Diamonds),
        Card(Rank::King, Suit::Clubs),
        Card(Rank::King, Suit::Spades),
        Card(Rank::Two, Suit::Hearts)
    };
    
    HandValue hv = AdvancedHandEvaluator::evaluate(hand);
    ASSERT_EQ(hv.rank, HandRank::FourOfAKind);
}

// Test: Full House detection
TEST(full_house_detection) {
    std::vector<Card> hand = {
        Card(Rank::Ace, Suit::Hearts),
        Card(Rank::Ace, Suit::Diamonds),
        Card(Rank::Ace, Suit::Clubs),
        Card(Rank::King, Suit::Hearts),
        Card(Rank::King, Suit::Diamonds)
    };
    
    HandValue hv = AdvancedHandEvaluator::evaluate(hand);
    ASSERT_EQ(hv.rank, HandRank::FullHouse);
}

// Test: Flush detection
TEST(flush_detection) {
    std::vector<Card> hand = {
        Card(Rank::Ace, Suit::Clubs),
        Card(Rank::Jack, Suit::Clubs),
        Card(Rank::Nine, Suit::Clubs),
        Card(Rank::Five, Suit::Clubs),
        Card(Rank::Three, Suit::Clubs)
    };
    
    HandValue hv = AdvancedHandEvaluator::evaluate(hand);
    ASSERT_EQ(hv.rank, HandRank::Flush);
}

// Test: Straight detection
TEST(straight_detection) {
    std::vector<Card> hand = {
        Card(Rank::Ten, Suit::Hearts),
        Card(Rank::Nine, Suit::Diamonds),
        Card(Rank::Eight, Suit::Clubs),
        Card(Rank::Seven, Suit::Spades),
        Card(Rank::Six, Suit::Hearts)
    };
    
    HandValue hv = AdvancedHandEvaluator::evaluate(hand);
    ASSERT_EQ(hv.rank, HandRank::Straight);
}

// Test: Three of a Kind detection
TEST(three_of_a_kind_detection) {
    std::vector<Card> hand = {
        Card(Rank::Queen, Suit::Hearts),
        Card(Rank::Queen, Suit::Diamonds),
        Card(Rank::Queen, Suit::Clubs),
        Card(Rank::Nine, Suit::Hearts),
        Card(Rank::Two, Suit::Diamonds)
    };
    
    HandValue hv = AdvancedHandEvaluator::evaluate(hand);
    ASSERT_EQ(hv.rank, HandRank::ThreeOfAKind);
}

// Test: Two Pair detection
TEST(two_pair_detection) {
    std::vector<Card> hand = {
        Card(Rank::Jack, Suit::Hearts),
        Card(Rank::Jack, Suit::Diamonds),
        Card(Rank::Five, Suit::Clubs),
        Card(Rank::Five, Suit::Spades),
        Card(Rank::Two, Suit::Hearts)
    };
    
    HandValue hv = AdvancedHandEvaluator::evaluate(hand);
    ASSERT_EQ(hv.rank, HandRank::TwoPair);
}

// Test: One Pair detection
TEST(one_pair_detection) {
    std::vector<Card> hand = {
        Card(Rank::Ten, Suit::Hearts),
        Card(Rank::Ten, Suit::Diamonds),
        Card(Rank::Seven, Suit::Clubs),
        Card(Rank::Four, Suit::Spades),
        Card(Rank::Two, Suit::Hearts)
    };
    
    HandValue hv = AdvancedHandEvaluator::evaluate(hand);
    ASSERT_EQ(hv.rank, HandRank::OnePair);
}

// Test: Hand comparison - Royal Flush beats everything
TEST(royal_flush_beats_straight_flush) {
    std::vector<Card> royal = {
        Card(Rank::Ace, Suit::Hearts),
        Card(Rank::King, Suit::Hearts),
        Card(Rank::Queen, Suit::Hearts),
        Card(Rank::Jack, Suit::Hearts),
        Card(Rank::Ten, Suit::Hearts)
    };
    
    std::vector<Card> straightFlush = {
        Card(Rank::Nine, Suit::Diamonds),
        Card(Rank::Eight, Suit::Diamonds),
        Card(Rank::Seven, Suit::Diamonds),
        Card(Rank::Six, Suit::Diamonds),
        Card(Rank::Five, Suit::Diamonds)
    };
    
    HandValue royalHV = AdvancedHandEvaluator::evaluate(royal);
    HandValue straightHV = AdvancedHandEvaluator::evaluate(straightFlush);
    
    ASSERT_TRUE(royalHV > straightHV);
}

// Test: Kicker comparison for same rank hands
TEST(kicker_comparison) {
    std::vector<Card> hand1 = {
        Card(Rank::Ace, Suit::Hearts),
        Card(Rank::Ace, Suit::Diamonds),
        Card(Rank::King, Suit::Clubs),
        Card(Rank::Queen, Suit::Spades),
        Card(Rank::Jack, Suit::Hearts)
    };
    
    std::vector<Card> hand2 = {
        Card(Rank::Ace, Suit::Clubs),
        Card(Rank::Ace, Suit::Spades),
        Card(Rank::Ten, Suit::Hearts),
        Card(Rank::Nine, Suit::Diamonds),
        Card(Rank::Eight, Suit::Clubs)
    };
    
    HandValue hv1 = AdvancedHandEvaluator::evaluate(hand1);
    HandValue hv2 = AdvancedHandEvaluator::evaluate(hand2);
    
    // Both are one pair of aces, but hand1 has better kickers
    ASSERT_EQ(hv1.rank, HandRank::OnePair);
    ASSERT_EQ(hv2.rank, HandRank::OnePair);
    ASSERT_TRUE(hv1 > hv2);
}

int main() {
    std::cout << "=== Hand Evaluator Unit Tests ===\n\n";
    
    RUN_TEST(royal_flush_detection);
    RUN_TEST(straight_flush_detection);
    RUN_TEST(four_of_a_kind_detection);
    RUN_TEST(full_house_detection);
    RUN_TEST(flush_detection);
    RUN_TEST(straight_detection);
    RUN_TEST(three_of_a_kind_detection);
    RUN_TEST(two_pair_detection);
    RUN_TEST(one_pair_detection);
    RUN_TEST(royal_flush_beats_straight_flush);
    RUN_TEST(kicker_comparison);
    
    std::cout << "\n✓ All tests passed!\n";
    return 0;
}
