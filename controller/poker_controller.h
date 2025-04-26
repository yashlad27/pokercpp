#ifndef POKER_CONTROLLER_H
#define POKER_CONTROLLER_H

#include "../model/player.h"

class PokerController
{
public:
    void runGame();

private:
    void playRound(Player &human, Player &bot);
    bool handleBetting(Player &human, Player &bot, const std::vector<Card> &community, GameStage stage);
    void showdown(Player &human, Player &bot, const std::vector<Card> &community);
};

#endif // Poker_CONTROLLER_H