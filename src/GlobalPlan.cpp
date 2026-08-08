#include "GlobalPlan.h"

#include <iostream>

int GlobalPlan::getType() const {
    return type;
}

void GlobalPlan::initialize() {
    //0 = N short, 1 = N long, 2 = E short, 3 = E long, 4 = S short, 5 = S long, 6 = W short, 7 = W long, 8 = N straight, 9 = E straight
    //10 = S straight, 11 = W straight
    const vector<vector<std::shared_ptr<Point>>> grid = map.getGrid();
    int dim = map.getDim();
    switch (type) {
        case 0: {
            points.push_back(grid.at(dim/3 + dim/10).at(dim/3 - dim/30));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3))).at(static_cast<int>(std::round(dim/3 + dim/15))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 - dim/7))).at(static_cast<int>(std::round(dim/3 + dim/9 - dim/28))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 - dim/5))).at(static_cast<int>(std::round(dim/3 + dim/9 - dim/29))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/30))).at(static_cast<int>(std::round(dim/3 + dim/9 - dim/29))));
            break;
        }
        case 1: {
            points.push_back(grid.at(dim/3 + dim/10).at(dim/3 + dim/9));
            points.push_back(grid.at(static_cast<int>(std::round(dim/2))).at(static_cast<int>(std::round(dim/3 + dim/4.5 - dim/30))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3))).at(static_cast<int>(std::round((2*dim)/3 - dim/15 - dim/30))));
            points.push_back(grid.at(static_cast<int>(std::round((2*dim)/3 + dim/20))).at(static_cast<int>(std::round((2*dim)/3 - dim/14.2 - dim/30))));
            points.push_back(grid.at(static_cast<int>(std::round(dim - dim/50))).at(static_cast<int>(std::round((2*dim)/3 - dim/14.2 - dim/30))));
            break;
        }
        case 2: {
            points.push_back(grid.at((2*dim)/3 + dim/30).at(dim/3 + dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/15))).at(static_cast<int>(std::round(dim/3))));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/9 + dim/28))).at(static_cast<int>(std::round(dim/3 - dim/7))));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/9 + dim/29))).at(static_cast<int>(std::round(dim/3 - dim/5))));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/9 + dim/30))).at(static_cast<int>(std::round(dim/30))));
            break;
        }
        case 3: {
            points.push_back(grid.at((2*dim)/3 - dim/9).at(dim/3 + dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/4.5 + dim/25))).at(static_cast<int>(std::round(dim/2))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/15 + dim/30))).at(static_cast<int>(std::round(2*dim/3))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/14.2 + dim/30))).at(static_cast<int>(std::round(2*dim/3 + dim/20))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/14.2 + dim/30))).at(static_cast<int>(std::round(dim - dim/50))));
            break;
        }
        case 4: {
            points.push_back(grid.at((2*dim)/3 - dim/10).at((2*dim)/3 + dim/30));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3))).at(static_cast<int>(std::round(2*dim/3 - dim/15))));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 + dim/7))).at(static_cast<int>(std::round(2*dim/3 - dim/9 + dim/28))));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 + dim/5))).at(static_cast<int>(std::round(2*dim/3 - dim/9 + dim/29))));
            points.push_back(grid.at(static_cast<int>(std::round(dim - dim/30))).at(static_cast<int>(std::round(2*dim/3 - dim/9 + dim/29))));
            break;
        }
        case 5: {
            points.push_back(grid.at((2*dim)/3 - dim/10).at((2*dim)/3 - dim/9));
            points.push_back(grid.at(static_cast<int>(std::round(dim/2))).at(static_cast<int>(std::round(2*dim/3 - dim/4.5 + dim/30))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3))).at(static_cast<int>(std::round(dim/3 + dim/15 + dim/30))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 - dim/20))).at(static_cast<int>(std::round(dim/3 + dim/14.2 + dim/30))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/50))).at(static_cast<int>(std::round(dim/3 + dim/14.2 + dim/30))));
            break;
        }
        case 6: {
            points.push_back(grid.at(dim/3 - dim/30).at((2*dim)/3 - dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/15))).at(static_cast<int>(std::round(2*dim/3))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/9 - dim/28))).at(static_cast<int>(std::round(2*dim/3 + dim/7))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/9 - dim/29))).at(static_cast<int>(std::round(2*dim/3 + dim/5))));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/9 - dim/29))).at(static_cast<int>(std::round(dim - dim/30))));
            break;
        }
        case 7: {
            points.push_back(grid.at(dim/3 + dim/9).at((2*dim)/3 - dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(dim/3 + dim/4.5 - dim/30))).at(static_cast<int>(std::round(dim/2))));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/15 - dim/30))).at(static_cast<int>(std::round(dim/3))));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/14.2 - dim/30))).at(static_cast<int>(std::round(dim/3 - dim/20))));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/14.2 - dim/30))).at(static_cast<int>(std::round(dim/50))));
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
            points.push_back(grid.at((2*dim)/3 - dim/10).at((2*dim)/3 - dim/30));
            points.push_back(grid.at(static_cast<int>(std::round(2*dim/3 - dim/10))).at(static_cast<int>(std::round(dim/10))));
            break;
        }
        case 11: {
            points.push_back(grid.at(dim/3 + dim/30).at((2*dim)/3 - dim/10));
            points.push_back(grid.at(static_cast<int>(std::round(dim - dim/10))).at(static_cast<int>(std::round(2*dim/3 - dim/10))));
            break;
        }
        default: std::cerr << "GlobalPlan not defined" << std::endl;
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

const std::shared_ptr<Point> & GlobalPlan::lastPoint() const {
    if (!points.empty()) {
        return points.back();
    }
    std::cerr << "There are no points left in the Global Plan" << std::endl;
    return nullptr;
}

void GlobalPlan::popCurrent() {
    if (!points.empty()) {
        points.erase(points.begin());
    }
}

int GlobalPlan::planToSpawn(int gplan){
    if (gplan == 0 or gplan == 1 or gplan == 8) {
        return 0;
    }
    if (gplan == 2 or gplan == 3 or gplan == 9) {
        return 1;
    }
    if (gplan == 4 or gplan == 5 or gplan == 10) {
        return 2;
    }
    if (gplan == 6 or gplan == 7 or gplan == 11) {
        return 3;
    }
    std::cerr << "gplan must be between 0 and 11" << std::endl;
    return -1;
}

//returns just the right coordinate for the calculation
int GlobalPlan::planToEnd(int gplan, int mapDim){
    if (gplan == 0 or gplan == 5 or gplan == 9) {
        return 0; //x axis
    }
    if (gplan == 7 or gplan == 2 or gplan == 10) {
        return 0; //y axis
    }
    if (gplan == 1 or gplan == 4 or gplan == 11) {
        return mapDim; // x axis
    }
    if (gplan == 3 or gplan == 6 or gplan == 8) {
        return mapDim; // y axis
    }
    std::cerr << "gplan must be a number between 0 and 11" << std::endl;
    return -1;
}
