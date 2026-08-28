#ifndef ACAV_OPTIMIZER_H
#define ACAV_OPTIMIZER_H
#include <vector>
#include "FSM.h"


class Optimizer {
private:
    const double maxspeed;
    FSM state;
public:
    Optimizer(double max): maxspeed(max), state(FSM::NORMAL){};
    double optimizer(double, std::vector<std::vector<double>>);
    void updateFSM(std::vector<std::vector<double>>);
    FSM getState() const;
};


#endif //ACAV_OPTIMIZER_H