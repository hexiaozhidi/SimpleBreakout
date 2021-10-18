#ifndef PARTICLE_H
#define PARTICLE_H

#include "GameObject.h"

class Particle : public GameObject {
public:
    Particle();

    Particle(const glm::vec2 &position, const glm::vec2 &size, const Color &tintColor, const Texture &texture,
        const glm::vec2 &velocity, const float &life);

    void reset(const glm::vec2 &position, const glm::vec2 &size, const Color &tintColor, const Texture &texture,
        const glm::vec2 &velocity, const float &life);

    void move(const float &deltaTime);

    const float &getLifeCountdown() const;

    void updateLifeCountdown(const float &deltaTime);

private:
    glm::vec2 velocity;
    float life;
    float lifeCountdown;
};

#endif