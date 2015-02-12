#ifndef __BEHAVIORS_H__
#define __BEHAVIORS_H__

#include <iostream>
#include "behaviorBase.h"
#include "Entity.h"

#include "BGLController.h"
#include "BGLAudio.h"

#include "rapidjson/document.h"
#include "components.h"
#include "Particles.h"
#include "platformdef.h"

class ExplosionBehavior : public Behavior
{
public:
	float aliveFor;
	float lifetime;
	int32_t explosions;
	float explosionPeriod;
	float explosionTimer;

	virtual void Start()
	{
		lifetime = 0.5f;
		aliveFor = 0.0f;
		explosions = 5.f;
		explosionPeriod = lifetime/explosions;
		explosionTimer = explosionPeriod;
	}

	virtual void Update(float dt)
	{
		explosionTimer += dt;
		aliveFor += dt;

		if(aliveFor >= lifetime)
		{
			Entity::Destroy(this->actor);
			return;
		}
		else if(explosionTimer >= explosionPeriod)
		{
			// TODO(brett): emit particles more generically with just a position
			float offsetX = RandomRange(-50, 50);
			float offsetY = RandomRange(-50, 50);
			glm::vec2 pos = glm::vec2(actor->position.x + offsetX, actor->position.y + offsetY);
			glm::vec2 size = glm::vec2(60, 60) * (2 * frand());
			ParticleHandler::Emit(pos, size, glm::vec3(), 0.2f);

			explosionTimer = 0.f;
		}
	}



private:
	static BehaviorRegistery<ExplosionBehavior> registration;
};

class SimpleEnemyBehavior : public Behavior
{
public:
	float speed;
	float lifetime;

	virtual void Start()
	{
		speed = 100.f;
		lifetime = 0.f;
	}

	virtual void OnCollide(Entity *other)
	{
		if(other->tag == "bullet")
		{
			Entity::Destroy(other);
			Entity::Destroy(this->actor);

			Entity *e = Entity::Create(actor->position, glm::vec2(2,2), glm::vec3());
			e->AddComponent<BehaviorComponent>("explosion");
		}
	}

	virtual void Update(float dt)
	{
		lifetime += dt;

		actor->position.x += -speed*dt;
		actor->position.y = 200.f * sinf(lifetime*1.2f) + 400;
	}

private:
	static BehaviorRegistery<SimpleEnemyBehavior> registration;
};

class PlayerBehavior : public Behavior
{
public:
	glm::vec2 speed;
	glm::vec2 vel;

	float elapsedTime;
	float shootCounter;
	float shootPeriod;

public:
	virtual void Initialize(rapidjson::Value configObject)
	{

	}

	virtual void Start()
	{
		speed = glm::vec2(80, 80);
		shootPeriod = 0.18f;
		shootCounter = 0.25f;
	}

	void Shoot()
	{
		if(shootCounter >= shootPeriod)
		{
			// create the entity to shoot
			Entity *e = Entity::Create("bullet", actor->position, glm::vec2(16, 16), glm::vec3());
			e->AddComponent<SpriteComponent>("bullet");
			e->AddComponent<BehaviorComponent>("bullet");
			e->AddComponent<PhysicsComponent>("");

			shootCounter = 0.0f;
		}
	}

	virtual void Update(float dt)
	{
		BGLInputState InputState = BGLController::GetInputState();

		float dx = 0.0f,
			  dy = 0.0f;

		float rz = 0.0f;

		if(InputState.controllerSz > 0)
		{
			dx = InputState.controllers[0].LX * speed.x * dt;
			dy = InputState.controllers[0].LY * speed.y * dt;

			rz = InputState.controllers[0].RX * dt;
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


		shootCounter += dt;

		actor->rotation.z += rz;

		vel = vel + glm::vec2(dx, dy);
		vel = vel * 0.88f;

		actor->position += vel;

		//std::cout << vel.x << ", " << vel.y << std::endl;

		glm::vec2 orScale = glm::vec2(80, 40);
		actor->scale = glm::vec2(
			orScale.x - (orScale.x * ( fabs(vel.y) / 100.0f)),
			orScale.y - (orScale.y * ( fabs(vel.x) / 100.0f)));


		// shoot bullets
		if(InputState.controllerSz > 0 && InputState.controllers[0].X.down )
		{
			Shoot();
		}

		if(InputState.keyboard.keys[SDLK_j].down)
		{
			Shoot();
		}

		// TODO(brett): should probably move this to the sprite component
		local_persist int32_t frameCount = 0;
		frameCount += 1;
		
		SpriteComponent *sprite = actor->GetComponent<SpriteComponent>();
		if(dy < -0.9f)
		{
			sprite->SetAnimation("up");
		}
		else if(dy > 0.9f)
		{
			sprite->SetAnimation("down");
		}
		else
		{
			sprite->SetAnimation("normal");
			if(!(frameCount % 6))
			{
				sprite->sprite.AdvanceAnimationFrame();
			}
		}

		//// NOTE(brett): Make sure the player does not leave the current view space

		glm::vec4 camera = sprite->Camera();
		if(actor->position.x < camera.x)
		{
			actor->position.x += camera.x - actor->position.x;
		}
		else if(actor->position.x > (camera.x + camera.z))
		{
			actor->position.x -= actor->position.x - (camera.x + camera.z);
		}
		if(actor->position.y < camera.y)
		{
			actor->position.y += camera.y - actor->position.y;
		}
		else if(actor->position.y > (camera.y + camera.w))
		{
			actor->position.y -= actor->position.y - (camera.y + camera.w);
		}

		//actor->position.x += cameraSpeed*dt;
	}


private:
	static BehaviorRegistery<PlayerBehavior> registration;
};



class PlayerBulletBehavior : public Behavior
{
public:
	glm::vec2 velocity;

	virtual void Start()
	{
		velocity.x = 2000.f;
	}

	virtual void OnCollide(Entity *e)
	{

	}

	virtual void Update(float dt)
	{
		actor->position += velocity * dt;

		if(actor->position.x > 100000.0f)
		{
			actor->Delete();
		}

	}



private:
	static BehaviorRegistery<PlayerBulletBehavior> registration;
};

#endif
