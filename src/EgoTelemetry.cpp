#include "EgoTelemetry.h"

void EgoTelemetry::update(double s, bool acc, double sACC, double optS, int choice, bool emBr) {
    speed = s;
    accTracking = acc;
    accSpeed = sACC;
    optimizerSpeed = optS;
    speedChoice = choice;
    emergencyBraking = emBr;
}

double EgoTelemetry::getMaxSpeed() const {
    return maxspeed;
}

double EgoTelemetry::getSpeed() const {
    return speed;
}

bool EgoTelemetry::getACCTracking() const {
    return accTracking;
}

double EgoTelemetry::getACCSpeed() const {
    return accSpeed;
}

double EgoTelemetry::getOptimizerSpeed() const {
    return optimizerSpeed;
}

int EgoTelemetry::getSpeedChoice() const {
    return speedChoice;
}

bool EgoTelemetry::getBraking() const {
    return emergencyBraking;
}


