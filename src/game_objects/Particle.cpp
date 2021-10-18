#include "Particle.h"

Particle::Particle() : GameObject(), velocity(0.0f), life(0.0f), lifeCountdown(0.0f) {
}

Particle::Particle(const glm::vec2 &position, const glm::vec2 &size, const Color &tintColor, const Texture &texture,
    const glm::vec2 &velocity, const float &life) :
    GameObject(position, size, 0.0f, tintColor, texture), velocity(velocity), life(life), lifeCountdown(life) {
}

void Particle::reset(const glm::vec2 &position, const glm::vec2 &size, const Color &tintColor, const Texture &texture,
    const glm::vec2 &velocity, const float &life) {
    this->position = position;
    this->size = size;
    this->tintColor = tintColor;
    this->texture = texture;
    this->velocity = velocity;
    this->life = life;
    this->lifeCountdown = life;
}

void Particle::move(const float &deltaTime) {
    if (lifeCountdown > 0.0f)
        position += velocity * deltaTime;
}

const float &Particle::getLifeCountdown() const {
    return lifeCountdown;
}

void Particle::updateLifeCountdown(const float &deltaTime) {
    lifeCountdown -= deltaTime;
}