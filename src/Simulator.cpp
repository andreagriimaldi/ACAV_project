#include "Simulator.h"

#include <iostream>

#include "Vehicle.h"

void Simulator::init() {

}

void Simulator::terminate() {
}

void Simulator::crash() {
}

// 0 N, 1 E, 2 S, 3 W
bool Simulator::isSpawnPointFree(int direction) const{
    int x, y;
    if (direction == 0) {
        x = mapDim/3 + mapDim/10;
        y = mapDim/3 - mapDim/30;
    }
    else if (direction == 1) {
        x = (2*mapDim)/3 + mapDim/30;
        y = mapDim/3 + mapDim/10;
    }
    else if (direction == 2) {
        x = (2*mapDim)/3 - mapDim/10;
        y = (2*mapDim)/3 + mapDim/30;
    }
    else if (direction == 3) {
        x = mapDim/3 - mapDim/30;
        y = (2*mapDim)/3 - mapDim/10;
    }
    else std::cerr << "Direction must be a number between 0 and 3" << std::endl;

    vector<std::pair<int, int>> COGs = map.getCOGs();

    for (const std::pair<int, int>& cog: COGs) {
        if (std::sqrt((cog.first - x)*(cog.first - x) + (cog.second - y)*(cog.second - y) ) < mapDim/3) { //TUNING PARAMETER
            return false;
        }
    }

    return true;
}

int Simulator::vehicleSpawnedFromHere(int direction) const{
    int count = 0;
    const vector<std::shared_ptr<Vehicle>>& vehicles = map.getVehicles();

    for (const std::shared_ptr<Vehicle>& v: vehicles) {
        if (GlobalPlan::planToSpawn(v->getGlobalPlan()) == direction) {
            count++;
        }
    }

    return count;
}


void Simulator::generateCPUVehicle(int spawn, double speed, int gplan) {

}

void Simulator::generateEgoVehicle(int spawn, double speed, int gplan) {
}

void Simulator::removeVehicles() {
}

bool Simulator::isVehicleAtTheEnd(std::pair<int, int> cog, int gplan) const{
    int x, y;
    x = GlobalPlan::planToEnd(gplan).first;
    y = GlobalPlan::planToEnd(gplan).second;

    if (std::sqrt((x - cog.first)*(x - cog.first) + (y - cog.second)*(y - cog.second) ) < ) {

    }
}


