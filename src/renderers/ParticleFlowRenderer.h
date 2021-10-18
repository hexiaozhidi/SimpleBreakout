#ifndef PARTICLEFLOWRENDERER_H
#define PARTICLEFLOWRENDERER_H

#include "../game_objects/ParticleFlow.h"
#include "../Shader.h"
#include "../Texture.h"

#include <vector>

class ParticleFlowRenderer {
public:
    ParticleFlowRenderer(const int &screenWidth, const int &screenHeight);

    ~ParticleFlowRenderer();

    void draw(const ParticleFlow &particleFlow);

private:
    const Shader &particleFlowShader;

    glm::mat4 projection;

    unsigned int vao;
    unsigned int vbo;

    unsigned int vboInstancedTextureIndices;
    std::vector<int> textureIndices;
    unsigned int vboInstancedModels;
    std::vector<glm::mat4> models;
    unsigned int vboInstancedTintColors;
    std::vector<Color> tintColors;

    unsigned int ebo;
};

#endif