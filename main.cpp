#include "controller/poker_controller.h"
#include "view/bot_thinking_config.h"
#include "utils/performance_monitor.h"
#include "utils/game_logger.h"
#include <iostream>

int main()
{
    // Initialize systems
    BotThinkingConfig::initialize(true);
    GameLogger::initialize();
    
    std::cout << "\n\033[1m\033[33m📊 Performance monitoring enabled\033[0m\n";
    std::cout << "\033[1m\033[33m📝 Game logging to: " << GameLogger::getLogFilePath() << "\033[0m\n\n";
    
    // Run game
    PokerController controller;
    controller.runGame();
    
    // Cleanup and show performance report
    BotThinkingConfig::cleanup();
    GameLogger::cleanup();
    
    std::cout << "\n";
    PerformanceMonitor::printReport();
    
    std::cout << "\n\033[1m\033[32m✅ Game log saved to: " << GameLogger::getLogFilePath() << "\033[0m\n";
    std::cout << "\033[1m\033[32m✅ Analyze with: cat " << GameLogger::getLogFilePath() << "\033[0m\n\n";
    
    return 0;
}