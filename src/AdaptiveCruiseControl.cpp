//
// Created by Andrea Grimaldi on 17/08/2026.
//

#include "AdaptiveCruiseControl.h"

bool AdaptiveCruiseControl::checkVehicleInFront(const std::vector<std::vector<double>>& per) {
    double dist_min = dim;
    int min_index = -1;
    for (int i = 1; i < per.size(); i++) {
        if (per.at(i).at(2) < dist_min and std::abs(per.at(i).at(1)) < 45) {
            dist_min = per.at(i).at(2);
            min_index = i;
        }
    }

    if (min_index == -1) {
        //there's no vehicle to track
        tracking = false;
        measures.clear();
        measureSize = 0;
        return false;
    }

    const std::vector<double>& obs = per.at(min_index);

    if (obs.at(2) < tracking_dist) {
        //there's a vehicle to track
        if (tracking) {
            //it was already tracked
            measures.emplace_front(obs.at(1), obs.at(2));
            measureSize++;
        }
        else {
            //it's a new one
            tracking = true;
            measures.emplace_front(obs.at(1), obs.at(2));
            measureSize++;
        }
        if (measureSize == maxMeasures) {
            measures.pop_back();
            measureSize--;
        }
        return true;
    }
    else {
        //there's no vehicle to track
        tracking = false;
        measures.clear();
        measureSize = 0;
        return false;
    }
}

//this returns the speed suggested by the ACC
double AdaptiveCruiseControl::update(double speed, const std::vector<std::vector<double>>& per) {
    if (checkVehicleInFront(per)) {
        //
    }
    return speed;
}
