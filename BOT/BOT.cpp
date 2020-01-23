#include "BOT.h"
#include "Program.h"

using namespace std;

Screen BOT::screen;
Input BOT::input;
Mouse BOT::mouse;
Keyboard BOT::keyboard;
Audio BOT::audio;
vector<unique_ptr<Program>> BOT::_programs;

void BOT::addProgram(unique_ptr<Program> program) {
    program->init();
    _programs.push_back(move(program));
}