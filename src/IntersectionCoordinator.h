#ifndef ACAV_INTERSECTIONCOORDINATOR_H
#define ACAV_INTERSECTIONCOORDINATOR_H
#include <queue>
#include <vector>

class Map;

class IntersectionCoordinator {
    private:
        Map& m;
        std::queue<std::string> queue;
        std::vector<std::string> currents;
        std::vector<std::string> currentlyGranted;
    public:
        IntersectionCoordinator(Map& map): m(map) {
            currents.reserve(4);
            currentlyGranted.reserve(4);
        };
        bool askPermission(std::string);
        int suggestedSpeed(std::string) const;
        void updateStatus();
};


#endif //ACAV_INTERSECTIONCOORDINATOR_H