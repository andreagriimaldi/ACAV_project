#include "AdaptiveCruiseControl.h"
#include <cmath>
#include <iostream>

#include "Vehicle.h"

bool AdaptiveCruiseControl::checkVehicleInFront(const std::vector<std::vector<double>>& per) {
    double dist_min = dim;
    int min_index = -1;
    for (int i = 1; i < per.size(); i++) {
        double lateral = per.at(i).at(2) * std::sin(per.at(i).at(1) * M_PI / 180.0);
        double forward = per.at(i).at(2) * std::cos(per.at(i).at(1) * M_PI / 180.0);
        bool inPath = forward > 0 and std::abs(lateral) < dim / 15.0;
        if (per.at(i).at(2) < dist_min and inPath) {
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
            //it was already tracked, but i have to check whether it's the old one or a new one
            if (std::abs(obs.at(1) - measures.front().first) < 10 and std::abs(obs.at(2) - measures.front().second) < dim/50.0) {
                //it's the same as before
                measures.emplace_front(obs.at(1), obs.at(2));
                measureSize++;
            }
            else {
                //it's a new target
                measures.clear();
                measureSize = 0;
                measures.emplace_front(obs.at(1), obs.at(2));
                measureSize++;
            }
        }
        else {
            //it's a new one
            tracking = true;
            measures.emplace_front(obs.at(1), obs.at(2));
            measureSize++;
        }
        if (measureSize == maxMeasures + 1) {
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

bool AdaptiveCruiseControl::emergencyBrake(const std::vector<std::vector<double>>& per) const {
    double r = dim/18.0;
    double toll = dim/30.0;
    double half_width = dim/10.0;

    for (const auto& obs: per) {
        double forward = obs.at(2) * std::cos(obs.at(1) * M_PI / 180.0);
        double lateral = obs.at(2) * std::sin(obs.at(1) * M_PI / 180.0);
        if (forward > 0 and std::abs(lateral) < half_width and forward < (2 * r + toll)) {
            return true;
        }
    }

    return false;
}

//this returns the speed suggested by the ACC
double AdaptiveCruiseControl::update(double speed, const std::vector<std::vector<double>>& per) {
    if (emergencyBrake(per)) {
        return speed/2; //TO CHANGE
    }
    if (checkVehicleInFront(per)) {
        return computeSpeed(speed);
    }
    return speed;
}

double AdaptiveCruiseControl::computeSpeed(double speed) const {
    const double bearing = measures.front().first * M_PI / 180.0;
    const double forward = measures.front().second * std::cos(bearing);

    const double time_gap = 5;
    const double distCOGs = dim/9 + dim/30;
    const double kp = 0.05;

    const double desired = distCOGs + speed * time_gap;
    const double e = forward - desired;

    double cmd = speed + kp * e;

    if (cmd < 0) {
        cmd = 0;
    }
    if (cmd - speed > Vehicle::a_str_max) {
        cmd = speed + Vehicle::a_str_max;
    }
    if (speed - cmd >  Vehicle::a_str_max) {
        cmd = speed - Vehicle::a_str_max;
    }

    return cmd;
}


