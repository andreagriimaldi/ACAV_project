#include "Optimizer.h"
#include <iostream>
#include <ostream>

double Optimizer::optimizer(double speed, double oldspeed, const std::vector<std::vector<double>>& futurePerc, int vState, const std::vector<std::vector<double>>& perc) {
    vehicleState = vState;
    updateFSM(speed, oldspeed, futurePerc, perc);
    switch (state) {
        case FSM::NORMAL: {
            return speed;
        }
        case FSM::REQUESTING_STOP: {
            return requestingStop(oldspeed, futurePerc);
        }
        case FSM::STOPPED: {
            return 0.0;
        }
        case FSM::RESTART: {
            return speed;
        }
    }
    std::cerr << "Vehicle's FSM is broken" << std::endl;
    return -1;
}

void Optimizer::updateFSM(double speed, double oldspeed, const std::vector<std::vector<double>>& futurePerc, const std::vector<std::vector<double>>& perc) {
    if (vehicleState == 1) {
        //Vehicle approaching
        if (oldspeed > 0.0) {
            if (!rightFree(perc) or !crossingAllowed(perc)) {
                if (oldspeed <= 0.1) {
                    state = FSM::STOPPED;
                }
                else state = FSM::REQUESTING_STOP;
            }
            else state = FSM::NORMAL;
        }
        else {
            //Vehicle is now stopped
            if (rightFree(perc) and crossingAllowed(perc)) {
                if (state == FSM::STOPPED) {
                    state = FSM::RESTART;
                }
            }
        }
    }
    else if (vehicleState == 2) {
        //Vehicle in the middle
    }
    else {
        //Vehicle straight or exiting (relying on ACC)
        state = FSM::NORMAL;
    }
}

FSM Optimizer::getState() const {
    return state;
}

double Optimizer::requestingStop(double oldspeed, const std::vector<std::vector<double>>& futurePerc) const {
    //this will return the suggested speed (when the vehicle has to stop)
}

bool Optimizer::rightFree(const std::vector<std::vector<double>>& perc) const {
    const double DEG = M_PI / 180.0;
    const double range = (2.0 * dim) / 3.0;
    const double lane = dim / 30.0;
    const double back = dim / 9.0;

    for (size_t i = 1; i < perc.size(); ++i) {
        const double st = perc.at(i).at(0);
        const double ang = perc.at(i).at(1);
        const double dist = perc.at(i).at(2);

        if (!(st == 1 or st == 2)) continue;
        if (dist > range) continue;

        const double right   = dist * std::sin(ang * DEG);
        const double forward = dist * std::cos(ang * DEG);

        if (right > lane and forward > -back) return false;
    }
    return true;
}

bool Optimizer::crossingAllowed(const std::vector<std::vector<double>>& perc) const {
    return true;
}


