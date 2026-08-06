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
    public:
        explicit Simulator(const int mDim, const bool ego): mapDim(mDim), map(Map(mapDim)), egoPresence(ego) {};
        void init();
        void terminate();
        void crash();
        bool isSpawnPointFree(int direction);
};


#endif //ACAV_SIMULATOR_H