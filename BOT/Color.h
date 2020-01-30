#pragma once

#include <iostream>

struct Color {
    float r = 0;
    float g = 0;
    float b = 0;

    Color() {}
    Color(int r, int g, int b) :
        r(r / 255.f), g(g / 255.f), b(b / 255.f) {}
    Color(float r, float g, float b) :
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

    int ir() const {
        return static_cast<int>(r * 255);
    }

    int ig() const {
        return static_cast<int>(g * 255);
    }

    int ib() const {
        return static_cast<int>(b * 255);
    }
    friend std::ostream& operator <<(std::ostream& out, const Color& c) {
        out << "red: " << c.r << ", green: " << c.g << ", blue: " << c.b << "\n";
        return out;
    }
};