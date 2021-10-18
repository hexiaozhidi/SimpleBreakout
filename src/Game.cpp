#include "Game.h"
#include "CollisionDetector.h"
#include "ResourceManager.h"

#include <GLFW/glfw3.h>

#include <sstream>
#include <iostream>

const int NUM_LIVES = 3;

Game::Game(const int &screenWidth, const int &screenHeight) :
    screenWidth(screenWidth), screenHeight(screenHeight),
    backgroundTexture(ResourceManager::getTexture("background")),
    gameState(GameState::TITLE), keysPressed(1024), keysProcessed(1024),
    levels(), currentLevel(0), numLives(NUM_LIVES), powerUps(),
    paddle(glm::vec2(0.5 * (screenWidth - PADDLE_SIZE.x), screenHeight - PADDLE_SIZE.y)),
    ball(glm::vec2(0.5 * screenWidth - BALL_RADIUS, screenHeight - PADDLE_SIZE.y - 2.0f * BALL_RADIUS)),
    spriteRenderer(screenWidth, screenHeight),
    particleFlowRenderer(screenWidth, screenHeight),
    postProcessingRenderer(screenWidth, screenHeight),
    textRenderer(screenWidth, screenHeight) {

    // load levels
    for (int i = 0; i < 100; ++i) {
        std::string path = std::string("levels/") + (i < 10 ? "0" : "") + std::to_string(i) + ".lvl";
        levels.emplace_back(path.c_str(), screenWidth, 2.0f / 3.0f * screenHeight);
        if (!levels.back().getBricks().size())
            levels.pop_back();
    }

    if (!levels.size())
        std::cout << "ERROR: No valid level." << std::endl;

    currentLevel = time(nullptr) % levels.size();
}

void Game::processInput(const float &deltaTime) {
    if (gameState == GameState::TITLE) {

        if (keysPressed[GLFW_KEY_ENTER] && !keysProcessed[GLFW_KEY_ENTER]) {
            setKeyProcessed(GLFW_KEY_ENTER, true);
            ResourceManager::playSoundEffect("select");
            resetLevel();
            postProcessingRenderer.reset();
            currentLevel = 0;
            gameState = GameState::MENU;
        }

    } else if (gameState == GameState::MENU) {

        if (keysPressed[GLFW_KEY_ENTER] && !keysProcessed[GLFW_KEY_ENTER]) {
            setKeyProcessed(GLFW_KEY_ENTER, true);
            ResourceManager::playSoundEffect("start");
            gameState = GameState::ACTIVE;
        }

        if (keysPressed[GLFW_KEY_LEFT] && !keysProcessed[GLFW_KEY_LEFT]) {
            setKeyProcessed(GLFW_KEY_LEFT, true);
            ResourceManager::playSoundEffect("select");
            currentLevel = (currentLevel + levels.size() - 1) % levels.size();
        }

        if (keysPressed[GLFW_KEY_RIGHT] && !keysProcessed[GLFW_KEY_RIGHT]) {
            setKeyProcessed(GLFW_KEY_RIGHT, true);
            ResourceManager::playSoundEffect("select");
            currentLevel = (currentLevel + 1) % levels.size();
        }

    } else if (gameState == GameState::ACTIVE) {

        if (keysPressed[GLFW_KEY_ENTER] && !keysProcessed[GLFW_KEY_ENTER]) {
            setKeyProcessed(GLFW_KEY_ENTER, true);
            ResourceManager::playSoundEffect("pause");
            gameState = GameState::PAUSED;
        }

        if (keysPressed[GLFW_KEY_R] && !keysProcessed[GLFW_KEY_R]) {
            setKeyProcessed(GLFW_KEY_R, true);
            ResourceManager::playSoundEffect("return");
            resetLevel();
            postProcessingRenderer.reset();
            gameState = GameState::MENU;
        }

        if (keysPressed[GLFW_KEY_LEFT]) {
            setKeyProcessed(GLFW_KEY_LEFT, true);
            glm::vec2 displacement(-std::min(paddle.getSpeed() * deltaTime, paddle.getPosition().x), 0.0f);
            paddle.setPosition(paddle.getPosition() + displacement);

            if (ball.getStuck())
                ball.setPosition(ball.getPosition() + displacement);
        }

        if (keysPressed[GLFW_KEY_RIGHT]) {
            setKeyProcessed(GLFW_KEY_RIGHT, true);
            glm::vec2 displacement(std::min(paddle.getSpeed() * deltaTime, screenWidth - (paddle.getPosition().x + paddle.getSize().x)), 0.0f);
            paddle.setPosition(paddle.getPosition() + displacement);

            if (ball.getStuck())
                ball.setPosition(ball.getPosition() + displacement);
        }

        if (keysPressed[GLFW_KEY_SPACE] && !keysProcessed[GLFW_KEY_SPACE]) {
            setKeyProcessed(GLFW_KEY_SPACE, true);
            ball.setStuck(false);
        }

    } else if (gameState == GameState::PAUSED) {

        if (keysPressed[GLFW_KEY_ENTER] && !keysProcessed[GLFW_KEY_ENTER]) {
            setKeyProcessed(GLFW_KEY_ENTER, true);
            ResourceManager::playSoundEffect("pause");
            gameState = GameState::ACTIVE;
        }

        if (keysPressed[GLFW_KEY_R] && !keysProcessed[GLFW_KEY_R]) {
            setKeyProcessed(GLFW_KEY_R, true);
            ResourceManager::playSoundEffect("return");
            resetLevel();
            postProcessingRenderer.reset();
            gameState = GameState::MENU;
        }

    } else if (gameState == GameState::WON) {

        if (keysPressed[GLFW_KEY_ENTER] && !keysProcessed[GLFW_KEY_ENTER]) {
            setKeyProcessed(GLFW_KEY_ENTER, true);
            ResourceManager::playSoundEffect("return");
            postProcessingRenderer.reset();
            gameState = GameState::MENU;
        }

    } else if (gameState == GameState::LOST) {

        if (keysPressed[GLFW_KEY_ENTER] && !keysProcessed[GLFW_KEY_ENTER]) {
            setKeyProcessed(GLFW_KEY_ENTER, true);
            ResourceManager::playSoundEffect("start");
            resetLevel();
            postProcessingRenderer.reset();
            gameState = GameState::ACTIVE;
        }

        if (keysPressed[GLFW_KEY_R] && !keysProcessed[GLFW_KEY_R]) {
            setKeyProcessed(GLFW_KEY_R, true);
            ResourceManager::playSoundEffect("return");
            resetLevel();
            postProcessingRenderer.reset();
            gameState = GameState::MENU;
        }
    }
}

