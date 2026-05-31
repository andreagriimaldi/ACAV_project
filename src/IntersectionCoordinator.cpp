#include "IntersectionCoordinator.h"

#include <iostream>

#include "Vehicle.h"

bool IntersectionCoordinator::askPermission(const std::string& id) {

    for (string& s: currentlyGranted) {
        if (s == id) {
            return true;
        }
    }

    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    for (const auto& v: vehicles) {
        if (v->getID() == "ego") {
            if (v->getPercState() == 1 && currents.size() > 0) {
                return false;
            }
        }
    }

    if (currents.size() + currentlyGranted.size() > 1) {
        return false;
    }

    currentlyGranted.push_back(id);

    return true;
}

//This directly returns the speed for CPUVehicle::avoidObstacles()
int IntersectionCoordinator::suggestedSpeed(const std::string& id, double speed) const {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();

    if (!inTheMiddle(id)) {
        std::cerr << "suggestedSpeed() should be called by vehicles in the middle" << std::endl;
        return 2;
    }

    if (currents.size() < 2) {
        return 2;
    }

    //Let's find which vehicles are in the middle and save them in a shared pointer
    std::shared_ptr<Vehicle> vehicle1 = idVehicleMiddle(id); //CURRENT
    std::shared_ptr<Vehicle> vehicle2 = otherVehicleMiddle(id); //OTHER

    int glob1 = vehicle1->getGlobalPlan();
    int glob2 = vehicle2->getGlobalPlan();

    std::vector<int> collision = pathCollisionFinder(glob1, glob2);
    //Here it will be decided how to act on the speed of vehicle1
}

// res[0] = 0 if NO collision, 1 if collision, res[1] = x of collision 1, res[2] = y of collision 1, res[3] = x of collision 2, res[4] = y of collision 2
const std::vector<int> IntersectionCoordinator::pathCollisionFinder(int glob1, int glob2) const {
    std::vector<int> res(5);

    int DIM = m.getDim();

    const std::vector<std::vector<std::array<double, 5>>> collisionMatrix = {
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.483524*DIM, 0.503001*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.450617*DIM, 0.464609*DIM, 0.549383*DIM, 0.535391*DIM}, {0, 0, 0, 0, 0}, {1, 0.498431*DIM, 0.520392*DIM, 0, 0}, {1, 0.433333*DIM, 0.444444*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.540000*DIM, 0, 0}, {1, 0.666667*DIM, 0.566667*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.483524*DIM, 0.503001*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.503630*DIM, 0.482013*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.534815*DIM, 0.452778*DIM, 0.470973*DIM, 0.543928*DIM}, {1, 0.433333*DIM, 0.666667*DIM, 0, 0}, {1, 0.555556*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.464000*DIM, 0.566667*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.549383*DIM, 0.535391*DIM, 0.450617*DIM, 0.464609*DIM}, {0, 0, 0, 0, 0}, {1, 0.503630*DIM, 0.482013*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.520392*DIM, 0.501569*DIM, 0, 0}, {1, 0.433333*DIM, 0.460000*DIM, 0, 0}, {1, 0.333333*DIM, 0.433333*DIM, 0, 0}, {1, 0.566667*DIM, 0.555556*DIM, 0, 0}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.498431*DIM, 0.520392*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.470973*DIM, 0.543928*DIM, 0.534815*DIM, 0.452778*DIM}, {0, 0, 0, 0, 0}, {1, 0.520392*DIM, 0.501569*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.540000*DIM, 0.433333*DIM, 0, 0}, {1, 0.566667*DIM, 0.333333*DIM, 0, 0}, {1, 0.444444*DIM, 0.566667*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.433333*DIM, 0.444444*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.433333*DIM, 0.666667*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.433333*DIM, 0.460000*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.433333*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.433333*DIM, 0.566667*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.555556*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.333333*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.540000*DIM, 0.433333*DIM, 0, 0}, {1, 0.433333*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.540000*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.555556*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.333333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.566667*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.666667*DIM, 0.566667*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.464000*DIM, 0.566667*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.444444*DIM, 0.566667*DIM, 0, 0}, {1, 0.433333*DIM, 0.566667*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.566667*DIM, 0, 0}, {0, 0, 0, 0, 0}},
    };

    //TEST

    const auto& c1 = collisionMatrix[1][5];

    std::cerr << "COLLISION: " << c1.at(0) << "    (x,y) of first: (" << c1.at(1) << "," << c1.at(2) << ")    (x,y) of second: (" << c1.at(3) << "," << c1.at(4) << ")" << std::endl;

    //TEST

    const auto& c = collisionMatrix[glob1][glob2];

    for (int i = 1; i < 5; ++i) {
        res[i] = static_cast<int>(std::lround(c[i]));
    }

    return res;
}

void IntersectionCoordinator::updateStatus() {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    currents.clear();
    for (const auto& v: vehicles) {
        if (v->getPercState() == 2) {
            currents.push_back(v->getID());
            auto it = std::find(currentlyGranted.begin(), currentlyGranted.end(), v->getID());
            if (it != currentlyGranted.end()) {
                currentlyGranted.erase(it);
            }
        }
    }
}


//MAYBE USELESS
bool IntersectionCoordinator::isEgoInTheMiddle() const {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    for (const auto& v: vehicles) {
        if (v->getID() == "ego") {
            if (v->getPercState() == 2) {
                return true;
            }
        }
    }
    return false;
}

bool IntersectionCoordinator::inTheMiddle(const std::string &id) const {
    for (const auto&s: currents) {
        if (s == id) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<Vehicle> IntersectionCoordinator::idVehicleMiddle(const std::string &id) const {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    for (auto& v: vehicles) {
        if (v->getID() == id && v->getPercState() == 2) {
            return v;
        }
    }
    std::cerr << "A vehicle with this id is not in the middle" << std::endl;
    return {nullptr};
}

std::shared_ptr<Vehicle> IntersectionCoordinator::otherVehicleMiddle(const std::string &id) const {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    for (auto& v: vehicles) {
        if (v->getID() != id && v->getPercState() == 2) {
            return v;
        }
    }
    std::cerr << "There is not another vehicle in the middle" << std::endl;
    return {nullptr};
}
