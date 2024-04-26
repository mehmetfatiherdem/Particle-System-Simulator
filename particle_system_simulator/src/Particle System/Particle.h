#pragma once

#include <glm/vec3.hpp>
#include "RenderPipeline/Color/Color4.h"
#include "RenderPipeline/Object/MeshRenderer.h"

struct Particle
{
	MeshRenderer* renderer;
	glm::vec3 velocity;
	glm::vec3 angularVelocity;
	Color4 color;
	float remainingLifetime;

	bool isEnabled() { return renderer->isEnabled(); }
	void enable() { renderer->enable(); }
	void disable() { renderer->disable(); }
	void toggle() { renderer->toggle(); }
	void setEnabled(bool enabled) { renderer->setEnabled(enabled); }
};