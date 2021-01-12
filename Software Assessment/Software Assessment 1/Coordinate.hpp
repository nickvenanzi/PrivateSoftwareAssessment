#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <math.h>

/*
Object representing an (x, y) coordinate in the
rendered window
*/
struct Coordinate {
    float x;
    float y;

    bool operator== (const Coordinate &other) const {
        return  approxEqual(x, other.x) && approxEqual(y, other.y);
    }

    Coordinate operator + (Coordinate const &other) {
        return {x + other.x, y + other.y};
    }

    Coordinate operator - (Coordinate const &other) {
        return {x - other.x, y - other.y};
    }

    static const float EPSILON = 0.00001f;

    static bool approxEqual(float a, float b) {
        return (a > b ? a - b : b - a) < EPSILON;
    }

    static float computeDistance(Coordinate c1, Coordinate c2) {
        return sqrt((c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y));
    }
};

#endif