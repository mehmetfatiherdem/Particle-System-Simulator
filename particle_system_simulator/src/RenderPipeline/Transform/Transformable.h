#pragma once

#include "Transform.h"
#include "Data/TransformProps.h"

class Transformable
{
protected:
	Transform transform;

	Transformable() : transform() { }
	Transformable(const TransformProps& props) : transform(props.position, props.rotation, props.scale) {}

public:
	Transform& getTransform() { return transform; }
	glm::mat4 getModelMatrix() const { return transform.getModelMatrix(); }
};
