#include "Texture.h"

#include <glad/glad.h>

#include <iostream>

Texture::Texture() : id(0) { }

Texture::Texture(const int &width, const int &height, const int &numComponents,
    const unsigned char *data, const bool &gammaCorrection) : id(0) {

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    switch (numComponents) {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
            break;
        case 2:
            std::cout << "ERROR: Found texture with 2 components." << std::endl;
            glBindTexture(GL_TEXTURE_2D, 0);
            return;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, gammaCorrection ? GL_SRGB : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        default:
            glTexImage2D(GL_TEXTURE_2D, 0, gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
}

const unsigned int &Texture::getId() const {
    return id;
}

void Texture::clear() const {
    glDeleteTextures(1, &id);
}