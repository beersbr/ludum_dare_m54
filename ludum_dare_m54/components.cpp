
#include "components.h"

/******************************************************************************************
PHYSICS COMPONENT
******************************************************************************************/

std::string PhysicsComponent::Family = "physics";
std::list<PhysicsComponent *> PhysicsComponent::entityComponents;

void PhysicsComponent::Update(float dt)
{

	std::list<PhysicsComponent *>::iterator it = entityComponents.begin();
	for( ; it != entityComponents.end(); ++it)
	{
		// TODO(brett): I don't wanna do this everytime. Should just make the rect part of
		// the physics component
		glm::vec2 pos = (*it)->owner->position;
		float ww = (*it)->width/2.0f;
		float hh = (*it)->height/2.0f;
		BGLRect myRect = BGLRectMake(pos.x-ww, pos.y-hh, ww+ww, hh+hh);

		std::list<PhysicsComponent *>::iterator jt = entityComponents.begin();
		for( ; jt != entityComponents.end(); ++jt)
		{
			if(it == jt) continue;

			glm::vec2 pos2 = (*jt)->owner->position;
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


/******************************************************************************************
BEHAVIOR COMPONENT
******************************************************************************************/

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


/******************************************************************************************
SPRITER COMPONENT
******************************************************************************************/

std::string SpriteComponent::Family = "sprite";
glm::mat4 SpriteComponent::ProjectionMatrix = glm::ortho(0.0f, 1200.0f, 800.0f, 0.0f, -1.0f, 1.0f);
BGLSpriteBatch *SpriteComponent::renderLayers[SPRITE_LAYER_COUNT];
glm::vec4 SpriteComponent::camera[SPRITE_LAYER_COUNT];

std::list<SpriteComponent *> SpriteComponent::entityComponents;

void SpriteComponent::Update(float dt)
{
	// Render each of our sprites at the owners location and orientation.
	std::list<SpriteComponent *>::iterator it = entityComponents.begin();
	for( ; it != entityComponents.end(); ++it)
	{
		SpriteComponent *s = (*it);
		renderLayers[s->layer]->DrawSprite(&s->sprite, s->owner->position, s->owner->scale, s->owner->rotation);
	}

	// Render the batches.
	for(int32_t i = SPRITE_LAYER_COUNT - 1; i >= 0; --i)
	{
		glm::mat4 layerCamera = glm::translate(glm::mat4(), glm::vec3(-camera[i].x, -camera[i].y, 0.f));
		
		renderLayers[i]->BeginBatch();
		renderLayers[i]->RenderBatch(ProjectionMatrix, layerCamera);

		// TODO(brett): sometimes we dont need to draw and rerender things. Especially in the case of the background, we
		// can just render it each time without doing any extra array manipulation.
		renderLayers[i]->ClearBatch();
	}
}