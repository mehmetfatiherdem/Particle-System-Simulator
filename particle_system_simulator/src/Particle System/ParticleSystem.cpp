#include <functional>
#include <algorithm>
#include <iostream>
#include "ParticleSystem.h"
#include "Time Management/Time.h"
#include "RenderPipeline/Application.h"
#include "RenderPipeline/Shader/Shader.h"

ParticleSystem::ParticleSystem(const ParticleSystemProps& props, const Material& material, std::unique_ptr<Emitter> emitter)
	: props(props), emitter(std::move(emitter)), sphere(createQuad()), scene(Application::getInstance().getScene()),
	material(material), poolIndex(props.maxParticles - 1)
{
	particlePool.resize(props.maxParticles);
	this->material.setColor(props.startColor);

	std::for_each(particlePool.begin(), particlePool.end(), [&](Particle& particle) 
		{
			particle.renderer = scene.createObject(TransformProps{}, sphere, Shader::instancedShader(), material);
			particle.disable();
			particle.renderer->setPreRenderAction([](Transform& transform)
				{
					glDepthMask(GL_FALSE);
					transform.setForwardVector(Application::getInstance().getScene().getCamera().getTransform().getForwardVector());
				});

			particle.renderer->setPostRenderAction([]()
				{
					glDepthMask(GL_TRUE);
				});
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
