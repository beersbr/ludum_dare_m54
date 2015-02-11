#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include <list>
#include "BGL.h"
#include "components.h"

#define MAX_PARTICLES 10000

static uint32_t ParticleExplostionFrameCount = 3;
static BGLRect ParticleExplosion[] = { { 96, 144, 32, 32 }, { 96, 176, 32, 32 }, { 96, 208, 32, 32 } };

struct Particle
{
	BGLSprite sprite;
	float aliveFor;
	float liveTime;
	bool alive;
	glm::vec2 position;
	glm::vec2 scale;
	glm::vec3 rotation;
};

class ParticleHandler
{
public:

	static void Initialize()
	{
		for(int32_t i = 0; i < MAX_PARTICLES; ++i)
		{
			freeParticles.push_back(&particles[i]);
		}
	}

	static void Emit(glm::vec2 pos, glm::vec2 scale, glm::vec3 rotation, float life)
	{
		Particle *p = freeParticles.back();
		freeParticles.pop_back();

		p->aliveFor = 0.f;
		p->liveTime = life;
		p->alive = true;
		p->position = pos;
		p->scale = scale;
		p->rotation = rotation;

		BGLSprite::Create(&p->sprite, 
						  BGLBatchTextureHandler::GetArrayTexture("spritesheet"), 
						  ParticleExplostionFrameCount, 
						  (BGLRect *)&ParticleExplosion[0]);
		
		usedParticles.push_back(p);
	}

	static void Update(float dt)
	{
		std::list<Particle *>::iterator it = usedParticles.begin();
		for( ; it != usedParticles.end(); )
		{
			(*it)->aliveFor += dt;

			if((*it)->aliveFor >= (*it)->liveTime)
			{
				(*it)->alive = false;
				freeParticles.push_front((*it));
				usedParticles.erase(it++);

				continue;
			}

			uint32_t frame = ((*it)->aliveFor * ParticleExplostionFrameCount) / (*it)->liveTime;
			(*it)->sprite.SetAnimationFrame(frame);

			SpriteComponent::renderLayers[DOODAD]->DrawSprite(&(*it)->sprite, (*it)->position, (*it)->scale, (*it)->rotation);

			it++;
		}
	}

private:
	static Particle particles[MAX_PARTICLES];
	static std::list<Particle *> freeParticles;
	static std::list<Particle *> usedParticles;

};


#endif