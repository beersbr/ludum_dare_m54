#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "component.h"
#include "Entity.h"

// NOTE(brett): this will all be moved. Just here for testing.
// TODO(brett): This component is not needed anymore. These type of things will be done on the script
class KinematicComponent : public Component
{
public:
	KinematicComponent(Entity *who) : Component(who)
	{
	
		entityComponents.push_back(this);
	}

	virtual ~KinematicComponent()
	{
		entityComponents.remove(this);
	}

	virtual void Initialize(std::unordered_map<std::string, float> args)
	{
		vel.x = args["vx"];
		vel.y = args["vy"];
	}


	virtual void Cleanup()
	{
		entityComponents.remove(this);
		owner = 0;
	}

	static void Update(float dt);

	static std::string Family;
public:
	glm::vec2 vel;

private:
	static std::list<KinematicComponent *> entityComponents;
};

enum PhysicsColliderType { CIRCLE, AABB };
class PhysicsComponent : public Component
{
public:
	PhysicsComponent(Entity *who) : Component(who)
	{
		entityComponents.push_back(this);
	}

	virtual void Initialize(std::unordered_map<std::string, float> args)
	{
		type = args["type"];
		radius = args["radius"];
		width = args["width"];
		height = args["height"];
		offsetX = args["offsetX"];
		offsetY = args["offsetY"];
	}

	virtual ~PhysicsComponent()
	{
		entityComponents.remove(this);
	}

	virtual void Cleanup()
	{
		entityComponents.remove(this);
		owner = 0;
	}

	static void Update(float dt);

public:
	static std::string Family;

	glm::vec2 size;
	float radius;

	float width;
	float height;

	float offsetX;
	float offsetY;

	int32_t type;

private:
	static std::list<PhysicsComponent *> entityComponents;
};


class Behavior
{
public:

	Behavior()
	{
		actor = 0;
	}

	~Behavior()
	{
		actor = 0;
	}

	// This happens when a behavior is loaded by the system
	virtual void Load(Entity *owner)
	{ 
		actor = owner; 
	}

	// NOTE(brett): we can do things based on events here because entities are tagged
	// NOTE(brett): this function would be called by a collider comopnent in the physics family
	//virtual void OnCollide(Entity *other){}

	virtual void OnCollide(Entity *other){}

	// This is where any startup code from the programmer should go
	// This is called after Load
	virtual void Start(){}

	// This is called each update step of the game
	virtual void Update(float dt){}
	virtual void End(){}

public:
	Entity *actor;
};


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

		start_x = actor->pos.x;
		start_y = actor->pos.y;
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

};

// The behavior comopnent gives all the other components a purpose. It allows the
// entity to respond to any event that any of the other components might throw.
//
// NOTE(brett): Perhaps the entity should just have a list of behavior 'scripts'
// and the behavior component gives an api into manipulating that variable
class BehaviorComponent : public Component
{
public:
	BehaviorComponent(Entity *who) : Component(who)
	{
		entityComponents.push_back(this);
	}
	
	virtual void Initialize(std::unordered_map<std::string, float> args)
	{
		// NOTE(brett): maybe the behaviors (when loaded, probably at the game load) will be put into a map
		// that this object can then use to create them from -- as they will be in a dynamic lib
		behavior = new CosineEnemyBehavior();
		behavior->Load(owner);
		behavior->Start();
	}

	virtual ~BehaviorComponent()
	{
		entityComponents.remove(this);
	}

	virtual void Cleanup()
	{
		entityComponents.remove(this);
		owner = 0;
		delete behavior;
	}

public:
	static void Update(float dt);
	static std::string Family;

public:
	Behavior *behavior;

private:
	static std::list<BehaviorComponent *> entityComponents;
};

#endif