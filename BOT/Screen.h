#pragma once
#include <future>

#include "Point.h"
#include "Color.h"
#include "BmpRect.h"

class Screen {
    BYTE* _screen;
    Color* _colorSums;
    int _width;
    int _height;

public:
    Screen();

    void update();

    Color colorAt(int x, int y) const;
    Color colorAt(const Point& p) const;
    Color colorSumAt(int x, int y) const;
    Color colorSumAtRect(const Rect& rect) const;

    void save();

    Point find(const BmpRect& rect);

    int width() const { return _width; }
    int height() const { return _height; }

private:
    void findInArea(std::promise<Point> && prom, int startX, int endX, const BmpRect& rect);

    void updateColorSums();

    bool matchRect(const Point& startPoint, const BmpRect& rect);
    bool matchRectBySum(const Point& startPoint, const BmpRect& rect);
};
