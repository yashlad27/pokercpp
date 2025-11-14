#ifndef BOT_THINKING_CONFIG_H
#define BOT_THINKING_CONFIG_H

#include <string>
#include <fstream>
#include <memory>

class BotThinkingConfig
{
private:
    static std::unique_ptr<std::ofstream> logFile;
    static bool useSeperateTerminal;
    static std::string logFilePath;

public:
    // Initialize the bot thinking output
    static void initialize(bool separateTerminal = true);
    
    // Clean up resources
    static void cleanup();
    
    // Get the output stream (either file or cout)
    static std::ostream& getOutputStream();
    
    // Check if using separate terminal
    static bool isUsingSeparateTerminal();
    
    // Get the log file path
    static std::string getLogFilePath();
    
    // Open a new terminal window to view bot thinking (macOS)
    static void openViewerTerminal();
};

#endif // BOT_THINKING_CONFIG_H
