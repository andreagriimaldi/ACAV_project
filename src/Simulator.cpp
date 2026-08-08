#include "Simulator.h"
#include <iostream>
#include <random>

#include "Vehicle.h"

void Simulator::init() {
    bool running = true;
    bool paused = false;

    while (running) {
        int cmd = renderer.pollEvents();
        if (cmd == 1) {
            running = false;
        }
        else if (cmd == 2) {
            paused = !paused;
        }

        if (!paused) {
            loop();
        }

        renderer.draw(map);
        SDL_Delay(16);

        if (crash()) {
            running = false;
        }
    }
    terminate();
}

void Simulator::loop() {
    time++;
    for (int i = 0; i < 4; i++) {
        //BETA VERSION
        if (isSpawnPointFree(i) && vehicleSpawnedFromHere(i) < 2) {
            generateCPUVehicle(i, 0, GlobalPlan::spawnToRandomPlan(i));
        }
    }
    map.moveVehicles();
    removeVehicles();
}

void Simulator::terminate() const {
    std::cout << "CPU Vehicle generated: " << cpuGenerated << std::endl;
    std::cout << "Ego Vehicle generated: " << egoGenerated << std::endl;
}

bool Simulator::crash() {
    if (map.crash()) {
        //save screenshot
        return true;
    }
    return false;
}

// 0 N, 1 E, 2 S, 3 W
bool Simulator::isSpawnPointFree(int direction) const{
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

    vector<std::pair<int, int>> COGs = map.getCOGs();

    for (const std::pair<int, int>& cog: COGs) {
        if (std::sqrt((cog.first - x)*(cog.first - x) + (cog.second - y)*(cog.second - y) ) < mapDim/3) { //TUNING PARAMETER
            return false;
        }
    }

    return true;
}

//returns the actual number
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(180, 240);
    map.generateVehicle(false, spawn, speed, gplan,  map.getDim()/(dist(gen)));
    cpuActual++;
    cpuGenerated++;
}

void Simulator::generateEgoVehicle(int spawn, double speed, int gplan) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(180, 240);
    map.generateVehicle(true, spawn, speed, gplan,  map.getDim()/(dist(gen)));
    egoActual++;
    egoGenerated++;
}

void Simulator::removeVehicles() {
    std::vector<std::shared_ptr<Vehicle>> toRemove;
    for (const std::shared_ptr<Vehicle>& v : map.getVehicles()) {
        if (isVehicleAtTheEnd({v->getCOGx(), v->getCOGy()}, v->getGlobalPlan())) {
            toRemove.push_back(v);
        }
    }
    for (std::shared_ptr<Vehicle>& v : toRemove) {
        if (v->getID() == "ego") {
            egoActual--;
        }
        else {
            cpuActual--;
        }
        map.removeVehicle(v);
    }
}

bool Simulator::isVehicleAtTheEnd(std::pair<int, int> cog, int gplan) const{
    int end = GlobalPlan::planToEnd(gplan, mapDim);
    int c;
    if (gplan == 0 or gplan == 5 or gplan == 9 or gplan == 1 or gplan == 4 or gplan == 11) {
        c = cog.first;
    }
    else {
        c = cog.second;
    }

    if (std::abs(end - c) < (mapDim/18 + mapDim/80)) {
        return true;
    }

    return false;
}


