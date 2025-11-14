// animation/card_animation.cpp
#include "card_animation.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "../view/ascii_art.h"

// ANSI control codes
#define CLEAR_LINE "\033[2K\r"
#define BOLD "\033[1m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define RESET "\033[0m"

void CardAnimation::dealCardAnimation(const Card &card, bool faceUp)
{
    // Simple sliding animation
    std::cout << CLEAR_LINE;
    for (int i = 0; i < 5; i++)
    {
        std::cout << CLEAR_LINE;
        for (int j = 0; j < i; j++)
        {
            std::cout << " ";
        }
        if (faceUp)
        {
            ASCIIArt::drawCard(card);
        }
        else
        {
            ASCIIArt::drawHiddenCard();
        }
        std::cout << std::flush;
        CardAnimation::sleep(100);
    }
    std::cout << std::endl;
}

void CardAnimation::shuffleAnimation()
{
    const char shuffleChars[] = {'/', '-', '\\', '|'};
    std::cout << "Shuffling deck ";

    for (int i = 0; i < 10; i++)
    {
        std::cout << shuffleChars[i % 4] << std::flush;
        CardAnimation::sleep(150);
        std::cout << "\b";
    }

    std::cout << " Done!" << std::endl;
    CardAnimation::sleep(500);
}

void CardAnimation::dealCommunityCards(const std::vector<Card> &cards, const std::string &stage)
{
    // Choose color based on stage
    const char* stageColor = CYAN;
    if (stage == "Flop") stageColor = YELLOW;
    else if (stage == "Turn") stageColor = MAGENTA;
    else if (stage == "River") stageColor = CYAN;
    
    std::cout << "\n" << BOLD << stageColor << "╔════════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << stageColor << "║" << RESET << "                  " << BOLD << stageColor << "🎴 " << stage << " 🎴" << RESET;
    
    // Add spacing to center the text
    int padding = 52 - 23 - stage.length();
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << BOLD << stageColor << "║" << RESET << "\n";
    std::cout << BOLD << stageColor << "╚════════════════════════════════════════════════════╝" << RESET << "\n";

    std::cout << "    ";
    // For each card, show it
    for (const auto &card : cards)
    {
        ASCIIArt::drawCard(card);
        std::cout << " ";
        CardAnimation::sleep(200);
    }
    std::cout << "\n";
}

void CardAnimation::clearLine()
{
    std::cout << CLEAR_LINE;
}

void CardAnimation::sleep(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}