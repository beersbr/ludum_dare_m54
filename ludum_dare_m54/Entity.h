#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <unordered_map>
#include <list>
#include "BGL.h"
#include "BGLAudio.h"
#include "SpriteSheet.h"
#include "componentBase.h"


#define MAX_ENTITY_COUNT 30000

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
	// Meta data for game entities
	EntityType typeId;
	std::string tag;

	// NOTE(brett): Who created me
	Entity *owner;

	// Transform information
	glm::vec2 position;
	glm::vec2 scale;
	glm::vec3 rotation;

	bool deleted;
	void Delete();

	template <typename T>
	void AddComponent(std::string name)
	{
		// TODO(brett): if the component already exists we need to get rid of the old one
		// if(components[T::family]) ...
		components[T::Family] = new T(this);

		// NOTE(brett): this is where we would fetch some data from a json file
		// std::unordered_map<std::string, float> data = resource_handler[T::family][name]

		std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, float> > > data;

		const float BACKGROUND = 0.f;
		const float SPRITESHEET = 0.f;

		data["sprite"]["background"]["texture"] = 0.f;
		data["sprite"]["background"]["layer"] = BACKGROUND_BACKGROUND;

		// player
		data["sprite"]["player"]["texture"] = 1.f;
		data["sprite"]["player"]["layer"] = PLAYER_LAYER;

		// bullet
		data["sprite"]["bullet"]["texture"] = 2.f;
		data["sprite"]["bullet"]["layer"] = ENEMY_LAYER;

		// enemy
		data["sprite"]["enemy"]["texture"] = 5.f;
		data["sprite"]["enemy"]["layer"] = ENEMY_LAYER;

		// tile
		data["sprite"]["tile"]["texture"] = 3.f;
		data["sprite"]["tile"]["layer"] = LEVEL_BACKGROUND;

		data["sprite"]["tileback"]["texture"] = 4.f;
		data["sprite"]["tileback"]["layer"] = LEVEL_FOREGROUND;

		data["behavior"]["player"]["name"] = 1.f;
		data["behavior"]["bullet"]["name"] = 2.f;
		data["behavior"]["enemy"]["name"] = 3.f;
		data["behavior"]["explosion"]["name"] = 4.f;

		//components[T::Family]->name = name;
		components[T::Family]->Initialize(data[T::Family][name]);
	}


	template <typename T>
	T *GetComponent()
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

	static void SweepDestruction();

	// TODO(brett): see if we can manipulate entities purely with data
	//void addComponent(std::string family, std::string type);
	//void removeComponent(std::string family);
	std::unordered_map<std::string, Component *> components;
	
	// No more asking for memory during run time
	static void Initialize();
	static Entity *Create(std::string tag, glm::vec2 pos, glm::vec2 scale, glm::vec3 rotation);
	static Entity *Create(glm::vec2 pos, glm::vec2 scale, glm::vec3 rotation);
	static void Destroy(Entity *entity);

	// TODO(brett): this is just for testing
	static std::list<Entity *> createdEntities;
	static std::list<Entity *> deletedEntities;
	static std::list<Entity *> freeEntities;
	static Entity entities[MAX_ENTITY_COUNT];
	

private:
	Entity(void);
    ~Entity(void);

};

