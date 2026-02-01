#ifndef ACAV_MAP_H
#define ACAV_MAP_H
#include <vector>

#include "IntersectionCoordinator.h"
#include "Point.h"

using std::vector;

class Vehicle;

class Map {
private:
    vector<vector<std::shared_ptr<Point>>> grid;
    const int dim;
    vector<std::shared_ptr<Vehicle>> vehicles;
    IntersectionCoordinator coordinator;
public:
    explicit Map(int d);
    ~Map();
    void initialize();
    bool crash() const;
    const vector<vector<std::shared_ptr<Point>>>& getGrid() const;
    int getDim() const;
    void updatePositions();
    void generateVehicle(bool, int, double, int, int);
    void moveVehicles();
    vector<std::pair<int, int>> getCOGs() const;
    IntersectionCoordinator& getCoordinator();
    const vector<std::shared_ptr<Vehicle>>& getVehicles() const;

    void FakeUpdate();
};


#endif //ACAV_MAP_H