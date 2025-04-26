// animation/card_animation.h
#ifndef CARD_ANIMATION_H
#define CARD_ANIMATION_H

#include <string>
#include <vector>
#include "../model/card.h"

class CardAnimation
{
public:
    static void dealCardAnimation(const Card &card, bool faceUp = true);
    static void shuffleAnimation();
    static void dealCommunityCards(const std::vector<Card> &cards, const std::string &stage);
    static void clearLine();
    static void sleep(int milliseconds);
};

#endif // CARD_ANIMATION_H