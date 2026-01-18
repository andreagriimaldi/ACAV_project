#include "Point.h"

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

string Point::getVehicle() const {
    return vehicleID;
}

void Point::setVehicle(string id) {
    if (!occupied()) {
        vehicleID = id;
        count++;
        setOccupied();
    }
    else {
        count++;
    }
}

bool Point::occupied() const {
    return isOccupied;
}

void Point::setOccupied() {
    isOccupied = true;
}

void Point::setNotOccupied() {
    isOccupied = false;
    count = 0;
    vehicleID = "";
    //MAYBE TO FIX
}

Point_type Point::getType() const {
    return type;
}

bool Point::incident() const {
    return count > 1;
}
