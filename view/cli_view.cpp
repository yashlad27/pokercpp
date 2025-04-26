#include "cli_view.h"
#include "ascii_art.h"
#include "../animation/card_animation.h"
#include <iostream>
#include <thread>
#include <chrono>

void CLIView::showWelcome()
{
    ASCIIArt::drawLogo();
    std::cout << "🃏 Welcome to Texas Hold'em (CLI Version)\n";
}

void CLIView::showCommunityCards(const std::vector<Card> &cards, const std::string &stage)
{
    CardAnimation::dealCommunityCards(cards, stage);
}

void CLIView::waitForEnter()
{
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore();
}

void CLIView::showChipCounts(const Player &p1, const Player &p2)
{
    ASCIIArt::drawPlayers(p1.getName(), p2.getName(), p1.getChipCount(), p2.getChipCount());
}

void CLIView::showResult(const Player &p1, const Player &p2)
{
    std::cout << "\n=== SHOWDOWN ===\n";
    ASCIIArt::drawTable(std::vector<Card>(), true, p1.getHand(), p2.getHand());
}

void CLIView::showHandType(const std::string &name, const std::string &handType)
{
    std::cout << name << "'s best hand: " << handType << "\n";
}

void CLIView::showDivider()
{
    std::cout << "\n============================\n";
}

void CLIView::showTable(const std::vector<Card> &community, const Player &human, const Player &bot, bool showBotCards)
{
    ASCIIArt::drawTable(community, showBotCards, human.getHand(), bot.getHand());
}