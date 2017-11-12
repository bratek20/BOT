#pragma once

#include "Point.h"

struct Rect {
    Point p1, p2;
    Rect() {
        p1 = Point::NONE;
        p2 = Point::NONE;
    }

    Rect(Point p1, Point p2) : p1(p1), p2(p2) {
        if (p1.x > p2.x) {
            std::swap(p1, p2);
        }
    }

    int width() const {
        return p2.x - p1.x + 1;
    }
    int height() const {
        return p2.y - p1.y + 1;
    }
};