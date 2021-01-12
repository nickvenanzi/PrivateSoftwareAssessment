#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H

#include "Grid.hpp"
#include <unordered_set>
#include "MatrixUtils/Matrix.hpp"

/*
Abstract Base Class for generating shapes using 
a least-squares method and given input GridPoints.
*/
class ShapeGenerator {

    public:
    
        /*
        Helper method for computing residual vector for
        each iteration of LS Method.
        */
        virtual Row computeResiduals() = 0;

        /*
        Helper method used to compute Jacobian for a given 
        shape. Returns the (i,j)th element of the Jacobian.
        */
        virtual float computePartial(int sample_index, int param) = 0;
        
        std::shared_ptr<Grid> grid_ptr;
        std::vector<int> pts;
        std::vector<float> params;

        /*
        Abstract method for computing the shape parameters using
        LS Method. Computes 'params' in 20 iterations.
        */
        void computeShape() {
            for (int i = 0; i < 100; i++) {
                Row residuals = computeResiduals();
                Matrix jacobian = computeJacobian();
                Matrix j_t = jacobian.transpose();
                Matrix j2 = Matrix::multiply(j_t, jacobian);
                Matrix shift_matrix = Matrix::multiply(j2.inverse(), j_t);
                
                for (int k = 0; k < params.size(); k++) {
                    params[k] -=  Row::dot(shift_matrix.getRow(k), residuals);
                }
            }
        }

        /*
        Helper method for computeShape(), used heavily in computing
        the shift vector in each iteration.
        */
        Matrix computeJacobian() {
            std::vector<Row> j_rows;
            for (int i = 0; i < pts.size(); i++) {
                // for each row of jacobian compute partial wrt each parameter:
                std::vector<float> j_row;
                for (int j = 0; j < params.size(); j++) {
                    j_row.push_back(computePartial(i, j));
                }
                j_rows.push_back(Row(j_row));
            }
            return Matrix(j_rows);
        }

};

#endif