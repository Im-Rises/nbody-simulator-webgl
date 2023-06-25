#ifndef SCENE_H
#define SCENE_H

#include "Camera/Camera.h"

#if defined(__EMSCRIPTEN__)
#include "Entity/NBodySimulatorPThreads/NBodySimulatorPThreads.h"
#else
#include "Entity/NBodySimulatorSSBO/NBodySimulatorSSBO.h"
#endif

class Scene {
private:
    bool isPaused = false;

public:
    Camera camera;

#if defined(__EMSCRIPTEN__)
    NBodySimulatorPThreads nbodySimulator;
#else
    NBodySimulatorSSBO nbodySimulator;
#endif

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
};

#endif // SCENE_H
