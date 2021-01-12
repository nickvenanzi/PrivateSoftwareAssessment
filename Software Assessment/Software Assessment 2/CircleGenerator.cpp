#include "CircleGenerator.hpp"

CircleGenerator::CircleGenerator(std::shared_ptr<Grid> grid_ptr_, std::unordered_set<int> toggled_points) {
    params = std::vector<float>(3, 0.f);
    //give a baseline guess-stimate of the correct circle center and radius
    //by averaging the coordinates of each point.
    int points = toggled_points.size();
    float avg_x = 0.f, avg_y = 0.f, avg_r = 0.f;
    for (auto iter = toggled_points.begin(); iter != toggled_points.end(); ++iter) {
        int index = *iter;
        int i = index % grid_ptr_->width;
        int j = index / grid_ptr_->height;
        GridPoint gp = grid_ptr_->getPoint(i, j);
        avg_x += gp.center.x;
        avg_y += gp.center.y;
    }
    avg_x = avg_x / float(points);
    avg_y = avg_y / float(points);
    Coordinate c = {avg_x, avg_y};

    for (auto iter = toggled_points.begin(); iter != toggled_points.end(); ++iter) {
        int index = *iter;
        int i = index % grid_ptr_->width;
        int j = index / grid_ptr_->height;
        GridPoint gp = grid_ptr_->getPoint(i, j);
        avg_r += Coordinate::computeDistance(gp.center, c);
    }
    avg_r = avg_r / float(points);
    params[0] = avg_x;
    params[1] = avg_y;
    params[2] = avg_r;

    grid_ptr = grid_ptr_;
    for (auto iter = toggled_points.begin(); iter != toggled_points.end(); ++iter) {
        pts.push_back(*iter);
    }
    computeShape();
}

float CircleGenerator::computePartial(int sample_index, int param) {
    int index = pts[sample_index];
    int i = index % grid_ptr->width;
    int j = index / grid_ptr->height;
    GridPoint gp = grid_ptr->getPoint(i, j);
    float x_i = gp.center.x;
    float y_i = gp.center.y;

    float x_c = params[0], y_c = params[1];
    float r_c = params[2];

    float dist = sqrt((x_i - x_c) * (x_i - x_c) + (y_i - y_c) * (y_i - y_c));
    if (param == 0) {
        // x_c
        return (x_c - x_i) / dist;
    } else if (param == 1) {
        //y_c
        return (y_c - y_i) / dist;
    } else {
        //r_c
        return -1.f;
    }
}

Row CircleGenerator::computeResiduals() {
    std::vector<float> residuals;
    for (int k = 0; k < pts.size(); k++) {
        int index = pts[k];
        int i = index % grid_ptr->width;
        int j = index / grid_ptr->height;
        GridPoint gp = grid_ptr->getPoint(i, j);
        float x_i = gp.center.x;
        float y_i = gp.center.y;

        float x_c = params[0], y_c = params[1];
        float r_c = params[2];

        float dist = sqrt((x_i - x_c) * (x_i - x_c) + (y_i - y_c) * (y_i - y_c));

        residuals.push_back(dist - r_c);
    }
    return Row(residuals);
}

