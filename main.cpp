#include <iostream>
#include <string>
#include "deck.h"
#include "player.h"

#include "hand_evaluator.h"
#include "advanced_hand_evaluator.h"

void displayChipCounts(const Player& p1, const Player& p2) {
    std::cout << p1.getName() << ": " << p1.getChipCount() << " chips\t";
    std::cout << p2.getName() << ": " << p2.getChipCount() << " chips\n";
}

// Helper to convert enum to readable text
std::string handRankToString(HandRank rank) {
    switch(rank) {
        case HandRank::HighCard: return "High Card";
        case HandRank::OnePair: return "One Pair";
        case HandRank::TwoPair: return "Two Pair";
        case HandRank::ThreeOfAKind: return "Three of a Kind";
        case HandRank::Straight: return "Straight";
        case HandRank::Flush: return "Flush";
        case HandRank::FullHouse: return "Full House";
        case HandRank::FourOfAKind: return "Four of a Kind";
        case HandRank::StraightFlush: return "Straight Flush";
        case HandRank::RoyalFlush: return "Royal Flush";
        default: return "Unknown";
    }
}

std::vector<Card> getCombinedHand(const Player& player, const std::vector<Card>& community) {
    std::vector<Card> fullHand = player.getHand();
    fullHand.insert(fullHand.end(), community.begin(), community.end());
    return fullHand;
}

void waitForEnter() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore();
}

int main() {
    std::cout << "🃏 Welcome to Texas Hold'em (CLI Version)\n";

    Deck deck;
    Player human("You", 1000);
    Player bot("Bot", 1000);

    std::cout << "\n--- Current Chip Counts ---\n";
    displayChipCounts(human, bot);

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
        std::cout << "Bot's hand: ";
        bot.showHand(true);
        return 0;
    } else if (action == "bet") {
        human.bet(100);
        // std::cout << "Bot calls your bet.\n";

        std::cout << "Bot is thinking...\n";

        HandValue botEval = AdvancedHandEvaluator::evaluate(getCombinedHand(bot, community));
        if (static_cast<int>(botEval.rank) >= static_cast<int>(HandRank::OnePair)) {
            std::cout << "Bot calls your bet.\n";
            bot.bet(100);
        } else {
            std::cout << "Bot folds.\n";
        return 0;
        }

        std::cout << "Bot's hand: ";
        bot.showHand(true); 
        bot.bet(100);
    } else {
        std::cout << "You checked. Bot checks.\n";
        std::cout << "Bot's hand: ";
        bot.showHand(true); 
    }

    // === Showdown ===
    auto humanFullHand = getCombinedHand(human, community);
    auto botFullHand = getCombinedHand(bot, community);

    HandValue hv1 = AdvancedHandEvaluator::evaluate(humanFullHand);
    HandValue hv2 = AdvancedHandEvaluator::evaluate(botFullHand);

    // Show hand types
    std::cout << "\nYour best hand: " << handRankToString(hv1.rank) << std::endl;
    std::cout << "Bot's best hand: " << handRankToString(hv2.rank) << std::endl;

    // Decide winner
    std::cout << "\nResult: ";
    const int pot = 200;

    if (hv1 > hv2) {
        std::cout << "You win!\n";
        human.bet(-pot);
    } else if (hv2 > hv1) {
        std::cout << "Bot wins!\n";
        bot.bet(-pot);
    } else {
        std::cout << "It's a tie!\n";
        human.bet(-100);
        bot.bet(-100);
    }

    std::cout << "\n--- Updated Chip Counts ---\n";
    displayChipCounts(human, bot);

    return 0;
}