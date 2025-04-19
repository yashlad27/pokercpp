#include "bot_player.h"
#include "advanced_hand_evaluator.h"
#include <cstdlib>

BotPlayer::BotPlayer(const std::string& name, int chips, BotDifficulty diff) 
    : Player(name, chips), difficulty(diff) {}

BotDifficulty BotPlayer::getDifficulty() const {
    return difficulty;
}

bool BotPlayer::shouldCallBet(const std::vector<Card>& fullHand) {
    HandValue eval = AdvancedHandEvaluator::evaluate(fullHand);

    switch(difficulty) {
        case BotDifficulty::Easy:
            return (rand()%4 == 0);         // 25% chance
        case BotDifficulty::Medium:
            return eval.rank >= HandRank::OnePair;
        case BotDifficulty::Hard:
            if(eval.rank >= HandRank::OnePair) return true;
            return (rand() % 5 == 0);       // 20% bluff
    }
    return false;
}