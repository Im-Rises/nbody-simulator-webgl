#ifndef BARNES_HUT_OCTREE_H
#define BARNES_HUT_OCTREE_H

#include <glm/glm.hpp>
#include <list>
#include <array>

constexpr int MAX_DEPTH = 1000;
constexpr int CAPACITY = 1;

struct Particle {
    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 sumOfForces;
    glm::vec3 color;
    const int id;

    explicit Particle(int id) : id(id), mass(1.0F), position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F)), sumOfForces(glm::vec3(0.0F)), color(1.0F, 1.0F, 1.0F) {}
    Particle(int id, float mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 color) : id(id), mass(mass), position(position), velocity(velocity), sumOfForces(glm::vec3(0.0F)), color(color) {}

    void appendForceFrom(glm::vec3 otherPos, float otherMass, float gravity, float softening) {
        const auto direction = otherPos - position;
        const auto distance = glm::length(direction);
        const auto magnitude = (gravity * mass * otherMass) / ((distance * distance) + softening);
        sumOfForces += magnitude * glm::normalize(direction);
    }
};

struct Bounds {
    const glm::vec3 center;
    const float halfDimension;


    Bounds(glm::vec3 center, float halfDimension) : center(center),
                                                    halfDimension(halfDimension) {}

    [[nodiscard]] auto contains(const Particle& particle) const -> bool {
        return particle.position.x >= center.x - halfDimension && particle.position.x <= center.x + halfDimension &&
               particle.position.y >= center.y - halfDimension && particle.position.y <= center.y + halfDimension &&
               particle.position.z >= center.z - halfDimension && particle.position.z <= center.z + halfDimension;
    }
};

class BarnesHutOctree {
private:
    struct BarnesHutOctreeNode {
    public:
        explicit BarnesHutOctreeNode(Bounds bounds, int depth);

        BarnesHutOctreeNode(const BarnesHutOctreeNode& other) = delete;
        BarnesHutOctreeNode(BarnesHutOctreeNode&& other) noexcept = delete;
        auto operator=(const BarnesHutOctreeNode& other) -> BarnesHutOctreeNode& = delete;
        auto operator=(BarnesHutOctreeNode&& other) noexcept -> BarnesHutOctreeNode& = delete;

        ~BarnesHutOctreeNode();

    public:
        auto insert(Particle* particle) -> bool;

        void subdivide();

        void computeMassDistribution();

        void computeSumOfForces(Particle& particle, float theta, float gravity, float softening) const;

    private:
        std::array<BarnesHutOctreeNode*, 8> children;
        std::list<Particle*> particles;
        const Bounds bounds;
        bool isLeaf;
        int depth;

        glm::vec3 centerOfMass;
        float mass;

        friend class BarnesHutOctree;
    };

public:
    explicit BarnesHutOctree(Bounds bounds);

public:
    void insert(Particle* particle);
    void computeMassDistribution();
    void computeSumOfForces(Particle& particle, float theta, float gravity, float softening) const;

private:
    BarnesHutOctreeNode root;
};

#endif // BARNES_HUT_OCTREE_H
