// advanced_hand_evaluator.cpp
#include "advanced_hand_evaluator.h"
#include <map>
#include <algorithm>
#include <stdexcept>

/**
 * Convert Rank enum to integer value for comparison
 */
static int rankToInt(Rank r)
{
    return static_cast<int>(r);
}

/**
 * Check if the given ranks form a straight
 * 
 * @param sortedRanks Vector of card ranks (ints)
 * @param highCard Output parameter: will contain the high card of the straight
 * @return true if the cards form a straight
 */
static bool isStraight(const std::vector<int> &sortedRanks, int &highCard)
{
    // Create a set of unique ranks
    std::vector<int> uniqueRanks = sortedRanks;
    std::sort(uniqueRanks.begin(), uniqueRanks.end());
    uniqueRanks.erase(
        std::unique(uniqueRanks.begin(), uniqueRanks.end()), uniqueRanks.end());

    // Handle Ace-low straight (A-2-3-4-5)
    if (std::find(uniqueRanks.begin(), uniqueRanks.end(), 14) != uniqueRanks.end())
    {
        uniqueRanks.insert(uniqueRanks.begin(), 1); // add ace as 1
    }

    // Look for 5 consecutive ranks
    int count = 1;
    for (size_t i = 1; i < uniqueRanks.size(); ++i)
    {
        if (uniqueRanks[i] == uniqueRanks[i - 1] + 1)
        {
            count++;
            if (count >= 5)
            {
                highCard = uniqueRanks[i];
                return true;
            }
        }
        else
        {
            count = 1;
        }
    }

    return false;
}

/**
 * Evaluate a poker hand and return its rank and kickers
 * 
 * This evaluator can handle more than 5 cards (e.g., 7 cards in Texas Hold'em)
 * and will find the best 5-card hand.
 * 
 * @param cards Vector of Card objects
 * @return HandValue containing the hand rank and kickers for tie-breaking
 * @throws std::invalid_argument if fewer than 5 cards are provided
 */
HandValue AdvancedHandEvaluator::evaluate(const std::vector<Card> &cards)
{
    // Validate input
    if (cards.size() < 5)
    {
        throw std::invalid_argument("At least 5 cards are required for hand evaluation");
    }

    // Count occurrences of each rank
    std::map<int, int> rankCount;
    std::map<Suit, std::vector<Card>> suitGroups;

    // Gather all ranks and group by suit
    std::vector<int> allRanks;
    for (const Card &c : cards)
    {
        int val = rankToInt(c.rank);
        allRanks.push_back(val);
        rankCount[val]++;
        suitGroups[c.suit].push_back(c);
    }

    // Categorize cards by frequency (1, 2, 3, or 4 of a kind)
    std::vector<int> singles, pairs, trips, quads;
    for (auto &[rank, count] : rankCount)
    {
        if (count == 4)
        {
            quads.push_back(rank);
        }
        else if (count == 3)
        {
            trips.push_back(rank);
        }
        else if (count == 2)
        {
            pairs.push_back(rank);
        }
        else
        {
            singles.push_back(rank);
        }
    }

    // Sort by rank (highest first) for consistent tie-breaking
    std::sort(pairs.rbegin(), pairs.rend());
    std::sort(trips.rbegin(), trips.rend());
    std::sort(quads.rbegin(), quads.rend());
    std::sort(singles.rbegin(), singles.rend());

    // Check for Straight Flush or Royal Flush
    for (auto &[suit, suitedCards] : suitGroups)
    {
        if (suitedCards.size() < 5)
            continue;
            
        std::vector<int> suitedRanks;
        for (const Card &c : suitedCards)
        {
            suitedRanks.push_back(rankToInt(c.rank));
        }

        int sfHighCard = 0;
        if (isStraight(suitedRanks, sfHighCard))
        {
            if (sfHighCard == 14) // A-K-Q-J-10
                return HandValue{HandRank::RoyalFlush, {14}};
            else
                return HandValue{HandRank::StraightFlush, {sfHighCard}};
        }
    }

    // Check for Four of a Kind
    if (!quads.empty())
    {
        // Highest card not in the quads as kicker
        int kicker = singles.empty() ? (trips.empty() ? 0 : trips[0]) : singles[0];
        return HandValue{
            HandRank::FourOfAKind, {quads[0], kicker}};
    }

    // Check for Full House (three of a kind + pair)
    if (!trips.empty() && (!pairs.empty() || trips.size() > 1))
    {
        int trip = trips[0];  // Highest three of a kind
        // Either another three of a kind, or highest pair
        int pair = (trips.size() > 1) ? trips[1] : pairs[0];
        return HandValue{HandRank::FullHouse, {trip, pair}};
    }

    // Check for Flush
    for (auto &[suit, suitedCards] : suitGroups)
    {
        if (suitedCards.size() >= 5)
        {
            std::vector<int> flushRanks;
            for (Card c : suitedCards)
                flushRanks.push_back(rankToInt(c.rank));
            std::sort(flushRanks.rbegin(), flushRanks.rend());
            flushRanks.resize(5);  // Keep top 5 cards of the flush
            return HandValue{HandRank::Flush, flushRanks};
        }
    }

    // Check for Straight
    int straightHigh = 0;
    if (isStraight(allRanks, straightHigh))
    {
        return HandValue{HandRank::Straight, {straightHigh}};
    }

    // Check for Three of a Kind
    if (!trips.empty())
    {
        std::vector<int> kickers;
        // Add singles and pairs as kickers
        for (int r : singles)
            kickers.push_back(r);
        for (int r : pairs)
            kickers.push_back(r);
        // Ensure we have at least 2 kickers (even if 0)
        while (kickers.size() < 2)
            kickers.push_back(0);
        return HandValue{HandRank::ThreeOfAKind, {trips[0], kickers[0], kickers[1]}};
    }

    // Check for Two Pair
    if (pairs.size() >= 2)
    {
        int kicker = singles.empty() ? 0 : singles[0];
        return HandValue{HandRank::TwoPair, {pairs[0], pairs[1], kicker}};
    }

    // Check for One Pair
    if (pairs.size() == 1)
    {
        std::vector<int> kickers = {pairs[0]};
        for (int i = 0; i < 3 && i < (int)singles.size(); ++i)
            kickers.push_back(singles[i]);
        return HandValue{HandRank::OnePair, kickers};
    }

    // High Card (default)
    std::sort(allRanks.rbegin(), allRanks.rend());
    allRanks.resize(5);  // Take top 5 cards
    return HandValue{HandRank::HighCard, allRanks};
}