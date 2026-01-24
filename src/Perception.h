#ifndef ACAV_PERCEPTION_H
#define ACAV_PERCEPTION_H
#include "Map.h"


class Perception {
    private:
        Map& m;
    public:
        explicit Perception(Map& map): m(map){};
        std::vector<std::vector<int>> getPerc(int, int, int) const;
        int computeState(int, int) const;
};


#endif //ACAV_PERCEPTION_H