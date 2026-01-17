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

    int half_w = dim / 18;  // half of dim/9
    int half_h = dim / 30;  // half of dim/15

    double angle_rad = -newh * M_PI / 180.0;
    double cos_a = std::cos(angle_rad);
    double sin_a = std::sin(angle_rad);

    for (int di = -half_w; di <= half_w; di++) {
        for (int dj = -half_h; dj <= half_h; dj++) {
            int new_i = static_cast<int>(std::round(new_x + di * cos_a - dj * sin_a));
            int new_j = static_cast<int>(std::round(new_y + di * sin_a + dj * cos_a));

            if (new_i >= 0 && new_i <= dim && new_j >= 0 && new_j <= dim) {
                updatedPosition.push_back(grid.at(new_i).at(new_j));
            }
        }
    }
}
