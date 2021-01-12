#ifndef ROW_H
#define ROW_H

#include <vector>
#include <string>
#include <sstream>

/*
Object representing the row of a matrix or a vector. Composed of floats.
*/
class Row {

    public:

        /*
        Constructor takes in initial vector of floats and creates Row object
        consisting of those elements.
        */
        Row(std::vector<float> init_row) {
            for (int i = 0; i < init_row.size(); i++) {
                row.push_back(init_row[i]);
            }
        }

        /*
        Assumes 0 <= 'n' < row.size(). Retrieves nth element in row.
        */
        float at(int n) {
            return row[n];
        }

        /*
        Assumes 0 <= 'n' < row.size(). Sets nth element in row to 'val'.
        */
        float set(int n, float val) {
            row[n] = val;
        }

        /*
        Scales every element in row up by factor of 'scale'.
        */
        void scale(float scale) {
            for (int i = 0; i < row.size(); i++) {
                row[i] = row[i] * scale;
            }
        }

        /*
        Method used solely for Gauss-Jordan Elimination. Takes in another
        row object and a scale factor and subtracts off each element of the 
        other_row scaled up by 'scale', from each element in the instance row.
        */
        void subtractOff(Row other_row, float scale) {
            for (int i = 0; i < row.size(); i++) {
                row[i] = row[i] - scale * other_row.at(i);
            }
        }

        /*
        Helper method used for debugging. Returns string form of 
        Row.

        i.e. string() -> [ 1.2  2.3  -0.9]
        */
        std::string string() {
            std::string out("[");
            for (int i = 0; i < row.size(); i++) {
                std::ostringstream ss;
                ss << at(i);
                std::string s(ss.str());
                out = out + " " + s;
            }
            return out + "]";
        }

        /*
        Copies row and returns copy. Used in Matrix::copy()
        */
        Row copy() {
            std::vector<float> new_row;
            for (int i = 0; i < row.size(); i++) {
                new_row.push_back(at(i));
            }
            return Row(new_row);
        }

        /*
        static method takes in two Rows and computes the respective
        dot product, and returns the result as a float. Assumes rows are 
        the same length.
        */
        static float dot(Row r1, Row r2) {
            float result = 0.f;
            for (int i = 0; i < r1.row.size(); i++) {
                result += r1.at(i) * r2.at(i);
            }
            return result;
        }

        std::vector<float> row;


};

#endif