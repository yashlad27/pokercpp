#include "bot_thinking_config.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

// Initialize static members
std::unique_ptr<std::ofstream> BotThinkingConfig::logFile = nullptr;
bool BotThinkingConfig::useSeperateTerminal = false;
std::string BotThinkingConfig::logFilePath = "/tmp/poker_bot_thinking.log";

void BotThinkingConfig::initialize(bool separateTerminal)
{
    useSeperateTerminal = separateTerminal;
    
    if (useSeperateTerminal) {
        // Clear the log file and open it for writing
        logFile = std::make_unique<std::ofstream>(logFilePath, std::ios::trunc);
        
        if (!logFile->is_open()) {
            std::cerr << "Warning: Could not open bot thinking log file at " 
                      << logFilePath << std::endl;
            useSeperateTerminal = false;
        } else {
            std::cout << "\n";
            std::cout << "╔════════════════════════════════════════════════════════════╗\n";
            std::cout << "║  🤖 BOT THINKING VIEWER - SEPARATE TERMINAL MODE           ║\n";
            std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
            std::cout << "Bot thinking process is being logged to a separate terminal.\n";
            std::cout << "To view the bot's internal calculations in real-time:\n\n";
            std::cout << "  Run this command in a NEW TERMINAL WINDOW:\n";
            std::cout << "  \033[1;36m$ tail -f " << logFilePath << "\033[0m\n\n";
            std::cout << "Or press Enter to auto-open a viewer terminal...\n";
            std::cout << "(Press 's' to skip): ";
            
            char choice;
            std::cin.get(choice);
            if (choice != 's' && choice != 'S') {
                openViewerTerminal();
            }
            std::cout << "\n";
        }
    }
}

void BotThinkingConfig::cleanup()
{
    if (logFile && logFile->is_open()) {
        logFile->close();
    }
    logFile.reset();
}

std::ostream& BotThinkingConfig::getOutputStream()
{
    if (useSeperateTerminal && logFile && logFile->is_open()) {
        return *logFile;
    }
    return std::cout;
}

bool BotThinkingConfig::isUsingSeparateTerminal()
{
    return useSeperateTerminal;
}

std::string BotThinkingConfig::getLogFilePath()
{
    return logFilePath;
}

void BotThinkingConfig::openViewerTerminal()
{
    std::string command;
    
    #ifdef __APPLE__
        // macOS - use Terminal.app
        command = "osascript -e 'tell application \"Terminal\" to do script \"tail -f " 
                  + logFilePath + "\"'";
    #elif __linux__
        // Linux - try common terminal emulators
        if (system("which gnome-terminal > /dev/null 2>&1") == 0) {
            command = "gnome-terminal -- bash -c 'tail -f " + logFilePath + "; exec bash'";
        } else if (system("which xterm > /dev/null 2>&1") == 0) {
            command = "xterm -e 'tail -f " + logFilePath + "' &";
        } else if (system("which konsole > /dev/null 2>&1") == 0) {
            command = "konsole -e 'tail -f " + logFilePath + "' &";
        } else {
            std::cout << "Could not find a suitable terminal emulator.\n";
            std::cout << "Please manually run: tail -f " << logFilePath << "\n";
            return;
        }
    #else
        std::cout << "Automatic terminal opening not supported on this platform.\n";
        std::cout << "Please manually run: tail -f " << logFilePath << "\n";
        return;
    #endif
    
    int result = system(command.c_str());
    if (result == 0) {
        std::cout << "✓ Opened bot thinking viewer in new terminal window.\n";
        sleep(1); // Give the terminal time to open
    } else {
        std::cout << "Failed to open terminal automatically.\n";
        std::cout << "Please manually run: tail -f " << logFilePath << "\n";
    }
}
