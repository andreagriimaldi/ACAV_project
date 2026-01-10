#ifndef ACAV_MAP_H
#define ACAV_MAP_H
#include <vector>
#include "Point.h"

using std::vector;

class Vehicle;

class Map {
private:
    vector<vector<std::shared_ptr<Point>>> grid;
    const int dim;
    vector<std::unique_ptr<Vehicle>> vehicles;
public:
    explicit Map(int d);
    ~Map();
    void initialize();
    bool crash() const;
    const vector<vector<std::shared_ptr<Point>>>& getGrid() const;
    int getDim() const;
    void updatePositions();
    void generateVehicle(bool, int);
};


#endif //ACAV_MAP_H