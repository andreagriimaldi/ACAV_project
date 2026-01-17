#ifndef ACAV_EGOVEHICLE_H
#define ACAV_EGOVEHICLE_H
#include "Vehicle.h"


class EgoVehicle: public Vehicle {
public:
    EgoVehicle(Map &m, const vector<std::shared_ptr<Point>> &surf, int h, double s, string id, int gplan)
        : Vehicle(m, surf, h, s, id, gplan) {
    }
    void move() override;
};


#endif //ACAV_EGOVEHICLE_H