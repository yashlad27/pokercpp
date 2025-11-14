#ifndef GAME_LOGGER_H
#define GAME_LOGGER_H

#include "../model/card.h"
#include "../model/hand_types.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

/**
 * Game State Logger for Analytics
 * 
 * Logs all game decisions and outcomes to CSV/JSON
 * Enables post-game analysis and strategy validation
 */

class GameLogger {
private:
    static std::ofstream logFile;
    static bool initialized;
    static int handNumber;
    static std::string sessionId;
    
    static std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    static std::string cardsToString(const std::vector<Card>& cards) {
        std::string result;
        for (size_t i = 0; i < cards.size(); ++i) {
            if (i > 0) result += " ";
            result += cards[i].toString();
        }
        return result;
    }
    
public:
    static void initialize(const std::string& filename = "/tmp/poker_game_log.csv") {
        if (initialized) return;
        
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Warning: Could not open game log file\n";
            return;
        }
        
        // Generate session ID
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        sessionId = std::to_string(ms);
        
        // Write CSV header if file is new/empty
        logFile.seekp(0, std::ios::end);
        if (logFile.tellp() == 0) {
            logFile << "session_id,timestamp,hand_number,player,hand_cards,community_cards,stage,"
                    << "action,amount,hand_rank,win_probability,ev,pot_odds,decision,outcome,chips_change\n";
        }
        
        initialized = true;
        handNumber = 0;
    }
    
    static void cleanup() {
        if (logFile.is_open()) {
            logFile.close();
        }
        initialized = false;
    }
    
    static void startNewHand() {
        handNumber++;
    }
    
    struct HandDecision {
        std::string player;
        std::vector<Card> hand;
        std::vector<Card> community;
        std::string stage;
        std::string action;  // "call", "fold", "bet", "check"
        int amount;
        HandRank handRank;
        double winProbability;
        double expectedValue;
        double potOdds;
        std::string decision;  // "CALL" or "FOLD"
        std::string outcome;   // "WIN", "LOSE", "TIE"
        int chipsChange;
    };
    
    static void logDecision(const HandDecision& decision) {
        if (!initialized || !logFile.is_open()) return;
        
        logFile << sessionId << ","
                << getCurrentTimestamp() << ","
                << handNumber << ","
                << decision.player << ","
                << "\"" << cardsToString(decision.hand) << "\","
                << "\"" << cardsToString(decision.community) << "\","
                << decision.stage << ","
                << decision.action << ","
                << decision.amount << ","
                << static_cast<int>(decision.handRank) << ","
                << std::fixed << std::setprecision(4) << decision.winProbability << ","
                << std::fixed << std::setprecision(2) << decision.expectedValue << ","
                << std::fixed << std::setprecision(4) << decision.potOdds << ","
                << decision.decision << ","
                << decision.outcome << ","
                << decision.chipsChange << "\n";
        
        logFile.flush();  // Ensure data is written
    }
    
    static void logHandOutcome(const std::string& winner, const std::vector<Card>& playerHand,
                               const std::vector<Card>& botHand, const std::vector<Card>& community,
                               HandRank playerRank, HandRank botRank, int potSize) {
        if (!initialized || !logFile.is_open()) return;
        
        // Log player outcome
        HandDecision playerOutcome;
        playerOutcome.player = "Human";
        playerOutcome.hand = playerHand;
        playerOutcome.community = community;
        playerOutcome.stage = "Showdown";
        playerOutcome.action = "reveal";
        playerOutcome.amount = 0;
        playerOutcome.handRank = playerRank;
        playerOutcome.winProbability = 0.0;
        playerOutcome.expectedValue = 0.0;
        playerOutcome.potOdds = 0.0;
        playerOutcome.decision = "N/A";
        playerOutcome.outcome = winner;
        playerOutcome.chipsChange = (winner == "Human") ? potSize : (winner == "Tie") ? 0 : -potSize/2;
        
        logDecision(playerOutcome);
        
        // Log bot outcome
        HandDecision botOutcome;
        botOutcome.player = "Bot";
        botOutcome.hand = botHand;
        botOutcome.community = community;
        botOutcome.stage = "Showdown";
        botOutcome.action = "reveal";
        botOutcome.amount = 0;
        botOutcome.handRank = botRank;
        botOutcome.winProbability = 0.0;
        botOutcome.expectedValue = 0.0;
        botOutcome.potOdds = 0.0;
        botOutcome.decision = "N/A";
        botOutcome.outcome = (winner == "Human") ? "LOSE" : (winner == "Tie") ? "TIE" : "WIN";
        botOutcome.chipsChange = (winner == "Bot") ? potSize : (winner == "Tie") ? 0 : -potSize/2;
        
        logDecision(botOutcome);
    }
    
    static std::string getLogFilePath() {
        return "/tmp/poker_game_log.csv";
    }
    
    static int getHandNumber() {
        return handNumber;
    }
};

#endif // GAME_LOGGER_H
