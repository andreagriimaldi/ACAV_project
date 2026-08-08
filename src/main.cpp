#include "Simulator.h"

int main(int argc, char* argv[]) {

    //This will be passed as an argument
    constexpr int GUI_SIZE = 800;
    constexpr int map_size = 1350;

    Simulator sim(GUI_SIZE, map_size, false);

    return 0;
}
