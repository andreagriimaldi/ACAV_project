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

    if (time % 500 == 0) {
        std::cout << "Time instant " << time << ", CPU Vehicles generated: " << cpuGenerated << ", Ego Vehicles generated: " << egoGenerated << std::endl;
    }

    static std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<double> chance(0.0, 1.0);

    for (int i = 0; i < 4; i++) {
        if (isSpawnPointFree(i) && vehicleSpawnedFromHere(i) < 2) {
            if (chance(gen) < 0.05) {
                generateCPUVehicle(i, 0, GlobalPlan::spawnToRandomPlan(i));
            }
        }
    }

    if (egoPresence && egoActual == 0) {
        static std::mt19937 genr{std::random_device{}()};
        std::uniform_int_distribution<int> legPick(0, 3);
        int start = legPick(genr);
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

void Simulator::terminate() const {
    if (map.crash()) {
        std::cerr << "A crash has happened" << std::endl;
    }
    std::cout << "The simulation lasted " << time << " time instants" << std::endl;
    std::cout << "CPU Vehicle generated: " << cpuGenerated << std::endl;
    std::cout << "Ego Vehicle generated: " << egoGenerated << std::endl;
}

bool Simulator::crash() {
    if (map.crash()) {
        std::string path = "crash_t" + std::to_string(time) + ".png";
        renderer.draw(map, &path);
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


