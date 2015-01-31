#pragma once
#include <glm/glm.hpp>
#include "language.h"

class Tile
{
public:
    Tile(void);
    Tile(int srx, int sry, int posx, int posy);
    ~Tile(void);
    glm::vec2 src; //Source on the tile map
    glm::vec2 pos; //Location in the game
};

