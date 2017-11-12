#pragma once
#include <future>
#include "Point.h"
#include "Color.h"

struct Rect;
class BmpRect;

class Screen {
    BYTE* _screen;
    Color* _colorSums;
    int _width;
    int _height;

    void updateColorSums();
    static void findInArea(std::promise<Point> && prom, const Screen& screen, int startX, int endX, const BmpRect& rect);

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
};
