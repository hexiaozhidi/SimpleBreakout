#ifndef POSTPROCESSINGRENDERER_H
#define POSTPROCESSINGRENDERER_H

#include "../Shader.h"
#include "../Texture.h"
#include "SpriteRenderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class PostProcessingRenderer {
public:
    PostProcessingRenderer(const unsigned int &screenWidth, const unsigned int &screenHeight);

    ~PostProcessingRenderer();

    void prepare() const;

    void draw(const float &time, const float &deltaTime);

    void update(const float &deltaTime);

    void reset();

    void applyGrayscale();

    const bool isRegular() const;

    void setShakeCountdown(const float &value);

    void setSwingCountdown(const float &value);

    void setBlurCountdown(const float &value);

    void setChaosCountdown(const float &value);

    void setConfusionCountdown(const float &value);


private:
    const Shader &postProcessingShader;

    int screenWidth;
    int screenHeight;

    unsigned int msfbo; // for rendering sprites
    unsigned int rbo; // colorbuffer of msfbo

    unsigned int fbo; // for post processing
    Texture postProcessingTexture; // colorbuffer of fbo

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    bool grayscale;
    float grayscaleFactor;

    float shakeCountdown;
    float swingCountdown;
    float blurCountdown;
    float chaosCountdown;
    float confusionCountdown;
};

#endif