#ifndef ACAV_INTERSECTIONCOORDINATOR_H
#define ACAV_INTERSECTIONCOORDINATOR_H
#include <queue>
#include <vector>

class Map;
class Vehicle;

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
        bool askPermission(const std::string&);
        int suggestedSpeed(const std::string&, double) const;
        const std::vector<int> pathCollisionFinder(int, int) const;
        void updateStatus();
        bool isEgoInTheMiddle() const;
        bool inTheMiddle(const std::string&) const;
        std::shared_ptr<Vehicle> idVehicleMiddle(const std::string &id) const;
        std::shared_ptr<Vehicle> otherVehicleMiddle(const std::string &id) const;
};


#endif //ACAV_INTERSECTIONCOORDINATOR_H