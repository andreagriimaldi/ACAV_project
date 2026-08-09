#include "MonteCarloSimulator.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <memory>

#include "Vehicle.h"
#include "GlobalPlan.h"

int MonteCarloSimulator::run(int horizon) {
    for (int t = 0; t < horizon; t++) {
        step();
        if (map.crash()) {
            return time;
        }
    }
    return -1;
}

void MonteCarloSimulator::step() {
    time++;

    std::uniform_real_distribution<double> chance(0.0, 1.0);
    for (int i = 0; i < 4; i++) {
        if (isSpawnPointFree(i) && vehicleSpawnedFromHere(i) < 2) {
            if (chance(rng) < 0.05) {
                generateCPUVehicle(i, 0, GlobalPlan::spawnToRandomPlan(i));
            }
        }
    }

    if (egoPresence && egoActual == 0) {
        std::uniform_int_distribution<int> legPick(0, 3);
        int start = legPick(rng);
        for (int k = 0; k < 4; k++) {
            int i = (start + k) % 4;
            if (isSpawnPointFree(i) && vehicleSpawnedFromHere(i) < 2) {
                generateEgoVehicle(i, 0, GlobalPlan::spawnToRandomPlan(i));
                break;
            }
        }
    }

    map.moveVehicles();
    removeVehicles();
}

// 0 N, 1 E, 2 S, 3 W
bool MonteCarloSimulator::isSpawnPointFree(int direction) const {
    int x, y;
    if (direction == 0) {
        x = mapDim/3 + mapDim/10;
        y = mapDim/18;
    }
    else if (direction == 1) {
        x = mapDim - mapDim/18;
        y = mapDim/3 + mapDim/10;
    }
    else if (direction == 2) {
        x = (2*mapDim)/3 - mapDim/10;
        y = mapDim - mapDim/18;
    }
    else if (direction == 3) {
        x = mapDim/18;
        y = (2*mapDim)/3 - mapDim/10;
    }
    else {
        std::cerr << "Direction must be a number between 0 and 3" << std::endl;
        return false;
    }

    const std::vector<std::pair<int,int>> COGs = map.getCOGs();
    for (const std::pair<int,int>& cog : COGs) {
        if (std::sqrt((cog.first - x)*(cog.first - x)
                    + (cog.second - y)*(cog.second - y)) < mapDim/3) {
            return false;
        }
    }
    return true;
}

int MonteCarloSimulator::vehicleSpawnedFromHere(int direction) const {
    int count = 0;
    const std::vector<std::shared_ptr<Vehicle>>& vehicles = map.getVehicles();
    for (const std::shared_ptr<Vehicle>& v : vehicles) {
        if (GlobalPlan::planToSpawn(v->getGlobalPlan()) == direction) {
            count++;
        }
    }
    return count;
}

void MonteCarloSimulator::generateCPUVehicle(int spawn, double speed, int gplan) {
    std::uniform_int_distribution<int> dist(180, 240);
    map.generateVehicle(false, spawn, speed, gplan, map.getDim() / dist(rng));
    cpuActual++;
    cpuGenerated++;
}

void MonteCarloSimulator::generateEgoVehicle(int spawn, double speed, int gplan) {
    std::uniform_int_distribution<int> dist(180, 240);
    map.generateVehicle(true, spawn, speed, gplan, map.getDim() / dist(rng));
    egoActual++;
    egoGenerated++;
}

void MonteCarloSimulator::removeVehicles() {
    std::vector<std::shared_ptr<Vehicle>> toRemove;
    for (const std::shared_ptr<Vehicle>& v : map.getVehicles()) {
        if (isVehicleAtTheEnd({v->getCOGx(), v->getCOGy()}, v->getGlobalPlan())) {
            toRemove.push_back(v);
        }
    }
    for (std::shared_ptr<Vehicle>& v : toRemove) {
        if (v->getID() == "ego") {
            egoActual--;
            egoCompleted++;
        }
        else {
            cpuActual--;
            cpuCompleted++;
        }
        map.removeVehicle(v);
    }
}

bool MonteCarloSimulator::isVehicleAtTheEnd(std::pair<int,int> cog, int gplan) const {
    int end = GlobalPlan::planToEnd(gplan, mapDim);
    int c;
    if (gplan == 0 or gplan == 5 or gplan == 9 or gplan == 1 or gplan == 4 or gplan == 11) {
        c = cog.first;
    }
    else {
        c = cog.second;
    }
    return std::abs(end - c) < (mapDim/18 + mapDim/80);
}