void Game::update(const float &deltaTime) {
    ResourceManager::playBackgroundMusic();

    if (gameState == GameState::TITLE) { // simulate a player
        if (postProcessingRenderer.isRegular())
            postProcessingRenderer.setSwingCountdown(10.0f);

        // move the paddle
        float difference = ball.getPosition().x + 0.5f * ball.getSize().x - (paddle.getPosition().x + 0.5f * paddle.getSize().x);
        if (difference < -0.3f * paddle.getSize().x) {
            glm::vec2 displacement(-std::min(paddle.getSpeed() * deltaTime, paddle.getPosition().x), 0.0f);
            paddle.setPosition(paddle.getPosition() + displacement);
        } else if (difference > 0.3f * paddle.getSize().x) {
            glm::vec2 displacement(std::min(paddle.getSpeed() * deltaTime, screenWidth - (paddle.getPosition().x + paddle.getSize().x)), 0.0f);
            paddle.setPosition(paddle.getPosition() + displacement);
        }

        // shoot the ball
        if (ball.getStuck() && rand() % 100 < 1) {
            ball.setStuck(false);
            paddle.setPosition(paddle.getPosition() + (rand() % 2 ? 1.0f : -1.0f) * paddle.getSpeed() * deltaTime);
        }

        // update the paddle
        paddle.updateStickyCountdown(deltaTime);

        // update the ball
        ball.move(deltaTime, screenWidth);
        ball.updatePassThroughCountdown(deltaTime);
        ball.updateParticleFlow(deltaTime);

        if (ball.getPosition().y >= screenHeight) { // lose a life
            resetLife();
            postProcessingRenderer.reset();
        }

        // update the power-ups
        for (auto it = powerUps.begin(); it != powerUps.end(); ) {
            it->move(deltaTime);
            if (it->getPosition().y >= screenHeight)
                it = powerUps.erase(it);
            else
                ++it;
        }

        // do collisions
        doCollisions();
        if (levels[currentLevel].isCompleted()) {
            resetLevel();
            currentLevel = time(nullptr) % levels.size();
        }

    } else if (gameState == GameState::ACTIVE) {

        // update the paddle
        paddle.updateStickyCountdown(deltaTime);

        // update the ball
        ball.move(deltaTime, screenWidth);
        ball.updatePassThroughCountdown(deltaTime);
        ball.updateParticleFlow(deltaTime);

        if (ball.getPosition().y >= screenHeight) { // lose a life
            ResourceManager::playSoundEffect("lost");
            if (--numLives) {
                resetLife();
                postProcessingRenderer.reset();
            } else {
                resetLevel();
                postProcessingRenderer.applyGrayscale();
                gameState = GameState::LOST;
            }
        }

        // update the power-ups
        for (auto it = powerUps.begin(); it != powerUps.end(); ) {
            it->move(deltaTime);
            if (it->getPosition().y >= screenHeight)
                it = powerUps.erase(it);
            else
                ++it;
        }

        // do collisions
        doCollisions();
        if (levels[currentLevel].isCompleted()) {
            resetLevel();
            postProcessingRenderer.setSwingCountdown(1000.0f);
            gameState = GameState::WON;
        }
    }
}

