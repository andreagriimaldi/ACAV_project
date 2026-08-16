#include "MotionPrediction.h"

#include <algorithm>
#include <complex>

#include "Perception.h"
#include "Vehicle.h"

//must be called when ego vehicle is approaching or is in the middle
void MotionPrediction::update() {
    if (relTime % freq == 0) {
        updateVehicles();
        appendTimeInstants();
        computeMotionPrediction();
    }
    relTime++;
}

void MotionPrediction::updateVehicles() {
    for (const auto& v: vehicles) {
        if (v->getID() != id) {
            double state = Perception::computeState(v->getCOGx(), v->getCOGy(), v->getGlobalPlan(), m.getDim());
            if ((state == 1 or state == 2) and vehiclesSpeed.count(v->getID()) == 0) {
                vehiclesSpeed[v->getID()];
            }
            if (!(state == 1 or state == 2)) {
                vehiclesSpeed.erase(v->getID());
            }
        }
    }
}

void MotionPrediction::appendTimeInstants() {
    for (const auto& v : vehicles) {
        auto it = vehiclesSpeed.find(v->getID());
        if (!(it == vehiclesSpeed.end())) {
            it->second.emplace_front(v->getSpeed());
            if (it->second.size() > measures) {
                it->second.pop_back();
            }
        }
    }
}

void MotionPrediction::computeMotionPrediction() {
    std::unordered_map<std::string, std::pair<int, int>> mpred;

    for (const auto& v: vehicles) {
        auto it = vehiclesSpeed.find(v->getID());
        if (it != vehiclesSpeed.end()) {
            int COGx = v->getCOGx(), COGy = v->getCOGy();
            std::deque<double> hist = it->second;

            double num = 0, den = 0, w = 1.0, lambda = 0.6;
            for (size_t i = 0; i + 1 < hist.size(); i++) {
                double a_i = (hist[i] - hist[i+1]) / freq;
                num += w * a_i;
                den += w;
                w *= lambda;
            }
            double a = (den > 0) ? num / den : 0.0;

            a = std::clamp(a, -0.3, 0.3);

            double x = COGx, y = COGy;
            auto [wp0, wp1] = v->twoNextPoints();
            double wayX = wp0.first,  wayY = wp0.second;
            double nextX = wp1.first, nextY = wp1.second;
            bool onSecond = false;
            bool justOnePoint = (nextX == -1);

            double speed = v->getSpeed();
            double vmax = v->getMaxSpeed();

            for (int k = 1; k <= timespan; k++) {
                speed = std::clamp(speed + a, 0.0, vmax);

                if (!onSecond && !justOnePoint) {
                    double sx = nextX - wayX, sy = nextY - wayY;
                    if ((x - wayX) * sx + (y - wayY) * sy > 0.0) {
                        onSecond = true;
                        wayX = nextX;
                        wayY = nextY;
                    }
                }

                double dx = wayX - x, dy = wayY - y;
                double d = std::hypot(dx, dy);
                if (d > 1e-9) {
                    dx /= d;
                    dy /= d;
                }
                x += dx * speed;
                y += dy * speed;
            }

            mpred[v->getID()] = {std::round(x), std::round(y)};
        }
    }

    actualMP = mpred;
}

//This is the method used by EgoVehicle
const std::unordered_map<std::string, std::pair<int, int>>& MotionPrediction::getMotionPrediction() const {
    return actualMP;
}

double MotionPrediction::dist(double x1, double y1, double x2, double y2) const {
    return std::sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

