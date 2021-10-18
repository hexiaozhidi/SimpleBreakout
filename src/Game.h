#ifndef GAME_H
#define GAME_H

#include "game_objects/Ball.h"
#include "game_objects/Brick.h"
#include "game_objects/Paddle.h"
#include "game_objects/PowerUp.h"

#include "Level.h"
#include "renderers/SpriteRenderer.h"
#include "renderers/ParticleFlowRenderer.h"
#include "renderers/PostProcessingRenderer.h"
#include "renderers/TextRenderer.h"

#include <vector>

enum class GameState {
    TITLE,
    MENU,
    ACTIVE,
    PAUSED,
    WON,
    LOST
};

class Game {
public:
    Game(const int &screenWidth, const int &screenHeight);

    void processInput(const float &deltaTime);

    void update(const float &deltaTime);

    void render(const float &time, const float &deltaTime);

    void setKeyPressed(const int &key, const bool &value);

    void setKeyProcessed(const int &key, const bool &value);

private:
    int screenWidth;
    int screenHeight;
    Texture backgroundTexture;

    GameState gameState;
    std::vector<bool> keysPressed;
    std::vector<bool> keysProcessed;

    std::vector<Level> levels;
    int currentLevel;
    int numLives;

    std::vector<PowerUp> powerUps;
    Paddle paddle;
    Ball ball;

    SpriteRenderer spriteRenderer;
    ParticleFlowRenderer particleFlowRenderer;
    PostProcessingRenderer postProcessingRenderer;
    TextRenderer textRenderer;

    void doCollisions();

    void activatePowerUp(const PowerUp &powerUp);

    void resetLife();

    void resetLevel();
};

#endif