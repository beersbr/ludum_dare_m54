#ifndef __BEHAVIORS_H__
#define __BEHAVIORS_H__

#include <iostream>
#include "behaviorBase.h"
#include "Entity.h"

#include "BGLController.h"
#include "BGLAudio.h"

#include "rapidjson/document.h"

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


class PlayerBehavior : public Behavior
{
public:
	glm::vec2 speed;
	glm::vec2 vel;

	float elapsedTime;

public:
	virtual void Initialize(rapidjson::Value configObject)
	{

	}

	virtual void Start()
	{

	}


	virtual void Update(float dt)
	{
		BGLInputState InputState = BGLController::GetInputState();

		speed = glm::vec2(150, 150);

		float dx = 0.0f,
			  dy = 0.0f;

		if(InputState.controllerSz > 0)
		{
			dx = InputState.controllers[0].LX * speed.x * dt;
			dy = InputState.controllers[0].LY * speed.y * dt;
		}

		if(InputState.keyboard.keys[SDLK_w].down)
		{
			dy = speed.y * dt * -1.0;
		}
		if(InputState.keyboard.keys[SDLK_s].down)
		{
			dy = speed.y * dt * 1.0;
		}
		if(InputState.keyboard.keys[SDLK_a].down)
		{
			dx = speed.x * dt * -1.0;
		}
		if(InputState.keyboard.keys[SDLK_d].down)
		{
			dx = speed.x * dt * 1.0;
		}

		actor->position.x += dx;
		actor->position.y += dy;

		//// NOTE(brett): Make sure the player does not leave the current view space
		//if(player.pos.x < camera.x)
		//{
		//	player.pos.x += camera.x - player.pos.x;
		//}
		//else if(player.pos.x > (camera.x + camera.w))
		//{
		//	player.pos.x -= player.pos.x - (camera.x + camera.w);
		//}
		//if(player.pos.y < camera.y)
		//{
		//	player.pos.y += camera.y - player.pos.y;
		//}
		//else if(player.pos.y > (camera.y + camera.h))
		//{
		//	player.pos.y -= player.pos.y - (camera.y + camera.h);
		//}

		//player.pos.x += cameraSpeed*dt;
	}


private:
	static BehaviorRegistery<PlayerBehavior> registration;
};

#endif
