// montecarlo/MonteCarloSimulator.cpp
#include "MonteCarloSimulator.h"
#include "../model/deck.h"
#include "../model/advanced_hand_evaluator.h"

#include <random>
#include <algorithm>
#include <iostream>
#include <map>
#include <cmath>  // for sqrt, max, min

MonteCarloSimulator::MonteCarloSimulator(const std::vector<Card> &playerHand,
                                         const std::vector<Card> &communityCards,
                                         int simulations)
    : playerHand(playerHand), communityCards(communityCards),
      numSimulations(simulations), winCount(0), tieCount(0), loseCount(0)
{
}

void MonteCarloSimulator::runSimulation()
{
    winCount = tieCount = loseCount = 0;

    std::vector<Card> fullHand = playerHand;
    fullHand.insert(fullHand.end(), communityCards.begin(), communityCards.end());

    for (int i = 0; i < numSimulations; ++i)
    {
        // Get remaining cards to deal from
        std::vector<Card> deck = getRemainingDeck();

        // Shuffle the deck
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deck.begin(), deck.end(), g);

        // Deal opponent hand and complete the board if needed
        auto [opponentHand, completeBoard] = dealRandomOpponentAndBoard(deck);

        // Combine player hand with board
        std::vector<Card> playerFullHand = playerHand;
        playerFullHand.insert(playerFullHand.end(), completeBoard.begin(), completeBoard.end());

        // Combine opponent hand with board
        std::vector<Card> opponentFullHand = opponentHand;
        opponentFullHand.insert(opponentFullHand.end(), completeBoard.begin(), completeBoard.end());

        // Evaluate both hands
        HandValue playerValue = AdvancedHandEvaluator::evaluate(playerFullHand);
        HandValue opponentValue = AdvancedHandEvaluator::evaluate(opponentFullHand);

        // Compare results
        if (playerValue > opponentValue)
        {
            winCount++;
        }
        else if (opponentValue > playerValue)
        {
            loseCount++;
        }
        else
        {
            tieCount++;
        }
    }
}

double MonteCarloSimulator::getWinPercentage() const
{
    if (numSimulations == 0)
        return 0.0;
    return static_cast<double>(winCount) / numSimulations;
}

double MonteCarloSimulator::getTiePercentage() const
{
    if (numSimulations == 0)
        return 0.0;
    return static_cast<double>(tieCount) / numSimulations;
}

double MonteCarloSimulator::getLosePercentage() const
{
    if (numSimulations == 0)
        return 0.0;
    return static_cast<double>(loseCount) / numSimulations;
}

// Calculate standard deviation of win rate using binomial distribution
// For binary outcomes: σ = sqrt(p(1-p)/n)
double MonteCarloSimulator::getWinRateStdDev() const
{
    if (numSimulations == 0)
        return 0.0;
    
    double p = getWinPercentage();
    double variance = p * (1.0 - p) / numSimulations;
    return std::sqrt(variance);
}

// Calculate confidence interval for win rate
// Returns pair of (lower_bound, upper_bound)
// Uses normal approximation for binomial: mean ± z * σ
std::pair<double, double> MonteCarloSimulator::getConfidenceInterval(double confidence) const
{
    if (numSimulations == 0)
        return {0.0, 0.0};
    
    double winRate = getWinPercentage();
    double stdDev = getWinRateStdDev();
    
    // Z-scores for common confidence levels
    // 95% = 1.96, 99% = 2.576, 90% = 1.645
    double z = 1.96;  // Default to 95% confidence
    
    if (confidence >= 0.99)
        z = 2.576;
    else if (confidence >= 0.95)
        z = 1.96;
    else if (confidence >= 0.90)
        z = 1.645;
    
    double margin = z * stdDev;
    double lowerBound = std::max(0.0, winRate - margin);
    double upperBound = std::min(1.0, winRate + margin);
    
    return {lowerBound, upperBound};
}

double MonteCarloSimulator::getFlushDrawOdds() const
{
    // Count cards by suit
    std::map<Suit, int> suitCount;
    for (const Card &c : playerHand)
    {
        suitCount[c.suit]++;
    }
    for (const Card &c : communityCards)
    {
        suitCount[c.suit]++;
    }

    // Check if we have 4 cards of the same suit
    Suit flushSuit = Suit::Clubs; // Default
    bool hasFlushDraw = false;

    for (const auto &[suit, count] : suitCount)
    {
        if (count == 4)
        {
            hasFlushDraw = true;
            flushSuit = suit;
            break;
        }
    }

    if (!hasFlushDraw)
    {
        return 0.0;
    }

    // Calculate how many flush cards are left in the deck
    int totalCards = playerHand.size() + communityCards.size();
    int cardsRemaining = 13 - suitCount[flushSuit]; // 13 cards per suit
    int deckSize = 52 - totalCards;

    // Probability of drawing a flush card
    return static_cast<double>(cardsRemaining) / deckSize;
}

