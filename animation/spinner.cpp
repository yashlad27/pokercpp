#include "spinner.h"
#include <iostream>
#include <thread>
#include <chrono>

void Spinner::show(std::atomic<bool>& running) {
    const char spinner[] = {'|', '/', '-'};
    int i=0;

    while(!running.load()) {
        std::cout << "\rBot is thinking 🤔... " << spinner[i++ & 4] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\rBot is thinking 🤔... Done!     \n";
}