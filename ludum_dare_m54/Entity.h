#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <unordered_map>
#include <list>
#include "BGL.h"
#include "BGLAudio.h"
#include "SpriteSheet.h"
#include "component.h"


enum EntityType { 
	ENEMY, 
	PROJECTILE, 
	TILE, 
	ENTITY_TYPE_COUNT
};

// Entity describes an object that can be interacted with in a game, this includes PLAYER, ENEMY, BULLET
// The entity will be considered a drawable object, so needs to include a spritemap
class Entity
{
public:
    Entity(void);
	Entity(bool dontTrack);
    ~Entity(void);

	EntityType typeId;
	std::string tag;

	glm::mat4 transform;		// this is going to be the position fo the entity as well as the rotation and scaling
	bool deleted;
	void Delete();

    glm::vec2 pos;                      // x,y position in top left cornder
    //glm::vec2 dir;                    // Direction? Might incldue facing for enemy firing, think on this one.
    glm::vec2 vel;                      // current vlocity in x,y direction
    glm::vec2 size;                    // How big is the object, to the game?
    std::vector<SpriteSheet*>* sheetData; //All sprite sheets this entity can use
    SpriteSheet* spriteSheet;		    // currently used spritesheet

    BGLSprite sprite;                      // Actual BGL sprite
    int curFrame;                       // Current frame of animation as dictated by the sprite sheet class

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

		data["AABB"]["type"]    = AABB;
		data["AABB"]["width"]   = 40.0f;
		data["AABB"]["height"]  = 40.0f;
		data["AABB"]["offsetX"] = 0.0f;
		data["AABB"]["offsetY"] = 0.0f;

		components[T::Family]->name = name;
		components[T::Family]->Initialize(data[name]);
	}


	template <typename T>
	T* GetComponent()
	{
		return (T *)components[T::Family];
	}


	void CleanupComponents()
	{
		std::unordered_map<std::string, Component *>::iterator it = components.begin();
		for( ; it != components.end(); ++it)
		{
			if(it->second)
				it->second->Cleanup();
		}
	}

	// TODO(brett): see if we can manipulate entities purely with data
	//void addComponent(std::string family, std::string type);
	//void removeComponent(std::string family);
	std::unordered_map<std::string, Component *> components;


	// TODO(brett): this is just for testing
	static std::list<Entity *> createdEntities;
	static std::list<Entity *> deletedEntities;
};


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

bglinternal void 
createPlayerBullet(glm::vec2 pos, glm::vec2 size)
{
	// NOTE(brett): Entity should provide a pool to add entities to. For now I'm throwing it on the heap

	// NOTE(brett): this would pull from the kinematic :: create_player_bullet tag and just give the data to the
	// appropriate components (drawable, kinematic, collidable, etc)
	Entity *e = new Entity(false);
	e->sprite = BGLSprite::Create("spritesheet", "", size.x, size.y, 0, 1, &BGLRectMake(0, 96, 16, 16));
	e->pos = pos;
	e->tag = "Bullet";
	e->AddComponent<KinematicComponent>("player_simple_projectile");
	e->AddComponent<PhysicsComponent>("AABB");

}

bglinternal void
createPlayerEnemy(glm::vec2 pos, glm::vec2 size)
{
	Entity *e = new Entity(false);
	e->sprite = BGLSprite::Create("spritesheet", "", size.x, size.y, 0, 1, &BGLRectMake(32, 0, 32, 16));
	e->pos = pos;
	e->tag = "Enemy";
	e->AddComponent<BehaviorComponent>("");
	e->AddComponent<PhysicsComponent>("AABB");
}