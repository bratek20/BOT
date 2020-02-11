#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

struct Point {  
    int x = 0;
    int y = 0;

    Point();
    Point(int x, int y);
    Point(POINT p);

    bool operator==(const Point& p);
    bool operator!=(const Point& p);
    Point operator+(const Point& p);
    Point operator-(const Point& p);

    std::string toString() const;
    friend std::ostream& operator <<(std::ostream& out, const Point& p);

    static Point NONE;
};