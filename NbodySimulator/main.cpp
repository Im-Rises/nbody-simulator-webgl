#include <iostream>
#include "NBodySimulatorLauncher.h"

// This is a workaround for the Nvidia and AMD drivers to force the use of the GPU instead of the integrated GPU
// This is not a good practice, but it's the only way to force the use of the GPU
// https://stackoverflow.com/questions/68469954/how-to-choose-specific-gpu-when-create-opengl-context

#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) uint32_t NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif
#endif


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
