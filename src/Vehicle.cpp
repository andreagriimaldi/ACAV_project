#include "Vehicle.h"

#include <iostream>

#include "Map.h"

string Vehicle::getID() const {
    return ID;
}

int Vehicle::getHeading() const {
    return heading;
}

double Vehicle::getSpeed() const {
    return speed;
}

int Vehicle::getCOGx() const {
    int x_max = -1;
    int x_min = map.getDim() + 2;
    for (size_t i = 0; i < surface.size(); i++) {
        if (surface.at(i)->getX() > x_max) {
            x_max = surface.at(i)->getX();
        }
        if (surface.at(i)->getX() < x_min) {
            x_min = surface.at(i)->getX();
        }
    }
    return (x_max + x_min)/2;
}

int Vehicle::getCOGy() const {
    int y_max = -1;
    int y_min = map.getDim() + 2;
    for (size_t i = 0; i < surface.size(); i++) {
        if (surface.at(i)->getY() > y_max) {
            y_max = surface.at(i)->getY();
        }
        if (surface.at(i)->getY() < y_min) {
            y_min = surface.at(i)->getY();
        }
    }
    return (y_max + y_min)/2;
}

const vector<std::shared_ptr<Point>>& Vehicle::getOldPosition() const {
    return surface;
}

const vector<std::shared_ptr<Point>>& Vehicle::updateMap() const {
    return updatedPosition;
}

void Vehicle::updateSurface() {
    surface = updatedPosition;
    updatedPosition.clear();
}

void Vehicle::changeHeading(int newh) {
    heading = newh;
}

void Vehicle::computeNewPosition(int newh, int new_x, int new_y) {
    int dim = map.getDim();
    const vector<vector<std::shared_ptr<Point>>>& grid = map.getGrid();
    updatedPosition.clear();

    int half_w = dim / 18;
    int half_h = dim / 30;

    double angle_rad = -newh * M_PI / 180.0;
    double cos_a = std::cos(angle_rad);
    double sin_a = std::sin(angle_rad);

    for (int di = -half_w; di <= half_w; di++) {
        for (int dj = -half_h; dj <= half_h; dj++) {
            int new_i_l = static_cast<int>(std::floor(new_x + di * cos_a - dj * sin_a));
            int new_j_l = static_cast<int>(std::floor(new_y + di * sin_a + dj * cos_a));
            int new_i_h = static_cast<int>(std::ceil(new_x + di * cos_a - dj * sin_a));
            int new_j_h = static_cast<int>(std::ceil(new_y + di * sin_a + dj * cos_a));

            if (new_i_l >= 0 && new_i_l <= dim && new_j_l >= 0 && new_j_l <= dim && new_i_h >= 0 && new_i_h <= dim && new_j_h >= 0 && new_j_h <= dim ) {
                updatedPosition.push_back(grid.at(new_i_l).at(new_j_l));
                updatedPosition.push_back(grid.at(new_i_h).at(new_j_h));
                updatedPosition.push_back(grid.at(new_i_l).at(new_j_h));
                updatedPosition.push_back(grid.at(new_i_h).at(new_j_l));
            }
        }
    }
}

double Vehicle::computeSteering() const {
    return computeSteeringFrom(getCOGx(), getCOGy(), heading);
}

void Vehicle::updateBicycle(double v) {

    std::cout << "=== UPDATE ===" << std::endl;
    std::cout << "COG: (" << getCOGx() << ", " << getCOGy() << ")" << std::endl;
    std::cout << "Heading: " << heading << std::endl;
    std::cout << "Velocity: " << v << std::endl;

    double L = (map.getDim() / 9) * 0.65;

    const double max_disp_per_substep = 0.05; //Integration accuracy (tuning parameter)
    int substeps = std::max(1, static_cast<int>(std::ceil(v / max_disp_per_substep)));
    double dt = 1.0 / substeps;

    double current_x = getCOGx();
    double current_y = getCOGy();
    double current_heading = heading;

    for (int i = 0; i < substeps; ++i) {
        if (p.countToVisit() > 0) {
            double dx = p.nextPoint()->getX() - current_x;
            double dy = p.nextPoint()->getY() - current_y;
            double dist = std::sqrt(dx * dx + dy * dy);

            if (dist < map.getDim() / 30) {
                p.popCurrent();
            }
        }

        double current_steering = computeSteeringFrom(current_x, current_y, current_heading);

        double theta_rad = (current_heading * M_PI) / 180.0;

        double displacement = v * dt;
        current_x += displacement * std::cos(theta_rad);
        current_y -= displacement * std::sin(theta_rad);

        double d_heading = (v / L) * std::tan(current_steering) * dt * (180.0 / M_PI);
        current_heading += d_heading;

        while (current_heading >= 360.0) current_heading -= 360.0;
        while (current_heading < 0.0) current_heading += 360.0;
    }

    int new_head = static_cast<int>(std::round(current_heading));

    speed = v;
    changeHeading(new_head);

    computeNewPosition(new_head, static_cast<int>(std::round(current_x)), static_cast<int>(std::round(current_y)));
}

double Vehicle::computeSteeringFrom(double x, double y, double hdg) const {
    if (p.countToVisit() > 0) {
        double d_next_x = p.nextPoint()->getX() - x;
        double d_next_y = p.nextPoint()->getY() - y;

        double dist = std::sqrt(d_next_x * d_next_x + d_next_y * d_next_y);

        double alpha = std::atan2(-d_next_y, d_next_x) - (hdg * M_PI) / 180.0;

        double L = (map.getDim() / 9) * 0.65;
        double delta = std::atan2(2 * L * std::sin(alpha), dist);

        return delta;
    }
    return 0;
}
