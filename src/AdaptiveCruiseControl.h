//
// Created by Andrea Grimaldi on 17/08/2026.
//

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
    const int maxMeasures = 20;
public:
    explicit AdaptiveCruiseControl(int d, std::string idV): dim(d), tracking_dist(dim/5), id(idV){};
    bool checkVehicleInFront(const std::vector<std::vector<double>>&);
    double update(double, const std::vector<std::vector<double>>&);
};


#endif //ACAV_ADAPTIVECRUISECONTROL_H