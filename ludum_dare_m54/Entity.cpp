#include "Entity.h"


std::vector<Entity *> Entity::createdEntities;

std::string KinematicComponent::Family = "kinematic";

std::list<KinematicComponent *> KinematicComponent::entities;
void KinematicComponent::Update(float dt)
{
	std::list<KinematicComponent *>::iterator it = entities.begin();
	for( ; it != entities.end(); ++it)
	{
		(*it)->owner->pos += (*it)->vel * dt;
	}
}

Entity::Entity(void) : pos(0.0f,0.0f), vel(0.0f,0.0f), size(0.0f,0.0f)
{
	createdEntities.push_back(this);
}

Entity::Entity(bool dontTrack = true) : pos(0.0f,0.0f), vel(0.0f,0.0f), size(0.0f,0.0f)
{
	if(!dontTrack)
		createdEntities.push_back(this);
}


Entity::~Entity(void)
{
}

void Entity::Render()
{
	sprite.model = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, 0.0f));
	sprite.Render();
}

void Entity::Update(float dt)
{

}