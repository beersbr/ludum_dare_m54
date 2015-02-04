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


std::string PhysicsComponent::Family = "physics";
std::list<PhysicsComponent *> PhysicsComponent::entityComponents;

void PhysicsComponent::Update(float dt)
{

}


std::string BehaviorComponent::Family = "behavior";
std::list<BehaviorComponent *> BehaviorComponent::entityComponents;
void BehaviorComponent::Update(float dt)
{
	std::list<BehaviorComponent *>::iterator it = entityComponents.begin();
	for( ; it != entityComponents.end(); ++it)
	{
		(*it)->behavior->Update(dt);
	}
}



std::list<Entity *> Entity::createdEntities;

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
