#ifndef ACAV_ADAPTIVECRUISECONTROL_H
#define ACAV_ADAPTIVECRUISECONTROL_H
#include <deque>
#include <string>
#include <vector>


class AdaptiveCruiseControl {
private:
    const int dim;
    const double tracking_dist; //TUNING PARAMETER
    std::string id;
    bool tracking = false;
    std::deque<std::pair<double, double>> measures;
    int measureSize = 0;
    const int maxMeasures = 5;
    double speed_before = 0;
public:
    explicit AdaptiveCruiseControl(int d, std::string idV): dim(d), tracking_dist(dim/5), id(idV){};
    bool checkVehicleInFront(const std::vector<std::vector<double>>&);
    bool emergencyBrake(const std::vector<std::vector<double>> &per) const;
    double update(double, const std::vector<std::vector<double>>&);
    double computeSpeed(double) const;
    bool isTracking() const;
};


#endif //ACAV_ADAPTIVECRUISECONTROL_H