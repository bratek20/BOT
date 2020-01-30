#include "IconFinder.h"
#include "BOT.h"
#include "BmpRect.h"
#include "Logger.h"
#include "ScreenHelperCuda.h"

#include <string>

using namespace std;

void IconFinder::init() {
    BOT::input.onKeyPressed('p', std::bind(&IconFinder::addPoint, this));
    BOT::input.onKeyPressed('c', std::bind(&IconFinder::captureRect, this));
    BOT::input.onKeyPressed('s', std::bind(&IconFinder::startSearching, this, false));
    BOT::input.onKeyPressed('d', std::bind(&IconFinder::startSearching, this, true));
    BOT::input.onKeyPressed('z', std::bind(&Screen::save, &BOT::screen));
}

void IconFinder::addPoint() {
    _points.push_back(BOT::mouse.getPosition());
    Logger::info("IconFinder::addPoint()", "point added!");
    BOT::audio.play();
}

void IconFinder::captureRect() {
    auto pSize = _points.size();
    if (pSize < 2) {
        Logger::error("IconFinder::captureRect()", "two points have to be added!");
        return;
    }

    Logger::info("IconFinder::captureRect()", "start capturing...");
    BOT::screen.update();
    _rect = BmpRect(_points[pSize-2], _points[pSize - 1]);
    Logger::info("IconFinder::captureRect()", "rect captured");
    BOT::audio.play();

    _points.clear();
}

void IconFinder::startSearching(bool useCUDA) {
    using namespace std::chrono;

    BOT::screen.update();
    auto start = system_clock::now();
    
    Logger::gap();
    Logger::info("IconFinder::startSearching()", "find called, use CUDA? " + to_string(useCUDA));
    Logger::info("IconFinder::startSearching()", "rect area: " + to_string(_rect.area()));
    auto p = useCUDA ? ScreenHelperCUDA::find(_rect) : BOT::screen.find(_rect);

    auto end = system_clock::now();

    duration<double> dur = end - start;
    Logger::info("IconFinder::startSearching()", "duration: " + std::to_string(dur.count()));

    if (p == Point::NONE) {
        Logger::error("IconFinder::startSearching()", "rect not found!");
    }
    else {
        Logger::info("IconFinder::startSearching()", p);
        BOT::mouse.setPosition(p);
    }
    BOT::audio.play();
}