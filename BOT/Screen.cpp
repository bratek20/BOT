#include "Screen.h"
#include "Logger.h"
#include "Heuristics.h"
#include <cstdio>
#include <string>

using namespace std;

Screen::Screen() {   
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    _width = rect.right;
    _height = rect.bottom;
    _colorSums = new Color[_width * _height];
}

void Screen::update() {
    HDC hdc, hdcTemp;

    hdc = GetDC(HWND_DESKTOP);

    hdcTemp = CreateCompatibleDC(hdc);
    BITMAPINFO bitmap;
    bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
    bitmap.bmiHeader.biWidth = _width;
    bitmap.bmiHeader.biHeight = _height;
    bitmap.bmiHeader.biPlanes = 1;
    bitmap.bmiHeader.biBitCount = 32;
    bitmap.bmiHeader.biCompression = BI_RGB;
    bitmap.bmiHeader.biSizeImage = _width * 4 * _height;
    bitmap.bmiHeader.biClrUsed = 0;
    bitmap.bmiHeader.biClrImportant = 0;
    HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&_screen), NULL, NULL);
    SelectObject(hdcTemp, hBitmap2);
    BitBlt(hdcTemp, 0, 0, _width, _height, hdc, 0, 0, SRCCOPY);

    updateColorSums();
}

void Screen::updateColorSums() {
    _colorSums[0] = colorAt(0, 0);
    for (int i = 1; i < _width; i++) {
        _colorSums[i] = _colorSums[i-1] + colorAt(i, 0);
    }
    for (int i = 1; i < _height; i++) {
        _colorSums[i*_width] = _colorSums[(i - 1)*_width] + colorAt(0, i);
    }

    for (int i = 1; i < _height; i++) {
        for (int j = 1; j < _width; j++){
            _colorSums[i*_width + j] =
                _colorSums[(i - 1)*_width + j] + _colorSums[i*_width + j - 1] - _colorSums[(i - 1)*_width + j - 1] + colorAt(j, i);
        }
    }
}

Color Screen::colorAt(int x, int y) const {
    int i = _height - 1 - y;
    int j = x;

    int k = (i * _width + j) * 4;

    return Color(_screen[k + 2], _screen[k + 1], _screen[k]);
}

Color Screen::colorAt(const Point& p) const {
    return colorAt(p.x, p.y);
}

Color Screen::colorSumAt(int x, int y) const {
    return _colorSums[y*_width + x];
}

Color Screen::colorSumAtRect(const Rect& rect) const {
    Color c = colorSumAt(rect.p2.x, rect.p2.y);
    if (rect.p1.y > 0) {
        c = c - colorSumAt(rect.p2.x, rect.p1.y - 1);
    }
    if (rect.p1.x > 0) {
        c = c - colorSumAt(rect.p1.x - 1, rect.p2.y);
    }
    if (rect.p1.y > 0 && rect.p1.x > 0) {
        c = c + colorSumAt(rect.p1.x - 1, rect.p1.y - 1);
    }

    return c;
}

void Screen::save() {
    FILE* fp;
    fopen_s(&fp, "screenshot.ppm", "w");

    fprintf(fp, "P3\n%d %d\n255\n", _width, _height);
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            Color color = colorAt(x, y);
            fprintf(fp, "%4d%4d%4d", color.r, color.g, color.b);
        }
    }

    fclose(fp);
    Logger::info("screen", "save");
}

Point Screen::find(const BmpRect& rect) {
    static const int SECTIONS = 6;

    int offX = _width / SECTIONS;
    future<Point> ans[SECTIONS];
    thread threads[SECTIONS];
    for (int startX = 0, endX = offX, i=0; i < SECTIONS; startX += offX, endX += offX, i++) {
        promise<Point> prom;
        ans[i] = prom.get_future();
        threads[i] = thread(findInArea, move(prom), ref(*this), startX, endX, ref(rect) );
    }

    vector<Point> points;
    for (int i = 0; i < SECTIONS; i++) {
        threads[i].join();
        auto point = ans[i].get();
        if (point != Point::NONE) {
            points.push_back(point);
        }
    }

    Logger::info("Screen::find", "founded " + to_string(points.size()) + " points");
    if (!points.empty()) {
        return points[0];
    }
    return Point::NONE;
}

void Screen::findInArea(promise<Point> && prom, const Screen& screen, int startX, int endX,  const BmpRect& rect) {
    endX = min(endX, screen.width() - rect.width());

    for (int y = 0; y < screen.height() - rect.height(); y++) {
        for (int x = startX; x < endX; x++) {
            Point p = Point(x, y);
            if (Heuristics::matchRectBySum(p, rect)) {
                prom.set_value(p);
                return;
            }
        }
    }

    prom.set_value(Point::NONE);
}
