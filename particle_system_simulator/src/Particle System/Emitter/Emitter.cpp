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

	glm::vec3 position, velocity;

	for (size_t i = 0; i < particleCount; ++i, poolIndex = (poolIndex - 1) % particlePool.size())
	{
		emit(props, position, velocity);

		Particle& particle = particlePool[poolIndex];

		particle.renderer->getTransform().setPosition(position);
		particle.renderer->getTransform().setEulerRotation(props.startRotation);
		particle.renderer->getTransform().setScale(glm::vec3{props.startSize, props.startSize, props.startSize});
		particle.velocity = velocity;
		particle.color = props.startColor;
		particle.remainingLifetime = props.startLifetime;
		particle.enable();
	}
}