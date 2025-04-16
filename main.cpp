#include <iostream>
#include "deck.h"
#include "player.h"

int main() {
    std::cout << "🃏 Starting a Texas Hold'em test round...\n\n";

    // initialise deck and players
    Deck deck;
    Player human("You", 1000);
    Player bot("Bot", 1000);

    human.recieveCard(deck.dealCard());
    human.recieveCard(deck.dealCard());

    bot.recieveCard(deck.dealCard());
    bot.recieveCard(deck.dealCard());
    
    std::cout << "Your Hand: ";
    human.showHand(true);

    std::cout << "Bot's Hand: ";
    bot.showHand(false); 

    std::cout << "\n===Dealing the Flop===\n";
    std::vector<Card> community;

    for(int i=0; i<3; ++i) {
        community.push_back(deck.dealCard());
    }

    std::cout << "Flop: ";
    for (const Card& c : community) {
        std::cout<<c.toString()<<" ";
    }

    std::cout << std::endl;

    std::cout << "\n===Dealing the Turn===\n";
    community.push_back(deck.dealCard());
    std::cout << "Turn: " << community[3].toString() << std::endl;

    std::cout << "\n===Dealing the River ===\n";
    community.push_back(deck.dealCard());
    std::cout << "River: " << community[4].toString() << std::endl;

    std::cout << "\n===Showdown===\n";
    std::cout << "Bot's hand revealed: ";
    bot.showHand(true);

    return 0;
}