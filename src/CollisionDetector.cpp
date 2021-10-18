#include "CollisionDetector.h"

#include <algorithm>

const std::vector<glm::vec2> CollisionDetector::compass = { glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(-1.0f, 0.0f) }; // UP, RIGHT, DOWN, LEFT

bool CollisionDetector::ballHasCollided(const Ball &ball, const GameObject &box, glm::vec2 &difference) {
    glm::vec2 ballCenter = ball.getPosition() + ball.getRadius();
    glm::vec2 boxHalfSize = 0.5f * box.getSize();
    glm::vec2 boxCenter = box.getPosition() + boxHalfSize;
    glm::vec2 closestPoint = boxCenter + glm::clamp(ballCenter - boxCenter, -boxHalfSize, boxHalfSize); // the closest point that belongs to the box, to the ball center
    difference = closestPoint - ballCenter;
    return glm::length(difference) < ball.getRadius();
}

bool CollisionDetector::paddleHasCollided(const Paddle &paddle, const PowerUp &powerUp) {
    float left1 = paddle.getPosition().x, right1 = left1 + paddle.getSize().x, up1 = paddle.getPosition().y, down1 = up1 + paddle.getSize().y;
    float left2 = powerUp.getPosition().x, right2 = left2 + powerUp.getSize().x, up2 = powerUp.getPosition().y, down2 = up2 + powerUp.getSize().y;
    return std::min(right1, right2) > std::max(left1, left2) && std::min(down1, down2) > std::max(up1, up2);
}

Direction CollisionDetector::getNearestDirection(const glm::vec2 &direction) {
    glm::vec2 dir = glm::normalize(direction);
    Direction res = Direction::UP;
    float dotMax = glm::dot(dir, compass[0]);

    for (int i = 1; i < 4; ++i) {
        float d = glm::dot(dir, compass[i]);
        if (d > dotMax) {
            res = static_cast<Direction>(i);
            dotMax = d;
        }
    }

    return res;
}