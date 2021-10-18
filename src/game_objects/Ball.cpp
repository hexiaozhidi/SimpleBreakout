#include "Ball.h"

Ball::Ball() :
    GameObject(), velocity(0.0f), radius(0.0f), powerLevel(0), passThroughLevel(0), passThroughCountdown(0.0f), stuck(false), particleFlow() {
}

Ball::Ball(const glm::vec2 &position) :
    GameObject(position, glm::vec2(2.0f * BALL_RADIUS), 0.0f, Palette::getColorByName("white"), ResourceManager::getTexture("ball")),
    velocity(BALL_VELOCITY), radius(BALL_RADIUS), powerLevel(1), passThroughLevel(0), passThroughCountdown(0.0f), stuck(true),
    particleFlow({ ResourceManager::getTexture("particle_circle"), ResourceManager::getTexture("particle_cross"),
    ResourceManager::getTexture("particle_square"), ResourceManager::getTexture("particle_triangle") }, position + BALL_RADIUS, 1.0f, velocity) {
}

void Ball::reset(const glm::vec2 &position) {
    this->position = position;
    size = glm::vec2(2.0f * BALL_RADIUS);
    tintColor = Palette::getColorByName("white");

    velocity = BALL_VELOCITY;
    radius = BALL_RADIUS;
    powerLevel = 1;

    passThroughLevel = 0;
    passThroughCountdown = 0.0f;

    stuck = true;
    particleFlow.reset(position + BALL_RADIUS, 1.0f, velocity);
}

void Ball::move(const float &deltaTime, const int &screenWidth) {
    if (stuck)
        return;

    position += velocity * deltaTime;

    if (position.x < 0.0f) {
        position.x = 0.0f;
        velocity.x = -velocity.x;
    } else if (position.x > screenWidth - size.x) {
        position.x = screenWidth - size.x;
        velocity.x = -velocity.x;
    }

    if (position.y < 0.0f) {
        position.y = 0.0f;
        velocity.y = -velocity.y;
    }
}

const glm::vec2 &Ball::getVelocity() const {
    return velocity;
}

const float &Ball::getRadius() const {
    return radius;
}

const int &Ball::getPowerLevel() const {
    return powerLevel;
}

const int &Ball::getPassThroughLevel() const {
    return passThroughLevel;
}

const bool &Ball::getStuck() const {
    return stuck;
}

const ParticleFlow &Ball::getParticleFlow() const {
    return particleFlow;
}

void Ball::setSize(const glm::vec2 &value) {
    size = value;
    radius = 0.5f * value.x;
}

void Ball::setVelocity(const glm::vec2 &value) {
    velocity = value;
}

void Ball::setRadius(const float &value) {
    radius = value;
    size = glm::vec2(2.0f * value);
}

void Ball::setStuck(const bool &value) {
    stuck = value;
}

void Ball::updatePowerLevel(const int &deltaValue) {
    powerLevel = std::max(1, powerLevel + deltaValue);
}

void Ball::updatePassThroughCountdown(const float &deltaTime) {
    if (deltaTime < 0.0f) {
        ++passThroughLevel;
        passThroughCountdown = -deltaTime;
        tintColor = Palette::getColorByName(passThroughLevel == 1 ? "orange" : "red");
    } else if (passThroughCountdown > 0.0f) {
        passThroughCountdown -= deltaTime;
        if (passThroughCountdown <= 0.0f) {
            passThroughLevel = 0;
            tintColor = Palette::getColorByName("white");
        }
    }
}

void Ball::updateParticleFlow(const float &deltaTime) {
    particleFlow.update(deltaTime, position + radius, radius / BALL_RADIUS, stuck ? glm::vec2(0.0f) : velocity);
}