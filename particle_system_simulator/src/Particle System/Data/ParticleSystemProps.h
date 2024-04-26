#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "RenderPipeline/Color/Color4.h"

struct ParticleSystemProps
{
	float startLifetime = 10.0f;
	float startSpeed = 1.0f;
	float startSize = 1.0f;
	glm::vec3 startRotation = glm::vec3{0.0f, 0.0f, 0.0f};
	Color4 startColor = Color4{glm::vec3{1.0f, 1.0f, 1.0f}};
	float gravityModifier = 1.0f;
	uint32_t maxParticles = 1000;
	glm::vec3 position;
};