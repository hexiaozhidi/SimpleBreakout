#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../Palette.h"
#include "../ResourceManager.h"
#include "../Texture.h"

#include <glm/glm.hpp>

class GameObject {
public:
    GameObject();

    GameObject(const glm::vec2 &position, const glm::vec2 &size, const float &rotationAngle = 0.0f,
        const Color &tintColor = Palette::getColorByName("white"), const Texture & texture = ResourceManager::getTexture());

    const glm::vec2 &getPosition() const;

    const glm::vec2 &getSize() const;

    const float &getRotationAngle() const;

    const Color &getTintColor() const;

    const Texture &getTexture() const;

    void setPosition(const glm::vec2 &value);

    void setSize(const glm::vec2 &value);

    void setRotationAngle(const float &value);

    void setTintColor(const Color &value);

    void setTexture(const Texture &value);

protected:
    glm::vec2 position;
    glm::vec2 size;
    float rotationAngle; // in degrees, clockwise (in the screen space)
    Color tintColor;
    Texture texture;
};

#endif