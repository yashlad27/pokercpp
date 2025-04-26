#include "bot_player.h"
#include "advanced_hand_evaluator.h"
#include <cstdlib>
#include <future>
#include <random>
#include <algorithm>
#include <chrono>
#include <map>
#include <iostream>

BotPlayer::BotPlayer(const std::string &name, int chips, BotDifficulty diff)
    : Player(name, chips), difficulty(diff) {}

BotDifficulty BotPlayer::getDifficulty() const {
    return difficulty;
}

bool BotPlayer::shouldCallBet(const std::vector<Card>& fullHand, GameStage stage) {
    HandValue eval = AdvancedHandEvaluator::evaluate(fullHand);

    // Add logging for better visualization
    std::cout << "Bot has: " << static_cast<int>(eval.rank) << " (";
    switch(eval.rank) {
        case HandRank::HighCard: std::cout << "High Card"; break;
        case HandRank::OnePair: std::cout << "One Pair"; break;
        case HandRank::TwoPair: std::cout << "Two Pair"; break;
        case HandRank::ThreeOfAKind: std::cout << "Three of a Kind"; break;
        case HandRank::Straight: std::cout << "Straight"; break;
        case HandRank::Flush: std::cout << "Flush"; break;
        case HandRank::FullHouse: std::cout << "Full House"; break;
        case HandRank::FourOfAKind: std::cout << "Four of a Kind"; break;
        case HandRank::StraightFlush: std::cout << "Straight Flush"; break;
        case HandRank::RoyalFlush: std::cout << "Royal Flush"; break;
    }
    std::cout << ")" << std::endl;

    switch (difficulty) {
        case BotDifficulty::Easy:
            return shouldCallEasy();
        case BotDifficulty::Medium:
            return shouldCallMedium(eval, stage, fullHand);
        case BotDifficulty::Hard:
            return shouldCallHard(eval, stage, fullHand);
        case BotDifficulty::HardPlus:
            return shouldCallHardPlus(fullHand);
    }
    return false;
}

bool BotPlayer::shouldCallEasy() const {
    return (rand() % 4 == 0);  // 25% chance
}

bool BotPlayer::shouldCallMedium(const HandValue& eval, GameStage stage, const std::vector<Card>& fullHand) const {
    // Base decision on hand strength and game stage
    if (eval.rank >= HandRank::ThreeOfAKind) {
        return true; // Strong hand, always call
    }
    
    if (eval.rank >= HandRank::OnePair) {
        return true; // At least a pair, call
    }
    
    // Check for drawing hands in earlier stages
    if (stage != GameStage::River && hasDrawingHand(fullHand)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        
        // 60% chance to call with a drawing hand
        return dis(gen) <= 60;
    }
    
    // Add occasional random bluffing (10% chance)
    return shouldBluff(eval.rank);
}

bool BotPlayer::shouldCallHard(const HandValue& eval, GameStage stage, const std::vector<Card>& fullHand) const {
    // More sophisticated strategy for Hard bot
    if (eval.rank >= HandRank::TwoPair) {
        return true; // Strong hand, always call
    }
    
    if (eval.rank >= HandRank::OnePair) {
        // With one pair, call most of the time (80%)
        return (rand() % 5 != 0);
    }
    
    // More aggressive with drawing hands
    if (stage != GameStage::River && hasDrawingHand(fullHand)) {
        return true; // Always call with drawing hands
    }
    
    // More aggressive bluffing (20% chance with weak hands)
    return shouldBluff(eval.rank);
}

