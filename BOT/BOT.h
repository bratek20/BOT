#pragma once

#include <memory>
#include "Screen.h"
#include "Input.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Audio.h"
#include "Program.h"
#include <vector>

class Program;

struct BOT {
    static Screen screen;
    static Input input;
    static Mouse mouse;
    static Keyboard keyboard;
    static Audio audio;

    static void addProgram(std::unique_ptr<Program> program);

private:
    static std::vector<std::unique_ptr<Program>> _programs;
};