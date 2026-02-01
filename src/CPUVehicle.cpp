#include "CPUVehicle.h"

#include <iostream>
#include <ostream>

void CPUVehicle::move() {
    double steer = computeSteering();
    double s = computeNewSpeed(steer);

    std::vector<std::vector<double>> perc = per.getPerc(getCOGx(), getCOGy(), heading);
    if (perc.size() > 1) {
        avoidObstacles(s, perc);
    }

    updateBicycle(s);
}

double CPUVehicle::computeNewSpeed(double steer) const {
    double L = (map.getDim()/9) * 0.65;

    double a_lat_max = 0.05; //TUNING PARAMETER
    double a_str_max = 0.3; //TUNING PARAMETER

    double v_max = maxspeed;

    if (std::abs(steer) > 0.001) {
        double v_lat = std::sqrt(a_lat_max * L / std::abs(std::tan(steer)));
        v_max = std::min(v_max, v_lat);
    }

    if (v_max > speed + a_str_max) {
        v_max = speed + a_str_max;
    }
    else if (v_max < speed - a_str_max) {
        v_max = speed - a_str_max;
    }

    return v_max;
}

void CPUVehicle::avoidObstacles(double& s, std::vector<std::vector<double>>& per) const {
    std::vector<std::vector<double>> perc = per;
    int state = static_cast<int>(perc.at(0).at(0));
    perc.erase(perc.begin());

    double dist = map.getDim()/5;
    double close_dist = map.getDim()/8;

    switch (state) {
        case 0: {
            //Straight
            for (const auto& obs: perc) {
                if (obs.at(2) < dist/2 && std::abs(obs.at(1)) < 45) {
                    s = s/2;
                    break;
                }
            }
            break;
        }
        case 1: {
            //Approaching/exiting
            for (const auto& obs: perc) {
                int obsState = static_cast<int>(obs.at(0));
                if (obsState == 2 && std::abs(obs.at(1)) < 90) {
                    s = s/3;
                    break;
                }
                if (obs.at(2) < dist && std::abs(obs.at(1)) < 30) {
                    s = 0;
                    break;
                }
            }
            break;
        }
        case 2: {
            //Middle
            for (const auto& obs: perc) {
                if (obs.at(2) < close_dist && std::abs(obs.at(1)) < 60) {
                    s = s/5;
                    break;
                }
            }
            break;
        }
        case 3: {

        }
        default: std::cerr << "Error in ego vehicle localization" << std::endl;
    }
}
