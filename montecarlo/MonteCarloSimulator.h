#ifndef MONTE_CARLO_SIMULATOR_H
#define MONTE_CARLO_SIMULATOR_H

#include "../model/card.h"
#include <vector>

class MonteCarloSimulator
{
public:
    MonteCarloSimulator(const std::vector<Card> &playerHand,
                        const std::vector<Card> &communityCards,
                        int simulations = 10000);

    void runSimulation();
    double getWinPercentage() const;
    double getTiePercentage() const;
    double getLosePercentage() const;

    // TODO: Add flush and straight draw probability methods
    double getFlushDrawOdds() const;
    double getStraightDrawOdds() const;

private:
    std::vector<Card> playerHand;
    std::vector<Card> communityCards;
    int numSimulations;
    int winCount;
    int tieCount;
    int loseCount;

    std::vector<Card> getRemainingDeck() const;
    std::pair<std::vector<Card>, std::vector<Card>> dealRandomOpponentAndBoard(const std::vector<Card> &deck) const;
    int evaluateHand(const std::vector<Card> &hand, const std::vector<Card> &board) const;
};

#endif