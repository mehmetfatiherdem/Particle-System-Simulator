#pragma once

#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <memory>
#include <stdint.h>
#include "Data/ParticleSystemProps.h"
#include "Particle System/Particle.h"
#include "Emitter/Emitter.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Scene/Scene.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Texture/Texture.h"
#include "Components/Component.h"
#include "Components/ComponentComparator.h"

//Friction over lifetime
//Rotation over lifetime

using OrderedComponentSet = std::set<Component*, ComponentComparator>;

class ParticleSystem
{
private:
	std::vector<Particle> particlePool;
	uint32_t poolIndex;

	Mesh sphere;
	Material material;

	ParticleSystemProps props;
	std::unique_ptr<Emitter> emitter;
	OrderedComponentSet components;

	Scene& scene;
	
public:
	ParticleSystem() = delete;
	ParticleSystem(const ParticleSystemProps& props, const Material& material, std::unique_ptr<Emitter> emitter);
	~ParticleSystem() = default;

	void addComponent(Component* component);

	void update();
};
