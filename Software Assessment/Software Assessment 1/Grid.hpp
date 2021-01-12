#ifndef GRID_H
#define GRID_H

#include "GridPoint.hpp"
#include <vector>

/*
Object representing abstract set of GridPoints composing 
the rendered grid of squares in the window
*/
class Grid {
    public:
        /*  
        Constructs Grid with m x n GridPoints where m is 'w' and 
        n is 'h'. Each GridPoint is a square of sideLength 'pointSize' and 
        is spaced 'deltaX' apart horizontally and 'deltaY' apart vertically.
        Grid is oriented from 'topLeftCorner'. 'pointSize' must be less than deltaX and deltaY.
        */
        Grid(Coordinate topLeftCorner, int w, int h, float deltaX, float deltaY, float pointSize);

        /*
        This method retrieves the GridPoint located at row 'i' and
        column 'j' in the grid. 0 <= i < width && 0 <= j < height.
        */
        GridPoint getPoint(int i, int j) const;

        /*
        This method is used to fill the grid into all available
        window space anytime the window resizes. 'deltaX' and 
        'deltaY' should both be nonnegative floats.
        */
        void resizeGrid(float deltaX, float deltaY);

        int width, height;
        
    private:
        std::vector<GridPoint> gridPoints;
        Coordinate referencePoint;

};

#endif