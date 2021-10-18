#ifndef PADDLE_H
#define PADDLE_H

#include "GameObject.h"

const glm::vec2 PADDLE_SIZE(100.0f, 20.0f);
const Color PADDLE_TINT_COLOR = Palette::getColorByHex("#eac88e");
const float PADDLE_SPEED = 500.0f;

class Paddle : public GameObject {
public:
    Paddle();

    Paddle(const glm::vec2 &position);

    void reset(const glm::vec2 &position);

    const float &getSpeed() const;

    const bool &getSticky() const;

    void updateStickyCountdown(const float &deltaTime);

private:
    float speed;

    bool sticky;
    float stickyCountdown;
};

#endif