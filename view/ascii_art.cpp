// view/ascii_art.cpp
#include "ascii_art.h"
#include <iostream>
#include <iomanip>

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define BLACK "\033[30m"
#define WHITE_BG "\033[47m"

void ASCIIArt::drawTable(const std::vector<Card> &communityCards,
                         bool showBotCards,
                         const std::vector<Card> &playerCards,
                         const std::vector<Card> &botCards)
{
    std::cout << "\n\n";
    std::cout << "  +-----------------------------------------------------+\n";
    std::cout << "  |                                                     |\n";
    std::cout << "  |                     BOT's HAND                      |\n";
    std::cout << "  |                                                     |\n";

    std::cout << "  |  ";
    if (botCards.empty())
    {
        drawHiddenCard();
        std::cout << " ";
        drawHiddenCard();
    }
    else if (!showBotCards)
    {
        for (size_t i = 0; i < botCards.size(); i++)
        {
            drawHiddenCard();
            std::cout << " ";
        }
    }
    else
    {
        for (size_t i = 0; i < botCards.size(); i++)
        {
            drawCard(botCards[i]);
            std::cout << " ";
        }
    }
    std::cout << "                               |\n";

    std::cout << "  |                                                     |\n";
    std::cout << "  |                                                     |\n";
    std::cout << "  |                  COMMUNITY CARDS                    |\n";
    std::cout << "  |                                                     |\n";

    std::cout << "  |  ";
    if (communityCards.empty())
    {
        std::cout << "           No cards dealt yet                ";
    }
    else
    {
        for (const auto &card : communityCards)
        {
            drawCard(card);
            std::cout << " ";
        }
        // Fill in empty spaces if fewer than 5 cards
        for (size_t i = communityCards.size(); i < 5; i++)
        {
            std::cout << "     ";
        }
    }
    std::cout << "  |\n";

    std::cout << "  |                                                     |\n";
    std::cout << "  |                                                     |\n";
    std::cout << "  |                    YOUR HAND                        |\n";
    std::cout << "  |                                                     |\n";

    std::cout << "  |  ";
    if (playerCards.empty())
    {
        drawHiddenCard();
        std::cout << " ";
        drawHiddenCard();
    }
    else
    {
        for (const auto &card : playerCards)
        {
            drawCard(card);
            std::cout << " ";
        }
    }
    std::cout << "                               |\n";

    std::cout << "  |                                                     |\n";
    std::cout << "  +-----------------------------------------------------+\n\n";
}

void ASCIIArt::drawCard(const Card &card)
{
    // Determine color based on suit
    std::string color = RESET;
    if (card.suit == Suit::Hearts || card.suit == Suit::Diamonds)
    {
        color = RED;
    }
    else
    {
        color = BLACK;
    }

    std::cout << WHITE_BG << color;

    // Get card rank as string
    std::string rank;
    if (static_cast<int>(card.rank) <= 10 && static_cast<int>(card.rank) >= 2)
    {
        rank = std::to_string(static_cast<int>(card.rank));
        if (rank.length() == 1)
            rank = " " + rank;
    }
    else
    {
        switch (card.rank)
        {
        case Rank::Jack:
            rank = " J";
            break;
        case Rank::Queen:
            rank = " Q";
            break;
        case Rank::King:
            rank = " K";
            break;
        case Rank::Ace:
            rank = " A";
            break;
        default:
            rank = "??";
            break;
        }
    }

    // Get suit symbol
    std::string suit;
    switch (card.suit)
    {
    case Suit::Hearts:
        suit = "♥";
        break;
    case Suit::Diamonds:
        suit = "♦";
        break;
    case Suit::Clubs:
        suit = "♣";
        break;
    case Suit::Spades:
        suit = "♠";
        break;
    default:
        suit = "?";
        break;
    }

    // Print card
    std::cout << "[" << rank << suit << "]" << RESET;
}

void ASCIIArt::drawHiddenCard()
{
    std::cout << WHITE_BG << BLACK << "[░░]" << RESET;
}

void ASCIIArt::drawCards(const std::vector<Card> &cards)
{
    for (const auto &card : cards)
    {
        drawCard(card);
        std::cout << " ";
    }
    std::cout << std::endl;
}

void ASCIIArt::drawLogo()
{
    std::cout << "\n";
    std::cout << "  ╔═══════════════════════════════════════════════╗\n";
    std::cout << "  ║                                               ║\n";
    std::cout << "  ║   ♠️  ♥️  TEXAS HOLD'EM POKER  ♣️  ♦️   ║\n";
    std::cout << "  ║                                               ║\n";
    std::cout << "  ╚═══════════════════════════════════════════════╝\n\n";
}

void ASCIIArt::drawPlayers(const std::string &playerName, const std::string &botName,
                           int playerChips, int botChips)
{
    std::cout << "  " << botName << ": " << botChips << " chips\n";
    std::cout << "  " << playerName << ": " << playerChips << " chips\n\n";
}

std::string ASCIIArt::cardTop()
{
    return "+-----+";
}

std::string ASCIIArt::cardBottom()
{
    return "+-----+";
}

std::string ASCIIArt::cardMiddle(const Card &card)
{
    // Get card rank as string
    std::string rank;
    if (static_cast<int>(card.rank) <= 10 && static_cast<int>(card.rank) >= 2)
    {
        rank = std::to_string(static_cast<int>(card.rank));
        if (rank.length() == 1)
            rank = rank + " ";
    }
    else
    {
        switch (card.rank)
        {
        case Rank::Jack:
            rank = "J ";
            break;
        case Rank::Queen:
            rank = "Q ";
            break;
        case Rank::King:
            rank = "K ";
            break;
        case Rank::Ace:
            rank = "A ";
            break;
        default:
            rank = "??";
            break;
        }
    }

    // Get suit symbol
    std::string suit;
    switch (card.suit)
    {
    case Suit::Hearts:
        suit = "♥";
        break;
    case Suit::Diamonds:
        suit = "♦";
        break;
    case Suit::Clubs:
        suit = "♣";
        break;
    case Suit::Spades:
        suit = "♠";
        break;
    default:
        suit = "?";
        break;
    }

    // Determine color based on suit
    std::string color = RESET;
    if (card.suit == Suit::Hearts || card.suit == Suit::Diamonds)
    {
        color = RED;
    }
    else
    {
        color = BLACK;
    }

    return "| " + color + rank + suit + RESET + " |";
}