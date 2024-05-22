#pragma once

#include <glm/vec3.hpp>
#include "RenderPipeline/Color/Color4.h"
#include "RenderPipeline/Object/MeshRenderer.h"

//I somehow couldn't make this a struct (it wouldn't let me strangely) and had to make it a class and then make the members public instead
class Particle
{
public:
	MeshRenderer* renderer = nullptr;
	glm::vec3 velocity = glm::vec3{3.0f, 3.0f, 3.0f};
	float rotation = 0.0f;
	float remainingLifetime = 0.0f;

	bool isEnabled() { return renderer->isEnabled(); }
	void enable() { renderer->enable(); }
	void disable() { renderer->disable(); }
	void toggle() { renderer->toggle(); }
	void setEnabled(bool enabled) { renderer->setEnabled(enabled); }
};