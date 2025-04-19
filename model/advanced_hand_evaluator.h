#ifndef ADVANCED_HAND_EVALUATOR_H
#define ADVANCED_HAND_EVALUATOR_H

#include "card.h"
#include "hand_evaluator.h"
#include <vector>

class AdvancedHandEvaluator
{
public:
    static HandValue evaluate(const std::vector<Card> &cards);
};

#endif