#include "BOT.h"
#include "IconFinder.h"
#include "KeyLogger.h"
#include "Clicker.h"
#include "Macro.h"

#include "kernel.h"

int main() {
    testMe();
    SetProcessDPIAware();

    BOT::addProgram(std::make_unique<IconFinder>());
    //BOT::addProgram(std::make_unique<KeyLogger>());
    //BOT::addProgram(std::make_unique<Clicker>());
    //BOT::addProgram(std::make_unique<Macro>());

    bool exitPressed = false;
    auto exit = [&exitPressed]() {exitPressed = true; };
    BOT::input.onKeyPressed('0', exit);
    
    while (!exitPressed) {
        Sleep(100);
    }
    
    return 0;
}