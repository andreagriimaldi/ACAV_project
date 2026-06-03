#include "IntersectionCoordinator.h"

#include <iostream>

#include "Vehicle.h"

bool IntersectionCoordinator::askPermission(const std::string& id) {

    for (string& s: currentlyGranted) {
        if (s == id) {
            return true;
        }
    }

    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    for (const auto& v: vehicles) {
        if (v->getID() == "ego") {
            if (v->getPercState() == 1 && !currents.empty()) {
                return false;
            }
        }
    }

    if (currents.size() + currentlyGranted.size() > 1) {
        return false;
    }

    currentlyGranted.push_back(id);

    return true;
}

//This directly returns the speed for CPUVehicle::avoidObstacles()
double IntersectionCoordinator::suggestedSpeed(const std::string& id, double speed) const {
    if (!inTheMiddle(id)) {
        std::cerr << "suggestedSpeed() should be called by vehicles in the middle" << std::endl;
        return speed;
    }

    if (currents.size() < 2) {
        return speed;
    }

    //Let's find which vehicles are in the middle and save them in a shared pointer
    std::shared_ptr<Vehicle> vehicle1 = idVehicleMiddle(id); //CURRENT
    std::shared_ptr<Vehicle> vehicle2 = otherVehicleMiddle(id); //OTHER

    int glob1 = vehicle1->getGlobalPlan();
    int glob2 = vehicle2->getGlobalPlan();

    int x1 = oldCOGs.at(vehicle1->getID()).x;
    int y1 = oldCOGs.at(vehicle1->getID()).y;
    int h1 = oldCOGs.at(vehicle1->getID()).heading;

    int x2 = oldCOGs.at(vehicle2->getID()).x;
    int y2 = oldCOGs.at(vehicle2->getID()).y;
    int h2 = oldCOGs.at(vehicle2->getID()).heading;

    std::vector<int> collision = pathCollisionFinder(glob1, glob2);

    if (collision.at(0) == 0) {
        return speed;
    }

    if (collision.at(3) == 0) {
        //This is the case where the paths intersect in just a point
        return speedForPoint(x1, y1, h1, x2, y2, h2, collision.at(1), collision.at(2), id, vehicle2->getID(), speed);
    }
    else {
        //This is the case where the paths intersect in two points (true just for GlobalPlans 1-5 and 3-7)
        int centerX = (collision.at(1) + collision.at(3))/2;
        int centerY = (collision.at(2) + collision.at(4))/2;
        double dist1 = distance(x1, y1, centerX, centerY);
        double dist2 = distance(x2, y2, centerX, centerY);

        if (dist1 < dist2 or (dist1 == dist2 and id < vehicle2->getID())) {
            //Vehicle1 is closer to the center and so it goes on
            return speed;
        }
        else {
            if (!pastPoint(x1, y1, h1, centerX, centerY, 0.0)) {
                //Vehicle1 is not past the center
                if (pastPoint(x2, y2, h2, centerX, centerY, 0.0)) {
                    //Vehicle2 is past the center, but Vehicle1 is not
                    double gap = distance(x1, y1, x2, y2);
                    if (!pastPoint(x1, y1, h1, x2, y2, m.getDim()/15.0)) {
                        double bodyLen = m.getDim()/9.0;
                        if (gap < bodyLen + m.getDim()/30.0) {
                            return speed/3;
                        }
                        if (gap < m.getDim()/5.0) {
                            return speed/1.2;
                        }
                    }
                    return speed;
                }
                else {
                    //Vehicle2 is not past the center
                    double dToCenter = distance(x1, y1, centerX, centerY);
                    double bodyLen  = m.getDim()/9.0;
                    double stopBand = bodyLen + m.getDim()/12.0;
                    double slowBand = 2*stopBand;

                    if (dToCenter < stopBand) {
                        return speed / 4; //TO CHANGE MAYBE
                    }
                    if (dToCenter < slowBand) {
                        return speed / 1.5;
                    }
                    return speed;
                }
            }
            else {
                return speed;
            }
        }
    }

}

