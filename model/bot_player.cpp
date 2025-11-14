#include "bot_player.h"
#include "advanced_hand_evaluator.h"
#include "../view/bot_thinking_visualizer.h"
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

    // Show bot thinking header
    std::string diffStr;
    switch (difficulty) {
        case BotDifficulty::Easy: diffStr = "EASY"; break;
        case BotDifficulty::Medium: diffStr = "MEDIUM"; break;
        case BotDifficulty::Hard: diffStr = "HARD"; break;
        case BotDifficulty::HardPlus: diffStr = "HARD+"; break;
    }
    BotThinkingVisualizer::showThinkingHeader(getName(), diffStr);
    
    // Show hand evaluation
    BotThinkingVisualizer::showHandEvaluation(eval, fullHand);
    
    // Show game stage
    std::string stageStr;
    switch (stage) {
        case GameStage::PreFlop: stageStr = "Pre-Flop"; break;
        case GameStage::Flop: stageStr = "Flop"; break;
        case GameStage::Turn: stageStr = "Turn"; break;
        case GameStage::River: stageStr = "River"; break;
    }
    
    // Calculate hand strength for decision factors
    double handStrength = 0.0;
    switch (eval.rank) {
        case HandRank::RoyalFlush:     handStrength = 1.0; break;
        case HandRank::StraightFlush:  handStrength = 0.95; break;
        case HandRank::FourOfAKind:    handStrength = 0.88; break;
        case HandRank::FullHouse:      handStrength = 0.78; break;
        case HandRank::Flush:          handStrength = 0.68; break;
        case HandRank::Straight:       handStrength = 0.58; break;
        case HandRank::ThreeOfAKind:   handStrength = 0.45; break;
        case HandRank::TwoPair:        handStrength = 0.35; break;
        case HandRank::OnePair:        handStrength = 0.22; break;
        case HandRank::HighCard:       handStrength = 0.08; break;
    }
    
    bool hasDraws = hasDrawingHand(fullHand);
    BotThinkingVisualizer::showDecisionFactors(stageStr, eval, hasDraws, handStrength);

    bool decision;
    switch (difficulty) {
        case BotDifficulty::Easy:
            decision = shouldCallEasy();
            break;
        case BotDifficulty::Medium:
            decision = shouldCallMedium(eval, stage, fullHand);
            break;
        case BotDifficulty::Hard:
            decision = shouldCallHard(eval, stage, fullHand);
            break;
        case BotDifficulty::HardPlus:
            decision = shouldCallHardPlus(fullHand);
            break;
        default:
            decision = false;
    }
    
    return decision;
}

bool BotPlayer::shouldCallEasy() const {
    bool willCall = (rand() % 4 == 0);  // 25% chance
    
    std::string reasoning = "Random decision (25% chance to call)";
    BotThinkingVisualizer::showFinalDecision(willCall, reasoning);
    
    return willCall;
}

bool BotPlayer::shouldCallMedium(const HandValue& eval, GameStage stage, const std::vector<Card>& fullHand) const {
    std::string reasoning;
    bool decision = false;
    
    // Base decision on hand strength and game stage
    if (eval.rank >= HandRank::ThreeOfAKind) {
        decision = true;
        reasoning = "Strong hand (Three of a Kind or better) - Always call";
        BotThinkingVisualizer::showFinalDecision(decision, reasoning);
        return decision;
    }
    
    if (eval.rank >= HandRank::OnePair) {
        decision = true;
        reasoning = "At least one pair - Calling";
        BotThinkingVisualizer::showFinalDecision(decision, reasoning);
        return decision;
    }
    
    // Check for drawing hands in earlier stages
    bool hasFlush = hasFlushDraw(fullHand);
    bool hasStraight = hasStraightDraw(fullHand);
    
    if (stage != GameStage::River && (hasFlush || hasStraight)) {
        BotThinkingVisualizer::showDrawingHandAnalysis(hasFlush, hasStraight, fullHand);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        
        // 60% chance to call with a drawing hand
        decision = dis(gen) <= 60;
        reasoning = decision ? "Drawing hand detected - Calling (60% chance)" : 
                              "Drawing hand but folding (40% chance)";
        BotThinkingVisualizer::showFinalDecision(decision, reasoning);
        return decision;
    }
    
    // Add occasional random bluffing (10% chance)
    int bluffChance = (eval.rank == HandRank::HighCard) ? 10 : 15;
    decision = shouldBluff(eval.rank);
    
    if (decision) {
        BotThinkingVisualizer::showBluffCalculation(eval.rank, bluffChance, true);
        reasoning = "Attempting a bluff with weak hand";
    } else {
        reasoning = "Weak hand, no draws - Folding";
    }
    
    BotThinkingVisualizer::showFinalDecision(decision, reasoning);
    return decision;
}

