#ifndef ACAV_INTERSECTIONCOORDINATOR_H
#define ACAV_INTERSECTIONCOORDINATOR_H
#include <map>
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
        std::map<std::string, std::pair<int, int>> oldCOGs;
    public:
        IntersectionCoordinator(Map& map): m(map) {
            currents.reserve(4);
            currentlyGranted.reserve(4);
        };
        bool askPermission(const std::string&);
        double suggestedSpeed(const std::string&, double) const;
        const std::vector<int> pathCollisionFinder(int, int) const;
        void updateStatus();
        bool isEgoInTheMiddle() const;
        bool inTheMiddle(const std::string&) const;
        std::shared_ptr<Vehicle> idVehicleMiddle(const std::string &id) const;
        std::shared_ptr<Vehicle> otherVehicleMiddle(const std::string &id) const;

        double distance(int x1, int y1, int x2, int y2) const;
};


#endif //ACAV_INTERSECTIONCOORDINATOR_H