#pragma once

#include <vector>
#include "EntityComponentSystem/Entity.h"
#include "Data/ParticleSystemProps.h"
#include "Particle.h"

class ParticleSystem : public Entity
{
private:
	std::vector<Particle> particlePool;
	std::vector<Particle> activeParticles;

	glm::vec3 position;

	ParticleSystemProps props;
	
public:
	ParticleSystem(const ParticleSystemProps& props);
	~ParticleSystem();

	void update()
	{

	}
};

void test()
{
	ParticleSystem system(ParticleSystemProps{});
}