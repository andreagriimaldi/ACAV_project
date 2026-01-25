#include "CPUVehicle.h"

#include <iostream>
#include <ostream>

void CPUVehicle::move() {
    //THE FINAL VERSION WILL DECIDE NEW COG AND HEADING AND WILL CALL COMPUTENEWPOSITION()
    //REMEMBER THAT THE NEW COG AND HEADING WILL BE AFFECTED BY CURRENT SPEED (NO ACC HERE, JUST COMPUTE THE SPEED)

    //HERE I NEED TO COMPUTE THE NEW SPEED
    double steer = computeSteering();
    updateBicycle(computeNewSpeed(steer));
}

double CPUVehicle::computeNewSpeed(double steer) const {
    double L = (map.getDim()/9) * 0.65;

    double a_lat_max = 0.05; //TUNING PARAMETER
    double a_str_max = 0.3; //TUNING PARAMETER

    double v_max = maxspeed;

    if (std::abs(steer) > 0.001) {
        double v_lat = std::sqrt(a_lat_max * L / std::abs(std::tan(steer)));
        v_max = std::min(v_max, v_lat);
    }

    if (v_max > speed + a_str_max) {
        v_max = speed + a_str_max;
    }
    else if (v_max < speed - a_str_max) {
        v_max = speed - a_str_max;
    }

    return v_max;
}
