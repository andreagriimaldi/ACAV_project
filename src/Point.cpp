#include "Point.h"

string Point::getVehicle() const {
    return vehicleID;
}

string Point::setVehicle(string id) {
    if (!occupied()) {
        vehicleID = id;
        count++;
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
    if (count == 1) {
        isOccupied = false;
    }
}

Point_type Point::getType() const {
    return type;
}

bool Point::incident() const {
    return count > 1;
}
