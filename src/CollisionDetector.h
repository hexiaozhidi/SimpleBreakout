#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "game_objects/Ball.h"
#include "game_objects/Brick.h"
#include "game_objects/Paddle.h"
#include "game_objects/PowerUp.h"

#include <vector>

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class CollisionDetector {
public:
    static bool ballHasCollided(const Ball &ball, const GameObject &box, glm::vec2 &difference);

    static bool paddleHasCollided(const Paddle &paddle, const PowerUp &powerUp);

    static Direction getNearestDirection(const glm::vec2 &direction);

private:
    static const std::vector<glm::vec2> compass;

    CollisionDetector() = default;
};

#endif