bool BotPlayer::shouldCallHardPlus(const std::vector<Card>& fullHand) {
    // For HardPlus, we'll use Monte Carlo simulation on river
    // For earlier stages, we'll use a more sophisticated strategy
    
    const int simulations = 200;
    const int threads = 4;
    const int simsPerThread = simulations / threads;
    std::vector<std::future<int>> futures;

    auto simulate = [&]() -> int {
        int wins = 0;
        for (int i = 0; i < simsPerThread; ++i) {
            std::vector<Card> deck;
            for (int r = 2; r <= 14; ++r) {
                for (int s = 0; s < 4; ++s) {
                    Card c(static_cast<Rank>(r), static_cast<Suit>(s));
                    if (std::find(fullHand.begin(), fullHand.end(), c) == fullHand.end())
                        deck.push_back(c);
                }
            }
            std::shuffle(deck.begin(), deck.end(), std::mt19937(std::random_device()()));

            std::vector<Card> opponentHand = { deck[0], deck[1] };
            std::vector<Card> opponentFullHand = opponentHand;
            opponentFullHand.insert(opponentFullHand.end(), fullHand.begin() + 2, fullHand.end());

            HandValue opponentEval = AdvancedHandEvaluator::evaluate(opponentFullHand);
            HandValue botEval = AdvancedHandEvaluator::evaluate(fullHand);

            if (botEval > opponentEval)
                ++wins;
        }
        return wins;
    };

    for (int i = 0; i < threads; ++i) {
        futures.push_back(std::async(std::launch::async, simulate));
    }

    int totalWins = 0;
    for (auto& f : futures) {
        totalWins += f.get();
    }

    double winRate = static_cast<double>(totalWins) / simulations;
    std::cout << "Bot estimates " << (winRate * 100) << "% chance to win" << std::endl;
    
    // More aggressive threshold for betting
    return winRate >= 0.4; // Call if 40% or better chance to win
}

// New helper methods for hand evaluation
bool BotPlayer::hasDrawingHand(const std::vector<Card>& fullHand) const {
    return hasFlushDraw(fullHand) || hasStraightDraw(fullHand);
}

bool BotPlayer::hasFlushDraw(const std::vector<Card>& fullHand) const {
    // Count cards by suit
    std::map<Suit, int> suitCount;
    for (const Card& c : fullHand) {
        suitCount[c.suit]++;
    }
    
    // 4 cards of same suit is a flush draw
    for (const auto& [suit, count] : suitCount) {
        if (count == 4) {
            return true;
        }
    }
    return false;
}

bool BotPlayer::hasStraightDraw(const std::vector<Card>& fullHand) const {
    std::vector<int> ranks;
    for (const Card& c : fullHand) {
        ranks.push_back(static_cast<int>(c.rank));
    }
    
    // Get unique ranks
    std::sort(ranks.begin(), ranks.end());
    auto last = std::unique(ranks.begin(), ranks.end());
    ranks.erase(last, ranks.end());
    
    // Check for 4 sequential cards (open-ended straight draw)
    int consecutive = 1;
    int maxConsecutive = 1;
    
    for (size_t i = 1; i < ranks.size(); ++i) {
        if (ranks[i] == ranks[i-1] + 1) {
            consecutive++;
            maxConsecutive = std::max(maxConsecutive, consecutive);
        } else if (ranks[i] > ranks[i-1] + 1) {
            consecutive = 1;
        }
    }
    
    // Also check for gutshot straight draws (3 sequential cards with a gap)
    bool hasGutshot = false;
    for (size_t i = 0; i < ranks.size() - 3; ++i) {
        if ((ranks[i+1] == ranks[i] + 1) && 
            (ranks[i+2] == ranks[i] + 2) && 
            (ranks[i+3] == ranks[i] + 3)) {
            hasGutshot = true;
            break;
        }
    }
    
    return (maxConsecutive >= 4) || hasGutshot;
}

// Bluffing logic
bool BotPlayer::shouldBluff(HandRank handRank) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int bluffChance;
    switch (handRank) {
        case HandRank::HighCard:
            bluffChance = (difficulty == BotDifficulty::Hard) ? 15 : 10; 
            break;
        case HandRank::OnePair:
            bluffChance = (difficulty == BotDifficulty::Hard) ? 20 : 15;
            break;
        default:
            bluffChance = 0; // No need to bluff with better hands
    }
    
    std::uniform_int_distribution<> dis(1, 100);
    return dis(gen) <= bluffChance;
}