#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

/**
 * Game Configuration Constants
 * Centralized configuration for poker game parameters
 * Following industry best practices for maintainability and testing
 */

namespace GameConfig {
    // Pot and betting configuration
    constexpr int DEFAULT_POT_SIZE = 200;
    constexpr int STANDARD_BET = 100;
    constexpr int STARTING_CHIPS = 1000;
    
    // Monte Carlo simulation parameters
    namespace MonteCarlo {
        constexpr int FAST_SIMULATIONS = 200;      // Quick decisions
        constexpr int STANDARD_SIMULATIONS = 1000; // Balanced
        constexpr int ACCURATE_SIMULATIONS = 10000; // High precision
        constexpr int THREAD_COUNT = 4;
        
        // Decision thresholds
        constexpr double CALL_THRESHOLD = 0.40;     // 40% win rate to call
        constexpr double STRONG_THRESHOLD = 0.60;   // 60% win rate = strong
        constexpr double CONFIDENCE_LEVEL = 0.95;   // 95% confidence interval
    }
    
    // Bot difficulty thresholds
    namespace BotBehavior {
        constexpr double EASY_CALL_RATE = 0.25;     // 25% random calls
        constexpr double MEDIUM_PAIR_CALL_RATE = 0.80; // 80% call with pair
        
        // Bluff probability by hand strength (0-100)
        constexpr int BLUFF_STRONG_HAND = 5;        // 5% with strong hand
        constexpr int BLUFF_MEDIUM_HAND = 20;       // 20% with medium hand
        constexpr int BLUFF_WEAK_HAND = 40;         // 40% with weak hand
    }
}

#endif // GAME_CONFIG_H
