#ifndef NBODY_SIMULATOR_H
#define NBODY_SIMULATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class NBodySimulator : public Entity {
private:
    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    Shader shader;

    GLuint VAO, VBO;

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;

        Particle() : position(glm::vec3(0.0F)), velocity(glm::vec3(1.0F, 1.0F, 1.0F)) {}
    };

    std::vector<Particle> particles;

    float isAttracting = 1.0F;

public:
    float spawnRadius = 3.0F;
    float gravity = 1.0F;
    float particleMass = 50.0F;
    float softening = 10.0F;
    float damping = 0.99F;

public:
    explicit NBodySimulator(int particleCount = 100000);

    NBodySimulator(const NBodySimulator&) = delete;
    auto operator=(const NBodySimulator&) -> NBodySimulator& = delete;
    NBodySimulator(NBodySimulator&&) = delete;
    auto operator=(NBodySimulator&&) -> NBodySimulator& = delete;

    ~NBodySimulator() override;

public:
    void update(const float& deltaTime) override;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;

    void reset();

private:
    void randomizeParticles();

public:
    //     void setAttractorPosition(const glm::vec3& pos);
    //
    //     void setIsAttracting(const bool& value);
    //
    //     [[nodiscard]] auto getIsAttracting() const -> bool;
    //
    void setParticlesCount(const size_t& count);

    [[nodiscard]] auto getParticlesCount() const -> size_t;
};

#endif // NBODY_SIMULATOR_H
