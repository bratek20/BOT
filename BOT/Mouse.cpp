#include "Mouse.h"
#include <Windows.h>
#include "Logger.h"
#include "BOT.h"

Mouse::Mouse() {

}

void Mouse::pressDown()
{
    INPUT pressedDown = {};
    pressedDown.type = INPUT_MOUSE;
    pressedDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    INPUT inputs[] = { pressedDown };

    SendInput(1, inputs, sizeof(pressedDown));
}

void Mouse::pressUp()
{
    INPUT pressedUp = {};
    pressedUp.type = INPUT_MOUSE;
    pressedUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;

    INPUT inputs[] = { pressedUp };

    SendInput(1, inputs, sizeof(pressedUp));
}

void Mouse::click() {
    INPUT pressedDown = {};
    pressedDown.type = INPUT_MOUSE;
    pressedDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    
    INPUT pressedUp = {};
    pressedUp.type = INPUT_MOUSE;
    pressedUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;

    INPUT inputs[] = { pressedDown, pressedUp };

    SendInput(2, inputs, sizeof(pressedDown));
}

Point Mouse::getPosition() {
    POINT pos;
    if (!GetCursorPos(&pos)) {
        Logger::error("Mouse::getPosition", "position not taken!");
        return Point();
    }

    return Point(pos);
}

void Mouse::setPosition(Point p) {
    bool ok = SetCursorPos(p.x, p.y);   
    if (ok == false)
    {
        Logger::info("Mouse::setPosition error", GetLastError());
    }
}

void Mouse::setPositionEvent(Point p) {
    INPUT setPos = {};
    setPos.type = INPUT_MOUSE;
    setPos.mi.mouseData = 0;
    setPos.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
    setPos.mi.dx = (p.x+1) * 65535 / BOT::screen.width();
    setPos.mi.dy = (p.y+1) * 65535 / BOT::screen.height();

    int cnt = SendInput(1, &setPos, sizeof(setPos));
    if (cnt == 0)
    {
        Logger::info("Mouse::setPositionEvent error", GetLastError());
    }     
}
