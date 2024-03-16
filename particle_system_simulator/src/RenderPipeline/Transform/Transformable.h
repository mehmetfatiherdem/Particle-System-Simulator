#pragma once

#include "Transform.h"

class Transformable
{
protected:
	Transform transform;

	Transformable() : transform() { }
	Transformable(const glm::vec3& position) : transform(position) { }
	Transformable(const glm::vec3& position, const glm::vec3& rotation) : transform(position, rotation) { }
	Transformable(const glm::vec3& position, const glm::quat& rotation) : transform(position, rotation) { }
	Transformable(const Transform& transform) : transform(transform) { }

public:
	Transform& getTransform() { return transform; }
	glm::mat4 getModelMatrix() const { return transform.getModelMatrix(); }
};
