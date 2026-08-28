#include "Optimizer.h"
#include <iostream>
#include <ostream>

double Optimizer::optimizer(double speed, std::vector<std::vector<double>> futurePerc, int vState) {
    vehicleState = vState;
    updateFSM(futurePerc);
    switch (state) {
        case FSM::NORMAL: {
            return speed;
        }
        case FSM::REQUESTING_STOP: {
            return requestingStop(speed, futurePerc);
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

void Optimizer::updateFSM(const std::vector<std::vector<double>> futurePerc) {
    if (vehicleState == 1) {
        //Vehicle approaching
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

double Optimizer::requestingStop(double speed, const std::vector<std::vector<double>> futurePerc) {

}


