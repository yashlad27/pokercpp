// animation/card_animation.cpp
#include "card_animation.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "../view/ascii_art.h"

// ANSI control codes
#define CLEAR_LINE "\033[2K\r"

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
    std::cout << "\n=== " << stage << " ===\n";

    // For each card, animate it being dealt
    for (const auto &card : cards)
    {
        dealCardAnimation(card, true);
        CardAnimation::sleep(300);
    }
}

void CardAnimation::clearLine()
{
    std::cout << CLEAR_LINE;
}

void CardAnimation::sleep(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}