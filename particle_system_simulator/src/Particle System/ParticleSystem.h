#pragma once

#include <vector>
#include <memory>
#include <stdint.h>
#include "Data/ParticleSystemProps.h"
#include "Particle System/Particle.h"
#include "Emitter/Emitter.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Scene/Scene.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Texture/Texture.h"

//Limit velocity over lifetime
//Friction over lifetime
//Color over lifetime
//Color by speed
//Size by speed
//Rotation over lifetime
//Rotation by speed


//min particle size
//max particle size

class ParticleSystem
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
