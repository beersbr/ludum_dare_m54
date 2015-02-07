#include "Entity.h"


std::list<Entity *> Entity::createdEntities;
std::list<Entity *> Entity::deletedEntities = std::list<Entity *>();

Entity::Entity(void) : pos(0.0f,0.0f), vel(0.0f,0.0f), size(0.0f,0.0f)
{
	createdEntities.push_back(this);
	deleted = false;
}

Entity::Entity(bool dontTrack = true) : pos(0.0f,0.0f), vel(0.0f,0.0f), size(0.0f,0.0f)
{
	deleted = false;
	if(!dontTrack)
		createdEntities.push_back(this);
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

void Entity::Render()
{
	sprite.model = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, 0.0f));
	sprite.Render();
}

void Entity::Update(float dt)
{

}
