#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include "language.h"
#include "BGL.h"
#include "Resourcer.h"


// Translates a given index to a location and size on the sprite
// This gets the coordinates for all the different animations and sets up the frame.
// When we want to change to a certain animation, we will have the frame data here.
class SpriteSheet
{
public:
    SpriteSheet(void);
    ~SpriteSheet(void);
    BGLRect* getFrameArray();
    //std::string entityID; //Entity this sprite sheet is related to | D: I don't think I have a good use for this yet.
    std::string animTag; //Descriptor of which frames this sprite is
    std::vector<BGLRect> frameData; //Each index in the this vector will return the information representing where on the sprite sheet that the drawer needs to draw.
};

