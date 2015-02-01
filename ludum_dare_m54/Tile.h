#pragma once

#include "language.h"
#include "Entity.h"
#include <glm/glm.hpp>


class Tile : public Entity
{
public:
    Tile(void);
    Tile(int srx, int sry, int posx, int posy);
    ~Tile(void);

    virtual void Update(float dt);
};

