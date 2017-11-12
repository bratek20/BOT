#pragma once
#include <thread>
#include <Windows.h>
#include <mmsystem.h>
#include <deque>

class Audio {
    std::deque<std::thread> _threads;
    static const unsigned int MAX_THREADS = 5;
public:
    ~Audio() {
        while (!_threads.empty()) {
            _threads.front().join();
            _threads.pop_front();
        }
    }

    void play() {    
        _threads.push_back(std::thread(playSound));
        while (_threads.size() > MAX_THREADS) {
            _threads.front().join();
            _threads.pop_front();
        }
    }

private:
    static void playSound() {
        PlaySound((LPCTSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ALIAS_ID | SND_ASYNC);
    }
};