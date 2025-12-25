#include "Map.h"
#include "Vehicle.h"

Map::Map(int d): dim(d) {
    grid.resize(dim + 1);
    for (int i = 0; i < dim + 1; i++) {
        grid.at(i).resize(dim + 1, Point(Point_type::Empty));
    }
    vehicles.reserve(10);
};

void Map::initialize() {

    for (int i = 0; i < dim + 1; i++) {
        for (int j = 0; j < dim + 1; j++) {
            bool west = (i > 0 and i < dim/3) and (j > dim/3 and j < (2/3)*dim);
            bool center = (i > dim/3 and i < (2/3)*dim and j > 0 and j < dim);
            bool east = (i > (2/3)*dim and i < dim) and (j > dim/3 and j < (2/3)*dim);
            bool bound = i == 0 or i == dim or j == 0 or j == dim;

            if (west or center or east) {
                grid.at(i).at(j) = Point(Point_type::Road);
            }

            if (bound) {
                grid.at(i).at(j) = Point(Point_type::Boundary);
            }
        }
    }

    //TO DO LATER: MANAGE POINT_TYPE INTERSECTION
}

bool Map::crash() const {
    //For each vehicle call incident() on all its points
}

const vector<vector<Point>>& Map::getGrid() const {
    return grid;
}

int Map::getDim() const {
    return dim;
}

void Map::updatePositions() {
    for (const std::unique_ptr<Vehicle>& v: vehicles) {
        for (Point p: v->getOldPosition()) {
            p.setNotOccupied();
        }
        for (Point p: v->updateMap()) {
            p.setVehicle(v->getID());
        }
        v->updateSurface();
    }
}

void Map::generateVehicle(bool, int) {
}


