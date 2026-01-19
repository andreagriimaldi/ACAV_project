#include <iostream>
#include <thread>

#include "Map.h"
#include "SDLRenderer.h"

int main(int argc, char* argv[]) {

    //This will be passed as an argument
    constexpr int GUI_SIZE = 800;

    Map m = Map(1350);
    m.initialize();
    SDLRenderer renderer(GUI_SIZE);

    //m.generateVehicle(true, 3, m.getDim()/30, 0);
    m.generateVehicle(false, 1, m.getDim()/300, 2);
    //m.generateVehicle(false, 1, 0, 2);
    //m.generateVehicle(false, 2, 0, 3);

    m.FakeUpdate();

    std::cout << std::endl;
    for (int i = 0; i < m.getDim() + 1; i++) {
        for (int j = 0; j < m.getDim() + 1; j++) {
            if (m.getGrid()[j][i]->occupied()) {
                if (m.getGrid()[j][i]->getVehicle() == "ego") {
                    std::cout << "❎";
                }
                else {
                    std::cout << "❌";
                }
            }
            else {
                switch (m.getGrid()[j][i]->getType()) {
                    case Point_type::Empty:
                        std::cout << "- ";
                        break;
                    case Point_type::Boundary:
                        std::cout << "h ";
                        break;
                    case Point_type::Road:
                        std::cout << "r ";
                        break;
                }
            }
        }
        std::cout << std::endl;
    }

    //m.moveVehicles();

    /*
    std::cout << std::endl;
    for (int i = 0; i < m.getDim() + 1; i++) {
        for (int j = 0; j < m.getDim() + 1; j++) {
            if (m.getGrid()[j][i]->occupied()) {
                if (m.getGrid()[j][i]->getVehicle() == "ego") {
                    std::cout << "❎";
                }
                else {
                    std::cout << "❌";
                }
            }
            else {
                switch (m.getGrid()[j][i]->getType()) {
                    case Point_type::Empty:
                        std::cout << "- ";
                        break;
                    case Point_type::Boundary:
                        std::cout << "h ";
                        break;
                    case Point_type::Road:
                        std::cout << "r ";
                        break;
                }
            }
        }
        std::cout << std::endl;
    }
    */

    bool running = true;
    while (running) {
        m.moveVehicles();
        running = renderer.pollEvents();
        renderer.draw(m);
        SDL_Delay(16);  // ~60 FPS cap
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}