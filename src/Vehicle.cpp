#include "Vehicle.h"
#include "Map.h"

string Vehicle::getID() const {
    return ID;
}

const vector<Point> & Vehicle::getOldPosition() const {
    return surface;
}

const vector<Point>& Vehicle::updateMap() const {
    return updatedPosition;
}

void Vehicle::updateSurface() {
    surface = updatedPosition;
}
