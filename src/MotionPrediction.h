#ifndef ACAV_MOTIONPREDICTION_H
#define ACAV_MOTIONPREDICTION_H
#include <fstream>

#include "Map.h"


class MotionPrediction {
private:
    const int measures = 3;
    const int timespan = 10;
    const int freq = 1;
    int relTime = 0;
    double predHeading = 0;
    Map& m;
    const string id;
    const std::vector<std::shared_ptr<Vehicle>>& vehicles;
    std::unordered_map<std::string, std::deque<double>> vehiclesSpeed;
    std::unordered_map<std::string, std::pair<int, int>> actualMP;
public:
    MotionPrediction(Map& map, string egoID): m(map), id(egoID), vehicles(m.getVehicles()) {};
    void update();
    void updateVehicles();
    void appendTimeInstants();
    void computeMotionPrediction();
    const std::unordered_map<std::string, std::pair<int, int>>& getMotionPrediction();
    double dist(double, double, double, double) const;
    double getPredHeading() const;
};


#endif //ACAV_MOTIONPREDICTION_H