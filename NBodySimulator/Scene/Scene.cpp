#include "Scene.h"

#ifdef __EMSCRIPTEN__
Scene::Scene(int display_w, int display_h) : camera(display_w, display_h), nbodySimulator(100) {
#else
Scene::Scene(int display_w, int display_h) : camera(display_w, display_h), nbodySimulator(1000) {
#endif
}

// void Scene::fixedUpdate(float deltaTime) {
// }

void Scene::update(float deltaTime) {
    camera.update(deltaTime);
    if (isPaused)
    {
        return;
    }
    nbodySimulator.update(deltaTime);
}

void Scene::render() {
    nbodySimulator.render(camera.getViewMatrix(), camera.getProjectionMatrix());
}

void Scene::updateProjectionMatrix(int display_w, int display_h) {
    camera.updateProjectionMatrix(display_w, display_h);
}

void Scene::togglePause() {
    isPaused = !isPaused;
    nbodySimulator.setIsPaused(isPaused);
}

void Scene::reset() {
    camera.reset();
    nbodySimulator.reset();
}

auto Scene::getIsPaused() const -> bool {
    return isPaused;
}
