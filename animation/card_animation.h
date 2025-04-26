// view/ascii_art.h
#ifndef ASCII_ART_H
#define ASCII_ART_H

#include <string>
#include <vector>
#include "../model/card.h"

class ASCIIArt {
public:
    // Draw the poker table with cards
    static void drawTable(const std::vector<Card>& communityCards, 
                         bool showBotCards = false,
                         const std::vector<Card>& playerCards = {},
                         const std::vector<Card>& botCards = {});
    
    // Draw individual card
    static void drawCard(const Card& card);
    
    // Draw a set of cards in a row
    static void drawCards(const std::vector<Card>& cards);
    
    // Draw poker logo
    static void drawLogo();
    
    // Draw player and bot positions
    static void drawPlayers(const std::string& playerName, const std::string& botName,
                          int playerChips, int botChips);
                          
    // Draw empty/face down card - moved from private to public
    static void drawHiddenCard();

private:
    // Top line of card
    static std::string cardTop();
    
    // Bottom line of card
    static std::string cardBottom();
    
    // Middle of card with rank and suit
    static std::string cardMiddle(const Card& card);
};

#endif // ASCII_ART_H