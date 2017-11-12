#pragma once

#include "Program.h"
#include <vector>
#include "Point.h"
#include <chrono>

class Clicker : public Program {
public:
    void init() override;

private:
    void changeEnableStatus();
    void saveClick();
    void repeatClicks();

    bool _savingEnabled = false;
    std::vector<std::pair<Point, std::chrono::time_point<std::chrono::system_clock>>> _clicks;
};