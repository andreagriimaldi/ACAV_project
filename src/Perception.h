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
        static double computeState(int, int, int, int);
        std::vector<std::vector<double>> getFuturePerc(int, int, int, const std::unordered_map<std::string, std::pair<int, int>>&) const;
};


#endif //ACAV_PERCEPTION_H