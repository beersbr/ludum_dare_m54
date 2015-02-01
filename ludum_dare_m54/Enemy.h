#pragma once
#include "Entity.h"
class Enemy : public Entity
{
public:
    Enemy(void);
    ~Enemy(void);

    virtual void Update(float dt);
};

