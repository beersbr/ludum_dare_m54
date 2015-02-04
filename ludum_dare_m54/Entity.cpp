#include "Entity.h"


#include <cmath>

std::string KinematicComponent::Family = "kinematic";
std::list<KinematicComponent *> KinematicComponent::entityComponents;

void KinematicComponent::Update(float dt)
{
	std::list<KinematicComponent *>::iterator it = entityComponents.begin();
	for( ; it != entityComponents.end(); ++it)
	{
		(*it)->owner->pos += (*it)->vel * dt;
	}
}



std::string AIComponent::Family = "ai";
std::list<AIComponent *> AIComponent::entityComponents;

void AIComponent::Update(float dt)
{
	// each component can get the owners component if one exists

	std::list<AIComponent *>::iterator it = entityComponents.begin();
	for( ; it != entityComponents.end(); ++it)
	{
		float liveTime = ((*it)->liveTime += dt);

		// NOTE(brett): some ai component types will want to move the current entity
		KinematicComponent *kinematicComponent = (*it)->owner->GetComponent<KinematicComponent>();
		if(!kinematicComponent) continue;

		kinematicComponent->vel.y = 200*(sinf((liveTime*4.0f)));
	}
}



std::vector<Entity *> Entity::createdEntities;

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
