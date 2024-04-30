
#include "SphereEmitter.h"

void SphereEmitter::emit(const ParticleSystemProps& props, std::vector<Particle>& particlePool, uint32_t& poolIndex, uint32_t particleCount)
{
	for (size_t i = 0; i < particleCount; ++i, poolIndex = (poolIndex - 1) % particlePool.size())
	{
		float dx, dy, x, y, z, randomRadius, randomXy, noise;

		do
		{
			dx = Random::getFloat(-1.0f, 1.0f);
			dy = Random::getFloat(-1.0f, 1.0f);
		} while (Trig::atan2(dy, dx) > arc);

		randomRadius = Random::getFloat(0.0f, radius);
		z = Random::getFloat(-randomRadius, randomRadius);
		randomXy = randomRadius * randomRadius - z * z;
		randomXy = std::sqrtf(randomXy / (dx * dx + dy * dy));
		y = randomXy * dy;
		x = randomXy * dx;
		noise = utility::math::max(x, y, z) / 5.0f;

		glm::vec3 offset = glm::vec3{x, y, z};
		glm::vec3 velocity = offset;
		velocity += glm::vec3{Random::getFloat(-noise, noise), Random::getFloat(-noise, noise), Random::getFloat(-noise, noise)};
		velocity = glm::normalize(velocity) * props.startSpeed;

		Particle& particle = particlePool[poolIndex];

		particle.renderer->getTransform().setPosition(props.position + offset);
		particle.renderer->getTransform().setEulerRotation(props.startRotation);
		particle.renderer->getTransform().setScale(glm::vec3{props.startSize, props.startSize, props.startSize});
		particle.velocity = velocity;
		particle.color = props.startColor;
		particle.remainingLifetime = props.startLifetime;
		particle.enable();
	}
}