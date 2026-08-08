#ifndef ACAV_SIMULATOR_H
#define ACAV_SIMULATOR_H
#include "Map.h"


class Simulator {
    private:
        const int mapDim;
        const Map map;
        const bool egoPresence;
        int cpuGenerated = 0;
        int egoGenerated = 0;
        int time = 0;
        int cpuActual = 0;
        int egoActual = 0;
    public:
        explicit Simulator(const int mDim, const bool ego): mapDim(mDim), map(Map(mapDim)), egoPresence(ego) {};
        void init();
        void loop();
        void terminate();
        void crash();
        bool isSpawnPointFree(int direction) const;
        int vehicleSpawnedFromHere(int direction) const;
        void generateCPUVehicle(int spawn, double speed, int gplan);
        void generateEgoVehicle(int spawn, double speed, int gplan);
        void removeVehicles();
        bool isVehicleAtTheEnd(std::pair<int, int>, int) const;
};


#endif //ACAV_SIMULATOR_H