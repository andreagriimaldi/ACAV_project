#ifndef ACAV_GLOBALPLAN_H
#define ACAV_GLOBALPLAN_H
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

        void initialize();
        int countToVisit() const;
        const std::shared_ptr<Point>& nextPoint() const;
        const std::shared_ptr<Point>& nextNextPoint() const;
        void popCurrent();

};


#endif //ACAV_GLOBALPLAN_H