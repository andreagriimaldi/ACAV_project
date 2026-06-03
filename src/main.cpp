#include <iostream>
#include <random>
#include <thread>

#include "Map.h"
#include "SDLRenderer.h"

int main(int argc, char* argv[]) {

    //This will be passed as an argument
    constexpr int GUI_SIZE = 800;

    Map m = Map(1350);
    m.initialize();
    SDLRenderer renderer(GUI_SIZE);

    //TEST START
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(180, 240);

    //m.getDim()/(dist(gen))

    m.generateVehicle(false, 0, 0, 8,  m.getDim()/(dist(gen)));   // N straight
    m.generateVehicle(false, 1, 0, 9,  m.getDim()/(dist(gen)));   // E straight
    m.generateVehicle(false, 2, 0, 5, m.getDim()/(dist(gen)));   // S straight
    m.generateVehicle(false, 3, 0, 7, m.getDim()/(dist(gen)));   // W straight

    //TEST END

    m.FakeUpdate();

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
        if (m.crash()) {
            std::cerr << "A crash has occured" << std::endl;
            break;
        }
        running = renderer.pollEvents();
        renderer.draw(m);
        SDL_Delay(16);  // ~60 FPS cap
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}