#pragma once

#include <glm/vec3.hpp>
#include "RenderPipeline/Color/Color4.h"
#include "RenderPipeline/Object/MeshRenderer.h"

class Particle
{
public:
	MeshRenderer* renderer = nullptr;
	glm::vec3 velocity = glm::vec3{3.0f, 3.0f, 3.0f};
	glm::vec3 angularVelocity = glm::vec3{0.1f, 0.1f, 0.1f};
	Color4 color = Color4{glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}};
	float remainingLifetime = 0.0f;

	bool isEnabled() { return renderer->isEnabled(); }
	void enable() { renderer->enable(); }
	void disable() { renderer->disable(); }
	void toggle() { renderer->toggle(); }
	void setEnabled(bool enabled) { renderer->setEnabled(enabled); }
};