#include "Map.h"

#include <iostream>
#include <random>

#include "CPUVehicle.h"
#include "EgoVehicle.h"
#include "Vehicle.h"

Map::Map(int d): dim(d) {
    grid.resize(dim + 1);
    for (int i = 0; i < dim + 1; i++) {
        grid.at(i).resize(dim + 1, std::make_shared<Point>(Point(Point_type::Empty)));
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

            if (west or center or east) {
                grid.at(i).at(j) = std::make_shared<Point>(Point(Point_type::Road));
            }

            if (bound) {
                grid.at(i).at(j) = std::make_shared<Point>(Point(Point_type::Boundary));
            }
        }
    }

    //TO DO LATER: MANAGE POINT_TYPE INTERSECTION
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
void Map::generateVehicle(bool ego, int spawn, double speed) {
    vector<std::shared_ptr<Point>> spawnPosition;
    switch (spawn) {
        case 0: {

            for (int i = dim/3 + dim/15; i < dim/3 + 2*(dim/15); i++) {
                spawnPosition.push_back(grid.at(i).at(dim/15));
            }
            for (int i = dim/3 + dim/15; i < dim/3 + 2*(dim/15); i++) {
                spawnPosition.push_back(grid.at(i).at(dim/15 + dim/9));
            }
            for (int j = dim/15; j < dim/15 + dim/9; j++) {
                spawnPosition.push_back(grid.at(dim/3 + dim/15).at(j));
            }
            for (int j = dim/15; j <= dim/15 + dim/9; j++) {
                spawnPosition.push_back(grid.at(dim/3 + 2*(dim/15)).at(j));
            }
            if (ego) {
                vehicles.push_back(std::make_unique<EgoVehicle>(*this, spawnPosition, 270, speed, "ego"));
            }
            else {
                auto s = [&]{ std::string r(8,'\0'); static std::mt19937 g{std::random_device{}()}; static std::uniform_int_distribution<int>d('a','z'); for(char& c:r) c=d(g); return r; }();
                vehicles.push_back(std::make_unique<CPUVehicle>(*this, spawnPosition, 270, speed, s));
            }
            break;

        }
        case 1: {

            for (int i = dim - dim/15 - dim/9; i < dim - dim/15; i++) {
                spawnPosition.push_back(grid.at(i).at(dim/3 + dim/15));
            }
            for (int i = dim - dim/15 - dim/9; i < dim - dim/15; i++) {
                spawnPosition.push_back(grid.at(i).at(dim/3 + 2*(dim/15)));
            }
            for (int j = dim/3 + dim/15; j < dim/3 + 2*(dim/15); j++) {
                spawnPosition.push_back(grid.at(dim - dim/15 - dim/9).at(j));
            }
            for (int j = dim/3 + dim/15; j <= dim/3 + 2*(dim/15); j++) {
                spawnPosition.push_back(grid.at(dim - dim/15).at(j));
            }
            if (ego) {
                vehicles.push_back(std::make_unique<EgoVehicle>(*this, spawnPosition, 0, speed, "ego"));
            }
            else {
                auto s = [&]{ std::string r(8,'\0'); static std::mt19937 g{std::random_device{}()}; static std::uniform_int_distribution<int>d('a','z'); for(char& c:r) c=d(g); return r; }();
                vehicles.push_back(std::make_unique<CPUVehicle>(*this, spawnPosition, 0, speed, s));
            }
            break;

        }
        case 2: {

            for (int i = 2*(dim/3 - dim/15); i < 2*(dim/3) - dim/15; i++) {
                spawnPosition.push_back(grid.at(i).at(dim - dim/15 - dim/9));
            }
            for (int i = 2*(dim/3 - dim/15); i < 2*(dim/3) - dim/15; i++) {
                spawnPosition.push_back(grid.at(i).at(dim - dim/15));
            }
            for (int j = dim - dim/15 - dim/9; j < dim - dim/15; j++) {
                spawnPosition.push_back(grid.at(2*(dim/3 - dim/15)).at(j));
            }
            for (int j = dim - dim/15 - dim/9; j <= dim - dim/15; j++) {
                spawnPosition.push_back(grid.at(2*(dim/3) - dim/15).at(j));
            }
            if (ego) {
                vehicles.push_back(std::make_unique<EgoVehicle>(*this, spawnPosition, 90, speed, "ego"));
            }
            else {
                auto s = [&]{ std::string r(8,'\0'); static std::mt19937 g{std::random_device{}()}; static std::uniform_int_distribution<int>d('a','z'); for(char& c:r) c=d(g); return r; }();
                vehicles.push_back(std::make_unique<CPUVehicle>(*this, spawnPosition, 90, speed, s));
            }
            break;

        }
        case 3: {

            for (int i = dim/15; i < dim/15 + dim/9; i++) {
                spawnPosition.push_back(grid.at(i).at(2*(dim/3 - dim/15)));
            }
            for (int i = dim/15; i < dim/15 + dim/9; i++) {
                spawnPosition.push_back(grid.at(i).at(2*(dim/3) - dim/15));
            }
            for (int j = 2*(dim/3 - dim/15); j < 2*(dim/3) - dim/15; j++) {
                spawnPosition.push_back(grid.at(dim/15).at(j));
            }
            for (int j = 2*(dim/3 - dim/15); j <= 2*(dim/3) - dim/15; j++) {
                spawnPosition.push_back(grid.at(dim/15 + dim/9).at(j));
            }
            if (ego) {
                vehicles.push_back(std::make_unique<EgoVehicle>(*this, spawnPosition, 0, speed, "ego"));
            }
            else {
                auto s = [&]{ std::string r(8,'\0'); static std::mt19937 g{std::random_device{}()}; static std::uniform_int_distribution<int>d('a','z'); for(char& c:r) c=d(g); return r; }();
                vehicles.push_back(std::make_unique<CPUVehicle>(*this, spawnPosition, 0, speed, s));
            }
            break;

        }
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


