#include "IconFinder.h"
#include "BOT.h"
#include "BmpRect.h"
#include "Logger.h"
#include "ScreenHelperCuda.h"

#include <string>

using namespace std;

void IconFinder::init() {
    BOT::input.onKeyPressed('p', std::bind(&IconFinder::addPoint, this));
    BOT::input.onKeyPressed('l', std::bind(&IconFinder::addPoints32, this));
    BOT::input.onKeyPressed('c', std::bind(&IconFinder::captureRect, this));
    BOT::input.onKeyPressed('s', std::bind(&IconFinder::startSearching, this, false, false));
    BOT::input.onKeyPressed('d', std::bind(&IconFinder::startSearching, this, true, false));
    BOT::input.onKeyPressed('f', std::bind(&IconFinder::startSearching, this, true, true));
    BOT::input.onKeyPressed('z', std::bind(&Screen::save, &BOT::screen));
}

void IconFinder::addPoint() {
    _points.push_back(BOT::mouse.getPosition());
    Logger::info("IconFinder::addPoint()", "point added!");
    BOT::audio.play();
}

void IconFinder::addPoints32() {
    auto pos = BOT::mouse.getPosition();
    auto p1 = pos - Point(16, 16);
    auto p2 = pos + Point(15, 15);
    _points.push_back(p1);
    _points.push_back(p2);
    Logger::info("IconFinder::addPoints32()", "points added! p1: " + p1.toString() + ", p2: " + p2.toString());
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

void IconFinder::startSearching(bool useCUDA, bool enableCUDAfast) {
    using namespace std::chrono;

    BOT::screen.update();
    auto start = system_clock::now();
    
    Logger::gap();
    Logger::info("IconFinder::startSearching()", "find called, use CUDA? " + to_string(useCUDA));
    Logger::info("IconFinder::startSearching()", "rect area: " + to_string(_rect.area()));
    auto p = useCUDA ? ScreenHelperCUDA::find(_rect, !enableCUDAfast) : BOT::screen.find(_rect);

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