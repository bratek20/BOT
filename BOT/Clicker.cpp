#include "Clicker.h"
#include "BOT.h"
#include "Logger.h"

using namespace std::chrono;

void Clicker::init() {
    BOT::input.onKeyPressed('e', std::bind(&Clicker::changeEnableStatus, this));
    //BOT::input.onLeftMouseClick(std::bind(&Clicker::saveClick, this));
    BOT::input.onKeyPressed('r', std::bind(&Clicker::repeatClicks, this));
    BOT::input.onKeyPressed('b', std::bind(&Clicker::saveClick, this));
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
    unsigned int waitTime = 1167;
    unsigned int roundTime = 2000;
    Point holdPoint{ 800,600 };
    Point returnPoint{ 830,919 };
    Point startGamePoint{ 955,427 };

    while (_savingEnabled)
    {
        BOT::mouse.setPosition(holdPoint);
        BOT::mouse.pressDown();
        Sleep(waitTime);
        BOT::mouse.pressUp();  

        /*Sleep(100);
        BOT::mouse.setPosition(returnPoint);
        BOT::mouse.click();
        Sleep(200);

        BOT::mouse.setPosition(startGamePoint);
        BOT::mouse.click();*/
        Sleep(roundTime);
    }
}

void Clicker::repeatClicks() {
    Logger::info("Clicker::repeatClicks()", BOT::mouse.getPosition());
    return;

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