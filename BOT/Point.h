#pragma once
#include <Windows.h>
#include <iostream>

struct Point {  
    int x = 0;
    int y = 0;

    Point();
    Point(int x, int y);
    Point(POINT p);

    bool operator==(const Point& p);
    bool operator!=(const Point& p);

    friend std::ostream& operator <<(std::ostream& out, const Point& p);

    static Point NONE;
};