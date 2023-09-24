#ifndef NBODY_SIMULATOR_BARNES_HUT_H
#define NBODY_SIMULATOR_BARNES_HUT_H

#include "BarnesHutOctree.h"
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../NBodyEntity.h"

class NBodySimulatorBarnesHut : public NBodyEntity {
private:
    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    Shader shader;

    GLuint VAO, VBO;

public:
    std::vector<Particle> particles;

public:
    explicit NBodySimulatorBarnesHut(int particleCount = 100000);

    NBodySimulatorBarnesHut(const NBodySimulatorBarnesHut&) = delete;
    auto operator=(const NBodySimulatorBarnesHut&) -> NBodySimulatorBarnesHut& = delete;
    NBodySimulatorBarnesHut(NBodySimulatorBarnesHut&&) = delete;
    auto operator=(NBodySimulatorBarnesHut&&) -> NBodySimulatorBarnesHut& = delete;

    ~NBodySimulatorBarnesHut() override;

public:
    void update(const float& deltaTime) final;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) final;

    void reset() final;

private:
    void randomizeParticles();

public:
    void clearParticles();

    [[nodiscard]] auto getParticlesCount() const -> size_t final;

    void setParticlesCount(const size_t& count) final;
};



#endif // NBODY_SIMULATOR_BARNES_HUT_H
