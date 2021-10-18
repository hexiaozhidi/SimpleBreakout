#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "../game_objects/GameObject.h"
#include "../Level.h"
#include "../Shader.h"
#include "../Texture.h"

class SpriteRenderer {
public:
    SpriteRenderer(const int &screenWidth, const int &screenHeight);

    ~SpriteRenderer();

    void drawGameObject(const GameObject &gameObject) const;

    void drawLevel(Level &level) const;

    void draw(const Texture &texture, const glm::vec2 &position, const glm::vec2 &size,
        const float &rotationAngle = 0.0f, const Color &tintColor = Palette::getColorByName("white")) const;

private:
    const Shader &spriteShader;

    glm::mat4 projection;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};

#endif