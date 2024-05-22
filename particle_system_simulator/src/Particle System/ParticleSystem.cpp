#include <functional>
#include <algorithm>
#include <iostream>
#include "Time Management/Time.h"
#include "ResourceManagement/ResourceManager.h"
#include "MeshConstruction/Shapes.h"
#include "Components/Component.h"
#include "RenderPipeline/Application.h"
#include "RenderPipeline/Scene/Scene.h"
#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"
#include "Emitter/SphereEmitter.h"
#include "Persistence/Deserializer.h"
#include "Persistence/Serializer.h"
#include "Persistence/Serialization Utils/SerializationUtils.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : name(""), props(), material(Material::defaultMaterial()), emitter(std::move(SphereEmitter::defaultEmitter())),
	quad(createQuad()), scene(Application::getInstance().getScene()), enabled(false), poolIndex(0) { }

ParticleSystem::ParticleSystem(std::string&& name, const ParticleSystemProps& props, const Material& material, std::unique_ptr<Emitter> emitter)
	: name(std::move(name)), props(props), emitter(std::move(emitter)),
	quad(createQuad()), scene(Application::getInstance().getScene()), material(material), poolIndex(props.maxParticles - 1), enabled(true)
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
	components(std::move(other.components)), scene(other.scene), particlePool(),
	quad(std::move(other.quad))
{
	for (auto& particle : other.particlePool)
	{
		bool enabled = particle.isEnabled();
		scene.destroyObject(particle.renderer);
		particle.renderer = scene.createObject(TransformProps{}, quad, Shader::instancedShader(), material);

		if (!enabled)
		{
			particle.disable();
		}

		particle.renderer->setPreRenderAction([&](Transform& transform)
			{
				transform.lookAt(Application::getInstance().getScene().getCamera().getTransform());
				transform.rotateAround(transform.getForwardVector(), particle.rotation);
			});
	}

	this->particlePool = std::move(other.particlePool);
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
	if (!scene.isSkyboxEnabled())
	{
		int a = 5 + 5;
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

void ParticleSystem::serialize(Serializer& serializer, const std::string& objectName) const
{
	serializer.startObject(objectName);

	serializer["Name"].string(name.c_str());
	serializer["Enabled"].boolean(enabled);

	serializer["StartLifetime"].real(props.startLifetime);
	serializer["StartSpeed"].real(props.startSpeed);
	serializer["StartSize"].real(props.startSize);
	serializer["MinSize"].real(props.minSize);
	serializer["MaxSize"].real(props.maxSize);
	serializer["StartRotation"].real(props.startRotation);

	persistence::utils::serializeColor(serializer, props.startColor, "StartColor");

	serializer["GravityModifier"].real(props.gravityModifier);
	serializer["MaxParticles"].uint(props.maxParticles);

	persistence::utils::serializeVector(serializer, props.position, "Position");

	serializer["Shininess"].real(material.getShininess());

	const Texture* texture = material.getDiffuseMap();

	if (texture == nullptr)
	{
		serializer["DiffuseMap"].null();
	}
	else
	{
		serializer["DiffuseMap"].string(ResourceManager::getTextureName(texture).c_str());
	}

	texture = material.getSpecularMap();

	if (texture == nullptr)
	{
		serializer["SpecularMap"].null();
	}
	else
	{
		serializer["SpecularMap"].string(ResourceManager::getTextureName(texture).c_str());
	}

	emitter->serialize(serializer, "Emitter");

	serializer.startArray("Components");

	for (auto* component : components)
	{
		component->serialize(serializer);
	}

	serializer.endArray();

	serializer.endObject();
}
