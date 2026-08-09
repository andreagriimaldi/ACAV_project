#ifndef ACAV_MONTECARLOSIMULATOR_H
#define ACAV_MONTECARLOSIMULATOR_H

#include <random>
#include <utility>
#include "Map.h"

class MonteCarloSimulator {
private:
    const int mapDim;
    Map map;
    const bool egoPresence;

    int cpuGenerated = 0;
    int egoGenerated = 0;
    int cpuActual    = 0;
    int egoActual    = 0;
    int cpuCompleted = 0;
    int egoCompleted = 0;
    int time         = 0;

    std::mt19937 rng;

    void step();
    bool isSpawnPointFree(int direction) const;
    int  vehicleSpawnedFromHere(int direction) const;
    void generateCPUVehicle(int spawn, double speed, int gplan);
    void generateEgoVehicle(int spawn, double speed, int gplan);
    void removeVehicles();
    bool isVehicleAtTheEnd(std::pair<int,int> cog, int gplan) const;

public:
    explicit MonteCarloSimulator(int mDim, bool ego, unsigned seed)
        : mapDim(mDim), map(mDim), egoPresence(ego), rng(seed) {
        map.initialize();
    }
    int run(int horizon);
    int getCpuGenerated() const { return cpuGenerated; }
    int getEgoGenerated() const { return egoGenerated; }
    int getCpuCompleted() const { return cpuCompleted; }
    int getEgoCompleted() const { return egoCompleted; }
    int getTime()         const { return time; }
    bool crashed()        const { return map.crash(); }
};

#endif // ACAV_MONTECARLOSIMULATOR_H