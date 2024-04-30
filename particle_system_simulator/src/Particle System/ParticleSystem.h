#pragma once

#include <vector>
#include <memory>
#include <stdint.h>
#include "EntityComponentSystem/Entity.h"
#include "Data/ParticleSystemProps.h"
#include "Particle System/Particle.h"
#include "Emitter/Emitter.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Scene/Scene.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Texture/Texture.h"

class ParticleSystem : public Entity
{
private:
	std::vector<Particle> particlePool;
	uint32_t poolIndex;

	Mesh sphere;
	Material* material;

	ParticleSystemProps props;
	std::unique_ptr<Emitter> emitter;
	Scene& scene;
	
public:
	ParticleSystem() = delete;
	ParticleSystem(ParticleSystemProps props, std::unique_ptr<Emitter> emitter);
	~ParticleSystem() = default;

	void update();
};
