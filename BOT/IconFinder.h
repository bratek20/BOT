#pragma once

#include "Program.h"
#include <vector>
#include "Point.h"
#include "BmpRect.h"

class IconFinder : public Program {
public:
    void init() override;

private:
    void addPoint();
    void addPoints32();
    void captureRect();
    void startSearching(bool useCUDA, bool enableCUDAfast);

    BmpRect _rect;
    std::vector<Point> _points;
};