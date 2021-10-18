#ifndef PARTICLEFLOW_H
#define PARTICLEFLOW_H

#include "Particle.h"

#include <vector>

const int NUM_PARTICLES_MAX = 500;
const int NUM_PARTICLES_NEED_UPDATING_PER_FRAME = 1;
const float PARTICLE_UPDATING_PROBABILITY = 0.1f;
const float RANDOM_POSITION_FACTOR = 0.3f;
const glm::vec2 PARTICLE_SIZE(20.0f, 20.0f);
const float PARTICLE_VELOCITY_FACTOR = 0.1f;
const float PARTICLE_LIFE = 1.0f;

class ParticleFlow {
public:
    ParticleFlow();

    ParticleFlow(const std::vector<Texture> &textures, const glm::vec2 &centerPosition, const float &scale, const glm::vec2 &velocity,
        const int &numParticlesMax = NUM_PARTICLES_MAX,
        const int &numParticlesNeedUpdatingPerFrame = NUM_PARTICLES_NEED_UPDATING_PER_FRAME,
        const float &particleUpdatingProbability = PARTICLE_UPDATING_PROBABILITY,
        const float &randomPositionFactor = RANDOM_POSITION_FACTOR,
        const glm::vec2 &size = PARTICLE_SIZE, const float &velocityFactor = PARTICLE_VELOCITY_FACTOR,
        const float &life = PARTICLE_LIFE);

    void reset(const glm::vec2 &centerPosition, const float &scale, const glm::vec2 &velocity);

    void update(const float &deltaTime, const glm::vec2 &centerPosition, const float &scale, const glm::vec2 &velocity);

    const std::vector<Particle> &getParticles() const;

    const std::vector<Texture> &getTextures() const;

    const float &getLife() const;

private:
    std::vector<Particle> particles; // a min heap of particles with life countdown comparison
    std::vector<Texture> textures; // textures for generating particles

    int numParticlesMax;
    int numParticlesNeedUpdatingPerFrame;
    float particleUpdatingProbability;

    float randomPositionFactor;
    glm::vec2 size;
    float velocityFactor;
    float life;

    const glm::vec2 getRandomPosition(const glm::vec2 &centerPosition, const float &scale) const;

    const Texture getRandomTexture() const;
};

#endif