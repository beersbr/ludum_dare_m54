#include "Entity.h"


std::list<Entity *> Entity::createdEntities;
std::list<Entity *> Entity::deletedEntities;
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
	deleted = true;
	
}


void Entity::Initialize()
{
	// TODO(brett): do some studying on lists. look for a better way to do this.
	for(int32_t i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		freeEntities.push_back(&entities[i]);
	}
}

Entity *Entity::Create(std::string tag, glm::vec2 pos, glm::vec2 scale, glm::vec3 rotation)
{
	Entity *e = Create(pos, scale, rotation);
	e->tag = tag;
	return e;
}

Entity *Entity::Create(glm::vec2 pos, glm::vec2 scale, glm::vec3 rotation)
{
	// NOTE(brett): ran out of entities
	assert(freeEntities.size() > 0);

	Entity *e = freeEntities.back();
	freeEntities.pop_back();

	e->position = pos;
	e->scale = scale;
	e->rotation = rotation;

	createdEntities.push_back(e);

	e->deleted = false;

	return e;

}

void Entity::Destroy(Entity *e)
{
	deletedEntities.push_back(e);

	e->Delete();

	// TODO(brett): do any other things that an entity needs before getting rid of it

	createdEntities.remove(e);
}

void Entity::SweepDestruction()
{
	if(deletedEntities.size() == 0) return;

	std::list<Entity *>::iterator it = deletedEntities.begin();
	for( ; it != deletedEntities.end(); )
	{
		(*it)->CleanupComponents();
		freeEntities.push_front((*it));
		deletedEntities.erase(it++);
	}
}