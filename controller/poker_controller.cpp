#include "poker_controller.h"
#include "../model/deck.h"
#include "../model/player.h"
#include "../model/advanced_hand_evaluator.h"
#include "../view/cli_view.h"

#include <iostream>
#include <vector>
#include <string>

std::vector<Card> getCombinedHand(const Player& player, const std::vector<Card>& community) {
    std::vector<Card> fullHand = player.getHand();
    fullHand.insert(fullHand.end(), community.begin(), community.end());
    return fullHand;
}

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

void GameController::runGame() {
    CLIView::showWelcome();

    Deck deck;
    Player human("You", 1000);
    Player bot("Bot", 1000);

    CLIView::showDivider();
    CLIView::showChipCounts(human, bot);
    CLIView::showDivider();

    // Deal hole cards
    human.recieveCard(deck.dealCard());
    human.recieveCard(deck.dealCard());
    bot.recieveCard(deck.dealCard());
    bot.recieveCard(deck.dealCard());

    std::cout << "\nYour Hand: ";
    human.showHand(true);
    std::cout << "Bot's Hand: ";
    bot.showHand(false);

    CLIView::waitForEnter();

    // Flop
    std::vector<Card> community;
    for (int i = 0; i < 3; ++i)
        community.push_back(deck.dealCard());
    CLIView::showCommunityCards(community, "Flop");
    CLIView::waitForEnter();

    // Turn
    community.push_back(deck.dealCard());
    CLIView::showCommunityCards(community, "Turn");
    CLIView::waitForEnter();

    // River
    community.push_back(deck.dealCard());
    CLIView::showCommunityCards(community, "River");
    CLIView::waitForEnter();

    // === Player Action ===
    std::cout << "\nWhat do you want to do? (check / bet / fold): ";
    std::string action;
    std::cin >> action;

    if (action == "fold") {
        std::cout << "You folded. Bot wins the round.\n";
        std::cout << "Bot's hand: ";
        bot.showHand(true);
        return;
    }

    if (action == "bet") {
        human.bet(100);
        std::cout << "Bot is thinking...\n";

        auto botEval = AdvancedHandEvaluator::evaluate(getCombinedHand(bot, community));
        if (botEval.rank >= HandRank::OnePair) {
            std::cout << "Bot calls your bet.\n";
            bot.bet(100);
        } else {
            std::cout << "Bot folds.\n";
            std::cout << "Bot's hand: ";
            bot.showHand(true);
            return;
        }
    } else {
        std::cout << "You checked. Bot checks.\n";
        std::cout << "Bot's hand: ";
        bot.showHand(true);
    }

    // === Showdown ===
    CLIView::showResult(human, bot);

    auto humanFull = getCombinedHand(human, community);
    auto botFull   = getCombinedHand(bot, community);

    HandValue hv1 = AdvancedHandEvaluator::evaluate(humanFull);
    HandValue hv2 = AdvancedHandEvaluator::evaluate(botFull);

    CLIView::showHandType(human.getName(), handRankToString(hv1.rank));
    CLIView::showHandType(bot.getName(), handRankToString(hv2.rank));

    const int pot = 200;

    std::cout << "\nResult: ";
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

    CLIView::showDivider();
    CLIView::showChipCounts(human, bot);
    CLIView::showDivider();
}