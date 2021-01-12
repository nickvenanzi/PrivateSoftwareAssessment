#ifndef DIGITIZED_CIRCLE_H
#define DIGITIZED_CIRCLE_H

#include "Grid.hpp"
#include <unordered_set>
#include <unordered_map>

/*
Object representing abstract data needed to render the blue circle,
the inner and outer red circles, and keep track of which GridPoints to
highlight.
*/
class DigitizedCircle {

    public:

        /*
        Constructor computes the inner and outer circle radii and which 
        GridPoints within the 'grid_ptr' to highlight.
        */
        DigitizedCircle(std::shared_ptr<Grid> grid_ptr, Coordinate center, float radius);

        float innerRadius, outerRadius, radius_;
        Coordinate center_;
        std::unordered_set<int> highlighted_pts;
        
    private:
        /*
        iterates through each GridPoint and calculates shortest distance to the
        blue circle. Distance is positive if outside circle and negative if inside circle.

        Constructs signed_distance_field.
        */
        void setSignedDistanceField();

        /*
        Computes GridPoints to highlight and constructs hightlighted_pts.

        Computes innerRadius and outerRadius along the way

        Algorithm: SquareMarch (my 2-D version of CubeMarching)
        Using signed_distance_field, finds every pair of two GridPoints with
        the circle crossing that edge (GridPoints will have opposite signs in distance field).

        For each edge for which the circle crosses, highlight the GridPoint closer to the 
        circle (the GridPoint with the smaller absolute value signed distance value).

        Along the way compute the innerRadius and outerRadius by taking each highlighted 
        GridPoint and computing the furthest and closest distance from the blue circle's center to any of the 
        4 corners on the gridPoint. If the furthest corner distance is greater than outerRadius, update
        outerRadius. If the closest corner distance is less than innerRadius, update innerRadius.
        */
        void squareMarch();

        /*
        Computes distance from center of blue circle to furthest
        corner on GridPoint.
        */
        float getFurthestDistance(GridPoint gp);

        /*
        Computes distance from center of blue circle to closest
        corner on GridPoint.
        */
        float getClosestDistance(GridPoint gp);


        std::shared_ptr<Grid> grid_ptr_;
        std::unordered_map<int, float> signed_distance_field;

};

#endif