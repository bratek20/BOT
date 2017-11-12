#include "Point.h"

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

std::ostream& operator <<(std::ostream& out, const Point& p) {
    out << "x: " << p.x << ", y: " << p.y;
    return out;
}