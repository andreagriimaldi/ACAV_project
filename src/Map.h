#ifndef ACAV_MAP_H
#define ACAV_MAP_H
#include <vector>
#include "Point.h"

using std::vector;

class Map {
private:
    vector<vector<Point>> grid;
    const int dim;
public:
    Map(int d): dim(d) {
        grid.resize(dim + 1);
        for (int i = 0; i < dim + 1; i++) {
            grid.at(i).resize(dim + 1, Point(Point_type::Empty));
        }
    };
    void initialize();
    bool crash() const;
};


#endif //ACAV_MAP_H