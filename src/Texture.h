#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

class Texture {
public:
    Texture();

    Texture(const int &width, const int &height, const int &numComponents,
        const unsigned char *data, const bool &gammaCorrection = false);

    ~Texture();

    const unsigned int &getId() const;

    void clear() const;

private:
    unsigned int id;
};

#endif