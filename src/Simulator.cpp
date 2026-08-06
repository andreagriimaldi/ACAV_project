#include "Simulator.h"

#include <iostream>

void Simulator::init() {

}

void Simulator::terminate() {
}

void Simulator::crash() {
}

// 0 N, 1 E, 2 S, 3 W
bool Simulator::isSpawnPointFree(int direction) {
    int x, y;
    if (direction == 0) {
        x = mapDim/3 + mapDim/10;
        y = mapDim/3 - mapDim/30;
    }
    else if (direction == 1) {
        x = (2*mapDim)/3 + mapDim/30;
        y = mapDim/3 + mapDim/10;
    }
    else if (direction == 2) {
        x = (2*mapDim)/3 - mapDim/10;
        y = (2*mapDim)/3 + mapDim/30;
    }
    else if (direction == 3) {
        x = mapDim/3 - mapDim/30;
        y = (2*mapDim)/3 - mapDim/10;
    }
    else std::cerr << "Direction must be a number between 0 and 3" << std::endl;

    vector<std::pair<int, int>> COGs = map.getCOGs();

    for (const std::pair<int, int>& cog: COGs) {
        if (std::sqrt((cog.first - x)*(cog.first - x) + (cog.second - y)*(cog.second - y) ) < mapDim/3) { //TUNING PARAMETER
            return false;
        }
    }

    return true;
}
