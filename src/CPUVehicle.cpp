#include "CPUVehicle.h"

#include <iostream>
#include <ostream>

void CPUVehicle::move() {
    double steer = computeSteering();
    double s = computeNewSpeed(steer, maxspeed);

    std::vector<std::vector<double>> perc = per.getPerc(getCOGx(), getCOGy(), heading);
    if (perc.size() > 1) {
        avoidObstacles(s, perc);
    }

    updateBicycle(s);
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

            double suggested = coord.suggestedSpeed(ID, s);
            //std::cerr << "Vehicle " << ID << " suggested: " << suggested << " speed before: " << s << std::endl; //DEBUG 1
            if (suggested < 2 * maxspeed) {
                s = computeNewSpeed(0, suggested);
            }
            else std::cerr << "suggestedSpeed() must be reasonable" << std::endl;

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
