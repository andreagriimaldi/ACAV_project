#include "Optimizer.h"
#include <iostream>
#include <ostream>

double Optimizer::optimizer(double oldspeed, std::vector<std::vector<double>> futurePerc) {
    updateFSM(futurePerc);
    switch (state) {
        case FSM::NORMAL: {
            return oldspeed;
        }
        case FSM::REQUESTING_STOP: {

        }
        case FSM::STOPPED: {

        }
        case FSM::RESTART: {

        }
    }
    std::cerr << "Vehicle's FSM is broken" << std::endl;
    return -1;
}

void Optimizer::updateFSM(std::vector<std::vector<double>> futurePerc) {

}

FSM Optimizer::getState() const {
    return state;
}
