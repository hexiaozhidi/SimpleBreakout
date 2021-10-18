#include "Particle.h"
#include "ParticleFlow.h"

#include <algorithm>

ParticleFlow::ParticleFlow() :
    particles(), textures(), numParticlesMax(0), numParticlesNeedUpdatingPerFrame(0), particleUpdatingProbability(0.0f),
    randomPositionFactor(0.0f), size(0.0f), velocityFactor(0.0f), life(0.0f) {
}

ParticleFlow::ParticleFlow(const std::vector<Texture> &textures, const glm::vec2 &centerPosition, const float &scale,
    const glm::vec2 &velocity, const int &numParticlesMax, const int &numParticlesNeedUpdatingPerFrame,
    const float &particleUpdatingProbability, const float &randomPositionFactor,
    const glm::vec2 &size, const float &velocityFactor, const float &life) :
    particles(), textures(textures), numParticlesMax(std::min(numParticlesMax, NUM_PARTICLES_MAX)),
    numParticlesNeedUpdatingPerFrame(numParticlesNeedUpdatingPerFrame),
    particleUpdatingProbability(particleUpdatingProbability),
    randomPositionFactor(randomPositionFactor), size(size), velocityFactor(velocityFactor), life(life) {

    reset(centerPosition, scale, velocity);
}

void ParticleFlow::reset(const glm::vec2 &centerPosition, const float &scale, const glm::vec2 &velocity) {
    particles.clear();

    for (int i = 0; i < numParticlesNeedUpdatingPerFrame; ++i)
        particles.emplace_back(getRandomPosition(centerPosition, scale), scale * size,
        Palette::getRandomColorFromPalette(), getRandomTexture(), velocity * velocityFactor, life);
}

void ParticleFlow::update(const float &deltaTime, const glm::vec2 &centerPosition, const float &scale, const glm::vec2 &velocity) {
    for (Particle &particle : particles) {
        particle.updateLifeCountdown(deltaTime);
        particle.move(deltaTime);
    }

    for (int i = 0; i < numParticlesNeedUpdatingPerFrame; ++i)
        if (rand() % 1000 >= 1000 * particleUpdatingProbability)
            continue;
        else if (particles.front().getLifeCountdown() <= 0.0f || particles.size() == numParticlesMax) {
            auto cmp = [](const Particle &p1, const Particle &p2) -> bool { return p1.getLifeCountdown() > p2.getLifeCountdown(); };
            std::pop_heap(particles.begin(), particles.end(), cmp);
            particles.back().reset(getRandomPosition(centerPosition, scale), scale * size,
                Palette::getRandomColorFromPalette(), getRandomTexture(), velocity * velocityFactor, life);
        } else
            particles.emplace_back(getRandomPosition(centerPosition, scale), scale * size,
            Palette::getRandomColorFromPalette(), getRandomTexture(), velocity * velocityFactor, life);
}

const std::vector<Particle> &ParticleFlow::getParticles() const {
    return particles;
}

const std::vector<Texture> &ParticleFlow::getTextures() const {
    return textures;
}

const float &ParticleFlow::getLife() const {
    return life;
}

const glm::vec2 ParticleFlow::getRandomPosition(const glm::vec2 &centerPosition, const float &scale) const {
    glm::vec2 position = centerPosition - 0.5f * scale * size;
    glm::vec2 displacement = 0.001f * (glm::vec2(rand() % 2001, rand() % 2001) - 1000.0f) * scale * size; // -scale * size <= displacement <= scale * size
    return position + displacement * randomPositionFactor;
}

const Texture ParticleFlow::getRandomTexture() const {
    return textures[rand() % textures.size()];
}