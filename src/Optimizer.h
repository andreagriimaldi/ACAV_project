#ifndef ACAV_OPTIMIZER_H
#define ACAV_OPTIMIZER_H
#include <vector>
#include "FSM.h"


class Optimizer {
private:
    const double maxspeed;
    FSM state;
    int vehicleState = 0;
public:
    Optimizer(double max): maxspeed(max), state(FSM::NORMAL){};
    double optimizer(double, std::vector<std::vector<double>>, int);
    void updateFSM(std::vector<std::vector<double>>);
    FSM getState() const;
    double requestingStop(double, std::vector<std::vector<double>>);
};


#endif //ACAV_OPTIMIZER_H