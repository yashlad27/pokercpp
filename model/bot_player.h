#ifndef BOT_PLAYER_H
#define BOT_PLAYER_H

#include "player.h"
#include "hand_types.h"
#include "advanced_hand_evaluator.h"
#include <vector>
#include <string>

enum class GameStage 
{
    PreFlop,
    Flop,
    Turn,
    River
};

enum class BotDifficulty
{
    Easy,
    Medium,
    Hard,
    HardPlus
};

class BotPlayer : public Player
{
private:
    BotDifficulty difficulty;

    // basic decision making methods
    bool shouldCallEasy() const;
    bool shouldCallMedium(const HandValue& eval, GameStage stage, const std::vector<Card>& fullHand) const;
    bool shouldCallHard(const HandValue& eval, GameStage stage, const std::vector<Card>& fullHand) const;
    bool shouldCallHardPlus(const std::vector<Card>& fullHand);

    // hand strength awareness methods
    bool hasDrawingHand(const std::vector<Card>& fullHand) const;
    bool hasFlushDraw(const std::vector<Card>& fullHand) const;
    bool hasStraightDraw(const std::vector<Card>& fullHand) const;

    // Bluffing Logic
    bool shouldBluff(HandRank handRank) const;

public:
    BotPlayer(const std::string &name, int chips, BotDifficulty diff);

    BotDifficulty getDifficulty() const;

    bool shouldCallBet(const std::vector<Card> &fullHand, GameStage stage = GameStage::River);
};

#endif