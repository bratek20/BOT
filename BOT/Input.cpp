#include "Input.h"
#include "Logger.h"

using namespace std::chrono;

std::array<std::vector<Input::Callback>, Input::MAX_KEYS> Input::_callbacks;
std::array<std::chrono::time_point<std::chrono::system_clock>, Input::MAX_KEYS> Input::_lastPerform;
bool Input::_shouldRun = true;

Input::Input() {  
    _t = std::thread(run);
}

Input::~Input() {
    _shouldRun = false;
    _t.join();
}

void Input::run() {
    prep();
  
    static const unsigned int SLEEP_TIME = 25;
    while (_shouldRun) {
        GetFocus();

        if (isPressed(LeftMouseButton)) {
            perform(LeftMouseButton);
            //Logger::info("input", "left mouse button clicked");
        }
        if (isPressed(RightMouseButton)) {
            perform(RightMouseButton);
            //Logger::info("input", "right mouse button clicked");
        }
        for (char c = 'A'; c <= 'Z'; c++) {
            if (isPressed(c)) {
                perform(c);
                //Logger::info("input", c);
            }
        }
        for (char c = '0'; c <= '9'; c++) {
            if (isPressed(c)) {
                perform(c);
                //Logger::info("input", c);
            }
        }
        
        Sleep(SLEEP_TIME);
    }
}

void Input::prep() {
    for (auto& t : _lastPerform) {
        t = system_clock::now();
    }
}

bool Input::isPressed(char key) {
    unsigned char pressedMask = 1 << 7;
    duration<double> delta = system_clock::now() - _lastPerform[key];
    bool pressed = pressedMask & GetKeyState(key);

    static const double PERFORM_OFFSET = 0.15;
    if (delta.count() > PERFORM_OFFSET && pressed) {
        return true;
    }
    return false;
}

void Input::perform(char key) {
    _lastPerform[key] = system_clock::now();

    for (auto& cb : _callbacks[key]) {
        cb();
    }
}

void Input::onLeftMouseClick(Callback cb) {
    _callbacks[LeftMouseButton].push_back(cb);
}

void Input::onRightMouseClick(Callback cb) {
    _callbacks[RightMouseButton].push_back(cb);
}

void Input::onKeyPressed(char key, Callback cb) {
    if ('a' <= key && key <= 'z') {
        key = key - 'a' + 'A';
    }

    if ('A' <= key && key <= 'Z') {
        _callbacks[key].push_back(cb);
    }
    else if ('0' <= key && key <= '9') {
        _callbacks[key].push_back(cb);
    }
    else{
        Logger::error("Input::onKeyPressed", "Bad key code: " + key);
    }
    
}