#include "CPUVehicle.h"

void CPUVehicle::move() {
    //TEMP
    //updatedPosition.push_back(surface.at(0));

    //THE FINAL VERSION WILL DECIDE NEW COG AND HEADING AND WILL CALL COMPUTENEWPOSITION()
    //REMEMBER THAT THE NEW COG AND HEADING WILL BE AFFECTED BY CURRENT SPEED (NO ACC HERE, JUST COMPUTE THE SPEED)

    //deciding new speed
    updateBicycle(map.getDim()/500, computeSteering());
}
