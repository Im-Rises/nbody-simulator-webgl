#ifndef BARNES_HUT_OCTREE_H
#define BARNES_HUT_OCTREE_H

#include <glm/glm.hpp>
#include <list>
#include <array>

constexpr int MAX_DEPTH = 100;
constexpr int CAPACITY = 4;

struct Particle {
    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 sumOfForces;
    glm::vec3 color;
    int id;

    explicit Particle(int id) : id(id), mass(1.0F), position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F)), sumOfForces(glm::vec3(0.0F)), color(1.0F, 1.0F, 1.0F) {}
    Particle(int id, float mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 color) : id(id), mass(mass), position(position), velocity(velocity), sumOfForces(glm::vec3(0.0F)), color(color) {}
};

struct Bounds {
    glm::vec3 center;
    glm::vec3 halfDimension;


    Bounds(glm::vec3 center, glm::vec3 halfDimension) : center(center),
                                                        halfDimension(halfDimension) {}

    [[nodiscard]] auto contains(const Particle& particle) const -> bool {
        return particle.position.x >= center.x - halfDimension.x && particle.position.x <= center.x + halfDimension.x &&
               particle.position.y >= center.y - halfDimension.y && particle.position.y <= center.y + halfDimension.y &&
               particle.position.z >= center.z - halfDimension.z && particle.position.z <= center.z + halfDimension.z;
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
        Bounds bounds;
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
