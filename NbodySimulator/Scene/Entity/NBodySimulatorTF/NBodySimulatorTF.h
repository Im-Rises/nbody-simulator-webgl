#ifndef PARTICLE_SIMULATOR_TF_H
#define PARTICLE_SIMULATOR_TF_H

#include <array>

#include <glad/glad.h>

#include "../Entity.h"

class NBodySimulatorTF : public Entity {
    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    Shader shader;

    GLuint VAO[2];
    GLuint TFBO[2];
    GLuint VBO[2];

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;

        Particle() : position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F, 0.0F, 0.0F)) {}
    };

    GLuint currentVAO;
    GLuint currentTFBO;

    int particlesCount;

    float deltaTime = 0.0F;
    glm::vec3 attractorPosition = glm::vec3(0.0F, 0.0F, 0.0F);
    float isAttracting = 0.0F;

public:
    float spawnRadius = 2.0F;

    float damping = 0.99F;
    float particleMass = 50.0F;
    float attractorMass = 250.0F;
    float gravity = 1.0F;
    float softening = 10.0F;

public:
    explicit NBodySimulatorTF(int particlesCount = 100000);

    NBodySimulatorTF(const NBodySimulatorTF&) = delete;
    auto operator=(const NBodySimulatorTF&) -> NBodySimulatorTF& = delete;
    NBodySimulatorTF(NBodySimulatorTF&&) = delete;
    auto operator=(NBodySimulatorTF&&) -> NBodySimulatorTF& = delete;

    ~NBodySimulatorTF() override;

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


#endif // PARTICLE_SIMULATOR_TF_H
