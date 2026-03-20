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

void CPUVehicle::avoidObstacles(double& s, std::vector<std::vector<double>>& per) {
    std::vector<std::vector<double>> perc = per;
    int state = static_cast<int>(perc.at(0).at(0));
    perc.erase(perc.begin());

    double dist = map.getDim()/5;
    double close_dist = map.getDim()/8;

    switch (state) {
        case 0: {
            //Straight
            for (const auto& obs: perc) {
                if (obs.at(2) < close_dist && std::abs(obs.at(1)) < 45) {
                    s = s/2;
                    break;
                }
            }
            break;
        }
        case 1: {
            //Approaching

            //Here i have to check if i have permission to approach the intersection
            //I need to introduce a distance in which i check, so that if permission denied i can stop
            //using askPermission()

            bool permission = coord.askPermission(ID);
            if (!permission) {
                s = s/3; //MAYBE TO CHANGE (JUST THE VALUE)
            }

            break;
        }
        case 2: {
            //Middle

            //Here i need to check if i have to slow down for some reason
            //I guess calling suggestedSpeed() (?)

            int suggested = coord.suggestedSpeed(ID);
            std::cerr << "Vehicle " << ID << " suggested: " << suggested << " speed before: " << s << std::endl; //DEBUG 1
            if (suggested == 0) {
                //STOP
                s = s/3; //TO CHANGE
            }
            else if (suggested == 1) {
                //SLOW DOWN
                s = s/1.3; //TO CHANGE
            }
            else if (suggested == 2) {
                //do nothing
            }
            else std::cerr << "suggestedSpeed() must return the value 0, 1 or 2" << std::endl;

            break;
        }
        case 3: {
            //Exiting
            for (const auto& obs: perc) {
                if (obs.at(2) < dist && std::abs(obs.at(1)) < 45) {
                    s = s/3;
                    break;
                }
            }
            break;
        }
        default: std::cerr << "Error in ego vehicle localization" << std::endl;
    }
}
