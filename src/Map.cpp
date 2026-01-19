#include "Map.h"

#include <iostream>
#include <random>

#include "CPUVehicle.h"
#include "EgoVehicle.h"
#include "Vehicle.h"

Map::Map(int d): dim(d) {
    grid.resize(dim + 1);
    for (int i = 0; i < dim + 1; i++) {
        grid.at(i).resize(dim + 1, std::make_shared<Point>(Point(Point_type::Empty, 0, 0)));
    }
    vehicles.reserve(10);
}

Map::~Map() = default;

void Map::initialize() {

    for (int i = 0; i < dim + 1; i++) {
        for (int j = 0; j < dim + 1; j++) {
            bool west = (i > 0 and i <= dim/3) and (j > dim/3 and j < (2*dim)/3);
            bool center = (i > dim/3 and i < (2*dim)/3 and j > 0 and j < dim);
            bool east = (i >= (2*dim)/3 and i < dim) and (j > dim/3 and j < (2*dim)/3);
            bool bound = i == 0 or i == dim or j == 0 or j == dim;
            bool rot1 = (i == dim/3 + dim/10) and (j == dim/3 + dim/30);
            bool rot2 = (i == dim/3 + dim/10) and (j == dim/3 + dim/9);
            bool rot3 = (i == (2*dim)/3 - dim/30) and (j == dim/3 + dim/10);
            bool rot4 = (i == (2*dim)/3 - dim/9) and (j == dim/3 + dim/10);
            bool rot5 = (i == 2*dim/3 - dim/10) and (j == (2*dim)/3 - dim/30);
            bool rot6 = (i == 2*dim/3 - dim/10) and (j == (2*dim)/3 - dim/9);
            bool rot7 = (i == dim/3 + dim/30) and (j == 2*dim/3 - dim/10);
            bool rot8 = (i == dim/3 + dim/9) and (j == 2*dim/3 - dim/10);

            if (west or center or east) {
                grid.at(i).at(j) = std::make_shared<Point>(Point(Point_type::Road, i, j));
            }

            if (bound) {
                grid.at(i).at(j) = std::make_shared<Point>(Point(Point_type::Boundary, i, j));
            }

            if (rot1 or rot2 or rot3 or rot4 or rot5 or rot6 or rot7 or rot8) {
                grid.at(i).at(j) = std::make_shared<Point>(Point(Point_type::Rotation, i, j));
            }
        }
    }
}

bool Map::crash() const {
    //The check is performed with the updated positions
    for (const std::unique_ptr<Vehicle>& v: vehicles) {
        for (const std::shared_ptr<Point>& p: v->updateMap()) {
            if (p->incident())
                return true;
        }
    }

    return false;
}

const vector<vector<std::shared_ptr<Point>>>& Map::getGrid() const {
    return grid;
}

int Map::getDim() const {
    return dim;
}

void Map::updatePositions() {
    for (const std::unique_ptr<Vehicle>& v: vehicles) {
        for (const std::shared_ptr<Point>& p: v->getOldPosition()) {
            p->setNotOccupied();
        }
        for (const std::shared_ptr<Point>& p: v->updateMap()) {
            p->setVehicle(v->getID());
        }
        v->updateSurface();
    }
}

//ego is true when the ego vehicle is being generated
//int represents the spawn point (0 N, 1 E, 2 S, 3 W)
void Map::generateVehicle(bool ego, int spawn, double speed, int gplan) {
    vector<std::shared_ptr<Point>> spawnPosition;
    int x_min, x_max, y_min, y_max, heading;

    switch (spawn) {
        case 0: {
            x_min = dim/3 + dim/15;
            x_max = dim/3 + 2*(dim/15);
            y_min = dim/15;
            y_max = dim/15 + dim/9;
            heading = 270;
            break;
        }
        case 1: {
            x_min = dim - dim/15 - dim/9;
            x_max = dim - dim/15;
            y_min = dim/3 + dim/15;
            y_max = dim/3 + 2*(dim/15);
            heading = 180;
            break;
        }
        case 2: {
            x_min = 2*(dim/3 - dim/15);
            x_max = 2*(dim/3) - dim/15;
            y_min = dim - dim/15 - dim/9;
            y_max = dim - dim/15;
            heading = 90;
            break;
        }
        case 3: {
            x_min = dim/15;
            x_max = dim/15 + dim/9;
            y_min = 2*(dim/3 - dim/15);
            y_max = 2*(dim/3) - dim/15;
            heading = 0;
            break;
        }
        default: std::cerr << "Vehicle not initialized correctly" << std::endl;
            return;
    }

    for (int i = x_min; i <= x_max; i++) {
        for (int j = y_min; j <= y_max; j++) {
            spawnPosition.push_back(grid.at(i).at(j));
        }
    }

    if (ego) {
        vehicles.push_back(std::make_unique<EgoVehicle>(*this, spawnPosition, heading, speed, "ego", gplan));
    }
    else {
        auto s = [&]{ std::string r(8,'\0'); static std::mt19937 g{std::random_device{}()}; static std::uniform_int_distribution<int>d('a','z'); for(char& c:r) c=d(g); return r; }();
        vehicles.push_back(std::make_unique<CPUVehicle>(*this, spawnPosition, heading, speed, s, gplan));
    }
}

void Map::moveVehicles() {
    //TO BE REFINED
    for (const std::unique_ptr<Vehicle> &v: vehicles) {
        v->move();
    }
    updatePositions();
}


void Map::FakeUpdate() {
    for (const std::unique_ptr<Vehicle>& v: vehicles) {
        for (const std::shared_ptr<Point>& p: v->getOldPosition()) {
            p->setVehicle(v->getID());
        }
    }
}


