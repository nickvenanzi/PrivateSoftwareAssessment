#include "Grid.hpp"

Grid::Grid(Coordinate topLeftCorner, int w, int h, float deltaX, float deltaY, float pointSize) {
    width = w;
    height = h;
    referencePoint = topLeftCorner;

    //construct GridPoints left to right, top to bottom
    for (int j = 1; j <= height; j++) {
        for (int i = 1; i <= width; i++) {
            Coordinate distFromRef = {deltaX * i, - deltaY * j};
            Coordinate gridPointCenter = topLeftCorner + distFromRef;
            GridPoint gridPoint = {gridPointCenter, pointSize};
            gridPoints.push_back(gridPoint);
        }
    }
};

GridPoint Grid::getPoint(int i, int j) const {
    return gridPoints[i + width * j];
};

void Grid::resizeGrid(float deltaX, float deltaY) {
    //loop over each GridPoint and reset center
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            GridPoint gp = gridPoints[i + width * j];

            Coordinate distFromRef = {deltaX * i, - deltaY * j};
            gp.center = referencePoint + distFromRef;
        }
    }
};