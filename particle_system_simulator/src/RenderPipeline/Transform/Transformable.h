#pragma once

#include "Transform.h"
#include "Data/TransformProps.h"

class Transformable
{
protected:
	Transform transform;

	Transformable() : transform() { }
	Transformable(const glm::vec3& position) : transform(position) { }
	Transformable(const glm::vec3& position, const glm::vec3& rotation) : transform(position, rotation) { }
	Transformable(const glm::vec3& position, const glm::quat& rotation) : transform(position, rotation) { }
	Transformable(const TransformProps& props) : transform(props.position, props.rotation, props.scale) {}
	Transformable(const Transform& transform) : transform(transform) { }

public:
	Transform& getTransform() { return transform; }
	glm::mat4 getModelMatrix() const { return transform.getModelMatrix(); }
};
