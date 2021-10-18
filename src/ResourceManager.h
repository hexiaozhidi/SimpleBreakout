#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <irrklang/irrKlang.h>

#include <string>
#include <unordered_map>

struct Glyph {
    glm::ivec2 size;
    glm::ivec2 bearing; // vector from the origin of the glyph, to the top-left point of the glyph
    int advance; // distance (in 1 / 64 px) from the origin of the glyph, to the origin of the next glyph
    Texture texture; // 1-component alpha value texture 

    Glyph();

    Glyph(const glm::ivec2 &size, const glm::ivec2 &bearing, const int &advance, const unsigned char *data);
};

class ResourceManager {
public:
    static void generateNullShader();

    static const Shader &loadShader(const std::string &name, const char *vShaderPath, const char *fShaderPath, const char *gShaderPath = nullptr);
    
    static const Shader &loadShader(const std::string &name, const std::string &vShaderCode, const std::string &fShaderCode, const std::string &gShaderCode = "");

    static const Shader &getShader(const std::string &name = "");

    static void generateNullTexture();

    static const Texture &loadTexture(const std::string &name, const char *path, const bool &gammaCorrection = false);

    static const Texture &getTexture(const std::string &name = "");

    static void loadFont(const char *path, const int &fontSize);

    static const Glyph &getGlyph(const char &c);

    static void loadBackgroundMusic(const char *path, const float &volume = 1.0f);

    static void loadSoundEffect(const std::string &name, const char *path, const float &volume = 1.0f);

    static void playBackgroundMusic();

    static void playSoundEffect(const std::string &name);

    static void clear();

private:
    static Shader nullShader;
    static std::unordered_map<std::string, Shader> shaders;

    static Texture nullTexture;
    static std::unordered_map<std::string, Texture> textures;

    static std::unordered_map<char, Glyph> glyphs;

    static irrklang::ISoundEngine *soundEngine;

    static std::vector<irrklang::ISoundSource *> backgroundMusics;
    static int currentBackgroundMusicId;
    static irrklang::ISound *currentBackgroundMusic;

    static std::unordered_map<std::string, irrklang::ISoundSource *> soundEffects;
};

#endif