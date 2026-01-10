#ifndef ACAV_VEHICLE_H
#define ACAV_VEHICLE_H
#include <vector>
#include "Point.h"

using std::vector;
using std::string;

class Map;

class Vehicle {
private:
    string ID;
    const Map& map;
    vector<std::shared_ptr<Point>> surface;
    vector<std::shared_ptr<Point>> updatedPosition;
public:
    explicit Vehicle(Map& m, const vector<std::shared_ptr<Point>>& surf): map(m) {
        surface = surf;
    };
    virtual ~Vehicle() = default;
    string getID() const;
    const vector<std::shared_ptr<Point>>& getOldPosition() const;
    const vector<std::shared_ptr<Point>>& updateMap() const;
    virtual void move() = 0;
    void updateSurface();
};


#endif //ACAV_VEHICLE_H