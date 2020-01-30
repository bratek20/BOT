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
    void captureRect();
    void startSearching(bool useCUDA);

    BmpRect _rect;
    std::vector<Point> _points;
};