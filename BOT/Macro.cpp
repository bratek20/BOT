#include "Macro.h"
#include "Bot.h"
#include "Logger.h"

void Macro::init() {
    BOT::input.onKeyPressed('r', std::bind(&Macro::recordMacro, this));
}

void Macro::recordMacro() {
    BOT::input.onAnyKeyPressed([](char key) {
        if (key == 'R')
        {
            return;
        }
        BOT::audio.play();
        auto pos = BOT::mouse.getPosition();
        BOT::input.onKeyPressed(key, [pos, key]() {
            BOT::mouse.setPosition(pos);
            BOT::mouse.click();
            Logger::info("Macro", "action for " + key);
        });
    });
}