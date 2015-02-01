#pragma once

#include <glm\glm.hpp>
#include <vector>
#include "BGL.h"
#include "SpriteSheet.h"

// Entity describes an object that can be interacted with in a game, this includes PLAYER, ENEMY, BULLET
// The entity will be considered a drawable object, so needs to include a spritemap
class Entity
{
public:
    Entity(void);
    ~Entity(void);
    glm::vec2 pos;                    // x,y position in top left cornder
    //glm::vec2 dir;                  // Direction? Might incldue facing for enemy firing, think on this one.
    glm::vec2 vel;                    // current vlocity in x,y direction
    glm::vec2 size;                   // How big is the object, to the game?
    SpriteSheet* spriteSheet;		  // currently used spritesheet
    Sprite sprite;                    // Actual BGL sprite
    int curFrame;                     // Current frame of animation as dictated by the sprite sheet class


	virtual void Update(float dt) = 0;

};

