#pragma once

#include "Point.h"

class Mouse {

public:
    Mouse();

    void click();

    Point getPosition();
    void setPosition(Point p);
    void setPositionEvent(Point p);
};