void Game::render(const float &time, const float &deltaTime) {
    postProcessingRenderer.prepare();

    spriteRenderer.draw(backgroundTexture, glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, screenHeight));

    spriteRenderer.drawLevel(levels[currentLevel]);

    if (gameState != GameState::MENU) {
        if (gameState != GameState::LOST) {
            particleFlowRenderer.draw(ball.getParticleFlow());
            spriteRenderer.drawGameObject(ball);
        }

        spriteRenderer.drawGameObject(paddle);

        for (PowerUp &powerUp : powerUps)
            spriteRenderer.drawGameObject(powerUp);
    }

    postProcessingRenderer.draw(time, deltaTime);

    // render text
    if (gameState == GameState::TITLE) {

        textRenderer.draw("SIMPLE  BREAKOUT", glm::vec2(0.5f * screenWidth, 0.35f * screenHeight), 1.0f);
        textRenderer.draw("Press ENTER to play.", glm::vec2(0.5f * screenWidth, 0.6f * screenHeight), 0.4f);
        textRenderer.draw("Press ESC to exit.", glm::vec2(0.5f * screenWidth, 0.7f * screenHeight), 0.4f);

    } else if (gameState == GameState::MENU) {

        textRenderer.draw("Press LEFT or RIGHT to choose a level.", glm::vec2(0.5f * screenWidth, 0.45f * screenHeight), 0.55f);
        textRenderer.draw("Press ENTER to start.", glm::vec2(0.5f * screenWidth, 0.6f * screenHeight), 0.4f);
        textRenderer.draw("Press ESC to exit.", glm::vec2(0.5f * screenWidth, 0.7f * screenHeight), 0.4f);

    } else if (gameState == GameState::ACTIVE) {

        textRenderer.draw(std::string("Lives: ") + (numLives < 4 ? std::string(numLives, 'O') : std::string("O x ") + std::to_string(numLives)),
            glm::vec2(0.08f * screenWidth, 0.08f * screenHeight), 0.4f);
        textRenderer.draw("ENTER - pause; R - give up; ESC - exit", glm::vec2(0.75f * screenWidth, 0.08f * screenHeight), 0.4f);
        if (ball.getStuck()) {
            textRenderer.draw("Press LEFT or RIGHT to move the paddle.", glm::vec2(0.5f * screenWidth, 0.6f * screenHeight), 0.4f);
            textRenderer.draw("Press SPACE to release the ball.", glm::vec2(0.5f * screenWidth, 0.7f * screenHeight), 0.4f);
        }

    } else if (gameState == GameState::PAUSED) {

        textRenderer.draw(std::string("Lives: ") + (numLives < 4 ? std::string(numLives, 'O') : std::string("O x ") + std::to_string(numLives)),
            glm::vec2(0.08f * screenWidth, 0.08f * screenHeight), 0.4f);
        textRenderer.draw("PAUSED", glm::vec2(0.5f * screenWidth, 0.35f * screenHeight), 1.0f);
        textRenderer.draw("ENTER - continue; R - give up; ESC - exit", glm::vec2(0.75f * screenWidth, 0.08f * screenHeight), 0.4f);

    } else if (gameState == GameState::WON) {

        textRenderer.draw("YOU WON!!!", glm::vec2(0.5f * screenWidth, 0.35f * screenHeight), 1.0f);
        textRenderer.draw("Press ENTER to return.", glm::vec2(0.5f * screenWidth, 0.55f * screenHeight), 0.4f);
        textRenderer.draw("Press ESC to exit.", glm::vec2(0.5f * screenWidth, 0.65f * screenHeight), 0.4f);

    } else if (gameState == GameState::LOST) {

        textRenderer.draw("YOU LOST...", glm::vec2(0.5f * screenWidth, 0.35f * screenHeight), 1.0f);
        textRenderer.draw("Press ENTER to retry.", glm::vec2(0.5f * screenWidth, 0.5f * screenHeight), 0.4f);
        textRenderer.draw("Press R to return.", glm::vec2(0.5f * screenWidth, 0.6f * screenHeight), 0.4f);
        textRenderer.draw("Press ESC to exit.", glm::vec2(0.5f * screenWidth, 0.7f * screenHeight), 0.4f);

    }
}

