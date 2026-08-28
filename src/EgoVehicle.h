#ifndef ACAV_EGOVEHICLE_H
#define ACAV_EGOVEHICLE_H
#include "AdaptiveCruiseControl.h"
#include "EgoTelemetry.h"
#include "MotionPrediction.h"
#include "Optimizer.h"
#include "Vehicle.h"


class EgoVehicle: public Vehicle {
private:
    MotionPrediction mp;
    AdaptiveCruiseControl acc;
    Optimizer op;
    EgoTelemetry tel;
public:
    EgoVehicle(Map &m, const vector<std::shared_ptr<Point>> &surf, int h, double s, string id, int gplan, double maxs)
        : Vehicle(m, surf, h, s, id, gplan, maxs), mp(m, id), acc(map.getDim(),id, maxspeed), tel(maxspeed), op(maxspeed) {
    }
    void move() override;
    std::vector<std::vector<double>> computeFuture();
    double adaptiveCruiseControl(double, const std::vector<std::vector<double>>&);
    double optimizer(double, std::vector<std::vector<double>>, int);
    const EgoTelemetry& getTelemetry() const;
};


#endif //ACAV_EGOVEHICLE_H