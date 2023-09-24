#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "../../Shader/Shader.h"
#include <glm/glm.hpp>

class NBodyEntity {
public:
    glm::vec3 position = glm::vec3(0.0F, 0.0F, 0.0F);
    glm::vec3 rotation = glm::vec3(0.0F, 0.0F, 0.0F);
    glm::vec3 scale = glm::vec3(1.0F, 1.0F, 1.0F);

protected:
    glm::mat4 modelMatrix;

protected:
    bool isPaused = false;

public:
    NBodyEntity() = default;

    NBodyEntity(const NBodyEntity&) = delete;
    auto operator=(const NBodyEntity&) -> NBodyEntity& = delete;
    NBodyEntity(NBodyEntity&&) = delete;
    auto operator=(NBodyEntity&&) -> NBodyEntity& = delete;

    virtual ~NBodyEntity() = default;

public:
    virtual void update(const float& deltaTime) = 0;

    virtual void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) = 0;

    virtual void reset() = 0;

    void updateModelMatrix();

    void setIsPaused(bool isPaused);

    [[nodiscard]] auto getIsPaused() const -> bool;

public:
    float spawnRadius = 3.0F;
    float gravity = 1.0F;
    float particleMass = 1.0F;
    float softening = 10.0F;
    float damping = 0.99F;
    float interactionPercent = 1.0F;
    float theta = 1.0F;

public:
    [[nodiscard]] virtual auto getParticlesCount() const -> size_t = 0;
    virtual void setParticlesCount(const size_t& count) = 0;
};


#endif // ENTITY_H
