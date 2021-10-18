#ifndef BALL_H
#define BALL_H

#include "GameObject.h"
#include "ParticleFlow.h"

const glm::vec2 BALL_VELOCITY(0.0f, -250.0f);
const float BALL_RADIUS = 15.0f;

class Ball : public GameObject {
public:
    Ball();

    Ball(const glm::vec2 &position);

    void reset(const glm::vec2 &position);

    void move(const float &deltaTime, const int &screenWidth);

    const glm::vec2 &getVelocity() const;

    const float &getRadius() const;

    const int &getPowerLevel() const;

    const int &getPassThroughLevel() const;

    const bool &getStuck() const;

    const ParticleFlow &getParticleFlow() const;

    void setSize(const glm::vec2 &value);

    void setVelocity(const glm::vec2 &value);

    void setRadius(const float &value);

    void setStuck(const bool &value);

    void updatePowerLevel(const int &deltaValue);

    void updatePassThroughCountdown(const float &deltaTime);

    void updateParticleFlow(const float &deltaTime);

private:
    glm::vec2 velocity;
    float radius;
    int powerLevel;

    int passThroughLevel;
    float passThroughCountdown;

    bool stuck;

    ParticleFlow particleFlow;
};

#endif