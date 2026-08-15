#include "MotionPrediction.h"
#include <iostream>
#include "Perception.h"
#include "Vehicle.h"

//must be called when ego vehicle is approaching or is in the middle
void MotionPrediction::update() {
    updateVehicles();
    appendTimeInstants();
}

void MotionPrediction::updateVehicles() {
    for (const auto& v: vehicles) {
        if (v->getID() != id) {
            double state = Perception::computeState(v->getCOGx(), v->getCOGy(), v->getGlobalPlan(), m.getDim());
            if ((state == 1 or state == 2) and COGsOverTime.count(v->getID()) == 0) {
                COGsOverTime[v->getID()];
            }
            if (!(state == 1 or state == 2)) {
                COGsOverTime.erase(v->getID());
            }
        }
    }
}

void MotionPrediction::appendTimeInstants() {
    for (const auto& v : vehicles) {
        auto it = COGsOverTime.find(v->getID());
        if (!(it == COGsOverTime.end())) {
            it->second.emplace_front(v->getCOGx(), v->getCOGy());
            if (it->second.size() > timespan) {
                it->second.pop_back();
            }
        }
    }
}
