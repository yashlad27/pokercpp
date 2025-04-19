#include "bot_player.h"
#include "advanced_hand_evaluator.h"
#include <cstdlib>
#include <future>
#include <random>

BotPlayer::BotPlayer(const std::string &name, int chips, BotDifficulty diff)
    : Player(name, chips), difficulty(diff) {}

BotDifficulty BotPlayer::getDifficulty() const
{
    return difficulty;
}

bool BotPlayer::shouldCallBet(const std::vector<Card>& fullHand) {
    HandValue eval = AdvancedHandEvaluator::evaluate(fullHand);

    switch (difficulty) {
        case BotDifficulty::Easy:
            return shouldCallEasy();
        case BotDifficulty::Medium:
            return shouldCallMedium(eval);
        case BotDifficulty::Hard:
            return shouldCallHard(eval);
        case BotDifficulty::HardPlus:
            return shouldCallHardPlus(fullHand);
    }

    return false;
}

bool BotPlayer::shouldCallEasy() const {
    return (rand() % 4 == 0);  // 25% chance
}

bool BotPlayer::shouldCallMedium(const HandValue& eval) const {
    return eval.rank >= HandRank::OnePair;
}

bool BotPlayer::shouldCallHard(const HandValue& eval) const {
    if (eval.rank >= HandRank::OnePair)
        return true;
    return (rand() % 5 == 0);  // 20% bluff chance
}

// We'll build this next
bool BotPlayer::shouldCallHardPlus(const std::vector<Card>& fullHand) {
    // TODO: Monte Carlo Simulation
    return false;
}