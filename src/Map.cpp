#include "Map.h"

void Map::initialize() {

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            bool west = (i > 0 and i < dim/3) and (j > dim/3 and j < (2/3)*dim);
            bool center = (i > dim/3 and i < (2/3)*dim);
            bool right = (i > (2/3)*dim and i < dim) and (j > dim/3 and j < (2/3)*dim);

            if (west or center or right) {
                grid.at(i).at(j) = Point(Point_type::Road);
            }

            //BOUNDARY POINTS
        }
    }

    //TO DO LATER: MANAGE POINT_TYPE INTERSECTION
}
