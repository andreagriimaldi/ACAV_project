#include "Perception.h"

#include <iostream>

//getPerc.at(0) is the ego vehicle
std::vector<std::vector<double>> Perception::getPerc(int egoX, int egoY, int egoHeading) const {
    std::vector<std::pair<int, int>> COGs = m.getCOGs();
    std::vector<std::vector<double>> per;
    per.reserve(COGs.size());

    for (const auto& pair: COGs) {
        if (pair.first == egoX and pair.second == egoY) {
            per.push_back({computeState(pair.first, pair.second), 0, 0});
            break;
        }
    }

    for (const auto& pair: COGs) {
        if (pair.first != egoX or pair.second != egoY) {
            double dx = pair.first - egoX;
            double dy = egoY - pair.second;
            double theta = std::atan2(dy, dx) * 180.0 / M_PI;
            double difference = egoHeading - theta;
            while (difference > 180.0)  difference -= 360.0;
            while (difference < -180.0) difference += 360.0;

            double dist = std::sqrt((pair.first - egoX)*(pair.first - egoX) + (pair.second - egoY)*(pair.second - egoY));

            per.push_back({computeState(pair.first, pair.second), difference, dist});
        }
    }

    return per;
}

double Perception::computeState(int x, int y) const {
    int dim = m.getDim();
    if (x <= (2*dim)/9 or x >= (7*dim)/9 or y <= (2*dim)/9 or y >= (7*dim)/9) {
        return 0;
    }
    if ((x < (2*dim)/3 + dim/16 and x > dim/3 - dim/16) and (y < (2*dim)/3 + dim/16 and y > dim/3 - dim/16)) {
        return 2;
    }

    if (gplan == 0 or gplan == 1 or gplan == 8) {
        if (y < dim/3) {
            return 1;
        }
        return 3;
    }
    if (gplan == 2 or gplan == 3 or gplan == 9) {
        if (x > (2*dim)/3) {
            return 1;
        }
        return 3;
    }
    if (gplan == 4 or gplan == 5 or gplan == 10) {
        if (y > (2*dim)/3) {
            return 1;
        }
        return 3;
    }
    if (gplan == 6 or gplan == 7 or gplan == 11) {
        if (x < dim/3) {
            return 1;
        }
        return 3;
    }

    std::cerr << "GlobalPlan not initialized correctly" << std::endl;
    std::cerr << "GLOBAL PLAN: " << gplan << std::endl;
    return -1;
}