// res[0] = 0 if NO collision, 1 if collision, res[1] = x of collision 1, res[2] = y of collision 1, res[3] = x of collision 2, res[4] = y of collision 2
std::vector<int> IntersectionCoordinator::pathCollisionFinder(int glob1, int glob2) const {
    std::vector<int> res(5);

    int DIM = m.getDim();

    static const std::vector<std::vector<std::array<double, 5>>> collisionMatrix = {
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.350623*DIM, 0.404933*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.431720*DIM, 0.334946*DIM, 0, 0}, {1, 0.366129*DIM, 0.400538*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.484735*DIM, 0.499788*DIM, 0, 0}, {1, 0.644737*DIM, 0.577485*DIM, 0, 0}, {1, 0.463056*DIM, 0.467926*DIM, 0.536944*DIM, 0.532074*DIM}, {0, 0, 0, 0, 0}, {1, 0.496697*DIM, 0.517368*DIM, 0, 0}, {1, 0.433333*DIM, 0.424242*DIM, 0, 0}, {1, 0.439519*DIM, 0.433333*DIM, 0, 0}, {1, 0.566667*DIM, 0.540000*DIM, 0, 0}, {1, 0.486923*DIM, 0.535023*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.595067*DIM, 0.350623*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.665054*DIM, 0.431720*DIM, 0, 0}, {1, 0.599462*DIM, 0.366129*DIM, 0, 0}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.484735*DIM, 0.499788*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.504939*DIM, 0.485037*DIM, 0, 0}, {1, 0.423219*DIM, 0.645299*DIM, 0, 0}, {1, 0.531355*DIM, 0.465751*DIM, 0.474483*DIM, 0.532482*DIM}, {1, 0.466194*DIM, 0.490222*DIM, 0, 0}, {1, 0.575758*DIM, 0.433333*DIM, 0, 0}, {1, 0.566667*DIM, 0.439971*DIM, 0, 0}, {1, 0.464000*DIM, 0.566667*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.649377*DIM, 0.595067*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.568280*DIM, 0.665054*DIM, 0, 0}, {1, 0.633871*DIM, 0.599462*DIM, 0, 0}},
    {{1, 0.355263*DIM, 0.422515*DIM, 0, 0}, {1, 0.536944*DIM, 0.532074*DIM, 0.463056*DIM, 0.467926*DIM}, {0, 0, 0, 0, 0}, {1, 0.504939*DIM, 0.485037*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.517368*DIM, 0.503303*DIM, 0, 0}, {1, 0.433333*DIM, 0.460000*DIM, 0, 0}, {1, 0.513077*DIM, 0.464977*DIM, 0, 0}, {1, 0.566667*DIM, 0.575758*DIM, 0, 0}, {1, 0.560481*DIM, 0.566667*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.405074*DIM, 0.649490*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.400538*DIM, 0.633871*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.334946*DIM, 0.568280*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.496697*DIM, 0.517368*DIM, 0, 0}, {1, 0.577485*DIM, 0.355263*DIM, 0, 0}, {1, 0.474483*DIM, 0.532482*DIM, 0.531355*DIM, 0.465751*DIM}, {0, 0, 0, 0, 0}, {1, 0.517368*DIM, 0.503303*DIM, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.433333*DIM, 0.560481*DIM, 0, 0}, {1, 0.540000*DIM, 0.433333*DIM, 0, 0}, {1, 0.535023*DIM, 0.513077*DIM, 0, 0}, {1, 0.424242*DIM, 0.566667*DIM, 0, 0}},
    {{1, 0.417204*DIM, 0.349462*DIM, 0, 0}, {1, 0.433333*DIM, 0.424242*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.458889*DIM, 0.480000*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.433333*DIM, 0.460000*DIM, 0, 0}, {1, 0.410215*DIM, 0.630215*DIM, 0, 0}, {1, 0.433333*DIM, 0.560481*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.433333*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.433333*DIM, 0.566667*DIM, 0, 0}},
    {{1, 0.369785*DIM, 0.410215*DIM, 0, 0}, {1, 0.439519*DIM, 0.433333*DIM, 0, 0}, {1, 0.650538*DIM, 0.417204*DIM, 0, 0}, {1, 0.575758*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.524444*DIM, 0.455556*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.540000*DIM, 0.433333*DIM, 0, 0}, {1, 0.433333*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.540000*DIM, 0, 0}, {1, 0.589785*DIM, 0.369785*DIM, 0, 0}, {1, 0.566667*DIM, 0.439971*DIM, 0, 0}, {1, 0.582796*DIM, 0.650538*DIM, 0, 0}, {1, 0.566667*DIM, 0.575758*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.544444*DIM, 0.524444*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.433333*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.566667*DIM, 0, 0}},
    {{0, 0, 0, 0, 0}, {1, 0.475556*DIM, 0.544444*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.464000*DIM, 0.566667*DIM, 0, 0}, {1, 0.630215*DIM, 0.589785*DIM, 0, 0}, {1, 0.560481*DIM, 0.566667*DIM, 0, 0}, {1, 0.349462*DIM, 0.582796*DIM, 0, 0}, {1, 0.424242*DIM, 0.566667*DIM, 0, 0}, {1, 0.433333*DIM, 0.566667*DIM, 0, 0}, {0, 0, 0, 0, 0}, {1, 0.566667*DIM, 0.566667*DIM, 0, 0}, {0, 0, 0, 0, 0}},
    };

    const auto& c = collisionMatrix[glob1][glob2];

    res[0] = static_cast<int>(std::lround(c[0]));
    for (int i = 1; i < 5; ++i) {
        res[i] = static_cast<int>(std::lround(c[i]));
    }

    return res;
}

