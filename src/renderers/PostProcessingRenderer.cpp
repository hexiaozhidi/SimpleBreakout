#include "PostProcessingRenderer.h"

#include <iostream>

PostProcessingRenderer::PostProcessingRenderer(const unsigned int &screenWidth, const unsigned int &screenHeight) :
    postProcessingShader(ResourceManager::getShader("post_processing")),
    screenWidth(screenWidth), screenHeight(screenHeight),
    msfbo(0), rbo(0), fbo(0), postProcessingTexture(screenWidth, screenHeight, 4, nullptr),
    vao(0), vbo(0), ebo(0),
    grayscale(false), grayscaleFactor(1.0f),
    shakeCountdown(0.0f), swingCountdown(0.0f), blurCountdown(0.0f), chaosCountdown(0.0f), confusionCountdown(0.0f) {

    postProcessingShader.use().setInt("postProcessingTexture", 0);
    glBindTexture(GL_TEXTURE_2D, postProcessingTexture.getId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    std::vector<float> blurKernel = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };

    std::vector<float> edgeKernel = {
        -1.0, -1.0, -1.0,
        -1.0, 8.0, -1.0,
        -1.0, -1.0, -1.0
    };

    postProcessingShader.setFloat("blurKernel", blurKernel).setFloat("edgeKernel", edgeKernel);

    glGenFramebuffers(1, &msfbo);
    glGenRenderbuffers(1, &rbo);
    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, msfbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR: Incomplete multisample framebuffer for rendering sprites." << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture.getId(), 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR: Incomplete framebuffer for post processing." << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::vector<float> vertices = { // NDC triangle
        -1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        3.0f, -1.0f, 2.0f, 0.0f, // bottom-right
        -1.0f, 3.0f, 0.0f, 2.0f // top-left
    };

    std::vector<int> indices = { 0, 1, 2 };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

PostProcessingRenderer::~PostProcessingRenderer() {
    glDeleteFramebuffers(1, &msfbo);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void PostProcessingRenderer::prepare() const {
    glBindFramebuffer(GL_FRAMEBUFFER, msfbo);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessingRenderer::draw(const float &time, const float &deltaTime) {
    update(deltaTime);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, msfbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    postProcessingShader.use().setFloat("time", time);
    postProcessingShader.setFloat("grayscaleFactor", grayscaleFactor);
    postProcessingShader.setBool("shake", shakeCountdown > 0.0f);
    postProcessingShader.setBool("shake", shakeCountdown > 0.0f);
    postProcessingShader.setBool("swing", swingCountdown > 0.0f);
    postProcessingShader.setBool("blur", blurCountdown > 0.0f);
    postProcessingShader.setBool("chaos", chaosCountdown > 0.0f);
    postProcessingShader.setBool("confusion", confusionCountdown > 0.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, postProcessingTexture.getId());

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void PostProcessingRenderer::update(const float &deltaTime) {
    if (grayscale && grayscaleFactor > 0.0f)
        grayscaleFactor = std::max(0.0f, grayscaleFactor - 0.4f * deltaTime);

    if (shakeCountdown > 0.0f)
        shakeCountdown -= deltaTime;
    if (swingCountdown > 0.0f)
        swingCountdown -= deltaTime;
    if (blurCountdown > 0.0f)
        blurCountdown -= deltaTime;
    if (chaosCountdown > 0.0f)
        chaosCountdown -= deltaTime;
    if (confusionCountdown > 0.0f)
        confusionCountdown -= deltaTime;
}

void PostProcessingRenderer::reset() {
    grayscale = false;
    grayscaleFactor = 1.0f;
    swingCountdown = blurCountdown = chaosCountdown = confusionCountdown = 0.0f;
}

void PostProcessingRenderer::applyGrayscale() {
    reset();
    grayscale = true;
}

const bool PostProcessingRenderer::isRegular() const {
    return blurCountdown <= 0.0f && chaosCountdown <= 0.0f && confusionCountdown <= 0.0f;
}

void PostProcessingRenderer::setShakeCountdown(const float &value) {
    shakeCountdown = value;
}

void PostProcessingRenderer::setSwingCountdown(const float &value) {
    reset();
    swingCountdown = value;
}

void PostProcessingRenderer::setBlurCountdown(const float &value) {
    reset();
    blurCountdown = value;
}

void PostProcessingRenderer::setChaosCountdown(const float &value) {
    reset();
    chaosCountdown = value;
}

void PostProcessingRenderer::setConfusionCountdown(const float &value) {
    reset();
    confusionCountdown = value;
}