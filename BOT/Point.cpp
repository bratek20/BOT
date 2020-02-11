#include "Point.h"

using namespace std;

Point Point::NONE = Point(-1, -1);

Point::Point() {}
Point::Point(int x, int y) : x(x), y(y) {}
Point::Point(POINT p) : Point(p.x, p.y) {}

bool Point::operator==(const Point& p) {
    return x == p.x && y == p.y;
}
bool Point::operator!=(const Point& p) {
    return !(*this == p);
}

Point Point::operator+(const Point& p) {
    return Point(x + p.x, y + p.y);
}

Point Point::operator-(const Point& p) {
    return Point(x - p.x, y - p.y);
}

std::string Point::toString() const {
    return "(" + to_string(x) + ", " + to_string(y) + ")";
}

std::ostream& operator <<(std::ostream& out, const Point& p) {
    out << p.toString();
    return out;
}