
#include "components.h"

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

	std::list<PhysicsComponent *>::iterator it = entityComponents.begin();
	for( ; it != entityComponents.end(); ++it)
	{
		// TODO(brett): I don't wanna do this everytime. Should just make the rect part of
		// the physics component
		glm::vec2 pos = (*it)->owner->pos;
		float ww = (*it)->width/2.0f;
		float hh = (*it)->height/2.0f;
		BGLRect myRect = BGLRectMake(pos.x-ww, pos.y-hh, ww+ww, hh+hh);

		std::list<PhysicsComponent *>::iterator jt = entityComponents.begin();
		for( ; jt != entityComponents.end(); ++jt)
		{
			if(it == jt) continue;

			glm::vec2 pos2 = (*jt)->owner->pos;
			float ww2 = (*jt)->width/2.0f;
			float hh2 = (*jt)->height/2.0f;
			BGLRect theyRect = BGLRectMake(pos2.x-ww2, pos2.y-hh2, ww2+ww2, hh2+hh2);

			// NOTE(brett): this is done in the most brute forcey way possible for now. just for testing.
			if(BGLRectOverlap(myRect, theyRect))
			{
				BehaviorComponent *myBehavior = (*it)->owner->GetComponent<BehaviorComponent>();
				BehaviorComponent *theyBehavior = (*jt)->owner->GetComponent<BehaviorComponent>();
				
				if(myBehavior)
					myBehavior->behavior->OnCollide((*jt)->owner);

				if(theyBehavior)
					theyBehavior->behavior->OnCollide((*it)->owner);
			}
		}
	}
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
