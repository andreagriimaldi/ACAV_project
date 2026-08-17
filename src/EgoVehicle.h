#ifndef ACAV_EGOVEHICLE_H
#define ACAV_EGOVEHICLE_H
#include "AdaptiveCruiseControl.h"
#include "MotionPrediction.h"
#include "Vehicle.h"


class EgoVehicle: public Vehicle {
private:
    MotionPrediction mp;
    AdaptiveCruiseControl acc;
public:
    EgoVehicle(Map &m, const vector<std::shared_ptr<Point>> &surf, int h, double s, string id, int gplan, int maxs)
        : Vehicle(m, surf, h, s, id, gplan, maxs), mp(m, id), acc(map.getDim(),id) {
    }
    void move() override;
    std::vector<std::vector<double>> computeFuture();
    double adaptiveCruiseControl(double, const std::vector<std::vector<double>>&);
    double optimizer(double, std::vector<std::vector<double>>);
};


#endif //ACAV_EGOVEHICLE_H