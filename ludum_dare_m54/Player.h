#pragma once

#include "Entity.h"
#include "BGLController.h"

class Player : public Entity
{
public:
    Player(void);
    ~Player(void);

	virtual void Update(float dt);
	void Update(BGLInputState *state, float dt);
	virtual void Render();

    int curHealth;
	float moveSpeed;
	float shootBulletsInterval;
	float shootBulletsElapsed;

	float animationFrameInterval;
	float currentFrameTime; 
};

