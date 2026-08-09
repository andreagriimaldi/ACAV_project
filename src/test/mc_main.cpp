#include <iostream>
#include <string>

#include "MonteCarloSimulator.h"

// usage: ACAV_mc <seed> <horizon> [ego]
// prints one CSV line: seed,horizon,crashed,crash_tick,cpu_gen,cpu_done
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " <seed> <horizon> [ego]" << std::endl;
        return 1;
    }

    unsigned seed = static_cast<unsigned>(std::stoul(argv[1]));
    int horizon   = std::stoi(argv[2]);
    bool ego      = (argc >= 4 && std::stoi(argv[3]) != 0);

    constexpr int map_size = 1350;

    MonteCarloSimulator sim(map_size, ego, seed);
    int crashTick = sim.run(horizon);

    std::cout << seed << ","
              << horizon << ","
              << (crashTick >= 0 ? 1 : 0) << ","
              << crashTick << ","
              << sim.getCpuGenerated() << ","
              << sim.getCpuCompleted() << std::endl;

    return 0;
}