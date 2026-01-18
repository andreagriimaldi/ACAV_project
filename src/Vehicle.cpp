#include "Vehicle.h"
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
    changeHeading(newh);

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
    if (p.countToVisit() >= 2) {
        double d_next_x = p.nextPoint()->getX() - getCOGx();
        double d_next_y = p.nextPoint()->getY() - getCOGy();

        double d_nextnext_x = p.nextNextPoint()->getX() - getCOGx();
        double d_nextnext_y = p.nextNextPoint()->getY() - getCOGy();

        double dist1 = std::sqrt(d_next_x*d_next_x + d_next_y*d_next_y);
        double dist2 = std::sqrt(d_nextnext_x*d_nextnext_x + d_nextnext_y*d_nextnext_y);

        double alpha1 = std::atan2(d_next_y, d_next_x) - (heading * M_PI)/180;
        double alpha2 = std::atan2(d_nextnext_y, d_nextnext_x) - (heading * M_PI)/180;

        double delta1 = std::atan2(2 * (map.getDim()/9)*0.65 * std::sin(alpha1), dist1);
        double delta2 = std::atan2(2 * (map.getDim()/9)*0.65 * std::sin(alpha2), dist2);

        return delta1 * (dist1/(dist1 + dist2)) + delta2* (dist2/(dist1 + dist2));
    }
    else if (p.countToVisit() == 1){
        double d_next_x = p.nextPoint()->getX();
        double d_next_y = p.nextPoint()->getY();

        double dist1 = std::sqrt(d_next_x*d_next_x + d_next_y*d_next_y);

        double alpha1 = std::atan2(d_next_y, d_next_x) - (heading * M_PI)/180;

        double delta1 = std::atan2(2 * (map.getDim()/9)*0.65 * std::sin(alpha1), dist1);

        return delta1;
    }
    else return 0; //LAST WAYPOINT REACHED, SIMULATION MUST END
}
