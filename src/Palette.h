#ifndef PALETTE_H
#define PALETTE_H

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

using Color = glm::vec4;

class Palette {
public:
    static const Color getColorByRGB(const int &r, const int &g, const int &b);

    static const Color getColorByHex(const std::string &hexColorCode);

    static const Color getColorByName(const std::string &name);

    static const Color getRandomColorFromPalette();

    static const Color getRandomColor();

private:
    static const std::unordered_map<std::string, Color> colorDict;
    static std::vector<Color> colors;
};

#endif