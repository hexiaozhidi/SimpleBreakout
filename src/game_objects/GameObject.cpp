#include "GameObject.h"

GameObject::GameObject() :
    position(0.0f), size(0.0f), rotationAngle(0.0f), tintColor(1.0f), texture(ResourceManager::getTexture()) {
}

GameObject::GameObject(const glm::vec2 &position, const glm::vec2 &size, const float &rotationAngle,
    const Color &tintColor, const Texture &texture) :
    position(position), size(size), rotationAngle(rotationAngle), tintColor(tintColor), texture(texture) {
}

const glm::vec2 &GameObject::getPosition() const {
    return position;
}

const glm::vec2 &GameObject::getSize() const {
    return size;
}

const float &GameObject::getRotationAngle() const {
    return rotationAngle;
}

const Color &GameObject::getTintColor() const {
    return tintColor;
}

const Texture &GameObject::getTexture() const {
    return texture;
}

void GameObject::setPosition(const glm::vec2 &value) {
    position = value;
}

void GameObject::setSize(const glm::vec2 &value) {
    size = value;
}

void GameObject::setRotationAngle(const float &value) {
    rotationAngle = value;
}

void GameObject::setTintColor(const Color &value) {
    tintColor = value;
}

void GameObject::setTexture(const Texture &value) {
    texture = value;
}