void IntersectionCoordinator::updateStatus() {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    currents.clear();
    oldCOGs.clear();
    for (const auto& v: vehicles) {
        oldCOGs[v->getID()] = {v->getCOGx(), v->getCOGy(), v->getHeading()};
        if (v->getPercState() == 2) {
            currents.push_back(v->getID());
            auto it = std::find(currentlyGranted.begin(), currentlyGranted.end(), v->getID());
            if (it != currentlyGranted.end()) {
                currentlyGranted.erase(it);
            }
        }
    }
}


//MAYBE USELESS
bool IntersectionCoordinator::isEgoInTheMiddle() const {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    for (const auto& v: vehicles) {
        if (v->getID() == "ego") {
            if (v->getPercState() == 2) {
                return true;
            }
        }
    }
    return false;
}

bool IntersectionCoordinator::inTheMiddle(const std::string &id) const {
    for (const auto&s: currents) {
        if (s == id) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<Vehicle> IntersectionCoordinator::idVehicleMiddle(const std::string &id) const {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    for (auto& v: vehicles) {
        if (v->getID() == id && v->getPercState() == 2) {
            return v;
        }
    }
    std::cerr << "A vehicle with this id is not in the middle" << std::endl;
    return {nullptr};
}

std::shared_ptr<Vehicle> IntersectionCoordinator::otherVehicleMiddle(const std::string &id) const {
    std::vector<std::shared_ptr<Vehicle>> vehicles = m.getVehicles();
    for (auto& v: vehicles) {
        if (v->getID() != id && v->getPercState() == 2) {
            return v;
        }
    }
    std::cerr << "There is not another vehicle in the middle" << std::endl;
    return {nullptr};
}

bool IntersectionCoordinator::pastPoint(int vx, int vy, int heading, int px, int py, double toll) const {
    double dx = px - vx;
    double dy = py - vy;

    double hx = std::cos(heading * M_PI / 180.0);
    double hy = -std::sin(heading * M_PI / 180.0);

    return (dx * hx + dy * hy) < toll;
}

double IntersectionCoordinator::distance(int x1, int y1, int x2, int y2) const {
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double IntersectionCoordinator::speedForPoint(int x1, int y1, int h1, int x2, int y2, int h2, int cx, int cy, const std::string& id, const std::string& id2, double speed) const {
    if (pastPoint(x1, y1, h1, cx, cy, 0)) {
        //Vehicle1 already past the crash point
        return speed;
    }
    else {
        if (pastPoint(x2, y2, h2, cx, cy, 0)) {
            //Vehicle2 already past the point (Vehicle1 goes but carefully)
            double gap = distance(x1, y1, x2, y2);
            if (!pastPoint(x1, y1, h1, x2, y2, m.getDim()/15.0)) { //toll is a tuning parameter
                double bodyLen = m.getDim()/9.0;
                if (gap < bodyLen + m.getDim()/30.0) {
                    return speed/3;
                }
                if (gap < m.getDim()/5.0) {
                    return speed/1.2;
                }
            }
            return speed;
        }
        else {
            //Both vehicles still have to cross the point (the closest one goes)
            double d1 = distance(x1, y1, cx, cy);
            double d2 = distance(x2, y2, cx, cy);
            if (d1 < d2 or (d1 == d2 && id < id2)) {
                return speed; //Vehicle1 is closer to the point so it's the one who goes
            }
            else {
                //Vehicle1 is not the closest to the crash point
                double dToCenter = distance(x1, y1, cx, cy);
                double bodyLen  = m.getDim()/9.0;
                double stopBand = bodyLen + m.getDim()/12.0;
                double slowBand = 2*stopBand;

                if (dToCenter < stopBand) {
                    return speed / 4; //TO CHANGE MAYBE
                }
                if (dToCenter < slowBand) {
                    return speed / 1.5;
                }
                return speed;
            }
        }
    }
}