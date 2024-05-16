#include "RenderPipeline/Application.h"
#include "Time Management/Time.h"
#include "Persistence/Serializer.h"
#include "Emitter.h"

Emitter::~Emitter() { }

void Emitter::tryEmit(ParticleSystemProps& props, std::vector<Particle>& particlePool, uint32_t& poolIndex)
{
	float result = emissionRate * Time::deltaTime() + surplus;
	uint32_t particleCount = static_cast<uint32_t>(result);
	surplus = result - particleCount;

	if (props.currentParticles + particleCount > props.maxParticles)
	{
		surplus += (particleCount - props.maxParticles + props.currentParticles);
		particleCount = props.maxParticles - props.currentParticles;
		props.currentParticles = props.maxParticles;
	}
	else
	{
		props.currentParticles += particleCount;
	}

	glm::vec3 position, velocity;

	Transform& camT = Application::getInstance().getScene().getCamera().getTransform();

	for (size_t i = 0; i < particleCount; ++i, poolIndex = (poolIndex - 1) % particlePool.size())
	{
		emit(props, position, velocity);

		Particle& particle = particlePool[poolIndex];
		Transform& transform = particle.renderer->getTransform();

		transform.setPosition(position);
		transform.lookAt(camT);
		transform.rotateAround(transform.getForwardVector(), props.startRotation);
		transform.setScale(glm::vec3{props.startSize, props.startSize, props.startSize});
		particle.renderer->getMaterial().setColor(props.startColor);
		particle.velocity = velocity;
		particle.remainingLifetime = props.startLifetime;
		particle.enable();
	}
}

void Emitter::serialize(Serializer& serializer, const std::string& objectName) const
{
	serializer["EmissionRate"].real(emissionRate);
}