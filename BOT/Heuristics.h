#pragma once
#include "BOT.h"
#include "Screen.h"
#include "BmpRect.h"

/* !!! not always works !!!*/
class Heuristics {
public:
    static bool matchRect(const Point& startPoint, const BmpRect& rect) {
        int hit = 0, cnt=0;
        for (int y = startPoint.y; y <= startPoint.y + rect.height(); y++) {
            for (int x = startPoint.x; x <= startPoint.x + rect.width(); x++, cnt++) {
                if (BOT::screen.colorAt(x, y) == rect.colorAt(x - startPoint.x, y - startPoint.y)) {
                    hit++;
                }
                if (cnt > 10 && hit * 2 < cnt) {
                    return false;
                }
            }
        }
        int min = rect.width() * rect.height() * 95 / 100;
        if (cnt > min) {
            return true;
        }
        return false;
    }

    static bool matchRectBySum(const Point& startPoint, const BmpRect& rect) {
        Point endPoint = Point(startPoint.x + rect.width()-1, startPoint.y + rect.height()-1);
        Color c = BOT::screen.colorSumAtRect(Rect(startPoint, endPoint));

        Color rectSum = rect.colorSum();
        int size = rect.width() * rect.height();
        return similar(c.r, rectSum.r, size) && similar(c.g, rectSum.g, size) && similar(c.b, rectSum.b, size);

    }

private:
    static const int SIMILAR_OFF = 2;
    static bool similar(int c1, int c2, int rectSize) {
        return abs(c1 - c2) < SIMILAR_OFF * rectSize;
    }
};