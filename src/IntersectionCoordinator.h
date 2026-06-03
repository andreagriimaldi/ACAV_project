#ifndef ACAV_INTERSECTIONCOORDINATOR_H
#define ACAV_INTERSECTIONCOORDINATOR_H
#include <queue>
#include <unordered_map>
#include <vector>

class Map;
class Vehicle;

struct snapshot { int x, y, heading; };

class IntersectionCoordinator {
    private:
        Map& m;
        std::vector<std::string> queue;
        std::vector<std::string> currents;
        std::vector<std::string> currentlyGranted;
        std::unordered_map<std::string, snapshot> oldCOGs;
    public:
        IntersectionCoordinator(Map& map): m(map) {
            currents.reserve(4);
            currentlyGranted.reserve(4);
            queue.reserve(4);
        };
        bool askPermission(const std::string&);
        double suggestedSpeed(const std::string&, double) const;
        std::vector<int> pathCollisionFinder(int, int) const;
        void updateStatus();
        bool isEgoInTheMiddle() const;
        bool inTheMiddle(const std::string&) const;
        std::shared_ptr<Vehicle> idVehicleMiddle(const std::string &) const;
        std::shared_ptr<Vehicle> otherVehicleMiddle(const std::string &) const;
        bool pastPoint(int, int, int, int, int, double) const;
        double distance(int, int, int, int) const;
        double speedForPoint(int, int, int, int, int, int, int, int, const std::string&, const std::string&, double) const;
        void addToQueue(const std::string &);
        void removeFromQueue(const std::string &);
        std::shared_ptr<Vehicle> findVehicle(const std::string &) const;
        std::vector<std::string> othersInQueue(const std::string &) const;
        bool rightFree(int, int) const;
};


#endif //ACAV_INTERSECTIONCOORDINATOR_H