#include "Audio.h"

#include <Windows.h>
#include <mmsystem.h>

using namespace std;

Audio::~Audio() {
    while (!_threads.empty()) {
        _threads.front().join();
        _threads.pop_front();
    }
}

void Audio::play() {
    _threads.push_back(thread(playSound));
    while (_threads.size() > MAX_THREADS) {
        _threads.front().join();
        _threads.pop_front();
    }
}

void Audio::playSound() {
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ALIAS_ID | SND_ASYNC);
}
