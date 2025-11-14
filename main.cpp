#include "controller/poker_controller.h"
#include "view/bot_thinking_config.h"

int main()
{
    // Initialize bot thinking visualization (separate terminal mode)
    BotThinkingConfig::initialize(true);
    
    PokerController game;
    game.runGame();
    
    // Cleanup bot thinking resources
    BotThinkingConfig::cleanup();
    
    return 0;
}