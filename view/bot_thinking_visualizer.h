#ifndef BOT_THINKING_VISUALIZER_H
#define BOT_THINKING_VISUALIZER_H

#include <string>
#include <vector>
#include "../model/card.h"
#include "../model/hand_types.h"
#include "bot_thinking_config.h"

class BotThinkingVisualizer
{
public:
    // Main visualization header
    static void showThinkingHeader(const std::string& botName, const std::string& difficulty);
    
    // Show current hand evaluation
    static void showHandEvaluation(const HandValue& eval, const std::vector<Card>& hand);
    
    // Show drawing hand analysis
    static void showDrawingHandAnalysis(bool hasFlushDraw, bool hasStraightDraw, 
                                       const std::vector<Card>& fullHand);
    
    // Show bluff calculation
    static void showBluffCalculation(HandRank handRank, int bluffChance, bool willBluff);
    
    // Show final decision with reasoning
    static void showFinalDecision(bool shouldCall, const std::string& reasoning);
    
    // Show Monte Carlo simulation progress
    static void showMonteCarloHeader(int simulations);
    static void showMonteCarloProgress(int current, int total, int wins, int ties, int losses);
    static void showMonteCarloResult(double winRate, int totalWins, int totalLosses, int totalTies, int simulations);
    static void showConfidenceInterval(double lowerBound, double upperBound, double confidence = 0.95);
    
    // Show decision factors with weights
    static void showDecisionFactors(const std::string& stage, const HandValue& eval, 
                                   bool hasDraws, double handStrength);
    
    // Progress bar visualization
    static void drawProgressBar(double percentage, int width = 40);
    
    // Show hand strength meter
    static void showHandStrengthMeter(const HandValue& eval);
    
    // Show pot odds calculation and profitability
    static void showPotOddsAnalysis(double potOdds, double equity);
    
    // Show Kelly Criterion recommendation
    static void showKellyCriterion(double winProb, double potOdds, double kellyFraction);
    
    // Show Expected Value calculation
    static void showExpectedValue(double ev, int potSize, int callAmount);
    
private:
    // Helper functions for ASCII art
    static std::string getHandRankString(HandRank rank);
    static std::string getHandRankEmoji(HandRank rank);
    static void drawSeparator(char symbol = '=', int width = 60);
    static void drawThinSeparator(char symbol = '-', int width = 60);
};

#endif // BOT_THINKING_VISUALIZER_H
