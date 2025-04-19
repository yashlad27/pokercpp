#include "bot_player.h"
#include "advanced_hand_evaluator.h"
#include <cstdlib>
#include <future>
#include <random>
#include <algorithm>
#include <chrono>

BotPlayer::BotPlayer(const std::string &name, int chips, BotDifficulty diff)
    : Player(name, chips), difficulty(diff) {}

BotDifficulty BotPlayer::getDifficulty() const {
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

bool BotPlayer::shouldCallHardPlus(const std::vector<Card>& fullHand) {
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
    return winRate >= 0.5;
}
