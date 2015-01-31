#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "language.h"
#include "BGL.h"


//Translates a given index to a location and size on the sprite
class SpriteSheet
{
public:
    SpriteSheet(void);
    ~SpriteSheet(void);
    BGLRect* getFrameArray();
    std::vector<BGLRect> frameVect; //Each index in the this vector will return the information representing where on the sprite sheet that the drawer needs to draw.
};

