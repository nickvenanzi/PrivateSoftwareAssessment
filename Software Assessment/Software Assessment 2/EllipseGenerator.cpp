#include "EllipseGenerator.hpp"

EllipseGenerator::EllipseGenerator(std::shared_ptr<Grid> grid_ptr_, std::unordered_set<int> toggled_points) {
    params = std::vector<float>(5 + toggled_points.size(), 0.f);
    for (auto iter = toggled_points.begin(); iter != toggled_points.end(); ++iter) {
        pts.push_back(*iter);
    }

    float PI = 3.141592653f;
    //give a baseline guess-stimate of the correct circle center and radius
    //by averaging the coordinates of each point.
    int points = pts.size();
    float avg_x = 0.f, avg_y = 0.f, avg_r = 0.f;
    for (int k = 0; k < pts.size(); k++) {
        int index = pts[k];
        int i = index % grid_ptr_->width;
        int j = index / grid_ptr_->height;
        GridPoint gp = grid_ptr_->getPoint(i, j);
        avg_x += gp.center.x;
        avg_y += gp.center.y;
    }
    avg_x = avg_x / float(points);
    avg_y = avg_y / float(points);
    Coordinate c = {avg_x, avg_y};

    for (int k = 0; k < pts.size(); k++) {
        int index = pts[k];
        int i = index % grid_ptr_->width;
        int j = index / grid_ptr_->height;
        GridPoint gp = grid_ptr_->getPoint(i, j);
        avg_r += Coordinate::computeDistance(gp.center, c);
     
        params[5 + k] = atanf((gp.center.y - avg_y)/(gp.center.x - avg_x)) + (gp.center.x < avg_x ? PI : 0.f);
    }
    avg_r = avg_r / float(points);
    params[0] = avg_x;
    params[1] = avg_y;
    params[2] = avg_r;
    params[3] = avg_r / 2.f;
    // leave alpha at 0
    
    grid_ptr = grid_ptr_;
    for (int i = 0; i < toggled_points.size(); i++) {
        pts.push_back(pts[i]);
    }
    computeShape();
}

float EllipseGenerator::computePartial(int sample_index, int param) {
    int index = pts[sample_index];
    int i = index % grid_ptr->width;
    int j = index / grid_ptr->height;
    GridPoint gp = grid_ptr->getPoint(i, j);
    float x_i = gp.center.x;
    float y_i = gp.center.y;

    float x_c = params[0], y_c = params[1];
    params[2] = params[2] > 0.f ? params[2] : -params[2];
    params[3] = params[3] > 0.f ? params[3] : -params[3];

    float a = params[2], b = params[3];

    float alpha = params[4];
    int half_size = pts.size()/2;
    int real_index = sample_index >= half_size ? sample_index - half_size : sample_index;
    float phi = params[real_index + 5];

    if (param == 0) {
        // x_c
        if (real_index == sample_index) {
            // x comp.
            return -1.f;
        } else {
            //y comp.
            return 0.f;
        }
    } else if (param == 1) {
        //y_c
        if (real_index == sample_index) {
            // x comp.
            return 0.f;
        } else {
            //y comp.
            return -1.f;
        }
    } else if (param == 2) {
        //a
        if (real_index == sample_index) {
            // x comp.
            return -cos(alpha)*cos(phi);
        } else {
            //y comp.
            return -sin(alpha)*cos(phi);
        }
    } else if (param == 3) {
        //b
        if (real_index == sample_index) {
            // x comp.
            return sin(alpha)*sin(phi);
        } else {
            //y comp.
            return - cos(alpha)*sin(phi);
        }
    } else if (param == 4) {
        //alpha
        if (real_index == sample_index) {
            // x comp.
            return a*sin(alpha)*cos(phi) + b*cos(alpha)*sin(phi);
        } else {
            //y comp.
            return -a*cos(alpha)*cos(phi) + b*sin(alpha)*sin(phi);
        }
    } else if (param == real_index + 5) {
        //phi, i == j
        if (real_index == sample_index) {
            // x comp.
            return a*cos(alpha)*sin(phi) + b*sin(alpha)*cos(phi);
        } else {
            //y comp.
            return a*sin(alpha)*sin(phi) - b*cos(alpha)*cos(phi);
        }
    } else {
        //phi, i != j
        return 0.f;
    }
}

float EllipseGenerator::computeDistance(int sample_index) {
    int index = pts[sample_index];
    int i = index % grid_ptr->width;
    int j = index / grid_ptr->height;
    GridPoint gp = grid_ptr->getPoint(i, j);
    float x_i = gp.center.x;
    float y_i = gp.center.y;
    float x_c = params[0], y_c = params[1];
    params[2] = params[2] > 0.f ? params[2] : -params[2];
    params[3] = params[3] > 0.f ? params[3] : -params[3];

    float a = params[2], b = params[3];
    float alpha = params[4];
    int half_size = pts.size()/2;
    int real_index = sample_index >= half_size ? sample_index - half_size : sample_index;
    float phi = params[real_index + 5];

    if (real_index == sample_index) {
        //compute d_ix
        return x_i - x_c - a*cos(alpha)*cos(phi) + b*sin(alpha)*sin(phi);
    } else {
        //compute d_iy
        return y_i - y_c - a*sin(alpha)*cos(phi) - b*cos(alpha)*sin(phi);
    }
}


Row EllipseGenerator::computeResiduals() {
    std::vector<float> residuals;
    for (int k = 0; k < pts.size(); k++) {
        int size = pts.size();
        if (k < size/2) {
            //x_residual
            residuals.push_back(computeDistance(k));
        } else {
            //y_residual
            residuals.push_back(computeDistance(k));
        }
    }
    return Row(residuals);
}