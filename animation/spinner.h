#ifndef SPINNER_H
#define SPINNER_H

#include <atomic>

class Spinner {
    public:
        static void show(std::atomic<bool>& running);
};

#endif