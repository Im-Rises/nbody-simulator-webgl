#include "NBodySimulatorBarnesHut.h"

#include <random>
#include <iostream>

#include "../../../Utility/piDeclaration.h"

const char* const NBodySimulatorBarnesHut::VertexShaderSource =
    R"(#version 300 es

        precision highp float;

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_velocity;
        layout(location = 2) in vec3 a_color;

        uniform mat4 u_mvp;

        out vec3 v_color;

        void main()
        {
            gl_Position = u_mvp * vec4(a_position, 1.0);
            v_color = a_color;
            gl_PointSize = 2.0f;
        }
)";

const char* const NBodySimulatorBarnesHut::FragmentShaderSource =
    R"(#version 300 es

        precision highp float;

        in vec3 v_color;

        out vec4 o_fragColor;

        void main() {
            o_fragColor = vec4(v_color, 1.0f);
        }
)";

NBodySimulatorBarnesHut::NBodySimulatorBarnesHut(int particleCount) : shader(VertexShaderSource, FragmentShaderSource, false) {
    // Resize the particles vector
    setParticlesCount(particleCount);

    // Init the VAO
    glGenVertexArrays(1, &VAO);

    // Init the VBO
    glGenBuffers(1, &VBO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Set the VBO data
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_STATIC_DRAW);

    // Set the VAO attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
    glEnableVertexAttribArray(2);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

NBodySimulatorBarnesHut::~NBodySimulatorBarnesHut() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void NBodySimulatorBarnesHut::update(const float& deltaTime) {
    BarnesHutOctree octree(Bounds(glm::vec3(0.0F), glm::vec3(20.0F, 20.0F, 20.0F)));

    for (auto& particle : particles)
    {
        octree.insert(&particle);
    }

    octree.computeMassDistribution();

    for (auto& particle : particles)
    {
        octree.computeSumOfForces(particle, theta, gravity, softening);
        const glm::vec3 acceleration = particle.sumOfForces / particle.mass;

        particle.position += particle.velocity * deltaTime + 0.5F * acceleration * deltaTime * deltaTime;
        particle.velocity += acceleration * deltaTime;
        particle.velocity *= damping;

        particle.sumOfForces = glm::vec3(0.0F);
    }
}

void NBodySimulatorBarnesHut::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Set the VBO data
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_STATIC_DRAW);

    // Bind the shader
    shader.use();

    // Set the uniform variables
    shader.setMat4("u_mvp", cameraProjectionMatrix * cameraViewMatrix);

    // Draw the particles
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particles.size()));

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NBodySimulatorBarnesHut::reset() {
    randomizeParticles();
}

void NBodySimulatorBarnesHut::randomizeParticles() {
    // Init the random engine
    std::random_device rd;
    std::mt19937 randomEngine(rd());
    std::uniform_real_distribution<float> randomAngle(0.0F, static_cast<float>(2.0F * M_PI));
    std::uniform_real_distribution<float> randomColorValue(0.0F, 1.0F);

    // Init the particles as a sphere
    for (auto& particle : particles)
    {
        const float angle1 = randomAngle(randomEngine);
        const float angle2 = randomAngle(randomEngine);
        const float x = spawnRadius * std::sin(angle1) * std::cos(angle2);
        const float y = spawnRadius * std::sin(angle1) * std::sin(angle2);
        const float z = spawnRadius * std::cos(angle1);
        particle.position = glm::vec3(x, y, z) + position;
        particle.velocity = glm::vec3(0.0F, 0.0F, 0.0F);
        particle.color = glm::vec3(randomColorValue(randomEngine), randomColorValue(randomEngine), randomColorValue(randomEngine));
        particle.mass = particleMass;
    }
}

void NBodySimulatorBarnesHut::clearParticles() {
    // Clear the particles vector
    particles.clear();
}

auto NBodySimulatorBarnesHut::getParticlesCount() const -> size_t {
    return particles.size();
}

void NBodySimulatorBarnesHut::setParticlesCount(const size_t& count) {
    // Clear particles
    clearParticles();

    // Resize the particles vector
    for (int i = 0; i < count; i++)
        particles.emplace_back(i);

    // Init the particles
    randomizeParticles();
}
