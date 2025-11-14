#ifndef POKER_MATH_H
#define POKER_MATH_H

#include <algorithm>
#include <cmath>

/**
 * Poker Mathematics Utilities
 * 
 * Contains mathematical functions for optimal play:
 * - Kelly Criterion for optimal bet sizing
 * - Pot odds calculations
 * - Expected value (EV) calculations
 * - Risk of ruin calculations
 */

namespace PokerMath {

/**
 * Kelly Criterion for optimal bet sizing
 * 
 * Formula: f* = (bp - q) / b
 * where:
 *   f* = optimal fraction of bankroll to bet
 *   b = odds received on the bet (pot_size / bet_amount)
 *   p = probability of winning
 *   q = probability of losing (1 - p)
 * 
 * Returns: Optimal bet fraction [0, 1]
 *          0 = don't bet
 *          1 = bet entire bankroll
 * 
 * Example: 60% win rate, pot offers 2:1 odds
 *          f* = (2*0.6 - 0.4) / 2 = 0.4 (bet 40% of bankroll)
 */
inline double kellyFraction(double winProbability, double potOdds) {
    if (winProbability <= 0.0 || winProbability >= 1.0) {
        return 0.0;
    }
    
    double p = winProbability;
    double q = 1.0 - p;
    double b = potOdds;
    
    double kelly = (b * p - q) / b;
    
    // Clamp to [0, 1]
    return std::max(0.0, std::min(1.0, kelly));
}

/**
 * Fractional Kelly for risk management
 * Common practice: use half-Kelly or quarter-Kelly to reduce variance
 */
inline double fractionalKelly(double winProbability, double potOdds, double fraction = 0.5) {
    return kellyFraction(winProbability, potOdds) * fraction;
}

/**
 * Calculate pot odds as a ratio
 * 
 * Returns: pot_size / call_amount
 * 
 * Example: $100 pot, $50 to call = 2.0 (2:1 odds)
 */
inline double calculatePotOdds(int potSize, int callAmount) {
    if (callAmount <= 0) return 0.0;
    return static_cast<double>(potSize) / callAmount;
}

/**
 * Calculate pot odds as a percentage
 * 
 * Returns: Percentage of pot you're risking
 * 
 * Example: $100 pot, $50 to call = 33.3% (need 33.3% equity to break even)
 */
inline double calculatePotOddsPercentage(int potSize, int callAmount) {
    if (potSize + callAmount <= 0) return 0.0;
    return static_cast<double>(callAmount) / (potSize + callAmount);
}

/**
 * Calculate implied odds
 * Considers future betting rounds
 * 
 * impliedPot = currentPot + estimatedFutureBets
 */
inline double calculateImpliedOdds(int currentPot, int callAmount, int estimatedFutureBets) {
    int impliedPot = currentPot + estimatedFutureBets;
    return calculatePotOdds(impliedPot, callAmount);
}

/**
 * Expected Value (EV) calculation
 * 
 * EV = (P(win) × amountWon) - (P(lose) × amountLost)
 * 
 * Returns: Expected monetary value (can be negative)
 */
inline double calculateEV(double winProbability, int potSize, int callAmount) {
    double winAmount = potSize;  // If we win, we get the pot
    double loseAmount = callAmount;  // If we lose, we lose our call
    
    double ev = (winProbability * winAmount) - ((1.0 - winProbability) * loseAmount);
    return ev;
}

/**
 * Check if a call is profitable based on pot odds
 * 
 * Call is profitable if: equity > pot odds percentage
 */
inline bool isProfitableCall(double equity, int potSize, int callAmount) {
    double requiredEquity = calculatePotOddsPercentage(potSize, callAmount);
    return equity > requiredEquity;
}

/**
 * Calculate minimum defense frequency (MDF)
 * Used to avoid being exploited by bluffs
 * 
 * MDF = pot_size / (pot_size + bet_size)
 * 
 * Example: $100 pot, $50 bet = 66.7% (must defend 2/3 of the time)
 */
inline double calculateMDF(int potSize, int betSize) {
    if (potSize + betSize <= 0) return 0.0;
    return static_cast<double>(potSize) / (potSize + betSize);
}

/**
 * Sharpe Ratio for performance evaluation
 * 
 * Sharpe = (average_return - risk_free_rate) / std_dev_returns
 * 
 * Measures risk-adjusted returns
 */
inline double calculateSharpe(double avgReturn, double stdDev, double riskFreeRate = 0.0) {
    if (stdDev <= 0.0) return 0.0;
    return (avgReturn - riskFreeRate) / stdDev;
}

/**
 * Calculate stack-to-pot ratio (SPR)
 * Important for tournament and deep-stack play
 * 
 * SPR = effective_stack / pot_size
 */
inline double calculateSPR(int effectiveStack, int potSize) {
    if (potSize <= 0) return 0.0;
    return static_cast<double>(effectiveStack) / potSize;
}

/**
 * Risk of Ruin calculation (simplified)
 * Probability of losing entire bankroll
 * 
 * RoR = ((1-edge)/(1+edge))^(bankroll/bet_size)
 */
inline double calculateRiskOfRuin(double winRate, int bankroll, int betSize) {
    if (betSize <= 0 || bankroll <= 0) return 1.0;
    
    double edge = 2 * winRate - 1;  // Convert win rate to edge
    if (edge <= 0) return 1.0;  // Negative edge = certain ruin
    
    double ratio = (1.0 - edge) / (1.0 + edge);
    double units = static_cast<double>(bankroll) / betSize;
    
    return std::pow(ratio, units);
}

} // namespace PokerMath

#endif // POKER_MATH_H
