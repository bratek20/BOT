#pragma once
#include "Rect.h"
#include "Color.h"

class Screen;

class BmpRect : public Rect {
    Color* _colors;
    int _size;
    Color _colorSum;

public:
    BmpRect();

    BmpRect(Point _p1, Point _p2);
    
    BmpRect(const BmpRect& rect) = delete;
    BmpRect(BmpRect&& rect);

    BmpRect& operator=(const BmpRect& rect) = delete;
    BmpRect& operator=(BmpRect&& rect);

    ~BmpRect();

    Color colorAt(int x, int y) const;
    Color colorSum() const;

    void save();  
};