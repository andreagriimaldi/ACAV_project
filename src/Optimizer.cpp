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
            if (!rightFree(perc, shortTurn) or !crossingAllowed(perc, futurePerc, shortTurn)) {
                if (oldspeed <= 0.1) {
                    state = FSM::STOPPED;
                }
                else state = FSM::REQUESTING_STOP;
            }
            else if (shortTurn and colliding(perc, futurePerc, shortTurn)) {
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
            if (rightFree(perc, shortTurn) and crossingAllowed(perc, futurePerc, shortTurn)) {
                state = FSM::RESTART;
            }
        }
    }
    else if (vehicleState == 2) {
        if (oldspeed > 0.0) {
            if (colliding(perc, futurePerc, shortTurn)) {
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
            if (!colliding(perc, futurePerc, shortTurn)) {
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
        if (dist * std::cos(ang * DEG) < 0.0) continue;

        closest = std::min(closest, dist);
    }

    //TUNING PARAMETER
    const double f = std::clamp(closest / (dim / 3.0), 0.0, 1.0);
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

bool Optimizer::crossingAllowed(const std::vector<std::vector<double>>& perc, const std::vector<std::vector<double>>& futurePerc, bool shortTurn) {
    //basically this checks how many vehicles are in the middle and says y/n
    //MAYBE TO BE MODIFIED, ALLOW IF ONE IS CLOSER TO THE END AND THE OTHER DONT CARE
    if (shortTurn) {
        return true;
    }
    int count = 0;
    for (int i = 1; i < perc.size(); i++) {
        if (perc.at(i).at(0) == 2) {
            count++;
        }
    }
    return count < 3 and oneMovingAway(perc, futurePerc);
}

bool Optimizer::colliding(const std::vector<std::vector<double>>& perc, const std::vector<std::vector<double>>& futurePerc, bool shortTurn) const {
    if (shortTurn) {
        if (vehicleState == 1 or vehicleState == 2) {
            //when vehicle is approaching it must stop if there's an obstacle ahead
            for (int i = 1; i < perc.size(); i++) {
                int obsState = static_cast<int>(perc.at(i).at(0));
                double obsAngleDiff = perc.at(i).at(1);
                double obsDistance = perc.at(i).at(2);
                if ((obsState == 2 or obsState == 3) and std::abs(obsAngleDiff) < 45 and obsDistance < dim/6.0) {
                    return true;
                }
            }
            return false;
        }
    }
    //still to implement the case where the turn is not short
}

bool Optimizer::oneMovingAway(const std::vector<std::vector<double>>& perc, const std::vector<std::vector<double>>& futurePerc) {
    if (oneMovingAwayAllowed) {
        return true;
    }
    std::vector<std::vector<double>> middleT;
    for (int i = 1; i < perc.size(); i++) {
        if (perc.at(i).at(0) == 2) {
            middleT.push_back(perc.at(i));
        }
    }

    if (middleT.size() > 1) { //oneMovingAway() gets called when there are at least two vehicles in the middle
        for (const auto&v : middleT) {
            double bestCost = 2.0;
            double closestDistDiff = 0.0;
            double closestAngDiff = 0;
            bool matched = false;

            for (int i = 1; i < futurePerc.size(); i++) {
                const std::vector<double>& fut = futurePerc.at(i);
                const double cost = std::abs(v.at(1) - fut.at(1)) / 180.0 + std::abs(v.at(2) - fut.at(2)) / dim;
                if (cost < bestCost) {
                    bestCost = cost;
                    closestDistDiff = v.at(2) - fut.at(2);
                    closestAngDiff = std::abs(v.at(1)) - std::abs(fut.at(1));
                    matched = true;
                }
            }
            if (matched and (closestDistDiff < 0 and closestAngDiff <  0)) {
                std::cerr << "ONEMOVINGAWAY() TRUE" << std::endl;
                oneMovingAwayAllowed = true;
                return true;
            }
        }
        std::cerr << "ONEMOVINGAWAY() FALSE " << std::endl;
        return false;
    }
    return true;
}