bool BotPlayer::shouldCallHard(const HandValue& eval, GameStage stage, const std::vector<Card>& fullHand) const {
    std::string reasoning;
    bool decision = false;
    
    // More sophisticated strategy for Hard bot
    if (eval.rank >= HandRank::TwoPair) {
        decision = true;
        reasoning = "Strong hand (Two Pair or better) - Always call";
        BotThinkingVisualizer::showFinalDecision(decision, reasoning);
        return decision;
    }
    
    if (eval.rank >= HandRank::OnePair) {
        // With one pair, call most of the time (80%)
        decision = (rand() % 5 != 0);
        reasoning = decision ? "One pair - Calling (80% chance)" : 
                              "One pair but folding (20% chance)";
        BotThinkingVisualizer::showFinalDecision(decision, reasoning);
        return decision;
    }
    
    // More aggressive with drawing hands
    bool hasFlush = hasFlushDraw(fullHand);
    bool hasStraight = hasStraightDraw(fullHand);
    
    if (stage != GameStage::River && (hasFlush || hasStraight)) {
        BotThinkingVisualizer::showDrawingHandAnalysis(hasFlush, hasStraight, fullHand);
        decision = true;
        reasoning = "Drawing hand detected - Always calling (aggressive)";
        BotThinkingVisualizer::showFinalDecision(decision, reasoning);
        return decision;
    }
    
    // More aggressive bluffing (20% chance with weak hands)
    int bluffChance = (eval.rank == HandRank::HighCard) ? 15 : 20;
    decision = shouldBluff(eval.rank);
    
    if (decision) {
        BotThinkingVisualizer::showBluffCalculation(eval.rank, bluffChance, true);
        reasoning = "Aggressive bluff attempt";
    } else {
        reasoning = "Weak hand, no draws - Folding";
    }
    
    BotThinkingVisualizer::showFinalDecision(decision, reasoning);
    return decision;
}

bool BotPlayer::shouldCallHardPlus(const std::vector<Card>& fullHand) {
    // For HardPlus, we'll use Monte Carlo simulation
    
    const int simulations = 200;
    const int threads = 4;
    const int simsPerThread = simulations / threads;
    std::vector<std::future<std::pair<int, int>>> futures;

    // Show Monte Carlo simulation header
    BotThinkingVisualizer::showMonteCarloHeader(simulations);

    auto simulate = [&]() -> std::pair<int, int> {
        int wins = 0;
        int ties = 0;
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
            else if (botEval == opponentEval)
                ++ties;
        }
        return {wins, ties};
    };

    for (int i = 0; i < threads; ++i) {
        futures.push_back(std::async(std::launch::async, simulate));
    }

    int totalWins = 0;
    int totalTies = 0;
    for (auto& f : futures) {
        auto [wins, ties] = f.get();
        totalWins += wins;
        totalTies += ties;
    }
    
    int totalLosses = simulations - totalWins - totalTies;
    double winRate = static_cast<double>(totalWins) / simulations;
    
    // Show Monte Carlo results
    BotThinkingVisualizer::showMonteCarloResult(winRate, totalWins, totalLosses, totalTies, simulations);
    
    // More aggressive threshold for betting
    bool decision = winRate >= 0.4; // Call if 40% or better chance to win
    
    std::string reasoning;
    if (winRate >= 0.6) {
        reasoning = "Monte Carlo shows strong position (60%+) - Calling confidently";
    } else if (winRate >= 0.4) {
        reasoning = "Monte Carlo shows competitive position (40-60%) - Calling";
    } else {
        reasoning = "Monte Carlo shows weak position (<40%) - Folding";
    }
    
    BotThinkingVisualizer::showFinalDecision(decision, reasoning);
    
    return decision;
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