#ifndef ACAV_POINT_H
#define ACAV_POINT_H
#include <string>
#include "Point_type.h"

using std::string;

class Point {
private:
    string vehicleID;
    bool isOccupied;
    Point_type type;
    int count;
public:
    explicit Point(Point_type t) : vehicleID(""), isOccupied(false), type(t), count(0){};
    string getVehicle() const;
    void setVehicle(string);
    bool occupied() const;
    void setOccupied();
    void setNotOccupied();
    Point_type getType() const;
    bool incident() const;
};


#endif //ACAV_POINT_H