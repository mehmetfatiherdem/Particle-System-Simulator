#include <functional>
#include <algorithm>
#include <iostream>
#include "ParticleSystem.h"
#include "Time Management/Time.h"
#include "RenderPipeline/Application.h"
#include "RenderPipeline/Shader/Shader.h"

ParticleSystem::ParticleSystem(ParticleSystemProps props, Texture&& texture, std::unique_ptr<Emitter> emitter)
	: props(props), emitter(std::move(emitter)), sphere(createQuad()), scene(Application::getInstance().getScene()),
	material(nullptr), poolIndex(props.maxParticles - 1), texture(std::move(texture))
{
	particlePool.resize(props.maxParticles);

	Material material(&texture, nullptr, props.startColor, 0.0f);

	std::for_each(particlePool.begin(), particlePool.end(), [&](Particle& particle) 
		{
			particle.renderer = scene.createObject(TransformProps{}, sphere, Shader::instancedShader(), material);
			particle.disable();
		});
}

void ParticleSystem::addComponent(Component* component)
{
	components.insert(component);
}

void ParticleSystem::update()
{
	emitter->tryEmit(props, particlePool, poolIndex);

	for (uint32_t i = 0; i < particlePool.size(); ++i)
	{
		Particle& particle = particlePool[i];

		if (particle.isEnabled())
		{
			particle.remainingLifetime -= Time::deltaTime();

			if (particle.remainingLifetime <= 0)
			{
				particle.disable();
				props.currentParticles--;
				continue;
			}

			std::cout << particle.renderer->getMaterial().getColor().ambient.r <<
				"," << particle.renderer->getMaterial().getColor().ambient.g <<
				"," << particle.renderer->getMaterial().getColor().ambient.b << "\n";

			for (Component* component : components)
			{
				component->update(props, particle);
			}
			
			Transform& transform = particle.renderer->getTransform();
			transform.translate(particle.velocity * Time::deltaTime());
			//transform.rotate(particle.angularVelocity * Time::deltaTime());
		}
	}
}
