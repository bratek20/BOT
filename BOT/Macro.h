#pragma once

#include "Program.h"
#include <vector>
#include "Point.h"
#include <chrono>

class Macro : public Program {
public:
    void init() override;

private:
    void recordMacro();
};