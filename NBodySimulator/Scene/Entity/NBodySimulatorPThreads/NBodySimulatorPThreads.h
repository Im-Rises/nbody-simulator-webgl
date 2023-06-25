#ifndef NBODY_SIMULATOR_PTHREADS_H
#define NBODY_SIMULATOR_PTHREADS_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class NBodySimulatorPThreads : public Entity {
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
#ifdef __unix__
    float deltaTime = 0.0F;
#endif

public:
    float spawnRadius = 3.0F;
    float gravity = 1.0F;
    float particleMass = 1.0F;
    float softening = 10.0F;
    float damping = 0.99F;

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
    void update(const float& deltaTime) override;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;

    void reset();

private:
    void randomizeParticles();

public:
    void setParticlesCount(const size_t& count);

    [[nodiscard]] auto getParticlesCount() const -> size_t;
};

#endif // NBODY_SIMULATOR_PTHREADS_H
