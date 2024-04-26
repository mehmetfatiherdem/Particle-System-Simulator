#include "Emitter.h"
#include "Time Management/Time.h"

void Emitter::emit(const ParticleSystemProps& props, std::vector<Particle>& particlePool, uint32_t& poolIndex)
{
	float result = emissionRate * Time::deltaTime() + previousSurplus;
	uint32_t particleCount = static_cast<uint32_t>(result);
	previousSurplus = result - particleCount;
	emit(props, particlePool, poolIndex, particleCount);
}