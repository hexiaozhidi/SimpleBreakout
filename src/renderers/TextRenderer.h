#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "../Palette.h"
#include "../Shader.h"

#include <glm/glm.hpp>

#include <string>

class TextRenderer {
public:
    TextRenderer(const int &screenWidth, const int &screenHeight);

    ~TextRenderer();

    void draw(const std::string &text, const glm::vec2 &centerPosition, const float &scale,
        const Color &color = Palette::getColorByName("black")) const;

private:
    const Shader &textShader;

    glm::mat4 projection;

    int bearingYMax;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};

#endif 