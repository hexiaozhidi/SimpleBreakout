#include "Level.h"

#include <fstream>
#include <sstream>
#include <iostream>

Level::Level() : numDestroyableBricks(0), numDestroyedBricks(0), bricks() {
}

Level::Level(const char *path, const float &bricksAreaWidth, const float &bricksAreaHeight) :
    numDestroyableBricks(0), numDestroyedBricks(0), bricks() {

    std::ifstream file(path);
    if (!file) {
        //std::cout << "ERROR: Failed to load level code from file: " << path << std::endl;
        return;
    }

    std::vector<std::vector<std::string>> data;
    std::string line, word;
    int numCols = 0;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        data.emplace_back();

        while (lineStream >> word) {
            if (word.size() != 2) {
                std::cout << "ERROR: Invalid level code: " << word << " from file: " << path << std::endl;
                return;
            }

            data.back().push_back(word);
        }

        if (data.back().empty())
            data.pop_back();
        else if (data.back().size() > numCols)
            numCols = data.back().size();
    }

    int numRows = data.size();
    if (!numRows) {
        std::cout << "ERROR: Empty level from file: " << path << std::endl;
        return;
    }

    glm::vec2 size(bricksAreaWidth / numCols, bricksAreaHeight / numRows);
    for (int i = 0; i < numRows; ++i) {
        int n = data[i].size();
        for (int j = 0; j < n; ++j) {
            BrickMaterial material = getMaterial(data[i][j][0]);
            if (material == BrickMaterial::UNKNOWN)
                continue;
            else if (material != BrickMaterial::STEEL)
                ++numDestroyableBricks;

            glm::vec2 position(j * size.x, i * size.y);
            Color tintColor = getTintColor(data[i][j][1], data[i][j][0] == 'G' ? 0.6f : 1.0f);
            bricks.emplace_back(material, position, size, tintColor);
        }
    }

    if (!numDestroyableBricks) {
        std::cout << "ERROR: No destroyable bricks in the level from file: " << path << std::endl;
        bricks.clear();
    }
}

void Level::updateNumDestroyedBricks() {
    ++numDestroyedBricks;
}

bool Level::isCompleted() const {
    return numDestroyableBricks == numDestroyedBricks;
}

void Level::reset() {
    numDestroyedBricks = 0;
    for (auto &brick : bricks) {
        brick.reset();
    }
}

std::vector<Brick> &Level::getBricks() {
    return bricks;
}

BrickMaterial Level::getMaterial(const char &c) const {
    switch (c) {
        case 'G': case 'g':
            return BrickMaterial::GLASS;
        case 'R': case 'r':
            return BrickMaterial::ROCK;
        case 'H': case 'h':
            return BrickMaterial::HARDROCK;
        case 'S': case 's':
            return BrickMaterial::STEEL;
        default:
            break;
    }

    return BrickMaterial::UNKNOWN;
}

Color Level::getTintColor(const char &c, const float &alpha) const {
    Color tintColor = Palette::getColorByName("white");
    switch (c) {
        case '1':
            tintColor = Palette::getColorByName("red");
            break;
        case '2':
            tintColor = Palette::getColorByName("orange");
            break;
        case '3':
            tintColor = Palette::getColorByName("yellow");
            break;
        case '4':
            tintColor = Palette::getColorByName("green");
            break;
        case '5':
            tintColor = Palette::getColorByName("blue");
            break;
        case '6':
            tintColor = Palette::getColorByName("purple");
            break;
        case '7':
            tintColor = Palette::getColorByName("gray");
            break;
        case 'R': case 'r':
            tintColor = Palette::getColorByName("rock");
            break;
        case 'H': case 'h':
            tintColor = Palette::getColorByName("hardrock");
            break;
        case 'S': case 's':
            tintColor = Palette::getColorByName("steel");
            break;
        default:
            break;
    }

    tintColor.a = alpha;
    return tintColor;
}