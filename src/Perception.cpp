#include "Perception.h"

std::vector<std::vector<int>> Perception::getPerc(int egoX, int egoY, int egoHeading) const {
    std::vector<std::pair<int, int>> COGs = m.getCOGs();
    std::vector<std::vector<int>> per;
    per.reserve(COGs.size());

    for (const auto& pair: COGs) {
        if (pair.first == egoX and pair.second == egoY) {
            per.push_back({computeState(pair.first, pair.second), 0, 0});
        }
    }

    for (const auto& pair: COGs) {
        if (pair.first != egoX or pair.second != egoY) {
            double dx = pair.first - egoX;
            double dy = pair.second - egoY;
            double theta = std::atan2(dy, dx) * 180.0 / M_PI;
            double difference = egoHeading - theta;
            while (difference > 180.0)  difference -= 360.0;
            while (difference < -180.0) difference += 360.0;

            int diff = static_cast<int>(difference);
            int dist = static_cast<int>(std::sqrt((pair.first - egoX)*(pair.first - egoX) + (pair.second - egoY)*(pair.second - egoY)));

            per.push_back({computeState(pair.first, pair.second), diff, dist});
        }
    }

    return per;
}

int Perception::computeState(int x, int y) const {
    int dim = m.getDim();
    if (x <= (2*dim)/9 or x >= (7*dim)/9 or y <= (2*dim)/9 or y >= (7*dim)/9) {
        return 0;
    }
    if ((x < (2*dim)/3 - dim/15 and x > dim/3 + dim/15) and (y < (2*dim)/3 - dim/15 and y > dim/3 + dim/15)) {
        return 2;
    }
    return 1;
}
