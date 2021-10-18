#ifndef LEVEL_H
#define LEVEL_H

#include "game_objects/Brick.h"

#include <vector>

class Level {
public:
    Level();

    Level(const char *path, const float &bricksAreaWidth, const float &bricksAreaHeight);

    void updateNumDestroyedBricks();

    bool isCompleted() const;

    void reset();

    std::vector<Brick> &getBricks();

private:
    int numDestroyableBricks;
    int numDestroyedBricks;

    std::vector<Brick> bricks;

    BrickMaterial getMaterial(const char &c) const;

    Color getTintColor(const char &c, const float &alpha = 1.0f) const;
};

#endif