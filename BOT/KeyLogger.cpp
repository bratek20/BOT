#include "KeyLogger.h"
#include "BOT.h"
#include "Logger.h"

void KeyLogger::init() {
    for (char key = 'a'; key <= 'z'; key++) {
        auto cb = [key]() {Logger::info("Keylogger", key); };
        BOT::input.onKeyPressed(key, cb);
    }
}