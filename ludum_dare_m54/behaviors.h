#ifndef __BEHAVIORS_H__
#define __BEHAVIORS_H__

#include <iostream>
#include "behaviorBase.h"

class CosineEnemyBehavior : public Behavior
{
public:
	float start_x;
	float start_y;
	float elapsedTime;

	virtual void Start()
	{
		// NOTE(brett): could grab stuff out of the entities transform data OR could do something if
		// we have collided

		start_x = 0; //actor->pos.x;
		start_y = 0; //actor->pos.y;
		elapsedTime = 0.0f;
	}

	virtual void OnCollide(Entity *e)
	{
		std::cout << actor->tag << " collided with " << e->tag << std::endl;
		if(e->tag == "Bullet")
		{
			e->Delete();
			actor->Delete();
		}
	}

	virtual void Update(float dt)
	{
		elapsedTime += dt;


		// NOTE(brett): Because the position of the entity is part of the entity
		// I am just manipulating it directly instead of using a transform.
		
		// TODO(brett): add a transform component that would hold all the transformations
		// that an entity could have. That includes rotation and scaling
		// TODO(brett): that would mean that the renderer needs to change a big to be
		// a little smarter on how we access data.
		actor->pos.y = 150.0 * cosf(elapsedTime * 4.0f) + 400;
		actor->pos.x -= 100.0 * dt;
	}
	
private:
	static BehaviorRegistery<CosineEnemyBehavior> registration;
};

#endif