#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "Row.hpp"
#include <iostream>

/*
Abstract representation of a matrix. Used in Least Squares algorithm
*/
class Matrix {

    public:

        /*
        Constructor takes in a vector of Row objects, each representing
        a row in the matrix.
        */
        Matrix(std::vector<Row> rows_) {
            rows = rows_;
        }

        /*
        Assumes 'i' is nonnegative and less than the number of rows in matrix.
        */
        Row getRow(int i) {
            return rows[i];
        }

        /*
        Class method that produces an identity matrix of size 'n' x 'n'.
        */
        static Matrix identity(int n) {
            std::vector<Row> rows;
            std::vector<float> zeros(n, 0.f);
            for (int i = 0; i < n; i++) {
                Row row = Row(zeros);
                row.set(i, 1.f);
                rows.push_back(row);
            }
            return Matrix(rows);
        }

        /*
        String representation of Matrix, used for debugging purposes.
        */
        std::string string() {
            std::string out("[\n");
            for (int i = 0; i < rows.size(); i++) {
                out = out + " " + rows[i].string() + "\n";
            }
            return out + "]";
        }

        /*
        Computes inverse of a square matrix. Uses Gauss-Jordan elimination
        */
        Matrix inverse() {
            Matrix matrix = copy();

            //construct into augmented form
            Matrix aug_mat = Matrix::identity(matrix.num_rows());

            for (int row_num = 0; row_num < matrix.num_rows(); row_num++) {
                
                //for floating point accuracy concerns, row with largest leading coefficient
                //is swapped up to the current row and used for the current iteration.
                float current_best = matrix.getRow(row_num).at(row_num);
                int best_i = row_num;
                for (int other_row_num = row_num + 1; other_row_num < matrix.num_rows(); other_row_num++) {
                    float other_lead = matrix.getRow(other_row_num).at(row_num);
                    if (abs(other_lead) > abs(current_best)) {
                        current_best = other_lead;
                        best_i = other_row_num;
                    }
                }
                if (best_i != row_num) {
                    matrix.swap(row_num, best_i);
                    aug_mat.swap(row_num, best_i);
                }

                Row row = matrix.getRow(row_num);
                Row aug_row = aug_mat.getRow(row_num);

                // scale current row so that lead coeff. is 1.0f
                float scale = 1.f/row.at(row_num);

                row.scale(scale);
                aug_row.scale(scale);
                matrix.rows[row_num] = row;
                aug_mat.rows[row_num] = aug_row;


                //for each other_row != row, subtract off curr row
                for (int other_row_num = 0; other_row_num < matrix.num_rows(); other_row_num++) {
                    if (other_row_num == row_num) {
                        continue;
                    }
                    Row otherRow = matrix.getRow(other_row_num);
                    Row aug_otherRow = aug_mat.getRow(other_row_num);
                    float sub_scale = otherRow.at(row_num);
                    otherRow.subtractOff(row, sub_scale);
                    aug_otherRow.subtractOff(aug_row, sub_scale);

                    matrix.rows[other_row_num] = otherRow;
                    aug_mat.rows[other_row_num] = aug_otherRow;
                }
            }
            return aug_mat;
        }

        /*
        Produces transpose of the instance matrix.
        */
        Matrix transpose() {
            std::vector<Row> t_rows;
            for (int i = 0; i < rows[0].row.size(); i++) {
                std::vector<float> t_row;
                for (int j = 0; j < rows.size(); j++) {
                    t_row.push_back(rows[j].at(i));
                }
                t_rows.push_back(Row(t_row));
            }
            return Matrix(t_rows);
        }

        /*
        Static method takes in two matrices and multiplies
        m1 * m2. Returns the product.
        */
        static Matrix multiply(Matrix m1, Matrix m2) {
            Matrix m2_t = m2.transpose();
            std::vector<Row> result;
            for (int i = 0; i < m1.rows.size(); i++) {
                std::vector<float> new_row;
                for (int j = 0; j < m2_t.rows.size(); j++) {
                    new_row.push_back(Row::dot(m1.getRow(i), m2_t.getRow(j)));
                }
                result.push_back(Row(new_row));
            }
            return Matrix(result);
        }

        /*
        Method returns the number of rows in the matrix.
        */
        int num_rows() {
            return rows.size();
        }

        std::vector<Row> rows;

    private:

        /*
        Helper method used to assist Matrix inversion method.
        */
        void swap(int i, int j) {
            Row firstRow = getRow(i);
            Row secondRow = getRow(j);
            rows[i] = secondRow;
            rows[j] = firstRow;
        }

        /*
        Helper method used with in-place Gaussian row-echelon elimination in
        the matrix inversion process. Produces deep copy of matrix.
        */
        Matrix copy() {
            std::vector<Row> new_rows;
            for (int i = 0; i < rows.size(); i++) {
                new_rows.push_back(rows[i].copy());
            }
            return Matrix(new_rows);
        }

};

#endif
