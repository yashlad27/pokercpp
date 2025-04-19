#include "cli_view.h"
#include <iostream>
#include <thread>
#include <chrono>

void CLIView::showWelcome()
{
    std::cout << "🃏 Welcome to Texas Hold'em (CLI Version)\n";
}

void CLIView::showCommunityCards(const std::vector<Card> &cards, const std::string &stage)
{
    std::cout << "\n=== " << stage << " ===\n";
    std::cout << "Community Cards: ";
    for (const Card &c : cards)
    {
        std::cout << c.toString() << " ";
    }
    std::cout << std::endl;
}

void CLIView::waitForEnter()
{
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore();
}

void CLIView::showChipCounts(const Player &p1, const Player &p2)
{
    std::cout << p1.getName() << ": " << p1.getChipCount() << " chips\t";
    std::cout << p2.getName() << ": " << p2.getChipCount() << " chips\n";
}

void CLIView::showResult(const Player &p1, const Player &p2)
{
    std::cout << "\nYour hand: ";
    p1.showHand(true);
    std::cout << "Bot's hand: ";
    p2.showHand(true);
}

void CLIView::showHandType(const std::string &name, const std::string &handType)
{
    std::cout << name << "'s best hand: " << handType << "\n";
}

void CLIView::showDivider()
{
    std::cout << "\n============================\n";
}