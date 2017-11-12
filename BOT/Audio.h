#pragma once
#include <thread>
#include <deque>

class Audio {
    std::deque<std::thread> _threads;
    static const unsigned int MAX_THREADS = 5;
public:
    ~Audio();
    void play();

private:
    static void playSound();
};