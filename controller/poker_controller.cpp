#include "poker_controller.h"
#include "../model/deck.h"
#include "../model/player.h"
#include "../model/advanced_hand_evaluator.h"
#include "../view/cli_view.h"
#include "../animation/spinner.h"
#include "../model/bot_player.h"

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

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

void PokerController::runGame() {
    CLIView::showWelcome();

    std::string input;
    std::cout << "Choose bot difficulty (easy / medium / hard): ";
    std::cin >> input;

    BotDifficulty botDiff = BotDifficulty::Medium;
    if (input == "easy") botDiff = BotDifficulty::Easy;
    else if (input == "hard") botDiff = BotDifficulty::Hard;

    Player human("You", 1000);
    BotPlayer bot("Bot", 1000, botDiff);

    while (human.getChipCount() > 0 && bot.getChipCount() > 0) {
        CLIView::showDivider();
        CLIView::showChipCounts(human, bot);
        CLIView::showDivider();

        playRound(human, bot);

        std::string choice;
        std::cout << "\nDo you want to play another round? (yes/no): ";
        std::cin >> choice;

        if (choice != "yes" && choice != "y") {
            std::cout << "Thanks for playing! 🎉\n";
            break;
        }
    }

    std::cout << "\n🏁 Game Over!\n";
    if (human.getChipCount() <= 0) {
        std::cout << "😞 You ran out of chips. Bot wins the game!\n";
    } else if (bot.getChipCount() <= 0) {
        std::cout << "🎉 Bot is broke! You win the game!\n";
    }
}

void PokerController::playRound(Player& human, Player& bot) {
    Deck deck;
    human.clearHand();
    bot.clearHand();

    human.recieveCard(deck.dealCard());
    human.recieveCard(deck.dealCard());
    bot.recieveCard(deck.dealCard());
    bot.recieveCard(deck.dealCard());

    std::cout << "\nYour Hand: ";
    human.showHand(true);
    std::cout << "Bot's Hand: ";
    bot.showHand(false);

    CLIView::waitForEnter();

    std::vector<Card> community;
    for (int i = 0; i < 3; ++i)
        community.push_back(deck.dealCard());
    CLIView::showCommunityCards(community, "Flop");
    CLIView::waitForEnter();

    community.push_back(deck.dealCard());
    CLIView::showCommunityCards(community, "Turn");
    CLIView::waitForEnter();

    community.push_back(deck.dealCard());
    CLIView::showCommunityCards(community, "River");
    CLIView::waitForEnter();

    std::cout << "\nWhat do you want to do? (check / bet / fold): ";
    std::string action;
    std::cin >> action;

    if (action == "fold") {
        std::cout << "You folded. Bot wins the round.\n";
        std::cout << "Bot's hand: ";
        bot.showHand(true);
        bot.bet(-200);
        return;
    }

    if (action == "bet") {
        human.bet(100);

        std::atomic<bool> done(false);
        std::thread spinner(Spinner::show, std::ref(done));

        std::this_thread::sleep_for(std::chrono::seconds(2));

        auto botHand = getCombinedHand(bot, community);
        bool botCalls = static_cast<BotPlayer&>(bot).shouldCallBet(botHand);

        done = true;
        spinner.join();

        if (botCalls) {
            std::cout << "Bot calls your bet.\n";
            bot.bet(100);
        } else {
            std::cout << "Bot folds.\n";
            bot.showHand(true);
            human.bet(-200);
            return;
        }
    } else {
        std::cout << "You checked. Bot checks.\n";
        bot.showHand(true);
    }

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
}