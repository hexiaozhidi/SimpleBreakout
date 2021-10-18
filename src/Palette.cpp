#include "palette.h"

#include <iostream>

const std::unordered_map<std::string, Color> Palette::colorDict = {
    { "black", getColorByHex("#000000") },
    { "blue", getColorByHex("#66ccff") },
    { "gray", getColorByHex("#cccccc") },
    { "green", getColorByHex("#339966") },
    { "orange", getColorByHex("#ff9933") },
    { "purple", getColorByHex("#ccccff") },
    { "red", getColorByHex("#ff6666") },
    { "white", getColorByHex("#ffffff") },
    { "yellow", getColorByHex("#ffff99") },
    { "rock", getColorByHex("#faf3df") },
    { "hardrock", getColorByHex("#fa9e57") },
    { "steel", getColorByHex("#d6d6d6") }
};

std::vector<Color> Palette::colors;

const Color Palette::getColorByRGB(const int &r, const int &g, const int &b) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        std::cout << "ERROR: Invalid RGB value: (" << r << ", " << g << ", " << b << ")" << std::endl;
        return Color(1.0f);
    }

    return Color(glm::vec3(r, g, b) / 255.0f, 1.0f);
}

const Color Palette::getColorByHex(const std::string &hexColorCode) {
    if (hexColorCode.empty()) {
        std::cout << "ERROR: Empty hex color code." << std::endl;
        return Color(1.0f);
    }

    std::string hex = hexColorCode[0] == '#' ? hexColorCode.substr(1) : hexColorCode;
    if (hex.size() == 3)
        hex = std::string(2, hex[0]) + std::string(2, hex[1]) + std::string(2, hex[2]);
    else if (hex.size() != 6) {
        std::cout << "ERROR: Invalid hex color code: " << hexColorCode << std::endl;
        return Color(1.0f);
    }

    std::vector<int> dec(6);
    for (int i = 0; i < 6; ++i)
        if (hex[i] >= '0' && hex[i] <= '9')
            dec[i] = hex[i] - '0';
        else if (hex[i] >= 'A' && hex[i] <= 'F')
            dec[i] = hex[i] - 'A' + 10;
        else if (hex[i] >= 'a' && hex[i] <= 'f')
            dec[i] = hex[i] - 'a' + 10;
        else {
            std::cout << "ERROR: Invalid hex color code: " << hexColorCode << std::endl;
            return Color(1.0f);
        }

    return Color((glm::vec3(dec[0], dec[2], dec[4]) * 16.0f + glm::vec3(dec[1], dec[3], dec[5])) / 255.0f, 1.0f);
}

const Color Palette::getColorByName(const std::string &name) {
    auto it = colorDict.find(name);
    if (it == colorDict.end()) {
        std::cout << "ERROR: Invalid color name: " << name << std::endl;
        return Color(1.0f);
    }

    return it->second;
}

const Color Palette::getRandomColorFromPalette() {
    if (colors.empty())
        for (auto &p : colorDict)
            colors.push_back(p.second);

    return colors[rand() % colors.size()];
}

const Color Palette::getRandomColor() {
    return Color(rand() % 1001 * 0.001f, rand() % 1001 * 0.001f, rand() % 1001 * 0.001f, 1.0f);
}