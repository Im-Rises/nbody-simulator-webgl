#ifndef NBODY_SIMULATOR_H
#define NBODY_SIMULATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../NBodyEntity.h"

class NBodySimulator : public NBodyEntity {
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
    explicit NBodySimulator(int particleCount = 100000);

    NBodySimulator(const NBodySimulator&) = delete;
    auto operator=(const NBodySimulator&) -> NBodySimulator& = delete;
    NBodySimulator(NBodySimulator&&) = delete;
    auto operator=(NBodySimulator&&) -> NBodySimulator& = delete;

    ~NBodySimulator() override;

public:
    void update(const float& deltaTime) final;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) final;

    void reset() final;

private:
    void randomizeParticles();

public:
    void setParticlesCount(const size_t& count) final;

    [[nodiscard]] auto getParticlesCount() const -> size_t final;
};

#endif // NBODY_SIMULATOR_H
