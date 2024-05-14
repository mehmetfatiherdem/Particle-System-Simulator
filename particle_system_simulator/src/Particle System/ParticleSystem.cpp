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

Mesh ParticleSystem::quad = createQuad();

ParticleSystem::ParticleSystem(std::string&& name, const ParticleSystemProps& props, const Material& material, std::unique_ptr<Emitter> emitter)
	: name(std::move(name)), props(props), emitter(std::move(emitter)),
	scene(Application::getInstance().getScene()), material(material), poolIndex(props.maxParticles - 1), enabled(true)
{
	this->props.currentParticles = 0;
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

ParticleSystem::ParticleSystem(ParticleSystem&& other) noexcept
	: name(std::move(other.name)), enabled(other.enabled), poolIndex(other.poolIndex),
	material(std::move(other.material)), props(other.props), emitter(std::move(other.emitter)),
	components(std::move(other.components)), scene(other.scene), particlePool(std::move(other.particlePool))
{
	other.particlePool.clear();
	other.enabled = false;
}

ParticleSystem& ParticleSystem::operator=(ParticleSystem&& other) noexcept
{
	for (auto* cmp : components)
	{
		delete cmp;
	}

	for (auto& pr : particlePool)
	{
		scene.destroyObject(pr.renderer);
	}

	this->name = std::move(other.name);
	this->enabled = other.enabled;
	this->poolIndex = other.poolIndex;
	this->quad = std::move(other.quad);
	this->material = std::move(other.material);
	this->props = other.props;
	this->emitter = std::move(other.emitter);
	this->components = std::move(other.components);
	this->particlePool = std::move(other.particlePool);

	other.particlePool.clear();
	other.enabled = false;
	return *this;
}

ParticleSystem::~ParticleSystem()
{
	for (auto* cmp : components)
	{
		delete cmp;
	}

	for (auto& pr : particlePool)
	{
		scene.destroyObject(pr.renderer);
	}
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
				props.currentParticles--;
				continue;
			}

			for (Component* component : components)
			{
				component->tryUpdate(props, particle);
			}

			Transform& transform = particle.renderer->getTransform();
			particle.velocity.y -= props.gravityModifier * 9.8f;
			transform.translate(particle.velocity * Time::deltaTime());
			//transform.rotate(particle.angularVelocity * Time::deltaTime());
		}
	}

	if (enabled)
	{
		emitter->tryEmit(props, particlePool, poolIndex);
	}
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

void ParticleSystem::setMaxParticles(uint32_t maxParticles)
{
	if (maxParticles == props.maxParticles) return;

	props.maxParticles = maxParticles;
	props.currentParticles = 0;
	poolIndex = maxParticles - 1;

	if (maxParticles < particlePool.size())
	{
		for (size_t i = maxParticles; i < particlePool.size(); ++i)
		{
			scene.destroyObject(particlePool[i].renderer);
		}
	}

	particlePool.resize(maxParticles);

	for (auto& pr : particlePool)
	{
		if (pr.renderer == nullptr)
		{
			pr.renderer = scene.createObject(TransformProps{}, quad, Shader::instancedShader(), material);

			pr.renderer->setPreRenderAction([&](Transform& transform)
				{
					transform.lookAt(Application::getInstance().getScene().getCamera().getTransform());
					transform.rotateAround(transform.getForwardVector(), pr.rotation);
				});
		}

		pr.disable();
	}
}

void ParticleSystem::setDiffuseMap(Texture* diffuseMap)
{
	material.setDiffuseMap(diffuseMap);
	std::for_each(particlePool.begin(), particlePool.end(), [diffuseMap](Particle& particle)
		{
			particle.renderer->getMaterial().setDiffuseMap(diffuseMap);
		});
}

void ParticleSystem::setSpecularMap(Texture* specularMap)
{
	material.setSpecularMap(specularMap);
	std::for_each(particlePool.begin(), particlePool.end(), [specularMap](Particle& particle)
		{
			particle.renderer->getMaterial().setSpecularMap(specularMap);
		});
}
