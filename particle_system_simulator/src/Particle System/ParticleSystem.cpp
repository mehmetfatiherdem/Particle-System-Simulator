#include "ParticleSystem.h"
#include "Time Management/Time.h"
#include "RenderPipeline/Application.h"
#include "RenderPipeline/Shader/Shader.h"
#include <algorithm>
#include <functional>

ParticleSystem::ParticleSystem(ParticleSystemProps props, std::unique_ptr<Emitter> emitter)
	: props(props), emitter(std::move(emitter)), sphere(createSphere(500)), scene(Application::getInstance().getScene()),
	material(nullptr)
{
	particlePool.resize(props.maxParticles);

	std::for_each(particlePool.begin(), particlePool.end(), [&](Particle& particle) 
		{
			particle.renderer = scene.createObject(TransformProps{}, sphere, Shader::instancedShader());
			particle.disable();
		});
}

void ParticleSystem::update()
{
	for (uint32_t i = 0; i < particlePool.size(); ++i)
	{
		Particle& particle = particlePool[i];

		if (particle.isEnabled())
		{
			particle.remainingLifetime -= Time::deltaTime();

			if (particle.remainingLifetime <= 0)
			{
				particle.disable();
				--props.currentParticles;
			}
			
			Transform& transform = particle.renderer->getTransform();
			transform.translate(particle.velocity * Time::deltaTime());
			transform.rotate(particle.angularVelocity * Time::deltaTime());
		}
	}
}