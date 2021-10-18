#include "Paddle.h"

Paddle::Paddle() : GameObject(), speed(0.0f), sticky(false), stickyCountdown(0.0f) {
}

Paddle::Paddle(const glm::vec2 &position) :
    GameObject(position, PADDLE_SIZE, 0.0f, PADDLE_TINT_COLOR, ResourceManager::getTexture("paddle")),
    speed(PADDLE_SPEED), sticky(false), stickyCountdown(0.0f) {
}

void Paddle::reset(const glm::vec2 &position) {
    this->position = position;
    size = PADDLE_SIZE;
    tintColor = PADDLE_TINT_COLOR;

    speed = PADDLE_SPEED;
    sticky = false;
    stickyCountdown = 0.0f;
}

const float &Paddle::getSpeed() const {
    return speed;
}

const bool &Paddle::getSticky() const {
    return sticky;
}


void Paddle::updateStickyCountdown(const float &deltaTime) {
    if (deltaTime < 0.0f) {
        sticky = true;
        stickyCountdown = -deltaTime;
        tintColor = Palette::getColorByName("purple");
    } else if (stickyCountdown > 0.0f) {
        stickyCountdown -= deltaTime;
        if (stickyCountdown <= 0.0f) {
            sticky = false;
            tintColor = PADDLE_TINT_COLOR;
        }
    }
}