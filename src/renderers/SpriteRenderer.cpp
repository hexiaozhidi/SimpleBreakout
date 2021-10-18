/* Explanation about the transforms :
 *        ____________________________________________                       ____________________________________________
 *       | Position (0, h)     ||     Position (w, h) |                     | Position (-1, -1)   ||    Position (1, -1) |
 *       | TexCoord (0, 1)     ||     TexCoord (1, 1) |  Orthogonal         | TexCoord (0, 1)     ||     TexCoord (1, 1) |
 * Pos.y |                     ||                     |  Projection     |   |                     ||                     |
 *   ^   |                  \\ || //                  | ============>   v   |                  \\ || //                  |
 *   |   |                   \\||//                   |               Pos.y |                   \\||//                   |
 *       | Position (0, 0)    \\//    Position (w, 0) |                     | Position (-1, 1)   \\//    Position (1, 1) |
 *       | TexCoord (0, 0)     \/     TexCoord (1, 0) |                     | TexCoord (0, 0)     \/     TexCoord (1, 0) |
 *       |____________________________________________|                     |____________________________________________|
 *
 *                        ---> Pos.x                                                          ---> Pos.x
 *
 *                        World Space                                                          NDC Space
 *
 *
 *                                          ____________________________________________
 *                                         | Position (0, h)     /\     Position (w, h) |
 *                     ViewPort            | TexCoord (0, 0)    //\\    TexCoord (1, 0) |
 *                  Transformation   Pos.y |                   //||\\                   |
 *                 ================>   ^   |                  // || \\                  |
 *                                     |   |                     ||                     |
 *                                         | Position (0, 0)     ||     Position (w, 0) |
 *                                         | TexCoord (0, 1)     ||     TexCoord (1, 1) |
 *                                         |____________________________________________|
 *
 *                                                           ---> Pos.x
 *
 *                                                          Screen Space
 */

#include "SpriteRenderer.h"
#include "../ResourceManager.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

SpriteRenderer::SpriteRenderer(const int &screenWidth, const int &screenHeight) :
    spriteShader(ResourceManager::getShader("sprite")),
    projection(glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f)),
    vao(0), vbo(0), ebo(0) {

    spriteShader.use().setInt("spriteTexture", 0).setMat4("projection", projection);

    std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    std::vector<int> indices = { 0, 1, 2, 3, 2, 1 };

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

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void SpriteRenderer::drawGameObject(const GameObject &gameObject) const {
    draw(gameObject.getTexture(), gameObject.getPosition(), gameObject.getSize(), gameObject.getRotationAngle(), gameObject.getTintColor());
}

void SpriteRenderer::drawLevel(Level &level) const {
    for (const Brick &brick : level.getBricks()) {
        if (brick.getSolidLevel())
            drawGameObject(brick);
    }
}

void SpriteRenderer::draw(const Texture &texture, const glm::vec2 &position, const glm::vec2 &size, const float &rotationAngle, const Color &tintColor) const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f * size.x + position.x, 0.5f * size.y + position.y, 0.0f)); // step 4: move the sprite to the target position
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f)); // step 3: rotate the sprite (counterclockwise in the world space, thus clockwise in the screen space)
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // step 2: move the sprite such that its center will be moved to the origin
    model = glm::scale(model, glm::vec3(size, 1.0f)); // step 1: scale up the sprite to the target size
    spriteShader.use().setMat4("model", model).setVec4("tintColor", tintColor);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.getId());

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}