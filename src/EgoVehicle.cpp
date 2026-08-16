#include "EgoVehicle.h"

#include <iostream>

void EgoVehicle::move() {
    //TEMP
    //computeNewPosition(82, map.getDim()/2, map.getDim()/2);

    //THE FINAL VERSION WILL DECIDE NEW COG AND HEADING AND WILL CALL COMPUTENEWPOSITION()
    //REMEMBER THAT THE NEW COG AND HEADING WILL BE AFFECTED BY CURRENT SPEED (AND RELY ON ACC FOR THAT)

    //Decide new speed
    updateBicycle(0);
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
