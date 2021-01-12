#ifndef CIRCLE_GENERATOR_H
#define CIRCLE_GENERATOR_H

#include <unordered_set>
#include "MatrixUtils/Matrix.hpp"
#include "ShapeGenerator.hpp"

/*
Abstract representation for computing circle parameters to fit 
the input GridPoints.
*/
class CircleGenerator: public ShapeGenerator {

    public:
        /*
        Constructor for computing least-squares best fit for 
        a circle drawn around the given GridPoints.
        */
        CircleGenerator(std::shared_ptr<Grid> grid_ptr_, std::unordered_set<int> toggled_points);
        Row computeResiduals();
        float computePartial(int sample_index, int param);
};

#endif