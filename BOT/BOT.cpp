#include "BOT.h"
#include "Program.h"

Screen BOT::screen;
Input BOT::input;
Mouse BOT::mouse;
Keyboard BOT::keyboard;
Audio BOT::audio;
std::vector<std::unique_ptr<Program>> BOT::_programs;

void BOT::addProgram(std::unique_ptr<Program> program) {
    program->init();
    _programs.push_back(std::move(program));
}