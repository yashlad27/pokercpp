/**
 * Unit Tests for Monte Carlo Simulator
 * 
 * Tests statistical properties and confidence interval calculations
 * Critical for validating simulation accuracy
 */

#include "../montecarlo/MonteCarloSimulator.h"
#include "../model/card.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

// Simple test framework
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

#define ASSERT_NEAR(a, b, epsilon) do { \
    if (std::abs((a) - (b)) > (epsilon)) { \
        std::cerr << "FAILED: " << #a << " (" << (a) << ") not near " \
                  << #b << " (" << (b) << ") at line " << __LINE__ << "\n"; \
        exit(1); \
    } \
} while(0)

// Test: Percentages sum to ~100%
TEST(percentages_sum_to_one) {
    std::vector<Card> playerHand = {
        Card(Rank::Ace, Suit::Hearts),
        Card(Rank::King, Suit::Hearts)
    };
    std::vector<Card> community = {
        Card(Rank::Queen, Suit::Hearts),
        Card(Rank::Jack, Suit::Hearts),
        Card(Rank::Ten, Suit::Clubs)
    };
    
    MonteCarloSimulator sim(playerHand, community, 100);
    sim.runSimulation();
    
    double sum = sim.getWinPercentage() + sim.getTiePercentage() + sim.getLosePercentage();
    ASSERT_NEAR(sum, 1.0, 0.01);  // Should be ~1.0 with small tolerance
}

// Test: Confidence interval contains true win rate
TEST(confidence_interval_properties) {
    std::vector<Card> playerHand = {
        Card(Rank::Ace, Suit::Hearts),
        Card(Rank::Ace, Suit::Diamonds)
    };
    std::vector<Card> community = {
        Card(Rank::Two, Suit::Hearts),
        Card(Rank::Three, Suit::Clubs),
        Card(Rank::Four, Suit::Spades)
    };
    
    MonteCarloSimulator sim(playerHand, community, 1000);
    sim.runSimulation();
    
    auto [lower, upper] = sim.getConfidenceInterval(0.95);
    double winRate = sim.getWinPercentage();
    
    // Win rate should be within confidence interval
    ASSERT_TRUE(winRate >= lower);
    ASSERT_TRUE(winRate <= upper);
    
    // Confidence interval should be reasonable (not [0, 1])
    ASSERT_TRUE(upper - lower < 0.5);  // Less than 50% wide for 1000 sims
}

// Test: Standard deviation decreases with more simulations
TEST(stddev_decreases_with_sample_size) {
    std::vector<Card> playerHand = {
        Card(Rank::King, Suit::Hearts),
        Card(Rank::Queen, Suit::Hearts)
    };
    std::vector<Card> community = {
        Card(Rank::Ten, Suit::Hearts),
        Card(Rank::Nine, Suit::Clubs),
        Card(Rank::Two, Suit::Diamonds)
    };
    
    MonteCarloSimulator sim100(playerHand, community, 100);
    sim100.runSimulation();
    double stdDev100 = sim100.getWinRateStdDev();
    
    MonteCarloSimulator sim1000(playerHand, community, 1000);
    sim1000.runSimulation();
    double stdDev1000 = sim1000.getWinRateStdDev();
    
    // Standard error should decrease with sqrt(n)
    // σ_1000 should be roughly σ_100 / sqrt(10)
    ASSERT_TRUE(stdDev1000 < stdDev100);
    ASSERT_NEAR(stdDev1000 / stdDev100, 1.0 / std::sqrt(10.0), 0.2);
}

// Test: Royal flush should have very high win rate
TEST(royal_flush_wins) {
    std::vector<Card> playerHand = {
        Card(Rank::Ace, Suit::Hearts),
        Card(Rank::King, Suit::Hearts)
    };
    std::vector<Card> community = {
        Card(Rank::Queen, Suit::Hearts),
        Card(Rank::Jack, Suit::Hearts),
        Card(Rank::Ten, Suit::Hearts)  // Royal flush!
    };
    
    MonteCarloSimulator sim(playerHand, community, 100);
    sim.runSimulation();
    
    double winRate = sim.getWinPercentage();
    // Royal flush should win >90% (some ties possible)
    ASSERT_TRUE(winRate > 0.90);
}

// Test: High pair should beat random hands >50%
TEST(high_pair_beats_random) {
    std::vector<Card> playerHand = {
        Card(Rank::Ace, Suit::Hearts),
        Card(Rank::Ace, Suit::Diamonds)
    };
    std::vector<Card> community = {
        Card(Rank::Two, Suit::Clubs),
        Card(Rank::Seven, Suit::Spades),
        Card(Rank::Nine, Suit::Hearts)
    };
    
    MonteCarloSimulator sim(playerHand, community, 500);
    sim.runSimulation();
    
    double winRate = sim.getWinPercentage();
    // Pocket aces should win well over 50%
    ASSERT_TRUE(winRate > 0.50);
}

// Test: Confidence interval width scales correctly
TEST(confidence_interval_scaling) {
    std::vector<Card> playerHand = {
        Card(Rank::Ten, Suit::Hearts),
        Card(Rank::Nine, Suit::Hearts)
    };
    std::vector<Card> community = {
        Card(Rank::Two, Suit::Clubs),
        Card(Rank::Five, Suit::Diamonds),
        Card(Rank::Eight, Suit::Spades)
    };
    
    MonteCarloSimulator sim(playerHand, community, 400);
    sim.runSimulation();
    
    auto [lower95, upper95] = sim.getConfidenceInterval(0.95);
    auto [lower99, upper99] = sim.getConfidenceInterval(0.99);
    
    double width95 = upper95 - lower95;
    double width99 = upper99 - lower99;
    
    // 99% CI should be wider than 95% CI
    ASSERT_TRUE(width99 > width95);
}

int main() {
    std::cout << "=== Monte Carlo Simulator Unit Tests ===\n\n";
    
    RUN_TEST(percentages_sum_to_one);
    RUN_TEST(confidence_interval_properties);
    RUN_TEST(stddev_decreases_with_sample_size);
    RUN_TEST(royal_flush_wins);
    RUN_TEST(high_pair_beats_random);
    RUN_TEST(confidence_interval_scaling);
    
    std::cout << "\n✓ All tests passed!\n";
    return 0;
}
