#include "DigitizedCircle.hpp"

DigitizedCircle::DigitizedCircle(std::shared_ptr<Grid> grid_ptr, Coordinate center, float radius) {
    grid_ptr_ = grid_ptr;
    center_ = center;
    radius_ = radius;
    innerRadius = radius;
    outerRadius = radius;

    signed_distance_field = std::unordered_map<int, float>();
    highlighted_pts = std::unordered_set<int>();
    //Compute signed distance field before executing algorithm
    setSignedDistanceField();

    //compute hightlighted GridPoint coordinates
    //and inner/outer radii.
    squareMarch();
};

void DigitizedCircle::setSignedDistanceField() {

    for (int j = 0; j < grid_ptr_->height; j++) {
        for (int i = 0; i < grid_ptr_->width; i++) {
            GridPoint gp = grid_ptr_->getPoint(i, j);
            float distanceToCenter = Coordinate::computeDistance(gp.center, center_);
            int gp_index = i + grid_ptr_->width * j;
            signed_distance_field[gp_index] = distanceToCenter - radius_;
        }
    }
};

void DigitizedCircle::squareMarch() {
    //floating point tolerance parameter
    float epsilon = 0.000001f;

    // first loop across horizontal pairs of GridPoints
    for (int j = 0; j < grid_ptr_->height; j++) {
        for (int i = 0; i < grid_ptr_->width - 1; i++) {
            int neighbor_i = i + 1;

            // first check if neighbors have opposite signed distance sign
            float signedDistance = signed_distance_field[i + j * grid_ptr_->width];
            float neighborSignedDistance = signed_distance_field[neighbor_i + j * grid_ptr_->width];

            if (signedDistance * neighborSignedDistance > epsilon) {
                continue;
            }

            int closer_i = abs(signedDistance) < abs(neighborSignedDistance) ? i : neighbor_i;
            int closer_index = closer_i + j * grid_ptr_->width;

            // if closer GridPoint is already highlighted, move on
            if (highlighted_pts.find(closer_index) != highlighted_pts.end()) {
                continue;
            }
            highlighted_pts.insert(closer_index);

            //for newly highlighted GridPoint, determine if innerRadius must be shrunk or
            //if outRadius must grow.

            float furthestDistance = getFurthestDistance(grid_ptr_->getPoint(closer_i, j));
            float closestDistance = getClosestDistance(grid_ptr_->getPoint(closer_i, j));

            if (furthestDistance > outerRadius) {
                //furthest corner is outside blue circle
                outerRadius = furthestDistance;
            } 
            if (closestDistance < innerRadius) {
                //closest corner is inside blue circle
                innerRadius = closestDistance;
            }
        }
    }

    // next loop across vertical pairs of GridPoints
    for (int j = 0; j < grid_ptr_->height - 1; j++) {
        for (int i = 0; i < grid_ptr_->width; i++) {
            int neighbor_j = j + 1;
            // first check if neighbors have opposite signed distance sign
            float signedDistance = signed_distance_field[i + j * grid_ptr_->width];
            float neighborSignedDistance = signed_distance_field[i + neighbor_j * grid_ptr_->width];

            if (signedDistance * neighborSignedDistance > epsilon) {
                continue;
            }

            int closer_j = abs(signedDistance) < abs(neighborSignedDistance) ? j : neighbor_j;
            int closer_index = i + closer_j * grid_ptr_->width;

            // if closer GridPoint is already highlighted, move on
            if (highlighted_pts.find(closer_index) != highlighted_pts.end()) {
                continue;
            }
            highlighted_pts.insert(closer_index);

            //for newly highlighted GridPoint, determine if innerRadius must be shrunk or
            //if outRadius must grow.

            float furthestDistance = getFurthestDistance(grid_ptr_->getPoint(i, closer_j));
            float closestDistance = getClosestDistance(grid_ptr_->getPoint(i, closer_j));

            if (furthestDistance > outerRadius) {
                //furthest corner is outside blue circle
                outerRadius = furthestDistance;
            } 
            if (closestDistance < innerRadius) {
                //closest corner is inside blue circle
                innerRadius = closestDistance;
            }
        }
    }
};

float DigitizedCircle::getFurthestDistance(GridPoint gp) {
    std::vector<Coordinate> corners;
    Coordinate x_diff = {gp.sideLength/2.f, 0.f};
    Coordinate y_diff = {0.f, gp.sideLength/2.f};

    corners.push_back(gp.center + x_diff + y_diff);
    corners.push_back(gp.center - x_diff + y_diff);
    corners.push_back(gp.center - x_diff - y_diff);
    corners.push_back(gp.center + x_diff - y_diff);

    float furthest_dist = 0.f;
    for (int i = 0; i < corners.size(); i++) {
        Coordinate corner = corners[i];
        float dist_to_center = Coordinate::computeDistance(center_, corner);

        if (dist_to_center > furthest_dist) {
            furthest_dist = dist_to_center;
        }
    }
    return furthest_dist;
};

float DigitizedCircle::getClosestDistance(GridPoint gp) {
    std::vector<Coordinate> corners;
    Coordinate x_diff = {gp.sideLength/2.f, 0.f};
    Coordinate y_diff = {0.f, gp.sideLength/2.f};

    corners.push_back(gp.center + x_diff + y_diff);
    corners.push_back(gp.center - x_diff + y_diff);
    corners.push_back(gp.center - x_diff - y_diff);
    corners.push_back(gp.center + x_diff - y_diff);

    float closest_dist = MAXFLOAT;
    for (int i = 0; i < corners.size(); i++) {
        Coordinate corner = corners[i];
        float dist_to_center = Coordinate::computeDistance(center_, corner);

        if (dist_to_center < closest_dist) {
            closest_dist = dist_to_center;
        }
    }
    return closest_dist;
};