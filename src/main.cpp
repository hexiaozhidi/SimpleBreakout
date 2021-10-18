#include "Game.h"
#include "ResourceManager.h"
#include "ShaderCode.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

// timer
float lastTime = 0.0f;
float deltaTime = 0.0f;

// window
const int screenWidth = 1280;
const int screenHeight = 720;

// game
Game *game = nullptr;

int main() {
    // set GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // create GLFW window
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Simple Breakout", nullptr, nullptr);
    if (!window) {
        std::cout << "ERROR: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    // set GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR: Failed to initialize GLAD." << std::endl;
        return -1;
    }

    // set OpenGL parameters
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable 4-bytes-alignment restriction for loading fonts

    // load shaders
    ResourceManager::generateNullShader();
    //ResourceManager::loadShader("sprite", "shaders/sprite_vs.glsl", "shaders/sprite_fs.glsl");
    //ResourceManager::loadShader("particle_flow", "shaders/particle_flow_vs.glsl", "shaders/particle_flow_fs.glsl");
    //ResourceManager::loadShader("post_processing", "shaders/post_processing_vs.glsl", "shaders/post_processing_fs.glsl");
    //ResourceManager::loadShader("text", "shaders/text_vs.glsl", "shaders/text_fs.glsl");

    ResourceManager::loadShader("sprite", sprite_vShaderCode, sprite_fShaderCode);
    ResourceManager::loadShader("particle_flow", particle_flow_vShaderCode, particle_flow_fShaderCode);
    ResourceManager::loadShader("post_processing", post_processing_vShaderCode, post_processing_fShaderCode);
    ResourceManager::loadShader("text", text_vShaderCode, text_fShaderCode);

    // load textures
    ResourceManager::generateNullTexture();
    ResourceManager::loadTexture("background", "assets/textures/background.png");

    ResourceManager::loadTexture("glass", "assets/textures/glass.png");
    ResourceManager::loadTexture("rock", "assets/textures/rock.png");
    ResourceManager::loadTexture("rock_cracks", "assets/textures/rock_cracks.png");
    ResourceManager::loadTexture("hardrock", "assets/textures/hardrock.png");
    ResourceManager::loadTexture("hardrock_cracks", "assets/textures/hardrock_cracks.png");
    ResourceManager::loadTexture("hardrock_more_cracks", "assets/textures/hardrock_more_cracks.png");
    ResourceManager::loadTexture("steel", "assets/textures/steel.png");

    ResourceManager::loadTexture("paddle", "assets/textures/paddle.png");
    ResourceManager::loadTexture("ball", "assets/textures/ball.png");

    ResourceManager::loadTexture("particle_circle", "assets/textures/particle_circle.png");
    ResourceManager::loadTexture("particle_cross", "assets/textures/particle_cross.png");
    ResourceManager::loadTexture("particle_square", "assets/textures/particle_square.png");
    ResourceManager::loadTexture("particle_triangle", "assets/textures/particle_triangle.png");

    ResourceManager::loadTexture("power_up_paddle_expand", "assets/textures/power_up_paddle_expand.png");
    ResourceManager::loadTexture("power_up_paddle_shrink", "assets/textures/power_up_paddle_shrink.png");
    ResourceManager::loadTexture("power_up_paddle_sticky", "assets/textures/power_up_paddle_sticky.png");
    
    ResourceManager::loadTexture("power_up_ball_expand", "assets/textures/power_up_ball_expand.png");
    ResourceManager::loadTexture("power_up_ball_shrink", "assets/textures/power_up_ball_shrink.png");
    ResourceManager::loadTexture("power_up_ball_accelerate", "assets/textures/power_up_ball_accelerate.png");
    ResourceManager::loadTexture("power_up_ball_decelerate", "assets/textures/power_up_ball_decelerate.png");
    ResourceManager::loadTexture("power_up_ball_power_level_up", "assets/textures/power_up_ball_power_level_up.png");
    ResourceManager::loadTexture("power_up_ball_power_level_down", "assets/textures/power_up_ball_power_level_down.png");
    ResourceManager::loadTexture("power_up_ball_pass_through", "assets/textures/power_up_ball_pass_through.png");

    ResourceManager::loadTexture("power_up_one_more_life", "assets/textures/power_up_one_more_life.png");
    ResourceManager::loadTexture("power_up_blur", "assets/textures/power_up_blur.png");
    ResourceManager::loadTexture("power_up_chaos", "assets/textures/power_up_chaos.png");
    ResourceManager::loadTexture("power_up_confusion", "assets/textures/power_up_confusion.png");

    // load fonts (only the last loaded font is valid)
    ResourceManager::loadFont("assets/fonts/Sakuya.ttf", 72);

    // load background musics
    ResourceManager::loadBackgroundMusic("assets/background_musics/bgm_01.mp3", 0.3f);
    ResourceManager::loadBackgroundMusic("assets/background_musics/bgm_02.mp3", 0.3f);

    // load sound effects
    ResourceManager::loadSoundEffect("select", "assets/sound_effects/key_01.mp3");
    ResourceManager::loadSoundEffect("start", "assets/sound_effects/key_02.mp3");
    ResourceManager::loadSoundEffect("pause", "assets/sound_effects/key_03.mp3");
    ResourceManager::loadSoundEffect("return", "assets/sound_effects/key_04.mp3");

    ResourceManager::loadSoundEffect("paddle", "assets/sound_effects/event_01.mp3");
    ResourceManager::loadSoundEffect("lost", "assets/sound_effects/event_02.mp3");
    ResourceManager::loadSoundEffect("glass", "assets/sound_effects/event_03.mp3");
    ResourceManager::loadSoundEffect("rock", "assets/sound_effects/event_04.mp3");
    ResourceManager::loadSoundEffect("hardrock", "assets/sound_effects/event_05.mp3");
    ResourceManager::loadSoundEffect("steel", "assets/sound_effects/event_06.mp3");

    ResourceManager::loadSoundEffect("power_up_paddle_expand", "assets/sound_effects/power_up_01.mp3");
    ResourceManager::loadSoundEffect("power_up_paddle_shrink", "assets/sound_effects/power_up_02.mp3");
    ResourceManager::loadSoundEffect("power_up_paddle_sticky", "assets/sound_effects/power_up_03.mp3");

    ResourceManager::loadSoundEffect("power_up_ball_expand", "assets/sound_effects/power_up_04.mp3");
    ResourceManager::loadSoundEffect("power_up_ball_shrink", "assets/sound_effects/power_up_05.mp3");
    ResourceManager::loadSoundEffect("power_up_ball_accelerate", "assets/sound_effects/power_up_06.mp3");
    ResourceManager::loadSoundEffect("power_up_ball_decelerate", "assets/sound_effects/power_up_07.mp3");
    ResourceManager::loadSoundEffect("power_up_ball_power_level_up", "assets/sound_effects/power_up_08.mp3");
    ResourceManager::loadSoundEffect("power_up_ball_power_level_down", "assets/sound_effects/power_up_09.mp3");
    ResourceManager::loadSoundEffect("power_up_ball_pass_through", "assets/sound_effects/power_up_10.mp3");

    ResourceManager::loadSoundEffect("power_up_one_more_life", "assets/sound_effects/power_up_11.mp3");
    ResourceManager::loadSoundEffect("power_up_blur", "assets/sound_effects/power_up_12.mp3");
    ResourceManager::loadSoundEffect("power_up_chaos", "assets/sound_effects/power_up_13.mp3");
    ResourceManager::loadSoundEffect("power_up_confusion", "assets/sound_effects/power_up_14.mp3");

    // generate the game
    game = new Game(screenWidth, screenHeight);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // update the timer
        float time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;

        // process the input
        glfwPollEvents();
        game->processInput(deltaTime);

        // update the game
        game->update(deltaTime);

        // render the game
        glClear(GL_COLOR_BUFFER_BIT);
        game->render(time, deltaTime);
        glfwSwapBuffers(window);
    }

    // clear
    ResourceManager::clear();
    delete game;
    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    else if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            game->setKeyPressed(key, true);
        else if (action == GLFW_RELEASE) {
            game->setKeyPressed(key, false);
            game->setKeyProcessed(key, false);
        }
    }
}