#ifndef ACAV_GLOBALPLAN_H
#define ACAV_GLOBALPLAN_H
#include <vector>

#include "Map.h"
#include "Point.h"


class GlobalPlan {
    private:
        Map map;
        std::vector<std::shared_ptr<Point>> points;
        int type;
    public:
        explicit GlobalPlan(Map m, int t): map(m), type(t) {
            points.resize(20);
            initialize();
        };

        void initialize();
        int countToVisit();
        std::shared_ptr<Point> nextPoint();
        void popCurrent();

};


#endif //ACAV_GLOBALPLAN_H