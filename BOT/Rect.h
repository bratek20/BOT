#pragma once

#include "Point.h"

struct Rect {
    Point p1, p2;
    Rect() {
        p1 = Point::NONE;
        p2 = Point::NONE;
    }

    Rect(Point _p1, Point _p2) : p1(_p1), p2(_p2) {
        if (p1.x > p2.x) {
            std::swap(p1.x, p2.x);
        }
        if (p1.y > p2.y) {
            std::swap(p1.y, p2.y);
        }
    }

    int width() const {
        return p2.x - p1.x + 1;
    }
    
    int height() const {
        return p2.y - p1.y + 1;
    }

    int area() const {
        return width() * height();
    }
};