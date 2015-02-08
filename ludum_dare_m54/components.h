#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "componentBase.h"
#include "Entity.h"
#include "behaviors.h"

/******************************************************************************************
PHYSICS COMPONENT
******************************************************************************************/

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


/******************************************************************************************
BEHAVIOR COMPONENT
******************************************************************************************/

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
		behavior = BehaviorFactoryBase::CreateInstance("cosine_enemy_behavior");
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



/******************************************************************************************
SPRITER COMPONENT
******************************************************************************************/

enum SpriteRenderLayer {
	FRONT_LAYER = 0,
	PLAYER_LAYER,
	ENEMY_LAYER,
	LEVEL_LAYER,
	BACKGROUND_LAYER,
	RESET_LAYER,
	SPRITE_LAYER_COUNT
};

class SpriteComponent : public Component
{
public:
	SpriteComponent(Entity *who) : Component(who)
	{
		entityComponents.push_back(this);
	}


	virtual void Initialize(std::unordered_map<std::string, float> args)
	{

	}

	virtual void Cleanup()
	{
		entityComponents.remove(this);
		owner = 0;
	}


	virtual ~SpriteComponent()
	{
		entityComponents.remove(this);
	}

public:
	static void Update(float dt);
	static std::string Family;

	static BGLSpriteBatch renderLayers[SPRITE_LAYER_COUNT];

	int32_t renderLayer;
	BGLSprite sprite;

private:
	static std::list<SpriteComponent *> entityComponents;
};
#endif