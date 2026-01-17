#include "GlobalPlan.h"

#include <iostream>

void GlobalPlan::initialize() {
    //0 = N short, 1 = N long, 2 = E short, 3 = E long, 4 = S short, 5 = S long, 6 = W short, 7 = W long, 8 = N straight, 9 = E straight
    //10 = S straight, 11 = W straight
    const vector<vector<std::shared_ptr<Point>>> grid = map.getGrid();
    int dim = map.getDim();
    switch (type) {
        case 0: {
            points.push_back(grid.at(dim/3 + dim/10).at(dim/3 + dim/30));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/20))).at(static_cast<int>(std::round(dim/3 + dim/5))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 - dim/20))).at(static_cast<int>(std::round(dim/3 + dim/5 + dim/10))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/10))).at(static_cast<int>(std::round(dim/3 + dim/5))));
            break;
        }
        case 1: {
            points.push_back(grid.at(dim/3 + dim/10).at(dim/3 + dim/9));
            points.push_back(grid.at(static_cast<int>(std::round(dim/2))).at(static_cast<int>(std::round(dim/2))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 - dim/10))).at(static_cast<int>(std::round((2*dim)/3 - dim/9))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 + dim/9))).at(static_cast<int>(std::round((2*dim)/3 - dim/9 + dim/30))));
            points.push_back(grid.at(static_cast<int>(std::round(dim - dim/10))).at(static_cast<int>(std::round((2*dim)/3 - dim/9))));
            break;
        }
        case 2: {
            points.push_back(grid.at((2*dim)/3 - dim/30).at(dim/3 + dim/10));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 - dim/10))).at(static_cast<int>(std::round(dim/3 + dim/30))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 - dim/15))).at(static_cast<int>(std::round(dim/6))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 - dim/10))).at(static_cast<int>(std::round(dim/10))));
            break;
        }
        case 3: {
            points.push_back(grid.at((2*dim)/3 - dim/9).at(dim/3 + dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(dim/2))).at(static_cast<int>(std::round(dim/3 + dim/9 + dim/30))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/10))).at(static_cast<int>(std::round((2*dim)/3 - dim/10))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/10 - dim/30))).at(static_cast<int>(std::round((2*dim)/3 + dim/10))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/10))).at(static_cast<int>(std::round(dim - dim/10))));
            break;
        }
        case 4: {
            points.push_back(grid.at(2*dim/3 - dim/10).at((2*dim)/3 - dim/30));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 + dim/10))).at(static_cast<int>(std::round((2*dim)/3 - dim/15))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 + dim/6))).at(static_cast<int>(std::round((2*dim)/3 - dim/10))));
            points.push_back(grid.at(static_cast<int>(std::round(dim - dim/10))).at(static_cast<int>(std::round((2*dim)/3 - dim/10))));
            break;
        }
        case 5: {
            points.push_back(grid.at(2*dim/3 - dim/10).at((2*dim)/3 - dim/9));
            points.push_back(grid.at(static_cast<int>(std::round(dim/2 + dim/30))).at(static_cast<int>(std::round(dim/2))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/30))).at(static_cast<int>(std::round(dim/3 + dim/15))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 - dim/10 - dim/30))).at(static_cast<int>(std::round(dim/3 + dim/25))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/10))).at(static_cast<int>(std::round(dim/3 + dim/20))));
            break;
        }
        case 6: {
            points.push_back(grid.at(dim/3 + dim/30).at(2*dim/3 - dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/8))).at(static_cast<int>(std::round((2*dim)/3 - dim/10))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/9))).at(static_cast<int>(std::round((2*dim)/3 + dim/9))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/9))).at(static_cast<int>(std::round(dim - dim/10))));
            break;
        }
        case 7: {
            points.push_back(grid.at(dim/3 + dim/9).at(2*dim/3 - dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(dim/2 + dim/10))).at(static_cast<int>(std::round(dim/2))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 - dim/10))).at(static_cast<int>(std::round(dim/3 + dim/10))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 - dim/12))).at(static_cast<int>(std::round(dim/3 - dim/8))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 - dim/12))).at(static_cast<int>(std::round(dim/10))));
            break;
        }
        case 8: {
            points.push_back(grid.at(dim/3 + dim/10).at(dim/3 + dim/30));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/10))).at(static_cast<int>(std::round(dim - dim/10))));
            break;
        }
        case 9: {
            points.push_back(grid.at((2*dim)/3 - dim/30).at(dim/3 + dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(dim/10))).at(static_cast<int>(std::round(dim/3 + dim/10))));
            break;
        }
        case 10: {
            points.push_back(grid.at(2*dim/3 - dim/10).at((2*dim)/3 - dim/30));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/10))).at(static_cast<int>(std::round(dim/10))));
            break;
        }
        case 11: {
            points.push_back(grid.at(dim/3 + dim/30).at(2*dim/3 - dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(dim - dim/10))).at(static_cast<int>(std::round(2*dim/3 - dim/10))));
            break;
        }
    }
}

int GlobalPlan::countToVisit() const{
    return points.size();
}

const std::shared_ptr<Point>& GlobalPlan::nextPoint() const {
    if (!points.empty()) {
        return points.front();
    }
    std::cerr << "There are no points left in the Global Plan" << std::endl;
    return nullptr;
}

void GlobalPlan::popCurrent() {
    if (!points.empty()) {
        points.erase(points.begin());
    }
}
