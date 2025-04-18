#include <iostream>
#include <string>
#include "deck.h"
#include "player.h"

void waitForEnter() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore();
}

int main() {
    std::cout << "🃏 Welcome to Texas Hold'em (CLI Version)\n";

    Deck deck;
    Player human("You", 1000);
    Player bot("Bot", 1000);

    // Pre-flop: Deal 2 hole cards
    human.recieveCard(deck.dealCard());
    human.recieveCard(deck.dealCard());

    bot.recieveCard(deck.dealCard());
    bot.recieveCard(deck.dealCard());

    std::cout << "\nYour Hand: ";
    human.showHand(true);

    std::cout << "Bot's Hand: ";
    bot.showHand(false);

    waitForEnter();

    // === Flop ===
    std::cout << "\n=== Flop ===\n";
    std::vector<Card> community;
    for (int i = 0; i < 3; ++i) {
        community.push_back(deck.dealCard());
    }

    std::cout << "Community Cards: ";
    for (const Card& c : community) {
        std::cout << c.toString() << " ";
    }
    std::cout << std::endl;

    waitForEnter();

    // === Turn ===
    std::cout << "\n=== Turn ===\n";
    community.push_back(deck.dealCard());
    std::cout << "Community Cards: ";
    for (const Card& c : community) std::cout << c.toString() << " ";
    std::cout << std::endl;

    waitForEnter();

    // === River ===
    std::cout << "\n=== River ===\n";
    community.push_back(deck.dealCard());
    std::cout << "Community Cards: ";
    for (const Card& c : community) std::cout << c.toString() << " ";
    std::cout << std::endl;

    waitForEnter();

    // === Player Action ===
    std::cout << "\nWhat do you want to do? (check / bet / fold): ";
    std::string action;
    std::cin >> action;

    if (action == "fold") {
        std::cout << "You folded. Bot wins the round.\n";
        return 0;
    } else if (action == "bet") {
        human.bet(100);
        std::cout << "Bot calls your bet.\n";
        bot.bet(100);
    } else {
        std::cout << "You checked. Bot checks.\n";
    }

    // === Showdown ===
    std::cout << "\n=== Showdown ===\n";
    std::cout << "Your hand: ";
    human.showHand(true);

    std::cout << "Bot's hand: ";
    bot.showHand(true);

    std::cout << "\n(Winner evaluation logic coming soon...)\n";

    return 0;
}