#ifndef ACAV_CPUVEHICLE_H
#define ACAV_CPUVEHICLE_H
#include "Vehicle.h"


class CPUVehicle: public Vehicle{
public:
    CPUVehicle(Map &m, const vector<std::shared_ptr<Point>> &surf, int h, double s, string id, int gplan, int maxs)
        : Vehicle(m, surf, h, s, id, gplan, maxs) {
    }
    void move() override;
    double computeNewSpeed(double) const;
    void avoidObstacles(double&, std::vector<std::vector<double>>&) const;
};


#endif //ACAV_CPUVEHICLE_H