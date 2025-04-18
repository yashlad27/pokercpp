#include "hand_evaluator.h"
#include <map>
#include <algorithm>

// Convert Rank enum to int (for comparison)
static int rankToInt(Rank r) {
    return static_cast<int>(r);
}

// Sort descending helper
static bool rankCompare(const Card& a, const Card& b) {
    return rankToInt(a.rank) > rankToInt(b.rank);
}

// Operator > to compare two hand values
bool HandValue::operator>(const HandValue& other) const {
    if (rank != other.rank)
        return static_cast<int>(rank) > static_cast<int>(other.rank);

    // Compare kickers
    for (size_t i = 0; i < kickers.size() && i < other.kickers.size(); ++i) {
        if (kickers[i] != other.kickers[i])
            return kickers[i] > other.kickers[i];
    }
    return false; // Tie
}

bool HandValue::operator==(const HandValue& other) const {
    return rank == other.rank && kickers == other.kickers;
}

// Evaluate hand from 7 cards
HandValue evaluateHand(const std::vector<Card>& cards) {
    std::map<int, int> rankCount;
    for (const Card& c : cards) {
        rankCount[rankToInt(c.rank)]++;
    }

    // Count groups by size
    std::vector<int> pairs, trips, quads, singles;
    for (const auto& entry : rankCount) {
        int r = entry.first;
        int count = entry.second;
        if (count == 4) quads.push_back(r);
        else if (count == 3) trips.push_back(r);
        else if (count == 2) pairs.push_back(r);
        else singles.push_back(r);
    }

    // Sort for consistent tiebreakers
    std::sort(pairs.rbegin(), pairs.rend());
    std::sort(trips.rbegin(), trips.rend());
    std::sort(quads.rbegin(), quads.rend());
    std::sort(singles.rbegin(), singles.rend());

    // === Check Hands ===
    // Four of a Kind
    if (!quads.empty()) {
        return HandValue{
            HandRank::FourOfAKind,
            { quads[0], singles.empty() ? 0 : singles[0] }
        };
    }

    // Full House (trip + pair) — coming soon

    // Three of a Kind
    if (!trips.empty()) {
        std::vector<int> kickers = { trips[0] };
        if (singles.size() >= 2) {
            kickers.push_back(singles[0]);
            kickers.push_back(singles[1]);
        }
        return HandValue{ HandRank::ThreeOfAKind, kickers };
    }

    // Two Pair
    if (pairs.size() >= 2) {
        return HandValue{
            HandRank::TwoPair,
            { pairs[0], pairs[1], singles.empty() ? 0 : singles[0] }
        };
    }

    // One Pair
    if (pairs.size() == 1) {
        std::vector<int> kickers = { pairs[0] };
        for (int i = 0; i < 3 && i < (int)singles.size(); ++i) {
            kickers.push_back(singles[i]);
        }
        return HandValue{ HandRank::OnePair, kickers };
    }

    // High Card
    std::vector<int> topCards;
    for (int i = 0; i < 5 && i < (int)singles.size(); ++i) {
        topCards.push_back(singles[i]);
    }
    return HandValue{ HandRank::HighCard, topCards };
}