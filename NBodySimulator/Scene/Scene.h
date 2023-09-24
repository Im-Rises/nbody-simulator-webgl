#ifndef SCENE_H
#define SCENE_H

#include "Camera/Camera.h"

#ifdef __unix__
#include "Entity/NBodySimulatorPThreads/NBodySimulatorPThreads.h"
#endif

#include "Entity/NBodySimulator/NBodySimulator.h"
#include "Entity/NBodySimulatorBarnesHut/NBodySimulatorBarnesHut.h"
#include "Entity/NBodySimulatorSSBO/NBodySimulatorSSBO.h"

enum class NBodySimulatorType {
    CPU,
    PTHREADS,
    BARNES_HUT,
    GPU
};

class Scene {
private:
    bool isPaused = false;

public:
    Camera camera;
    NBodyEntity* nbodySimulator = nullptr;

public:
    Scene(int display_w, int display_h);

    void update(float deltaTime);

    void render();

public:
    void updateProjectionMatrix(int display_w, int display_h);

    void togglePause();

    void reset();

public:
    [[nodiscard]] auto getIsPaused() const -> bool;

    [[nodiscard]] bool getIsNbodySimulationType(NBodySimulatorType nbodySimulatorType) const;

#ifdef __unix__
    void usePThreadsNbodySimulator();
#endif

    void useCPUNbodySimulator();
    void useBarnesHutNbodySimulator();

#ifndef __EMSCRIPTEN__
    void useGPUNbodySimulator();
#endif

private:
    NBodySimulatorType nbodySimulatorType;
};

#endif // SCENE_H
