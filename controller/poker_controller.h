#ifndef POKER_CONTROLLER_H
#define POKER_CONTROLLER_H

#include "../model/player.h"

class PokerController
{
public:
    void runGame();

private:
    void playRound(Player &human, Player &bot);
};

#endif // Poker_CONTROLLER_H