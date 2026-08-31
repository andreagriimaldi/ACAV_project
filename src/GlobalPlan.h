#ifndef ACAV_GLOBALPLAN_H
#define ACAV_GLOBALPLAN_H
#include <memory>
#include <vector>

#include "Map.h"
#include "Point.h"


class GlobalPlan {
    private:
        const Map& map;
        std::vector<std::shared_ptr<Point>> points;
        int type;
    public:
        explicit GlobalPlan(const Map &m, int t): map(m), type(t) {
            points.reserve(5);
            initialize();
        };
        int getType() const;
        void initialize();
        int countToVisit() const;
        const std::shared_ptr<Point>& nextPoint() const;
        const std::shared_ptr<Point>& nextNextPoint() const;
        const std::shared_ptr<Point>& lastPoint() const;
        void popCurrent();
        static int planToSpawn(int);
        static int planToEnd(int, int);
        static int spawnToRandomPlan(int);
        static std::pair<double, double> entryDir(int);
        static std::pair<double, double> exitDir(int);
};


#endif //ACAV_GLOBALPLAN_H