void Game::setKeyPressed(const int &key, const bool &value) {
    keysPressed[key] = value;
}

void Game::setKeyProcessed(const int &key, const bool &value) {
    keysProcessed[key] = value;
}

void Game::doCollisions() {
    glm::vec2 difference(0.0f); // the vector from the ball center to a point that belongs to the box, this point is the closest one to the ball center

    // do ball-paddle collision
    if (!ball.getStuck() && CollisionDetector::ballHasCollided(ball, paddle, difference)) {
        Direction direction = CollisionDetector::getNearestDirection(difference); // incident direction
        if (direction == Direction::DOWN) { // valid collision
            ball.setStuck(paddle.getSticky());
            ResourceManager::playSoundEffect("paddle");

            float paddleCenterX = paddle.getPosition().x + 0.5f * paddle.getSize().x;
            float ballCenterX = ball.getPosition().x + ball.getRadius();
            float offset = std::min(std::max((ballCenterX - paddleCenterX) / (0.5f * paddle.getSize().x), -1.0f), 1.0f) * 0.8f; // -0.8 <= offset <= 0.8

            float speed = glm::length(ball.getVelocity());
            ball.setVelocity(glm::vec2(offset * speed, -sqrtf(1.0f - offset * offset) * speed));
        }
    }

    // do ball-brick collisions
    for (Brick &brick : levels[currentLevel].getBricks()) {
        int solidLevel = brick.getSolidLevel();
        if (solidLevel && CollisionDetector::ballHasCollided(ball, brick, difference)) {
            switch (brick.getMaterial()) {
                case BrickMaterial::GLASS:
                    ResourceManager::playSoundEffect("glass");
                    break;
                case BrickMaterial::ROCK:
                    ResourceManager::playSoundEffect("rock");
                    break;
                case BrickMaterial::HARDROCK:
                    ResourceManager::playSoundEffect("hardrock");
                    break;
                case BrickMaterial::STEEL:
                    ResourceManager::playSoundEffect("steel");
                    break;
                default:
                    std::cout << "ERROR: Unknown brick material." << std::endl;
                    break;
            }

            if (solidLevel > 0 && ball.getPassThroughLevel() >= solidLevel) // pass through and destroy the brick
                brick.setSolidLevel(0);
            else { // hit the brick and bounce
                if (brick.getSolidLevel() == -1) { // undestroyable brick
                    postProcessingRenderer.setShakeCountdown(0.05f);
                } else // destroyable brick
                    brick.setSolidLevel(std::max(solidLevel - ball.getPowerLevel(), 0));

                Direction direction = CollisionDetector::getNearestDirection(difference); // incident direction
                glm::vec2 position = ball.getPosition();
                glm::vec2 velocity = ball.getVelocity();

                // bounce
                switch (direction) {
                    case Direction::UP:
                        ball.setPosition(glm::vec2(position.x, position.y + (ball.getRadius() + difference.y)));
                        ball.setVelocity(glm::vec2(velocity.x, -velocity.y));
                        break;
                    case Direction::RIGHT:
                        ball.setPosition(glm::vec2(position.x - (ball.getRadius() - difference.x), position.y));
                        ball.setVelocity(glm::vec2(-velocity.x, velocity.y));
                        break;
                    case Direction::DOWN:
                        ball.setPosition(glm::vec2(position.x, position.y - (ball.getRadius() - difference.y)));
                        ball.setVelocity(glm::vec2(velocity.x, -velocity.y));
                        break;
                    case Direction::LEFT:
                        ball.setPosition(glm::vec2(position.x + (ball.getRadius() + difference.x), position.y));
                        ball.setVelocity(glm::vec2(-velocity.x, velocity.y));
                        break;
                    default:
                        break;
                }
            }

            if (!brick.getSolidLevel()) { // destroyed brick
                levels[currentLevel].updateNumDestroyedBricks();
                PowerUpType type = PowerUp::getRandomType();
                if (type != PowerUpType::UNKNOWN)
                    powerUps.emplace_back(type, brick.getPosition() + 0.5f * brick.getSize());
            }
        }
    }

    // do paddle-power-up collisions
    for (auto it = powerUps.begin(); it != powerUps.end(); )
        if (CollisionDetector::paddleHasCollided(paddle, *it)) {
            activatePowerUp(*it);
            it = powerUps.erase(it);
        } else
            ++it;
}

