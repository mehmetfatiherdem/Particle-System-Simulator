#include <memory>
#include <string>
#include <stdint.h>
#include <glm/vec3.hpp>
#include "ResourceManagement/ResourceManager.h"
#include "RenderPipeline/Application.h"
#include "RenderPipeline/Material/Material.h"
#include "Particle System/ParticleSystem.h"
#include "Particle System/Emitter/Emitter.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "Persistence/Deserializer.h"
#include "Persistence/SerializationUtils.h"
#include "EmitterDeserializer.h"
#include "ComponentDeserializer.h"
#include "ParticleSytemDeserializer.h"

class Texture;

void deserializeParticleSystem(const Deserializer& deserializer, Application& application)
{
	std::string name = deserializer["Name"].getString();
	bool enabled = deserializer["Enabled"].getBoolean();

	ParticleSystemProps props;

	props.startLifetime = deserializer["StartLifetime"].getReal();
	props.startSpeed = deserializer["StartSpeed"].getReal();
	props.startSize = deserializer["StartSize"].getReal();
	props.minSize = deserializer["MinSize"].getReal();
	props.maxSize = deserializer["MaxSize"].getReal();
	props.startRotation = deserializer["StartRotation"].getReal();

	persistence::utils::deserializeColor(deserializer["StartColor"], &props.startColor);

	props.gravityModifier = deserializer["GravityModifier"].getReal();
	props.maxParticles = deserializer["MaxParticles"].getUint();

	persistence::utils::deserializeVector(deserializer["Position"], &props.position);

	Texture* diffuseMap = nullptr, *specularMap = nullptr;

	if (!deserializer["DiffuseMap"].isNull())
	{
		diffuseMap = const_cast<Texture*>(ResourceManager::getTexture(deserializer["DiffuseMap"].getString()));
	}
	if (!deserializer["SpecularMap"].isNull())
	{
		specularMap = const_cast<Texture*>(ResourceManager::getTexture(deserializer["SpecularMap"].getString()));
	}

	Material material(diffuseMap, specularMap, props.startColor, deserializer["Shininess"].getReal());

	std::unique_ptr<Emitter> emitter = std::move(deserializeEmitter(deserializer["Emitter"]));

	ParticleSystem ps(std::move(name), props, material, std::move(emitter));

	if (!enabled)
	{
		ps.disable();
	}

	for (auto& componentDeserializer : deserializer["Components"].getArray())
	{
		ps.addComponent(deserializeComponent(componentDeserializer));
	}

	application.addParticleSystem(std::move(ps));
}