#include "NBodySimulatorSSBO.h"

#include "../../../Utility/piDeclaration.h"

#include <random>
#include <iostream>

NBodySimulatorSSBO::NBodySimulatorSSBO(int particlesCount) : shader("shaders/ParticleSimulator.vert",
                                                                       "shaders/ParticleSimulator.frag") {
    // Set the particles count
    this->particlesCount = particlesCount;
    std::vector<Particle> particles(particlesCount);
    randomizeParticles(particles);

    // Init the VAO and VBO
    glGenVertexArrays(1, &VAO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Generate the SSBO
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

NBodySimulatorSSBO::~NBodySimulatorSSBO() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &ssbo);
}

void NBodySimulatorSSBO::update(const float& deltaTime) {
    this->deltaTime = deltaTime;
}

void NBodySimulatorSSBO::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Shader use and uniform upload
    shader.use();
    shader.setMat4("u_mvp", cameraProjectionMatrix * cameraViewMatrix);
    shader.setFloat("u_deltaTime", deltaTime);
    shader.setFloat("u_damping", damping);
    shader.setFloat("u_particleMass", particleMass);
    shader.setFloat("u_gravity", gravity);
    shader.setFloat("u_softening", softening);
    shader.setFloat("u_isRunning", static_cast<float>(!isPaused));
    shader.setFloat("u_attractorMass", attractorMass);
    shader.setVec3("u_attractorPosition", attractorPosition);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // Draw the particles
    glDrawArrays(GL_POINTS, 0, particlesCount);

    // Barrier
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void NBodySimulatorSSBO::reset() {
    std::vector<Particle> particles(particlesCount);

    // Randomize particles positions and velocities
    randomizeParticles(particles);

    // Resend to the GPU
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void NBodySimulatorSSBO::randomizeParticles(std::vector<Particle>& particles) {
    // Init the random engine
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> randomFloats(0.0F, static_cast<float>(2.0F * M_PI));

    // Init the particles as a sphere
    for (auto& particle : particles)
    {
        const float angle1 = randomFloats(randomEngine);
        const float angle2 = randomFloats(randomEngine);
        const float x = spawnRadius * std::sin(angle1) * std::cos(angle2);
        const float y = spawnRadius * std::sin(angle1) * std::sin(angle2);
        const float z = spawnRadius * std::cos(angle1);
        particle.position = glm::vec3(x, y, z) + position;
        particle.velocity = glm::vec3(0.0F, 0.0F, 0.0F);
    }
}

void NBodySimulatorSSBO::setAttractorPosition(const glm::vec3& pos) {
    attractorPosition = pos;
}

void NBodySimulatorSSBO::setIsAttracting(const bool& value) {
    isAttracting = static_cast<float>(value);
}

auto NBodySimulatorSSBO::getIsAttracting() const -> bool {
    return isAttracting != 0.0F;
}

void NBodySimulatorSSBO::setParticlesCount(const int& value) {
    particlesCount = value;
    reset();
}

auto NBodySimulatorSSBO::getParticlesCount() const -> size_t {
    return particlesCount;
}
