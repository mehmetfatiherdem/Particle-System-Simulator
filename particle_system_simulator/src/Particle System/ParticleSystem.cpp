#include <functional>
#include <algorithm>
#include <iostream>
#include "ParticleSystem.h"
#include "Time Management/Time.h"
#include "RenderPipeline/Application.h"
#include "RenderPipeline/Shader/Shader.h"
#include "Components/RotationBySpeed.h"
#include "Components/VelocityOverLifetime.h"
#include "Components/SizeOverLifetime.h"
#include "Components/SizeBySpeed.h"
#include "Components/LimitVelocityOverLifetime.h"
#include "Components/ForceOverLifetime.h"
#include "Components/ColorOverLifetime.h"
#include "Components/ColorBySpeed.h"

ParticleSystem::ParticleSystem(std::string&& name, const ParticleSystemProps& props, const Material& material, std::unique_ptr<Emitter> emitter)
	: name(std::move(name)), props(props), emitter(std::move(emitter)), quad(createQuad()),
	scene(Application::getInstance().getScene()), material(material), poolIndex(props.maxParticles - 1), enabled(true)
{
	// setMaxParticleSize(props.maxParticles);
	particlePool.resize(props.maxParticles);
	this->material.setColor(props.startColor);

	std::for_each(particlePool.begin(), particlePool.end(), [&](Particle& particle)
		{
			particle.renderer = scene.createObject(TransformProps{}, quad, Shader::instancedShader(), material);
			particle.disable();
			particle.renderer->setPreRenderAction([&](Transform& transform)
				{
					transform.lookAt(Application::getInstance().getScene().getCamera().getTransform());
					transform.rotateAround(transform.getForwardVector(), particle.rotation);
				});
		});
}

void ParticleSystem::setMaxParticleSize(uint32_t size)
{
	props.maxParticles = size;

	std::for_each(particlePool.begin(), particlePool.end(), [&](Particle& particle)
		{
			scene.destroyObject(particle.renderer);
		});

	particlePool = std::vector<Particle>();
	poolIndex = size - 1;

	particlePool.resize(size);
	quad = createQuad();

	// do for each for the new particles
	std::for_each(particlePool.begin(), particlePool.end(), [&](Particle& particle)
			{
				particle.renderer = scene.createObject(TransformProps{}, quad, Shader::instancedShader(), material);
				particle.disable();
				particle.renderer->setPreRenderAction([&](Transform& transform)
					{
						transform.lookAt(Application::getInstance().getScene().getCamera().getTransform());
						transform.rotateAround(transform.getForwardVector(), particle.rotation);
					});
			});

}

void ParticleSystem::addComponent(Component* component)
{
	for (auto it : components)
	{
		if (it == component) return;
	}

	components.push_back(component);
}

void ParticleSystem::removeComponent(Component* component)
{
	components.erase(std::remove(components.begin(), components.end(), component));
	delete component;
}

void ParticleSystem::update()
{
	if (enabled)
	{
		emitter->tryEmit(props, particlePool, poolIndex);
	}

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
				component->tryUpdate(props, particle);
			}

			Transform& transform = particle.renderer->getTransform();
			transform.translate(particle.velocity * Time::deltaTime());
			//transform.rotate(particle.angularVelocity * Time::deltaTime());
		}
	}
}
