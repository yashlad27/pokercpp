#include "poker_controller.h"
#include "../model/deck.h"
#include "../model/player.h"
#include "../model/hand_types.h"
#include "../model/advanced_hand_evaluator.h"
#include "../view/cli_view.h"
#include "../animation/spinner.h"
#include "../model/bot_player.h"

#include <iostream>
#include <vector>
#include <string>
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

std::vector<Card> getCombinedHand(const Player &player, const std::vector<Card> &community)
{
    std::vector<Card> fullHand = player.getHand();
    fullHand.insert(fullHand.end(), community.begin(), community.end());
    return fullHand;
}

std::string handRankToString(HandRank rank)
{
    switch (rank)
    {
    case HandRank::HighCard:
        return "High Card";
    case HandRank::OnePair:
        return "One Pair";
    case HandRank::TwoPair:
        return "Two Pair";
    case HandRank::ThreeOfAKind:
        return "Three of a Kind";
    case HandRank::Straight:
        return "Straight";
    case HandRank::Flush:
        return "Flush";
    case HandRank::FullHouse:
        return "Full House";
    case HandRank::FourOfAKind:
        return "Four of a Kind";
    case HandRank::StraightFlush:
        return "Straight Flush";
    case HandRank::RoyalFlush:
        return "Royal Flush";
    default:
        return "Unknown";
    }
}

void PokerController::runGame()
{
    CLIView::showWelcome();

    std::string input;
    std::cout << BOLD << CYAN << "Choose bot difficulty " << RESET << "(" << GREEN << "easy" << RESET << " / " << YELLOW << "medium" << RESET << " / " << RED << "hard" << RESET << " / " << MAGENTA << "hardplus" << RESET << "): ";
    std::cin >> input;

    BotDifficulty botDiff = BotDifficulty::Medium;
    if (input == "easy")
        botDiff = BotDifficulty::Easy;
    else if (input == "hard")
        botDiff = BotDifficulty::Hard;
    else if (input == "hardplus")
        botDiff = BotDifficulty::HardPlus;

    Player human("You", 1000);
    BotPlayer bot("Bot", 1000, botDiff);

    while (human.getChipCount() > 0 && bot.getChipCount() > 0)
    {
        CLIView::showDivider();
        CLIView::showChipCounts(human, bot);
        CLIView::showDivider();

        playRound(human, bot);

        std::string choice;
        std::cout << "\n" << BOLD << BLUE << "Do you want to play another round? " << RESET << "(" << GREEN << "yes" << RESET << "/" << RED << "no" << RESET << "): ";
        std::cin >> choice;

        if (choice != "yes" && choice != "y")
        {
            std::cout << BOLD << GREEN << "\nThanks for playing! 🎉" << RESET << "\n";
            break;
        }
    }

    std::cout << "\n" << BOLD << YELLOW << "🏁 Game Over!" << RESET << "\n";
    if (human.getChipCount() <= 0)
    {
        std::cout << BOLD << RED << "😞 You ran out of chips. Bot wins the game!" << RESET << "\n";
    }
    else if (bot.getChipCount() <= 0)
    {
        std::cout << BOLD << GREEN << "🎉 Bot is broke! You win the game!" << RESET << "\n";
    }
}

void PokerController::playRound(Player &human, Player &bot)
{
    Deck deck;
    human.clearHand();
    bot.clearHand();

    human.recieveCard(deck.dealCard());
    human.recieveCard(deck.dealCard());
    bot.recieveCard(deck.dealCard());
    bot.recieveCard(deck.dealCard());

    std::cout << "\n" << BOLD << GREEN << "Your Hand: " << RESET;
    human.showHand(true);
    std::cout << BOLD << CYAN << "Bot's Hand: " << RESET;
    bot.showHand(false);

    CLIView::waitForEnter();

    // Flop
    std::vector<Card> community;
    for (int i = 0; i < 3; ++i)
        community.push_back(deck.dealCard());
    CLIView::showCommunityCards(community, "Flop");

    // Bot decision making at flop stage
    if (!handleBetting(human, bot, community, GameStage::Flop))
    {
        return; // Return if someone folded
    }

    // Turn
    community.push_back(deck.dealCard());
    CLIView::showCommunityCards(community, "Turn");

    if (!handleBetting(human, bot, community, GameStage::Turn))
    {
        return; // Return if someone folded
    }

    // River
    community.push_back(deck.dealCard());
    CLIView::showCommunityCards(community, "River");

    if (!handleBetting(human, bot, community, GameStage::River))
    {
        return; // Return if someone folded
    }

    // Showdown
    showdown(human, bot, community);
}

