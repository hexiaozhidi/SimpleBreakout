#ifndef POWERUP_H
#define POWERUP_H

#include "GameObject.h"

#include <vector>
#include <unordered_map>

const glm::vec2 POWER_UP_SIZE(80.0f,40.0f);
const float POWER_UP_SPEED = 150.0f;

enum class PowerUpType {
    UNKNOWN,
    PADDLE_EXPAND,
    PADDLE_SHRINK,
    PADDLE_STICKY,
    BALL_EXPAND,
    BALL_SHRINK,
    BALL_ACCELERATE,
    BALL_DECELERATE,
    BALL_POWER_LEVEL_UP,
    BALL_POWER_LEVEL_DOWN,
    BALL_PASS_THROUGH,
    ONE_MORE_LIFE,
    BLUR,
    CHAOS,
    CONFUSION
};

class PowerUp : public GameObject {
public:
    PowerUp();

    PowerUp(const PowerUpType &type, const glm::vec2 &centerPosition);

    void move(const float &deltaTime);

    const PowerUpType &getType() const;

    static const PowerUpType getRandomType();

private:
    PowerUpType type;
    float speed;

    static const std::unordered_map<PowerUpType, int> probabilityFactors;
};

#endif