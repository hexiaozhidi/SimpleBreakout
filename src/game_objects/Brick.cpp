#include "Brick.h"

#include <iostream>

Brick::Brick() : GameObject(), material(BrickMaterial::UNKNOWN), solidLevel(0) {
}

Brick::Brick(const BrickMaterial &material, const glm::vec2 &position, const glm::vec2 &size, const Color &tintColor) :
    GameObject(position, size, 0.0f, tintColor), material(material), solidLevel(0) {

    reset();
}

void Brick::reset() {
    switch (material) {
        case BrickMaterial::GLASS:
            setSolidLevel(1);
            break;
        case BrickMaterial::ROCK:
            setSolidLevel(2);
            break;
        case BrickMaterial::HARDROCK:
            setSolidLevel(3);
            break;
        case BrickMaterial::STEEL:
            setSolidLevel(-1);
            break;
        default:
            std::cout << "ERROR: Unknown brick material." << std::endl;
            break;
    }
}

const BrickMaterial &Brick::getMaterial() const {
    return material;
}

const int &Brick::getSolidLevel() const {
    return solidLevel;
}

void Brick::setSolidLevel(const int &value) {
    solidLevel = value;
    if (solidLevel)
        setTexture();
}

void Brick::setTexture() {
    switch (material) {
        case BrickMaterial::GLASS:
            texture = ResourceManager::getTexture("glass");
            break;
        case BrickMaterial::ROCK:
            texture = ResourceManager::getTexture(solidLevel == 2 ? "rock" : "rock_cracks");
            break;
        case BrickMaterial::HARDROCK:
            texture = ResourceManager::getTexture(solidLevel == 3 ? "hardrock" : solidLevel == 2 ? "hardrock_cracks" : "hardrock_more_cracks");
            break;
        case BrickMaterial::STEEL:
            texture = ResourceManager::getTexture("steel");
            break;
        default:
            std::cout << "ERROR: Unknown brick material." << std::endl;
            break;
    }
}