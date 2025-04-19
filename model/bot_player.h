#ifndef BOT_PLAYER_H
#define BOT_PLAYER_H

#include "player.h"
#include "hand_evaluator.h"
#include "advanced_hand_evaluator.h"
#include <vector>
#include <string>

enum class BotDifficulty
{
    Easy,
    Medium,
    Hard
};

class BotPlayer : public Player
{
private:
    BotDifficulty difficulty;

public:
    BotPlayer(const std::string &name, int chips, BotDifficulty diff);

    BotDifficulty getDifficulty() const;

    bool shouldCallBet(const std::vector<Card> &fullHand);
};

#endif