void Game::activatePowerUp(const PowerUp &powerUp) {
    switch (powerUp.getType()) {
        case PowerUpType::PADDLE_EXPAND:
            ResourceManager::playSoundEffect("power_up_paddle_expand");
            paddle.setSize(glm::vec2(std::min(0.6f * screenWidth, paddle.getSize().x + 0.5f * PADDLE_SIZE.x), paddle.getSize().y));
            break;
        case PowerUpType::PADDLE_SHRINK:
            ResourceManager::playSoundEffect("power_up_paddle_shrink");
            paddle.setSize(glm::vec2(std::max(0.02f * screenWidth, 0.5f * paddle.getSize().x), paddle.getSize().y));
            break;
        case PowerUpType::PADDLE_STICKY:
            ResourceManager::playSoundEffect("power_up_paddle_sticky");
            paddle.updateStickyCountdown(-10.0f);
            break;
        case PowerUpType::BALL_EXPAND:
            ResourceManager::playSoundEffect("power_up_ball_expand");
            ball.setRadius(std::min(5.0f * BALL_RADIUS, ball.getRadius() + BALL_RADIUS));
            break;
        case PowerUpType::BALL_SHRINK:
            ResourceManager::playSoundEffect("power_up_ball_shrink");
            ball.setRadius(std::max(0.5f * BALL_RADIUS, 0.5f * ball.getRadius()));
            break;
        case PowerUpType::BALL_ACCELERATE:
            ResourceManager::playSoundEffect("power_up_ball_accelerate");
            ball.setVelocity(1.25f * ball.getVelocity());
            break;
        case PowerUpType::BALL_DECELERATE:
            ResourceManager::playSoundEffect("power_up_ball_decelerate");
            ball.setVelocity(0.8f * ball.getVelocity());
            break;
        case PowerUpType::BALL_POWER_LEVEL_UP:
            ResourceManager::playSoundEffect("power_up_ball_power_level_up");
            ball.updatePowerLevel(1);
            break;
        case PowerUpType::BALL_POWER_LEVEL_DOWN:
            ResourceManager::playSoundEffect("power_up_ball_power_level_down");
            ball.updatePowerLevel(-1);
            break;
        case PowerUpType::BALL_PASS_THROUGH:
            ResourceManager::playSoundEffect("power_up_ball_pass_through");
            ball.updatePassThroughCountdown(-10.0f);
            break;
        case PowerUpType::ONE_MORE_LIFE:
            ResourceManager::playSoundEffect("power_up_one_more_life");
            ++numLives;
            break;
        case PowerUpType::BLUR:
            ResourceManager::playSoundEffect("power_up_blur");
            postProcessingRenderer.setBlurCountdown(10.0f);
            break;
        case PowerUpType::CHAOS:
            ResourceManager::playSoundEffect("power_up_chaos");
            postProcessingRenderer.setChaosCountdown(10.0f);
            break;
        case PowerUpType::CONFUSION:
            ResourceManager::playSoundEffect("power_up_confusion");
            postProcessingRenderer.setConfusionCountdown(10.0f);
            break;
        default:
            std::cout << "ERROR: Unknown power-up type." << std::endl;
            break;
    }
}

void Game::resetLife() {
    paddle.reset(glm::vec2(0.5 * (screenWidth - PADDLE_SIZE.x), screenHeight - PADDLE_SIZE.y));
    ball.reset(glm::vec2(0.5 * screenWidth - BALL_RADIUS, screenHeight - PADDLE_SIZE.y - 2.0f * BALL_RADIUS));
    powerUps.clear();

}

void Game::resetLevel() {
    numLives = 3;
    resetLife();
    levels[currentLevel].reset();
}