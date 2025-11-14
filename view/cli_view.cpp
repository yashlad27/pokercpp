#include "cli_view.h"
#include "ascii_art.h"
#include "../animation/card_animation.h"
#include <iostream>
#include <thread>
#include <chrono>

// ANSI color codes
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define WHITE "\033[37m"

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
    std::cout << "\n" << YELLOW << "⏸  Press ENTER to continue..." << RESET;
    std::cin.ignore();
}

void CLIView::showChipCounts(const Player &p1, const Player &p2)
{
    ASCIIArt::drawPlayers(p1.getName(), p2.getName(), p1.getChipCount(), p2.getChipCount());
}

void CLIView::showResult(const Player &p1, const Player &p2, const std::vector<Card> &community)
{
    std::cout << "\n" << BOLD << YELLOW << "╔════════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << YELLOW << "║" << RESET << "            " << BOLD << CYAN << "🎴 SHOWDOWN 🎴" << RESET << "                    " << BOLD << YELLOW << "║" << RESET << "\n";
    std::cout << BOLD << YELLOW << "╚════════════════════════════════════════════════════╝" << RESET << "\n\n";
    ASCIIArt::drawTable(community, true, p1.getHand(), p2.getHand());
}

void CLIView::showHandType(const std::string &name, const std::string &handType)
{
    std::string color = (name == "You") ? GREEN : CYAN;
    std::cout << BOLD << color << name << RESET << "'s best hand: " << BOLD << MAGENTA << handType << RESET << "\n";
}

void CLIView::showDivider()
{
    std::cout << "\n" << CYAN;
    for (int i = 0; i < 60; i++) std::cout << "=";
    std::cout << RESET << "\n";
}

void CLIView::showTable(const std::vector<Card> &community, const Player &human, const Player &bot, bool showBotCards)
{
    ASCIIArt::drawTable(community, showBotCards, human.getHand(), bot.getHand());
}