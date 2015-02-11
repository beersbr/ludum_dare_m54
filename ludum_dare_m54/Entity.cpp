#include "Entity.h"


std::list<Entity *> Entity::createdEntities;
std::list<Entity *> Entity::deletedEntities = std::list<Entity *>();
std::list<Entity *> Entity::freeEntities;
Entity Entity::entities[MAX_ENTITY_COUNT];


Entity::Entity(void) : position(0.0f,0.0f), 
					   scale(0.f, 0.f), 
					   rotation(0.f, 0.f, 0.f)
{
	createdEntities.push_back(this);
	deleted = false;
}

Entity::~Entity(void)
{
}

void Entity::Delete()
{
	if(!deleted)
	{
		deleted = true;
		deletedEntities.push_back(this);
	}
	
}


void Entity::Initialize()
{
	// TODO(brett): do some studying on lists. look for a better way to do this.
	for(int32_t i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		freeEntities.push_back(&entities[i]);
	}
}

Entity *Entity::Create(glm::vec2 pos, glm::vec2 scale, glm::vec3 rotation)
{
	// NOTE(brett): ran out of entities
	assert(freeEntities.size() > 0);

	return 0;

}

void Entity::Destroy(Entity *)
{

}