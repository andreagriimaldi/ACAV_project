#ifndef ACAV_OPTIMIZER_H
#define ACAV_OPTIMIZER_H
#include <vector>
#include "FSM.h"


class Optimizer {
private:
    const double maxspeed;
    FSM state;
    int vehicleState = 0;
    const int dim;
public:
    Optimizer(double max, int d): maxspeed(max), state(FSM::NORMAL), dim(d){};
    double optimizer(double, double, const std::vector<std::vector<double>>&, int, const std::vector<std::vector<double>>&);
    void updateFSM(double, double, const std::vector<std::vector<double>>&, const std::vector<std::vector<double>>&);
    FSM getState() const;
    double requestingStop(double, const std::vector<std::vector<double>>&) const;
    bool rightFree(const std::vector<std::vector<double>>&) const;
    bool crossingAllowed(const std::vector<std::vector<double>>&) const;
};


#endif //ACAV_OPTIMIZER_H