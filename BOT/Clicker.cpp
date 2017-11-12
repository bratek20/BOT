#include "Clicker.h"
#include "BOT.h"
#include "Logger.h"

using namespace std::chrono;

void Clicker::init() {
    BOT::input.onKeyPressed('e', std::bind(&Clicker::changeEnableStatus, this));
    BOT::input.onLeftMouseClick(std::bind(&Clicker::saveClick, this));
    BOT::input.onKeyPressed('r', std::bind(&Clicker::repeatClicks, this));
}

void Clicker::changeEnableStatus() {
    _savingEnabled = !_savingEnabled;
    if (_savingEnabled) {
        Logger::info("Clicker::changeEnableStatus() ", "saving enabled");
    }
    else {
        Logger::info("Clicker::changeEnableStatus() ", "saving disabled");
    }
    BOT::audio.play();
}

void Clicker::saveClick() {
    if (_savingEnabled) {
        _clicks.push_back({ BOT::mouse.getPosition(), system_clock::now()});
        Logger::info("Clicker::saveClick()", _clicks.back().first);
    }
}

void Clicker::repeatClicks() {
    if (_clicks.empty()) {
        Logger::error("Clicker::repeatClicks()", "no clicks saved!");
        return;
    }

    Logger::info("Clicker::repeatClicks()", "starting repeating...");
    BOT::audio.play();

    time_point<system_clock> lastTimePoint = _clicks[0].second;
    for (auto& click : _clicks) {
        duration<double> duration = click.second - lastTimePoint;
        Sleep(static_cast<unsigned int>(1000*duration.count()));

        lastTimePoint = click.second;
        BOT::mouse.setPosition(click.first);
        BOT::mouse.click();    
    }

    _clicks.clear();
    Logger::info("Clicker::repeatClicks()", "repeating has ended");
    BOT::audio.play();
}