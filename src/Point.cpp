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
    if (count == 0) {
        vehicleID = id;
        count++;
    }
    else {
        if (vehicleID != id) {
            count++;
        }
    }
}

bool Point::occupied() const {
    return count;
}

void Point::setNotOccupied() {
    if (count <= 1) {
        count = 0;
        vehicleID = "";
    }
    else {
        count--;
    }
}

Point_type Point::getType() const {
    return type;
}

bool Point::incident() const {
    return count > 1;
}
