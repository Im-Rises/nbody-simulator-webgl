#ifndef PARTICLE_SIMULATOR_SSBO_H
#define PARTICLE_SIMULATOR_SSBO_H

#include <array>

#include <glad/glad.h>
#include <vector>
#include "../Entity.h"

class NBodySimulatorSSBO : public Entity {
private:
    GLuint VAO;
    GLuint ssbo;

    Shader shader;

    // Can't use directly vec3 in SSBO (indexed by 4 bytes not 3): https://computergraphics.stackexchange.com/questions/5810/shader-storage-buffer-indexing-by-4-bytes-instead-of-3
    struct Particle {
        glm::vec3 position;
        [[maybe_unused]] float offset1{};
        glm::vec3 velocity;
        [[maybe_unused]] float offset2{};

        Particle() : position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F)) {}
    };

    int particlesCount;

    float deltaTime = 0.0F;
    float isAttracting = 0.0F;

public:
    float spawnRadius = 3.0F;
    float gravity = 1.0F;
    float particleMass = 50.0F;
    float softening = 10.0F;
    float damping = 0.99F;
    float attractorMass = 500.0F;
    glm::vec3 attractorPosition = glm::vec3(0.0F, 0.0F, 0.0F);

public:
    explicit NBodySimulatorSSBO(int particlesCount = 1000000);

    NBodySimulatorSSBO(const NBodySimulatorSSBO&) = delete;
    auto operator=(const NBodySimulatorSSBO&) -> NBodySimulatorSSBO& = delete;
    NBodySimulatorSSBO(NBodySimulatorSSBO&&) = delete;
    auto operator=(NBodySimulatorSSBO&&) -> NBodySimulatorSSBO& = delete;

    ~NBodySimulatorSSBO() override;

public:
    void update(const float& deltaTime) override;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;

    void reset();

private:
    void randomizeParticles(std::vector<Particle>& particles);

public:
    void setAttractorPosition(const glm::vec3& pos);

    void setIsAttracting(const bool& value);

    [[nodiscard]] auto getIsAttracting() const -> bool;

    void setParticlesCount(const int& value);

    [[nodiscard]] auto getParticlesCount() const -> size_t;
};

#endif // PARTICLE_SIMULATOR_SSBO_H
