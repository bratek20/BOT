#pragma once
#include <Windows.h>
#include "Logger.h"

class Keyboard {
public :
    void press(char key) {
        INPUT pressedDown;

        pressedDown.type = INPUT_KEYBOARD;
        pressedDown.ki.wScan = 0; 
        pressedDown.ki.time = 0;
        pressedDown.ki.dwExtraInfo = 0;

        pressedDown.ki.wVk = key;
        pressedDown.ki.dwFlags = 0; // 0 for key press

        INPUT pressedUP = pressedDown;
        pressedUP.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release

        INPUT inputs[] = { pressedDown, pressedUP };
        int cnt = SendInput(2, inputs, sizeof(pressedDown));
        if (cnt < 2)
        {
            Logger::error("Keyboard::press", GetLastError());
        }
    }
};