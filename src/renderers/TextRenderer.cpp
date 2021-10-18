#include "TextRenderer.h"
#include "../ResourceManager.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

TextRenderer::TextRenderer(const int &screenWidth, const int &screenHeight) :
    textShader(ResourceManager::getShader("text")),
    projection(glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f)),
    bearingYMax(ResourceManager::getGlyph('l').bearing.y), vao(0), vbo(0), ebo(0) {

    textShader.use().setInt("glyphTexture", 0).setMat4("projection", projection);


    std::vector<int> indices = { 0, 1, 2, 3, 2, 1 };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), nullptr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

TextRenderer::~TextRenderer() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void TextRenderer::draw(const std::string &text, const glm::vec2 &centerPosition, const float &scale, const Color &color) const {
    textShader.use().setVec4("color", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float sizeXText = 0.0f;
    for (const char &c : text)
        sizeXText += ResourceManager::getGlyph(c).advance >> 6;
    const Glyph &lastGlyph = ResourceManager::getGlyph(text.back());
    sizeXText = sizeXText - (lastGlyph.advance >> 6) + lastGlyph.bearing.x + lastGlyph.size.x;
    sizeXText *= scale;

    float positionXText = centerPosition.x - 0.5f * sizeXText, positionYText = centerPosition.y - 0.5f * bearingYMax;

    for (const char &c : text) {
        const Glyph &glyph = ResourceManager::getGlyph(c);
        float positionX = positionXText + glyph.bearing.x * scale;
        float positionY = positionYText + (bearingYMax - glyph.bearing.y) * scale;
        float width = glyph.size.x * scale;
        float height = glyph.size.y * scale;

        std::vector<float> vertices = {
            positionX, positionY, 0.0f, 0.0f,
            positionX, positionY + height, 0.0f, 1.0f,
            positionX + width, positionY, 1.0f, 0.0f,
            positionX + width, positionY + height, 1.0f, 1.0f
        };

        glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), &vertices[0]);
        glBindTexture(GL_TEXTURE_2D, glyph.texture.getId());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        positionXText += (glyph.advance >> 6) * scale;
    }

    glBindVertexArray(0);
}