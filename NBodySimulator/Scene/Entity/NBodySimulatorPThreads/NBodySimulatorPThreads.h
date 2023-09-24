#ifndef NBODY_SIMULATOR_PTHREADS_H
#define NBODY_SIMULATOR_PTHREADS_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../NBodyEntity.h"

class NBodySimulatorPThreads : public NBodyEntity {
private:
    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    Shader shader;

    GLuint VAO, VBO;

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 color;

        Particle() : position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F)), color(glm::vec3(1.0F, 1.0F, 1.0F)) {}
    };

public:
    std::vector<Particle> particles;
    std::vector<glm::vec3> sumForces;

public:
    const size_t numThreads = 4;

public:
    explicit NBodySimulatorPThreads(int particleCount = 100000);

    NBodySimulatorPThreads(const NBodySimulatorPThreads&) = delete;
    auto operator=(const NBodySimulatorPThreads&) -> NBodySimulatorPThreads& = delete;
    NBodySimulatorPThreads(NBodySimulatorPThreads&&) = delete;
    auto operator=(NBodySimulatorPThreads&&) -> NBodySimulatorPThreads& = delete;

    ~NBodySimulatorPThreads() override;

public:
    void update(const float& deltaTime) final;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) final;

    void reset() final;

private:
    void randomizeParticles();

public:
    void setParticlesCount(const size_t& count) final;

    [[nodiscard]] auto getParticlesCount() const -> size_t final;

    [[nodiscard]] auto getDeltaTime() const -> float;

private:
    float deltaTime = 0.0F;

};

#endif // NBODY_SIMULATOR_PTHREADS_H
