#include "EgoVehicle.h"

#include <iostream>

void EgoVehicle::move() {
    double steer = computeSteering();
    double speed = computeNewSpeed(steer, maxspeed);

    std::vector<std::vector<double>> perc = per.getPerc(getCOGx(), getCOGy(), heading);

    double accSpeed = adaptiveCruiseControl(speed, perc), optSpeed = maxspeed;
    int choice = 0;

    if ((perc.at(0).at(0) == 1 or perc.at(0).at(0) == 2) and perc.size() > 1) {
        std::vector<std::vector<double>> futurePerc = computeFuture();
        optSpeed = optimizer(speed, futurePerc);
    }

    if (accSpeed < speed or optSpeed < speed) {
        choice = accSpeed < optSpeed ? 1 : 2;
    }

    speed = std::min(accSpeed, optSpeed);

    if (speed > maxspeed) {
        speed = maxspeed;
    }

    tel.update(speed, acc.isTracking(), accSpeed, optSpeed, choice, acc.isBraking());

    updateBicycle(speed);
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

double EgoVehicle::optimizer(double oldspeed, std::vector<std::vector<double>> futurePer) {
    return oldspeed; //TO CHANGE
}

const EgoTelemetry & EgoVehicle::getTelemetry() const {
    return tel;
}