bool PokerController::handleBetting(Player &human, Player &bot, const std::vector<Card> &community, GameStage stage)
{
    CLIView::waitForEnter();

    std::cout << "\n" << BOLD << BLUE << "What do you want to do? " << RESET << "(" << GREEN << "check" << RESET << " / " << YELLOW << "bet" << RESET << " / " << RED << "fold" << RESET << "): ";
    std::string action;
    std::cin >> action;

    if (action == "fold")
    {
        std::cout << RED << "You folded. " << RESET << CYAN << "Bot wins the round." << RESET << "\n";
        std::cout << CYAN << "Bot's hand: " << RESET;
        bot.showHand(true);
        bot.addChips(200);
        std::cout << BOLD << CYAN << "Bot wins 200 chips! 💰" << RESET << "\n";
        return false;
    }

    if (action == "bet")
    {
        human.bet(100);

        std::atomic<bool> done(false);
        std::thread spinner(Spinner::show, std::ref(done));

        std::this_thread::sleep_for(std::chrono::seconds(2));

        auto botHand = getCombinedHand(bot, community);
        bool botCalls = static_cast<BotPlayer &>(bot).shouldCallBet(botHand, stage);

        done = true;
        spinner.join();

        if (botCalls)
        {
            std::cout << CYAN << "Bot calls your bet." << RESET << "\n";
            bot.bet(100);
        }
        else
        {
            std::cout << CYAN << "Bot folds." << RESET << "\n";
            bot.showHand(true);
            human.addChips(200);
            std::cout << BOLD << GREEN << "You win 200 chips! 💰" << RESET << "\n";
            return false;
        }
    }
    else
    {
        std::cout << BLUE << "You checked." << RESET << " " << CYAN << "Bot checks." << RESET << "\n";
    }

    return true;
}

void PokerController::showdown(Player &human, Player &bot, const std::vector<Card> &community)
{
    CLIView::showResult(human, bot, community);

    auto humanFull = getCombinedHand(human, community);
    auto botFull = getCombinedHand(bot, community);

    HandValue hv1 = AdvancedHandEvaluator::evaluate(humanFull);
    HandValue hv2 = AdvancedHandEvaluator::evaluate(botFull);

    CLIView::showHandType(human.getName(), handRankToString(hv1.rank));
    CLIView::showHandType(bot.getName(), handRankToString(hv2.rank));

    const int pot = 200;

    std::cout << "\n" << BOLD << YELLOW << "Result: " << RESET;
    if (hv1 > hv2)
    {
        std::cout << BOLD << GREEN << "You win! 🎉" << RESET << "\n";
        human.addChips(pot);
        std::cout << GREEN << "You won " << BOLD << pot << " chips! 💰" << RESET << "\n";
    }
    else if (hv2 > hv1)
    {
        std::cout << BOLD << CYAN << "Bot wins! 🤖" << RESET << "\n";
        bot.addChips(pot);
        std::cout << CYAN << "Bot won " << BOLD << pot << " chips! 💰" << RESET << "\n";
    }
    else
    {
        std::cout << BOLD << YELLOW << "It's a tie! 🤝" << RESET << "\n";
        human.addChips(100);
        bot.addChips(100);
        std::cout << YELLOW << "Pot split - both players get 100 chips back. 💰" << RESET << "\n";
    }
}