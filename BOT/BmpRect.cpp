#include "BmpRect.h"
#include "Rect.h"
#include "BOT.h"
#include "Screen.h"
#include "Logger.h"

BmpRect::BmpRect() {
    _colors = nullptr;
    _size = 0;
    _colorSum = Color(0, 0, 0);
}

BmpRect::BmpRect(Point _p1, Point _p2) : Rect(_p1, _p2) {
    _size = area();
    _colors = new Color[_size];
    _colorSum = BOT::screen.colorSumAtRect(*this);

    for (int y = p1.y, i = 0; y <= p2.y; y++) {
        for (int x = p1.x; x <= p2.x; x++, i++) {
            _colors[i] = BOT::screen.colorAt(x, y);
        }
    }
}

  
BmpRect::BmpRect(BmpRect&& rect) : Rect((Rect)rect) {
    _colorSum = rect._colorSum;
    _colors = rect._colors;
    rect._colors = nullptr;
}

 
BmpRect& BmpRect::operator=(BmpRect&& rect) {
    delete[] _colors;

    p1 = rect.p1;
    p2 = rect.p2;

    _colorSum = rect._colorSum;
    _size = rect._size;
    _colors = rect._colors;
    rect._colors = nullptr;

    return *this;
}

BmpRect::~BmpRect() {
    delete[] _colors;
}

Color BmpRect::colorAt(int x, int y) const {
    int idx = y*width() + x;
    return _colors[idx];
}

Color BmpRect::colorSum() const {
    return _colorSum;
}

void BmpRect::save() {
    FILE* fp;
    fopen_s(&fp, "BmpRect.ppm", "w");

    int w = width(), h = height();
    fprintf(fp, "P3\n%d %d\n255\n", w, h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Color color = colorAt(x, y);
            fprintf(fp, "%4d%4d%4d", color.r, color.g, color.b);
        }
    }

    fclose(fp);
    Logger::info("BmpRect::save()", "saved");
}


