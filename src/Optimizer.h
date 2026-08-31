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
    bool oneMovingAwayAllowed = false;
    int COGx = 0, COGy = 0;
    const int gplan;
    bool stopped = false;
    int timerStop = 0;
public:
    Optimizer(double max, int d, int plan): maxspeed(max), state(FSM::NORMAL), dim(d), gplan(plan){};
    double optimizer(double, double, const std::vector<std::vector<double>>&, int, const std::vector<std::vector<double>>&, bool, int, int);
    void updateFSM(double, double, const std::vector<std::vector<double>>&, const std::vector<std::vector<double>>&, bool);
    FSM getState() const;
    double requestingStop(double, const std::vector<std::vector<double>>&, bool) const;
    bool rightFree(const std::vector<std::vector<double>>&, bool) const;
    bool crossingAllowed(const std::vector<std::vector<double>>&, const std::vector<std::vector<double>>&, bool);
    bool colliding(const std::vector<std::vector<double>>&, const std::vector<std::vector<double>>&, bool);
    bool closestMovingAway(const std::vector<std::vector<double>>&, const std::vector<std::vector<double>>&);
    bool centerCollision(const std::vector<std::vector<double>>&, const std::vector<std::vector<double>>&);
    bool pastTheCenter() const;
};


#endif //ACAV_OPTIMIZER_H