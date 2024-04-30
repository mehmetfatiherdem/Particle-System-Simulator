#include "Emitter.h"
#include "Time Management/Time.h"

void Emitter::tryEmit(ParticleSystemProps& props, std::vector<Particle>& particlePool, uint32_t& poolIndex)
{
	float result = emissionRate * Time::deltaTime() + surplus;
	uint32_t particleCount = static_cast<uint32_t>(result);
	surplus = result - particleCount;

	if (props.currentParticles + particleCount > props.maxParticles)
	{
		props.currentParticles = props.maxParticles;
		particleCount = props.maxParticles - props.currentParticles;
	}
	else
	{
		props.currentParticles += particleCount;
	}

	if (particleCount == 0) return;

	emit(props, particlePool, poolIndex, particleCount);
}