double MonteCarloSimulator::getStraightDrawOdds() const
{
    std::vector<int> ranks;
    for (const Card &c : playerHand)
    {
        ranks.push_back(static_cast<int>(c.rank));
    }
    for (const Card &c : communityCards)
    {
        ranks.push_back(static_cast<int>(c.rank));
    }

    // Sort and remove duplicates
    std::sort(ranks.begin(), ranks.end());
    auto last = std::unique(ranks.begin(), ranks.end());
    ranks.erase(last, ranks.end());

    // Check for open-ended straight draw (4 cards in sequence)
    std::vector<int> neededCards;

    for (size_t i = 0; i < ranks.size() - 3; ++i)
    {
        if (ranks[i + 1] == ranks[i] + 1 &&
            ranks[i + 2] == ranks[i] + 1 &&
            ranks[i + 3] == ranks[i] + 1)
        {
            // Open-ended: we need the card before or after
            if (ranks[i] - 1 >= 2)
            { // 2 is the lowest card
                neededCards.push_back(ranks[i] - 1);
            }
            if (ranks[i + 3] + 1 <= 14)
            { // 14 (Ace) is the highest
                neededCards.push_back(ranks[i + 3] + 1);
            }
        }
    }

    // Check for inside straight draw (gap in the middle)
    for (size_t i = 0; i < ranks.size() - 3; ++i)
    {
        if (ranks[i + 1] == ranks[i] + 1 &&
            ranks[i + 2] == ranks[i] + 2 &&
            ranks[i + 3] == ranks[i] + 3)
        {
            neededCards.push_back(ranks[i] + 2);
        }
    }

    // Remove duplicates from needed cards
    std::sort(neededCards.begin(), neededCards.end());
    auto lastUnique = std::unique(neededCards.begin(), neededCards.end());
    neededCards.erase(lastUnique, neededCards.end());

    // Calculate odds
    if (neededCards.empty())
    {
        return 0.0;
    }

    int totalCards = playerHand.size() + communityCards.size();
    int deckSize = 52 - totalCards;

    // Each rank has 4 cards (one per suit)
    int totalOuts = neededCards.size() * 4;

    // Subtract cards we already know
    for (const Card &c : playerHand)
    {
        if (std::find(neededCards.begin(), neededCards.end(), static_cast<int>(c.rank)) != neededCards.end())
        {
            totalOuts--;
    }
    }
    for (const Card &c : communityCards)
    {
        if (std::find(neededCards.begin(), neededCards.end(), static_cast<int>(c.rank)) != neededCards.end())
        {
            totalOuts--;
        }
    }

    return static_cast<double>(totalOuts) / deckSize;
}

std::vector<Card> MonteCarloSimulator::getRemainingDeck() const
{
    std::vector<Card> deck;

    // Create a full deck
    for (int s = 0; s < 4; ++s)
    {
        for (int r = 2; r <= 14; ++r)
        {
            Card card(static_cast<Rank>(r), static_cast<Suit>(s));

            // Check if card is already in player hand or community cards
            bool isUsed = false;
            for (const Card &c : playerHand)
            {
                if (c.rank == card.rank && c.suit == card.suit)
                {
                    isUsed = true;
                    break;
                }
            }

            if (!isUsed)
            {
                for (const Card &c : communityCards)
                {
                    if (c.rank == card.rank && c.suit == card.suit)
                    {
                        isUsed = true;
                        break;
                    }
                }
            }

            if (!isUsed)
            {
                deck.push_back(card);
            }
        }
    }

    return deck;
}

std::pair<std::vector<Card>, std::vector<Card>> MonteCarloSimulator::dealRandomOpponentAndBoard(
    const std::vector<Card> &deck) const
{
    std::vector<Card> opponentHand;
    std::vector<Card> completeBoard = communityCards;

    // Deal opponent's two cards if they don't already have them
    size_t index = 0;
    while (opponentHand.size() < 2 && index < deck.size())
    {
        opponentHand.push_back(deck[index++]);
    }

    // Complete the board to 5 cards if needed
    while (completeBoard.size() < 5 && index < deck.size())
    {
        completeBoard.push_back(deck[index++]);
    }

    return {opponentHand, completeBoard};
}