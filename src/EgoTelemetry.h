#ifndef ACAV_EGOTELEMETRY_H
#define ACAV_EGOTELEMETRY_H


class EgoTelemetry {
private:
    const double maxspeed;
    double speed = 0.0;
    bool accTracking = false;
    double accSpeed = 0.0;
    double optimizerSpeed = 0.0;
    int speedChoice = 0; //0 for physics, 1 for ACC, 2 for optimizer
    bool emergencyBraking = false;
public:
    explicit EgoTelemetry(double s): maxspeed(s){};
    EgoTelemetry(): maxspeed(0.0){};
    void update(double, bool, double, double, int, bool);
    double getMaxSpeed() const;
    double getSpeed() const;
    bool getACCTracking() const;
    double getACCSpeed() const;
    double getOptimizerSpeed() const;
    int getSpeedChoice() const;
    bool getBraking() const;
};


#endif //ACAV_EGOTELEMETRY_H