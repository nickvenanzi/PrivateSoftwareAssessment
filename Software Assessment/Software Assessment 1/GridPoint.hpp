#ifndef GRID_POINT_H_
#define GRID_POINT_H_

#include "Coordinate.hpp"

/*
Object representing an square coordinate in the
rendered window
*/
struct GridPoint {
    Coordinate center;
    float sideLength;

    bool operator== (const GridPoint &other) const {
        return center == other.center && approxEqual(sideLength, other.sideLength);
    }

    static const float EPSILON = 0.00001f;

    static bool approxEqual(float a, float b) {
        return (a > b ? a - b : b - a) < EPSILON;
    }
};

#endif