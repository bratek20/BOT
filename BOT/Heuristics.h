#pragma once
#include "BOT.h"
#include "Screen.h"
#include "BmpRect.h"
#include "Params.h"

/* !!! not always works !!!*/
class Heuristics {
public:
    static bool matchRect(const Point& startPoint, const BmpRect& rect) {
        int hit = 0, cnt=0;
        int area = rect.area();
        auto matches = [area](int num) {
            return (float)num / area >= Params::MATCH_THRESHOLD;
        };

        for (int y = startPoint.y; y < startPoint.y + rect.height(); y++) {
            for (int x = startPoint.x; x < startPoint.x + rect.width(); x++) {
                if (BOT::screen.colorAt(x, y) == rect.colorAt(x - startPoint.x, y - startPoint.y)) {
                    hit++;
                }
                cnt++;

                int possibleBest = hit + area - cnt;
                if (!matches(possibleBest)) {
                    return false;
                }
            }
        }
        return matches(hit);
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