#include "EgoVehicle.h"

#include <iostream>

void EgoVehicle::move() {
    double steer = computeSteering();
    double new_speed = computeNewSpeed(steer, maxspeed);

    std::vector<std::vector<double>> perc = per.getPerc(getCOGx(), getCOGy(), heading);

    double accSpeed = adaptiveCruiseControl(new_speed, perc), optSpeed = maxspeed;
    int choice = 0;

    if ((perc.at(0).at(0) == 1 or perc.at(0).at(0) == 2) and perc.size() > 1) {
        std::vector<std::vector<double>> futurePerc = computeFuture();
        optSpeed = optimizer(new_speed, speed, futurePerc, static_cast<int>(perc.at(0).at(0)), perc);
    }

    if (accSpeed < new_speed or optSpeed < new_speed) {
        choice = accSpeed < optSpeed ? 1 : 2;
    }

    new_speed = std::min(accSpeed, optSpeed);

    if (new_speed > maxspeed) {
        new_speed = maxspeed;
    }

    tel.update(new_speed, acc.isTracking(), accSpeed, optSpeed, choice, acc.isBraking(), op.getState());

    updateBicycle(new_speed);
}

std::vector<std::vector<double>> EgoVehicle::computeFuture() {
    const std::unordered_map<std::string, std::pair<int, int>>& pred = mp.getMotionPrediction();

    for (const auto& entry: pred) {
        if (entry.first == "ego") {
            int egoX = entry.second.first;
            int egoY = entry.second.second;
            double heading = mp.getPredHeading();
            return per.getFuturePerc(egoX, egoY, heading, pred);
        }
    }

    std::cerr << "ego vehicle not in the prediction" << std::endl;
    return vector<vector<double>>({{0}});
}

double EgoVehicle::adaptiveCruiseControl(double oldspeed, const std::vector<std::vector<double>>& per) {
    return acc.update(oldspeed, per);
}

double EgoVehicle::optimizer(double new_speed, double oldspeed, const std::vector<std::vector<double>>& futurePer, int vState, const std::vector<std::vector<double>>& per) {
    return op.optimizer(new_speed, oldspeed, futurePer, vState, per);
}

const EgoTelemetry & EgoVehicle::getTelemetry() const {
    return tel;
}
