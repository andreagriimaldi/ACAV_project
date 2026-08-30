#ifndef ACAV_SIMULATOR_H
#define ACAV_SIMULATOR_H
#include "Map.h"
#include "SDLRenderer.h"


class Simulator {
    private:
        const int GUI_SIZE;
        const int mapDim;
        Map map;
        const bool egoPresence;
        int cpuGenerated = 0;
        int egoGenerated = 0;
        int time = 0;
        int cpuActual = 0;
        int egoActual = 0;
        SDLRenderer renderer;
        int stallTicks = 0;
    public:
        explicit Simulator(int sizeGUI, int mDim, const bool ego): GUI_SIZE(sizeGUI), mapDim(mDim), map(Map(mapDim)), egoPresence(ego), renderer(GUI_SIZE) {
            init();
        };
        void init();
        void loop();
        void terminate() const;
        bool crash();
        bool isSpawnPointFree(int direction) const;
        int vehicleSpawnedFromHere(int direction) const;
        void generateCPUVehicle(int spawn, double speed, int gplan);
        void generateEgoVehicle(int spawn, double speed, int gplan);
        void removeVehicles();
        bool isVehicleAtTheEnd(std::pair<int, int>, int) const;
        bool stall();
        void updateStall();
};


#endif //ACAV_SIMULATOR_H