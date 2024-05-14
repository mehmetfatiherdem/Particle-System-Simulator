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
	float minSize = 0.3f;
	float maxSize = 2.4f;
	float startRotation = 0.0f;
	Color4 startColor = Color4{glm::vec3{1.0f, 0.0f, 0.0f}};
	float gravityModifier = 0.0f;
	uint32_t maxParticles = 10;
	uint32_t currentParticles = 0;
	glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f};
};