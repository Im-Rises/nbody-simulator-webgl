#include "Scene.h"

Scene::Scene(int display_w, int display_h)
    : camera(display_w, display_h) {
#ifndef __EMSCRIPTEN__
    useGPUNbodySimulator();
#else
    useBarnesHutNbodySimulator();
#endif
}

void Scene::update(float deltaTime) {
    camera.update(deltaTime);
    if (isPaused)
    {
        return;
    }
    nbodySimulator->update(deltaTime);
}

void Scene::render() {
    nbodySimulator->render(camera.getViewMatrix(), camera.getProjectionMatrix());
}

void Scene::updateProjectionMatrix(int display_w, int display_h) {
    camera.updateProjectionMatrix(display_w, display_h);
}

void Scene::togglePause() {
    isPaused = !isPaused;
    nbodySimulator->setIsPaused(isPaused);
}

void Scene::reset() {
    camera.reset();
    nbodySimulator->reset();
}

auto Scene::getIsPaused() const -> bool {
    return isPaused;
}

bool Scene::getIsNbodySimulationType(NBodySimulatorType nbodySimulatorType) const {
    return this->nbodySimulatorType == nbodySimulatorType;
}

#if defined(__unix__) && !defined(__APPLE__)
void Scene::usePThreadsNbodySimulator() {
    if (nbodySimulator != nullptr)
    {
        delete nbodySimulator;
    }
    nbodySimulator = new NBodySimulatorPThreads(1000);
    nbodySimulatorType = NBodySimulatorType::PTHREADS;
}
#endif

void Scene::useCPUNbodySimulator() {
    if (nbodySimulator != nullptr)
    {
        delete nbodySimulator;
    }
    nbodySimulator = new NBodySimulator(1000);
    nbodySimulatorType = NBodySimulatorType::CPU;
}

void Scene::useBarnesHutNbodySimulator() {
    if (nbodySimulator != nullptr)
    {
        delete nbodySimulator;
    }
    nbodySimulator = new NBodySimulatorBarnesHut(1000);
    nbodySimulatorType = NBodySimulatorType::BARNES_HUT;
}

#ifndef __EMSCRIPTEN__
void Scene::useGPUNbodySimulator() {
    if (nbodySimulator != nullptr)
    {
        delete nbodySimulator;
    }
    nbodySimulator = new NBodySimulatorSSBO(10000);
    nbodySimulatorType = NBodySimulatorType::GPU;
}
#endif
