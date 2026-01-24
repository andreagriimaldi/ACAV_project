#ifndef ACAV_VEHICLE_H
#define ACAV_VEHICLE_H
#include <vector>

#include "GlobalPlan.h"
#include "Perception.h"
#include "Point.h"

using std::vector;
using std::string;

class Map;

class Vehicle {
protected:
    const string ID;
    const Map& map;
    int heading;
    double speed;
    vector<std::shared_ptr<Point>> surface;
    vector<std::shared_ptr<Point>> updatedPosition;
    GlobalPlan p;
    const double maxspeed; //TUNING PARAMETER (IT WILL BE PASSED AS A PARAMETER)
    Perception per;
public:
    explicit Vehicle(Map& m, const vector<std::shared_ptr<Point>>& surf, int h, double s, string id, int gplan): ID(id), map(m), heading(h), speed(s), p(m, gplan), maxspeed(map.getDim()/300), per(m) {
        surface = surf;
    };
    virtual ~Vehicle() = default;
    string getID() const;
    int getHeading() const;
    double getSpeed() const;
    int getCOGx() const;
    int getCOGy() const;
    const vector<std::shared_ptr<Point>>& getOldPosition() const;
    const vector<std::shared_ptr<Point>>& updateMap() const;
    virtual void move() = 0;
    void updateSurface();
    void changeHeading(int);
    void computeNewPosition(int, int, int);
    double computeSteering() const;
    void updateBicycle(double, double);
};


#endif //ACAV_VEHICLE_H