#include "hand_evaluator.h"
#include <map>
#include <algorithm>

// Convert Rank enum to int (for comparison)
static int rankToInt(Rank r)
{
    return static_cast<int>(r);
}

// Sort descending helper
static bool rankCompare(const Card &a, const Card &b)
{
    return rankToInt(a.rank) > rankToInt(b.rank);
}

bool isStraight(const std::vector<int> &ranks, int &highCard)
{
    std::vector<int> sorted = ranks;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    // Ace-low straight (A = 14 or 1)
    if (std::find(sorted.begin(), sorted.end(), 14) != sorted.end())
        sorted.insert(sorted.begin(), 1); // treat Ace as 1

    int count = 1;
    for (size_t i = 1; i < sorted.size(); ++i)
    {
        if (sorted[i] == sorted[i - 1] + 1)
        {
            count++;
            if (count >= 5)
            {
                highCard = sorted[i];
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

// Operator > to compare two hand values
bool HandValue::operator>(const HandValue &other) const
{
    if (rank != other.rank)
        return static_cast<int>(rank) > static_cast<int>(other.rank);

    // Compare kickers
    for (size_t i = 0; i < kickers.size() && i < other.kickers.size(); ++i)
    {
        if (kickers[i] != other.kickers[i])
            return kickers[i] > other.kickers[i];
    }
    return false; // Tie
}

bool HandValue::operator==(const HandValue &other) const
{
    return rank == other.rank && kickers == other.kickers;
}

// Evaluate hand from 7 cards
HandValue evaluateHand(const std::vector<Card> &cards)
{
    std::map<int, int> rankCount;
    for (const Card &c : cards)
    {
        rankCount[rankToInt(c.rank)]++;
    }

    // Count groups by size
    std::vector<int> pairs, trips, quads, singles;
    for (const auto &entry : rankCount)
    {
        int r = entry.first;
        int count = entry.second;
        if (count == 4)
            quads.push_back(r);
        else if (count == 3)
            trips.push_back(r);
        else if (count == 2)
            pairs.push_back(r);
        else
            singles.push_back(r);
    }

    // Sort for consistent tiebreakers
    std::sort(pairs.rbegin(), pairs.rend());
    std::sort(trips.rbegin(), trips.rend());
    std::sort(quads.rbegin(), quads.rend());
    std::sort(singles.rbegin(), singles.rend());

    // === Check Hands ===
    // Four of a Kind
    if (!quads.empty())
    {
        return HandValue{
            HandRank::FourOfAKind,
            {quads[0], singles.empty() ? 0 : singles[0]}};
    }

    // Full House (trip + pair) — coming soon

    // Count suits
    std::map<Suit, std::vector<int>> suitGroups;
    for (const Card &c : cards)
    {
        suitGroups[c.suit].push_back(rankToInt(c.rank));
    }

    // Check for flush
    Suit flushSuit;
    bool hasFlush = false;
    std::vector<int> flushRanks;

    for (const auto &entry : suitGroups)
    {
        if (entry.second.size() >= 5)
        {
            hasFlush = true;
            flushSuit = entry.first;
            flushRanks = entry.second;
            std::sort(flushRanks.rbegin(), flushRanks.rend());
            flushRanks.resize(5);
            break;
        }
    }

    if (hasFlush)
    {
        return HandValue{HandRank::Flush, flushRanks};
    }

    // Build rank list
    std::vector<int> allRanks;
    for (const Card &c : cards)
    {
        allRanks.push_back(rankToInt(c.rank));
    }
    std::sort(allRanks.rbegin(), allRanks.rend());

    int straightHigh = 0;
    if (isStraight(allRanks, straightHigh))
    {
        return HandValue{HandRank::Straight, {straightHigh}};
    }

    // Three of a Kind
    if (!trips.empty())
    {
        std::vector<int> kickers = {trips[0]};
        if (singles.size() >= 2)
        {
            kickers.push_back(singles[0]);
            kickers.push_back(singles[1]);
        }
        return HandValue{HandRank::ThreeOfAKind, kickers};
    }

    // Two Pair
    if (pairs.size() >= 2)
    {
        return HandValue{
            HandRank::TwoPair,
            {pairs[0], pairs[1], singles.empty() ? 0 : singles[0]}};
    }

    // One Pair
    if (pairs.size() == 1)
    {
        std::vector<int> kickers = {pairs[0]};
        for (int i = 0; i < 3 && i < (int)singles.size(); ++i)
        {
            kickers.push_back(singles[i]);
        }
        return HandValue{HandRank::OnePair, kickers};
    }

    // High Card
    std::vector<int> topCards;
    for (int i = 0; i < 5 && i < (int)singles.size(); ++i)
    {
        topCards.push_back(singles[i]);
    }
    return HandValue{HandRank::HighCard, topCards};
}