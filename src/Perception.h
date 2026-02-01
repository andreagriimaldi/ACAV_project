#ifndef ACAV_PERCEPTION_H
#define ACAV_PERCEPTION_H
#include "Map.h"


class Perception {
    private:
        Map& m;
        int gplan;
    public:
        explicit Perception(Map& map, int g): m(map), gplan(g){};
        std::vector<std::vector<double>> getPerc(int, int, int) const;
        double computeState(int, int) const;
};


#endif //ACAV_PERCEPTION_H