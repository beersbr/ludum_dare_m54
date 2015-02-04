#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <unordered_map>
#include <list>
#include "BGL.h"
#include "BGLAudio.h"
#include "SpriteSheet.h"
#include "component.h"

// Entity describes an object that can be interacted with in a game, this includes PLAYER, ENEMY, BULLET
// The entity will be considered a drawable object, so needs to include a spritemap
class Entity
{
public:
    Entity(void);
	Entity(bool dontTrack);
    ~Entity(void);

    glm::vec2 pos;                      // x,y position in top left cornder
    //glm::vec2 dir;                    // Direction? Might incldue facing for enemy firing, think on this one.
    glm::vec2 vel;                      // current vlocity in x,y direction
    glm::vec2 size;                    // How big is the object, to the game?
    std::vector<SpriteSheet*>* sheetData; //All sprite sheets this entity can use
    SpriteSheet* spriteSheet;		    // currently used spritesheet

    BGLSprite sprite;                      // Actual BGL sprite
    int curFrame;                       // Current frame of animation as dictated by the sprite sheet class

	int32_t typeId;						// this is the id for entities to be able to differ from one another

	virtual void Update(float dt);	
	virtual void Render();


	template <typename T>
	void AddComponent(std::string name)
	{
		// TODO(brett): if the component already exists we need to get rid of the old one
		// if(components[T::family]) ...

		components[T::Family] = new T(this);

		// NOTE(brett): this is where we would fetch some data from a json file
		// std::unordered_map<std::string, float> data = resource_handler[T::family][name]

		std::unordered_map<std::string, std::unordered_map<std::string, float> > data;
		data["player_simple_projectile"]["vx"] = 2000.0f;
		data["player_simple_projectile"]["vy"] = 0.0f;

		data["enemy_simple_ai"]["vx"] = -200.0f;
		data["enemy_simple_ai"]["vy"] = 0.0f;

		components[T::Family]->name = name;
		components[T::Family]->Initialize(data[name]);
	}


	template <typename T>
	T* GetComponent()
	{
		return (T *)components[T::Family];
	}

	// TODO(brett): see if we can manipulate entities purely with data
	//void addComponent(std::string family, std::string type);
	//void removeComponent(std::string family);
	std::unordered_map<std::string, Component *> components;


	// TODO(brett): this is just for testing
	static std::vector<Entity *> createdEntities;
};


// NOTE(brett): this will all be moved. Just here for testing.
class KinematicComponent : public Component
{
public:
	KinematicComponent(Entity *who)
	{
		owner = who;
		entityComponents.push_back(this);
	}

	

	virtual void Initialize(std::unordered_map<std::string, float> args)
	{
		vel.x = args["vx"];
		vel.y = args["vy"];
	}

	static void Update(float dt);

	static std::string Family;
public:
	glm::vec2 vel;

private:
	static std::list<KinematicComponent *> entityComponents;
};


class AIComponent : public Component
{
public:
	AIComponent(Entity *who)
	{
		owner = who;
		entityComponents.push_back(this);
		liveTime = 0.0f;
		spawnPos = owner->pos;
	}


	virtual void Initialize(std::unordered_map<std::string, float> args)
	{
		// TODO(brett): need values for an ai component. Probably a "script" which would just be a
		// class defined somewhere else that would dictate functionality

	}

	static void Update(float dt);

public:
	static std::string Family;

	// NOTE(brett): this might always be the player and can probably
	// be static
	Entity *target;

	float liveTime;
	glm::vec2 spawnPos;


private:
	static std::list<AIComponent *> entityComponents;
};


static void 
createPlayerBullet(glm::vec2 pos, glm::vec2 size)
{
	// NOTE(brett): Entity should provide a pool to add entities to. For now I'm throwing it on the heap

	// NOTE(brett): this would pull from the kinematic :: create_player_bullet tag and just give the data to the
	// appropriate components (drawable, kinematic, collidable, etc)
	Entity *e = new Entity(false);
	e->sprite = BGLSprite::Create("spritesheet", "", size.x, size.y, 0, 1, &BGLRectMake(0, 96, 16, 16));
	e->pos = pos;

	e->AddComponent<KinematicComponent>("player_simple_projectile");
}

static void
createPlayerEnemy(glm::vec2 pos, glm::vec2 size)
{
	Entity *e = new Entity(false);
	e->sprite = BGLSprite::Create("spritesheet", "", size.x, size.y, 0, 1, &BGLRectMake(32, 0, 32, 16));
	e->pos = pos;

	e->AddComponent<KinematicComponent>("enemy_simple_ai");
	e->AddComponent<AIComponent>("");
}