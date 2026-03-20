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

// 0: stop immediately, 1: slow down, 2: maintain speed
int IntersectionCoordinator::suggestedSpeed(const std::string& id) const {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();

    if (!inTheMiddle(id)) {
        std::cerr << "suggestedSpeed() should be called by vehicles in the middle" << std::endl;
        return 2;
    }

    if (currents.size() < 2) {
        return 2;
    }

    //Let's find which vehicles are in the middle and save them in a shared pointer
    std::shared_ptr<Vehicle> vehicle1 = idVehicleMiddle(currents.at(0));
    std::shared_ptr<Vehicle> vehicle2 = otherVehicleMiddle(currents.at(1));

    //For both cars we calculate a score between 0 and 1, weighted by the three criterias
    double w_right = 0.5;
    double w_progress = 0.2;
    double w_velocity = 0.3;

    double weight1 = 0.0, weight2 = 0.0;

    //FIRST CRITERIUM: Which vehicle has the right side free
    double dx = vehicle2->getCOGx() - vehicle1->getCOGx();
    double dy = vehicle1->getCOGy() - vehicle2 ->getCOGy();
    double theta = std::atan2(dy, dx) * 180.0 / M_PI;
    double difference = vehicle1->getHeading() - theta; //If the other vehicle is on the right, diff is positive
    while (difference > 180.0)  difference -= 360.0;
    while (difference < -180.0) difference += 360.0;

    if (difference < 0) {
        //The other vehicle is on the left
        weight1 += w_right*1;
    }
    else if (difference > 0) {
        //The other vehicle is on the right
        weight2 += w_right*1;
    }
    else {
        //The two vehicles face each other
        weight1 += w_right*0.5;
        weight2 += w_right*0.5;
    }

    //SECOND CRITERIUM: Which vehicle is closer to the final waypoint
    double dist1 = vehicle1->getDistanceFromEnd();
    double dist2 = vehicle2->getDistanceFromEnd();

    if (dist1 < dist2) {
        //Vehicle 1 is closer to the end
        weight1 += w_progress*1;
    }
    else if (dist1 > dist2) {
        //Vehicle 2 is closer to the end
        weight2 += w_progress*1;
    }
    else {
        //Same distance
        weight1 += w_progress*0.5;
        weight2 += w_progress*0.5;
    }

    //THIRD CRITERIUM: If vehicle 1 is going straight
    if (vehicle1->getSpeed() > vehicle2->getSpeed()) {
        weight1 += w_velocity*1;
    }
    else if (vehicle1->getSpeed() < vehicle2->getSpeed()) {
        weight2 += w_velocity*1;
    }
    else {
        weight1 += w_velocity*0.5;
        weight2 += w_velocity*0.5;
    }

    //FINAL DECISION
    if (weight1 == weight2) {
        if (vehicle1->getID() > vehicle2->getID()) {
            weight2 = 0;
        }
        else weight1 = 0;
    }

    std::cerr << "w1: " << weight1 << " w2: " << weight2 << " id1: " << vehicle1->getID() << " id2: " << vehicle2->getID() << std::endl; //DEBUG 2

    if (weight1 > weight2) {
        return 2; //Vehicle 1 can proceeds if it has an higher value
    }

    double dist = std::sqrt(dx*dx + dy*dy);

    if (dist < m.getDim()/5) { //TO CHANGE
        return 0;
    }
    return 1;
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
