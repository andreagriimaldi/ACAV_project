#ifndef ACAV_POINT_H
#define ACAV_POINT_H
#include <string>
#include "Point_type.h"

using std::string;

class Point {
private:
    string vehicleID;
    Point_type type;
    int count;
    int x,y;
public:
    explicit Point(Point_type t, int x, int y) : vehicleID(""), type(t), count(0), x(x), y(y) {};
    int getX() const;
    int getY() const;
    string getVehicle() const;
    void setVehicle(string);
    bool occupied() const;
    void setNotOccupied();
    Point_type getType() const;
    bool incident() const;
};


#endif //ACAV_POINT_H