#include "Optimizer.h"
#include <iostream>
#include <ostream>

#include "Vehicle.h"

double Optimizer::optimizer(double speed, double oldspeed, const std::vector<std::vector<double>>& futurePerc, int vState, const std::vector<std::vector<double>>& perc, bool shortTurn) {
    vehicleState = vState;
    updateFSM(speed, oldspeed, futurePerc, perc, shortTurn);
    switch (state) {
        case FSM::NORMAL: {
            return speed;
        }
        case FSM::REQUESTING_STOP: {
            if (vehicleState == 1) {
                return requestingStop(oldspeed, futurePerc, true);
            }
            return requestingStop(oldspeed, futurePerc, false);
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

void Optimizer::updateFSM(double speed, double oldspeed, const std::vector<std::vector<double>>& futurePerc, const std::vector<std::vector<double>>& perc, bool shortTurn) {
    if (vehicleState == 1) {
        //Vehicle approaching
        if (oldspeed > 0.0) {
            if (!rightFree(perc, shortTurn) or !crossingAllowed(perc, shortTurn)) {
                if (oldspeed <= 0.1) {
                    state = FSM::STOPPED;
                }
                else state = FSM::REQUESTING_STOP;
            }
            else state = FSM::NORMAL;
        }
        else {
            //Vehicle is now stopped
            state = FSM::STOPPED;
            if (rightFree(perc, shortTurn) and crossingAllowed(perc, shortTurn)) {
                state = FSM::RESTART;
            }
        }
    }
    else if (vehicleState == 2) {
        if (oldspeed > 0.0) {
            if (colliding(futurePerc)) {
                if (oldspeed <= 0.1) {
                    state = FSM::STOPPED;
                }
                else state = FSM::REQUESTING_STOP;
            }
            else state = FSM::NORMAL;
        }
        else {
            //Vehicle is now stopped
            state = FSM::STOPPED;
            if (!colliding(futurePerc)) {
                state = FSM::RESTART;
            }
        }
    }
    else {
        //Vehicle straight or exiting (relying on ACC)
        state = FSM::NORMAL;
    }
}

FSM Optimizer::getState() const {
    return state;
}

double Optimizer::requestingStop(double oldspeed, const std::vector<std::vector<double>>& futurePerc, bool approaching) const {
    //this will return the suggested speed (when the vehicle has to stop)
    if (approaching) {
        return std::clamp(oldspeed - 2 * Vehicle::a_str_max, 0.0, maxspeed);
    }
    //use futurePerc to compute how much to brake
    const double DEG = M_PI / 180.0;

    double closest = 2.0 * dim;
    for (int i = 1; i < futurePerc.size(); i++) {
        const double st   = futurePerc.at(i).at(0);
        const double ang  = futurePerc.at(i).at(1);
        const double dist = futurePerc.at(i).at(2);

        if (!(st == 1 or st == 2)) continue;
        if (dist * std::cos(ang * DEG) < 0.0) continue;   // behind me

        closest = std::min(closest, dist);
    }

    //TUNING PARAMETER
    const double f     = std::clamp(closest / (dim / 3.0), 0.0, 1.0);
    const double brake = (2.0 - f) * Vehicle::a_str_max;

    return std::clamp(oldspeed - brake, 0.0, maxspeed);
}

bool Optimizer::rightFree(const std::vector<std::vector<double>>& perc, bool shortTurn) const {
    if (shortTurn) {
        return true;
    }
    const double DEG = M_PI / 180.0;
    const double range = (1.5 * dim) / 3.0;
    const double lane = dim / 30.0;
    const double back = dim / 9.0;

    for (int i = 1; i < perc.size(); i++) {
        const double st = perc.at(i).at(0);
        const double ang = perc.at(i).at(1);
        const double dist = perc.at(i).at(2);

        if (!(st == 1 or st == 2)) continue;
        if (dist > range) continue;

        const double right   = dist * std::sin(ang * DEG);
        const double forward = dist * std::cos(ang * DEG);

        if (right > lane and forward > -back) {
            return false;
        }
    }
    return true;
}

bool Optimizer::crossingAllowed(const std::vector<std::vector<double>>& perc, bool shortTurn) const {
    //basically this checks how many vehicles are in the middle and says y/n
    if (shortTurn) {
        return true;
    }
    int count = 0;
    for (int i = 1; i < perc.size(); i++) {
        if (perc.at(i).at(0) == 2) {
            count++;
        }
    }
    return count < 3;
}

bool Optimizer::colliding(const std::vector<std::vector<double>>& futurePerc) const {
    return false; //TO CHANGE
}


