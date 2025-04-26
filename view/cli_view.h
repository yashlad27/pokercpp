#ifndef CLI_VIEW_H
#define CLI_VIEW_H

#include <vector>
#include <string>
#include "../model/card.h"
#include "../model/player.h"

class CLIView
{
public:
    static void showWelcome();
    static void showCommunityCards(const std::vector<Card> &cards, const std::string &stage);
    static void waitForEnter();
    static void showChipCounts(const Player &p1, const Player &p2);
    static void showResult(const Player &p1, const Player &p2);
    static void showHandType(const std::string &name, const std::string &handType);
    static void showDivider();
    static void showTable(const std::vector<Card> &community, const Player &human, const Player &bot, bool showBotCards = false);
};

#endif // CLI_VIEW_H