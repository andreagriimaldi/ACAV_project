#ifndef ACAV_OPTIMIZER_H
#define ACAV_OPTIMIZER_H


class Optimizer {
private:
    const double maxspeed;
public:
    Optimizer(double max): maxspeed(max){};
    double optimizer();
};


#endif //ACAV_OPTIMIZER_H