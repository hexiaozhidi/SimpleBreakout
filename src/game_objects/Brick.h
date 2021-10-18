#ifndef BRICK_H
#define BRICK_H

#include "GameObject.h"

enum class BrickMaterial {
    UNKNOWN,
    GLASS,
    ROCK,
    HARDROCK,
    STEEL
};

class Brick : public GameObject {
public:
    Brick();

    Brick(const BrickMaterial &material, const glm::vec2 &position, const glm::vec2 &size, const Color &tintColor = Palette::getColorByName("white"));

    void reset();

    const BrickMaterial &getMaterial() const;

    const int &getSolidLevel() const;

    void setSolidLevel(const int &val);

private:
    const BrickMaterial material;
    int solidLevel; // -1: undestroyable; 0: destroyed; >0: destroyable

    void setTexture();
};

#endif