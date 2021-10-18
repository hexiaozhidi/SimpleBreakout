#include "ResourceManager.h"

#include "stb_image.h"

#include <fstream>
#include <sstream>
#include <iostream>

Glyph::Glyph() : size(0), bearing(0), advance(0), texture() {
}

Glyph::Glyph(const glm::ivec2 &size, const glm::ivec2 &bearing, const int &advance, const unsigned char *data) :
    size(size), bearing(bearing), advance(advance), texture(size.x, size.y, 1, data) {
}

Shader ResourceManager::nullShader;
std::unordered_map<std::string, Shader> ResourceManager::shaders;

Texture ResourceManager::nullTexture;
std::unordered_map<std::string, Texture> ResourceManager::textures;

std::unordered_map<char, Glyph> ResourceManager::glyphs;

irrklang::ISoundEngine *ResourceManager::soundEngine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT,
    irrklang::ESEO_MULTI_THREADED | irrklang::ESEO_LOAD_PLUGINS | irrklang::ESEO_USE_3D_BUFFERS | irrklang::ESEO_PRINT_DEBUG_INFO_TO_DEBUGGER);

std::vector<irrklang::ISoundSource *> ResourceManager::backgroundMusics;
int ResourceManager::currentBackgroundMusicId = 0;
irrklang::ISound *ResourceManager::currentBackgroundMusic = nullptr;

std::unordered_map<std::string, irrklang::ISoundSource *> ResourceManager::soundEffects;


void ResourceManager::generateNullShader() {
    std::string vShaderCode(R"EOF(
    #version 330 core
    layout (location = 0) in vec2 AttribPosition;
    void main() { gl_Position = vec4(AttribPosition, 0.0, 1.0); }
    )EOF");

    std::string fShaderCode(R"EOF(
    #version 330 core
    out vec4 FragColor;
    void main() { FragColor = vec4(1.0, 1.0, 0.0, 1.0); }
    )EOF");

    nullShader = Shader(vShaderCode.c_str(), fShaderCode.c_str());
}

const Shader &ResourceManager::loadShader(const std::string &name, const char *vShaderPath, const char *fShaderPath, const char *gShaderPath) {
    std::ifstream vShaderFile, fShaderFile, gShaderFile;
    std::stringstream vShaderStream, fShaderStream, gShaderStream;
    std::string vShaderCode, fShaderCode, gShaderCode;

    // ensure that ifstream can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // load shader source code files
    try {
        vShaderFile.open(vShaderPath);
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vShaderCode = vShaderStream.str();

        fShaderFile.open(fShaderPath);
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fShaderCode = fShaderStream.str();

        if (gShaderPath) {
            gShaderFile.open(gShaderPath);
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            gShaderCode = gShaderStream.str();

            shaders.emplace(name, Shader(vShaderCode.c_str(), fShaderCode.c_str(), gShaderCode.c_str()));
        } else
            shaders.emplace(name, Shader(vShaderCode.c_str(), fShaderCode.c_str()));
    }
    catch (std::ifstream::failure &e) {
        std::cout << "ERROR: Failed to load shader source code from files:" << std::endl;
        std::cout << vShaderPath << std::endl << fShaderPath << std::endl;
        if (gShaderPath)
            std::cout << gShaderPath << std::endl;
        shaders.emplace(name, nullShader);
    }

    return shaders[name];
}

const Shader &ResourceManager::loadShader(const std::string &name, const std::string &vShaderCode, const std::string &fShaderCode, const std::string &gShaderCode) {
    if (gShaderCode != "")
        shaders.emplace(name, Shader(vShaderCode.c_str(), fShaderCode.c_str(), gShaderCode.c_str()));
    else
        shaders.emplace(name, Shader(vShaderCode.c_str(), fShaderCode.c_str()));

    return shaders[name];
}

const Shader &ResourceManager::getShader(const std::string &name) {
    if (name == "")
        return nullShader;

    if (shaders.find(name) == shaders.end()) {
        std::cout << "ERROR: Invalid shader name: " << name << std::endl;
        return nullShader;
    }

    return shaders[name];
}

void ResourceManager::generateNullTexture() {
    std::vector<unsigned char> data = { 255, 0, 255, 255 };
    nullTexture = Texture(1, 1, 4, &data[0]);
}

const Texture &ResourceManager::loadTexture(const std::string &name, const char *path, const bool &gammaCorrection) {
    int width, height, numComponents;
    unsigned char *data = stbi_load(path, &width, &height, &numComponents, 0);

    if (data)
        textures.emplace(name, Texture(width, height, numComponents, data, gammaCorrection));
    else {
        std::cout << "ERROR: Failed to load texture from file: " << path << std::endl;
        textures.emplace(name, nullTexture);
    }

    stbi_image_free(data);

    return textures[name];
}

const Texture &ResourceManager::getTexture(const std::string &name) {
    if (name == "")
        return nullTexture;

    if (textures.find(name) == textures.end()) {
        std::cout << "ERROR: Invalid texture name: " << name << std::endl;
        return nullTexture;
    }

    return textures[name];
}

void ResourceManager::loadFont(const char *path, const int &fontSize) {
    glyphs.clear();

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR: Failed to initialize FreeType library." << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face))
        std::cout << "ERROR: Failed to load font from file: " << path << std::endl;

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    for (unsigned char c = 0; c < 128; ++c) {
        if (!FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            glm::ivec2 size(face->glyph->bitmap.width, face->glyph->bitmap.rows);
            glm::ivec2 bearing(face->glyph->bitmap_left, face->glyph->bitmap_top);
            int advance = face->glyph->advance.x;
            unsigned char *data = face->glyph->bitmap.buffer;

            glyphs.emplace(c, Glyph(size, bearing, advance, data));
        } else
            std::cout << "ERROR: Failed to load glyph: " << c << std::endl;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

const Glyph &ResourceManager::getGlyph(const char &c) {
    return glyphs.find(c) == glyphs.end() ? glyphs['?'] : glyphs[c];
}

void ResourceManager::loadBackgroundMusic(const char *path, const float &volume) {
    irrklang::ISoundSource *soundSource = soundEngine->addSoundSourceFromFile(path);
    backgroundMusics.push_back(soundSource);
    soundSource->setDefaultVolume(volume);
}

void ResourceManager::loadSoundEffect(const std::string &name, const char *path, const float &volume) {
    irrklang::ISoundSource *soundSource = soundEngine->addSoundSourceFromFile(path);
    soundEffects.emplace(name, soundSource);
    soundSource->setDefaultVolume(volume);
}

void ResourceManager::playBackgroundMusic() {
    if (backgroundMusics.empty())
        return;
    else if (!currentBackgroundMusic) {
        currentBackgroundMusicId = time(nullptr) % backgroundMusics.size();
        currentBackgroundMusic = soundEngine->play2D(backgroundMusics[currentBackgroundMusicId], false, false, true);
    } else if (currentBackgroundMusic->isFinished()) {
        currentBackgroundMusicId = (currentBackgroundMusicId + 1) % backgroundMusics.size();
        currentBackgroundMusic = soundEngine->play2D(backgroundMusics[currentBackgroundMusicId], false, false, true);
    }
}

void ResourceManager::playSoundEffect(const std::string &name) {
    auto it = soundEffects.find(name);
    if (it != soundEffects.end() && it->second)
        soundEngine->play2D(it->second);
}

void ResourceManager::clear() {
    nullShader.clear();
    for (auto &p : shaders)
        p.second.clear();

    nullTexture.clear();
    for (auto &p : textures)
        p.second.clear();

    soundEngine->drop();
}