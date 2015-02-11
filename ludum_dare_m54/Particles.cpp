#include "Particles.h"

Particle ParticleHandler::particles[MAX_PARTICLES];
std::list<Particle *> ParticleHandler::freeParticles;
std::list<Particle *> ParticleHandler::usedParticles;