#include "PowerUp.h"

#include <iostream>

const std::unordered_map<PowerUpType, int> PowerUp::probabilityFactors = {
    { PowerUpType::UNKNOWN, 2900 },
    { PowerUpType::PADDLE_EXPAND, 60 },
    { PowerUpType::PADDLE_SHRINK, 60 },
    { PowerUpType::PADDLE_STICKY, 80 },
    { PowerUpType::BALL_EXPAND, 60 },
    { PowerUpType::BALL_SHRINK, 60 },
    { PowerUpType::BALL_ACCELERATE, 60 },
    { PowerUpType::BALL_DECELERATE, 60 },
    { PowerUpType::BALL_POWER_LEVEL_UP, 80 },
    { PowerUpType::BALL_POWER_LEVEL_DOWN, 80 },
    { PowerUpType::BALL_PASS_THROUGH, 40 },
    { PowerUpType::ONE_MORE_LIFE, 40 },
    { PowerUpType::BLUR, 120 },
    { PowerUpType::CHAOS, 120 },
    { PowerUpType::CONFUSION, 120 }
};

PowerUp::PowerUp() : GameObject(), type(PowerUpType::UNKNOWN), speed(0.0f) {
}

PowerUp::PowerUp(const PowerUpType &type, const glm::vec2 &centerPosition) :
    GameObject(centerPosition - POWER_UP_SIZE, POWER_UP_SIZE), type(type), speed(POWER_UP_SPEED) {

    switch (type) {
        case (PowerUpType::PADDLE_EXPAND):
            texture = ResourceManager::getTexture("power_up_paddle_expand");
            tintColor = Palette::getColorByName("blue");
            break;
        case (PowerUpType::PADDLE_SHRINK):
            texture = ResourceManager::getTexture("power_up_paddle_shrink");
            tintColor = Palette::getColorByName("red");
            break;
        case (PowerUpType::PADDLE_STICKY):
            texture = ResourceManager::getTexture("power_up_paddle_sticky");
            tintColor = Palette::getColorByName("purple");
            break;
        case (PowerUpType::BALL_EXPAND):
            texture = ResourceManager::getTexture("power_up_ball_expand");
            tintColor = Palette::getColorByName("green");
            break;
        case (PowerUpType::BALL_SHRINK):
            texture = ResourceManager::getTexture("power_up_ball_shrink");
            tintColor = Palette::getColorByName("yellow");
            break;
        case (PowerUpType::BALL_ACCELERATE):
            texture = ResourceManager::getTexture("power_up_ball_accelerate");
            tintColor = Palette::getColorByName("green");
            break;
        case (PowerUpType::BALL_DECELERATE):
            texture = ResourceManager::getTexture("power_up_ball_decelerate");
            tintColor = Palette::getColorByName("purple");
            break;
        case (PowerUpType::BALL_POWER_LEVEL_UP):
            texture = ResourceManager::getTexture("power_up_ball_power_level_up");
            tintColor = Palette::getColorByName("green");
            break;
        case (PowerUpType::BALL_POWER_LEVEL_DOWN):
            texture = ResourceManager::getTexture("power_up_ball_power_level_down");
            tintColor = Palette::getColorByName("red");
            break;
        case (PowerUpType::BALL_PASS_THROUGH):
            texture = ResourceManager::getTexture("power_up_ball_pass_through");
            tintColor = Palette::getColorByName("orange");
            break;
        case (PowerUpType::ONE_MORE_LIFE):
            texture = ResourceManager::getTexture("power_up_one_more_life");
            tintColor = Palette::getColorByName("blue");
            break;
        case (PowerUpType::BLUR):
            texture = ResourceManager::getTexture("power_up_blur");
            tintColor = Palette::getColorByName("gray");
            break;
        case (PowerUpType::CHAOS):
            texture = ResourceManager::getTexture("power_up_chaos");
            tintColor = Palette::getColorByName("gray");
            break;
        case (PowerUpType::CONFUSION):
            texture = ResourceManager::getTexture("power_up_confusion");
            tintColor = Palette::getColorByName("gray");
            break;
        default:
            std::cout << "ERROR: Unknown power-up type." << std::endl;
            break;
    }
}

void PowerUp::move(const float &deltaTime) {
    position.y += speed * deltaTime;
}

const PowerUpType &PowerUp::getType() const {
    return type;
}

const PowerUpType PowerUp::getRandomType() {
    int sum = 0;
    for (auto &p : probabilityFactors)
        sum += p.second;

    int r = rand() % sum;
    sum = 0;
    PowerUpType res = PowerUpType::UNKNOWN;
    for (auto &p : probabilityFactors) {
        sum += p.second;
        if (r < sum) {
            res = p.first;
            break;
        }
    }

    return res;
}