#include "Perception.h"

#include <iostream>

#include "Vehicle.h"

//getPerc.at(0) is the ego vehicle
std::vector<std::vector<double>> Perception::getPerc(int egoX, int egoY, int egoHeading) const {
    const std::vector<std::shared_ptr<Vehicle>>& vs = m.getVehicles();
    std::vector<std::vector<double>> per;
    per.reserve(vs.size());

    for (const auto& v: vs) {
        if (v->getCOGx() == egoX and v->getCOGy() == egoY) {
            per.push_back({computeState(egoX, egoY, gplan, m.getDim()), 0, 0});
            break;
        }
    }

    for (const auto& v: vs) {
        const int vx = v->getCOGx(), vy = v->getCOGy();
        if (!(vx == egoX and vy == egoY)) {
            double dx = vx - egoX;
            double dy = egoY - vy;
            double theta = std::atan2(dy, dx) * 180.0 / M_PI;
            double difference = egoHeading - theta;
            while (difference > 180.0)  difference -= 360.0;
            while (difference < -180.0) difference += 360.0;

            double dist = std::sqrt(dx*dx + dy*dy);
            double state = computeState(vx, vy, v->getGlobalPlan(), m.getDim());

            per.push_back({state, difference, dist});
        }
    }

    return per;
}

double Perception::computeState(int x, int y, int gplan, int dim) {
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

std::vector<std::vector<double>> Perception::getFuturePerc(int egoX, int egoY, int egoHeading, const std::unordered_map<std::string, std::pair<int, int>> & future) const {
    std::vector<std::vector<double>> futurePer;
    futurePer.reserve(future.size());

    for (const auto& entry: future) {
        if (entry.first == "ego") {
            futurePer.push_back({computeState(egoX, egoY, gplan, m.getDim()), 0, 0}); //these egoX and egoY are obtained from mp in EgoVehicle
            break;
        }
    }

    for (const auto& entry: future) {
        if (entry.first != "ego") {
            int gp = -1;
            for (const auto& v: m.getVehicles()) {
                if (v->getID() == entry.first) {
                    gp = v->getGlobalPlan();
                    break;
                }
            }

            double dx = entry.second.first - egoX;
            double dy = egoY - entry.second.second;
            double theta = std::atan2(dy, dx) * 180.0 / M_PI;
            double difference = egoHeading - theta;
            while (difference > 180.0)  difference -= 360.0;
            while (difference < -180.0) difference += 360.0;

            double dist  = std::sqrt(dx*dx + dy*dy);
            double state = computeState(entry.second.first, entry.second.second, gp, m.getDim());

            futurePer.push_back({state, difference, dist});
        }
    }

    return futurePer;
}
