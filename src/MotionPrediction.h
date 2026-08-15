#ifndef ACAV_MOTIONPREDICTION_H
#define ACAV_MOTIONPREDICTION_H
#include "Map.h"


class MotionPrediction {
private:
    const int timespan = 10;
    Map& m;
    const string& id;
    const std::vector<std::shared_ptr<Vehicle>>& vehicles;
    std::unordered_map<std::string, std::deque<std::pair<int,int>>> COGsOverTime;
public:
    MotionPrediction(Map& map, const string& egoID): m(map), id(egoID), vehicles(m.getVehicles()) {};
    void update();
    void updateVehicles();
    void appendTimeInstants();
};


#endif //ACAV_MOTIONPREDICTION_H