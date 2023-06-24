#include <iostream>
#include "NBodySimulatorLauncher.h"

auto main(int argc, char* argv[]) -> int {
    (void)argc;
    (void)argv;
    std::cout << NBodySimulatorLauncher::PROJECT_NAME << " " << NBodySimulatorLauncher::PROJECT_VERSION << std::endl
              << NBodySimulatorLauncher::PROJECT_LINK << std::endl
              << NBodySimulatorLauncher::PROJECT_AUTHOR << "\n"
              << std::endl;
    NBodySimulatorLauncher particleEmissionLauncher;
    particleEmissionLauncher.start();
    return 0;
}
