#pragma once

#include <string>
#include <stdint.h>
#include <vector>
#include <set>
#include <memory>
#include <algorithm>
#include "Data/ParticleSystemProps.h"
#include "Particle System/Particle.h"
#include "Emitter/Emitter.h"
#include "RenderPipeline/Material/Material.h"
#include "Components/ComponentComparator.h"
#include "Persistence/ISerializable.h"

class ParticleSystemEditor;
class Component;
class Texture;
class Scene;
class Deserializer;

using OrderedComponentSet = std::set<Component*, ComponentComparator>;

class ParticleSystem : public ISerializable
{
private:
	friend class ParticleSystemEditor;

	std::vector<Particle> particlePool;
	std::string name;
	bool enabled;
	uint32_t poolIndex;

	Material material;

	ParticleSystemProps props;
	std::unique_ptr<Emitter> emitter;
	std::vector<Component*> components;

	Mesh quad;

	Scene& scene;
	
public:
	ParticleSystem();
	ParticleSystem(std::string&& name, const ParticleSystemProps& props, const Material& material, std::unique_ptr<Emitter> emitter);
	ParticleSystem(const ParticleSystem& other) = delete;
	ParticleSystem(ParticleSystem&& other) noexcept;
	~ParticleSystem();

	ParticleSystem& operator=(const ParticleSystem& other) = delete;
	ParticleSystem& operator=(ParticleSystem&& other) noexcept;

	void addComponent(Component* component);
	void removeComponent(Component* component);

	void setMaxParticles(uint32_t maxParticles);
	void setDiffuseMap(Texture* diffuseMap);
	void setSpecularMap(Texture* specularMap);

	void update();

	bool isEnabled() const { return enabled; }
	void enable() { enabled = true; }
	void disable() { enabled = false; }
	void toggle() { enabled = !enabled; }

	const std::string& getName() const { return name; }

	virtual void serialize(Serializer& serializer, const std::string& objectName = "") const override;
};
