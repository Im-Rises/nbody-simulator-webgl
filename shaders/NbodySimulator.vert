#version 430 core

struct Particle {
    vec3 position;
    float offset1;
    vec3 velocity;
    float offset2;
};

layout (std430, binding = 0) buffer ParticlesSsbo {
    Particle particles[];
} particlesSsboData;

uniform mat4 u_mvp;
uniform float u_deltaTime;
uniform float u_damping;
uniform float u_particleMass;
uniform float u_gravity;
uniform float u_softening;
uniform float u_isRunning;
//uniform float u_attractorMass;
//uniform vec3 u_attractorPosition;

out vec3 v_vel;

void main()
{
    Particle particle = particlesSsboData.particles[gl_VertexID];

    vec3 sumForces = vec3(0.0);
    for (int i = 0; i < particlesSsboData.particles.length(); ++i) {
        if (i == gl_VertexID)
        continue;

        Particle otherParticle = particlesSsboData.particles[i];

        vec3 r = otherParticle.position - particle.position;
        float rSquared = dot(r, r) + u_softening;
        sumForces += normalize(r) * (u_gravity * u_particleMass * u_particleMass) / rSquared;
    }

//    vec3 r = u_attractorPosition - particle.position;
//    float rSquared = dot(r, r) + u_softening;
//    vec3 attractorForce = normalize(r) * (u_gravity * u_particleMass * u_particleMass) / rSquared;

    vec3 acceleration = sumForces / u_particleMass;

    vec3 position = particle.position + (particle.velocity * u_deltaTime + 0.5 * acceleration * u_deltaTime * u_deltaTime) * u_isRunning;
    vec3 velocity = particle.velocity + acceleration * u_deltaTime;

    particle.position = position;
    particle.velocity = velocity * u_damping;

    particlesSsboData.particles[gl_VertexID] = particle;

    gl_Position = u_mvp * vec4(particle.position, 1.0);

    v_vel = particle.velocity;
}
