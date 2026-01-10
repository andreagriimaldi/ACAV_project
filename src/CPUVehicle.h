#ifndef ACAV_CPUVEHICLE_H
#define ACAV_CPUVEHICLE_H
#include "Vehicle.h"


class CPUVehicle: public Vehicle{
public:
    CPUVehicle(Map &m, const vector<std::shared_ptr<Point>> &surf)
        : Vehicle(m, surf) {
    }
    void move() override;
};


#endif //ACAV_CPUVEHICLE_H