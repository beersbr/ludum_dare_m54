#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include <glm/gtx/transform.hpp>

#include "componentBase.h"
#include "Entity.h"
#include "behaviorBase.h"

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
		//behavior = BehaviorFactoryBase::CreateInstance("cosine_enemy_behavior");

		if(args["name"] == 1.f)
		{
			behavior = BehaviorFactoryBase::CreateInstance("player");
		}
		if(args["name"] == 2.f)
		{
			behavior = BehaviorFactoryBase::CreateInstance("bullet");
		}

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
	UI_LAYER = 0,
	LEVEL_FOREGROUND,
	DOODAD,
	PLAYER_LAYER,
	ENEMY_LAYER,
	LEVEL_BACKGROUND,
	BACKGROUND_FOREGROUND,
	BACKGROUND_BACKGROUND,
	SPRITE_LAYER_COUNT
};

class SpriteComponent : public Component
{
public:
	SpriteComponent(Entity *who) : Component(who)
	{
		entityComponents.push_back(this);
	}

	// This component needs a start up so that we can create the batchers 
	// AFTER glew has been initialized. This allows for the static member 
	// AND for no 0x0 execution errors
	static void InitializeSpriteComponent()
	{
		for(int32_t i = 0; i < SPRITE_LAYER_COUNT; ++i)
		{
			renderLayers[i] = new BGLSpriteBatch();
			renderLayers[i]->shader = BGLShaderHandler::Get("spriteEx");

			camera[i] = glm::vec4(0, 0, 1200, 800);

		}
	}

	virtual void Initialize(std::unordered_map<std::string, float> args)
	{

		layer = args["layer"];

		assert(layer < SPRITE_LAYER_COUNT);

		if(args["texture"] == 0.f)
		{
			BGLBatchTexture t = BGLBatchTextureHandler::GetArrayTexture("background");
			BGLRect frame = { 0, 0, 600, 400 };
			sprite = BGLSprite::Create(t, 1, (BGLRect *)&frame);
		}
		if(args["texture"] == 1.f)
		{
			BGLBatchTexture t = BGLBatchTextureHandler::GetArrayTexture("spritesheet");
			BGLRect frames[2] = {
				{0, 0, 32, 16},
				{0, 32, 32, 16}
			};
			sprite = BGLSprite::Create(t, 2, (BGLRect *)&(frames[0]));
			sprites["normal"] = sprite;
			sprites["up"] =  BGLSprite::Create(t, 1, (BGLRect *)&(BGLRectMake( 0, 64, 32, 16 )));
			sprites["down"] =  BGLSprite::Create(t, 1, (BGLRect *)&(BGLRectMake( 0, 48, 32, 16)));
		}
		if(args["texture"] == 2.f)
		{
			BGLBatchTexture t = BGLBatchTextureHandler::GetArrayTexture("spritesheet");
			BGLRect frame = { 0, 80, 16, 16};
			sprite = BGLSprite::Create(t, 1, (BGLRect *)&frame);
		}
		if(args["texture"] == 3.f)
		{
			BGLBatchTexture t = BGLBatchTextureHandler::GetArrayTexture("spritesheet");
			BGLRect frame = { 32, 208, 16, 16};
			sprite = BGLSprite::Create(t, 1, (BGLRect *)&frame);
		}

		if(args["texture"] == 4.f)
		{
			BGLBatchTexture t = BGLBatchTextureHandler::GetArrayTexture("spritesheet");
			BGLRect frame = { 0, 208, 16, 16};
			sprite = BGLSprite::Create(t, 1, (BGLRect *)&frame);
		}
	}

	glm::vec4 Camera()
	{
		return camera[layer];
	}

	void SetAnimation(std::string tag)
	{
		sprite = sprites[tag];
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

	static glm::mat4 ProjectionMatrix;
	static BGLSpriteBatch *renderLayers[SPRITE_LAYER_COUNT];
	static glm::vec4 camera[SPRITE_LAYER_COUNT];

	int32_t layer;
	BGLSprite sprite;

	std::unordered_map<std::string, BGLSprite> sprites;


private:
	static std::list<SpriteComponent *> entityComponents;
};

#endif