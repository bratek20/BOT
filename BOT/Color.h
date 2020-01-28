#pragma once

#include <iostream>

struct Color {
    int r = 0;
    int g = 0;
    int b = 0;

    Color() {}
    Color(int r, int g, int b) :
        r(r), g(g), b(b) {}

    bool operator==(const Color& c) {
        return r == c.r && g == c.g && b == c.b;
    } 

    bool operator!=(const Color& c) {
        return !(*this == c);
    }

    Color operator+(const Color& c) {
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color operator-(const Color& c) {
        return Color(r - c.r, g - c.g, b - c.b);
    } 

    friend std::ostream& operator <<(std::ostream& out, const Color& c) {
        out << "red: " << c.r << ", green: " << c.g << ", blue: " << c.b << "\n";
        return out;
    }
};