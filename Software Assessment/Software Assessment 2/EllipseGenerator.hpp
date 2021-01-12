#ifndef ELLIPSE_GENERATOR_H
#define ELLIPSE_GENERATOR_H

#include "Grid.hpp"
#include <unordered_set>
#include "MatrixUtils/Matrix.hpp"
#include "ShapeGenerator.hpp"

/*
Abstract representation for computing ellipse parameters to fit 
the input GridPoints.
*/
class EllipseGenerator: public ShapeGenerator {

    public:
        /*
        Constructor for computing least-squares best fit for 
        an ellipse drawn around the given GridPoints.
        */
        EllipseGenerator(std::shared_ptr<Grid> grid_ptr_, std::unordered_set<int> toggled_points);
        Row computeResiduals();
        float computePartial(int sample_index, int param);
    
    private:
        /*
        Helper function to help compute residuals
        */
        float computeDistance(int sample_index);
};

#endif