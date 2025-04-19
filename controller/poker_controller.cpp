#include "poker_controller.h"
#include "../model/deck.h"
#include "../model/player.h"
#include "../model/advanced_hand_evaluator.h"
#include "../view/cli_view.h"

#include <iostream>
#include <string>
#include <vector>

void GameController::runGame() {
    CLIView::showWelcome();

    Deck deck;
    Player human("You", 1000);
    Player bot("Bot", 1000);

    CLIView::showDivider();
    CLIView::showChipCounts(human, bot);
    CLIView::showDivider();

    // DEAL + STAGE LOGIC TO BE MOVED HERE (next step)
}