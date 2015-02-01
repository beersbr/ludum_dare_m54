#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
    Player(void);
    ~Player(void);

	virtual void Update(float dt);

private:
    int curHealth;
};

