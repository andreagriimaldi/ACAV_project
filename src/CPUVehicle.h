#ifndef ACAV_CPUVEHICLE_H
#define ACAV_CPUVEHICLE_H
#include "Vehicle.h"


class CPUVehicle: public Vehicle{
private:
    IntersectionCoordinator& coord;
public:
    CPUVehicle(Map &m, const vector<std::shared_ptr<Point>> &surf, int h, double s, string id, int gplan, int maxs)
        : Vehicle(m, surf, h, s, id, gplan, maxs), coord(map.getCoordinator()){
    }
    void move() override;
    double computeNewSpeed(double, double) const;
    void avoidObstacles(double&, std::vector<std::vector<double>>&);
};


#endif //ACAV_CPUVEHICLE_H