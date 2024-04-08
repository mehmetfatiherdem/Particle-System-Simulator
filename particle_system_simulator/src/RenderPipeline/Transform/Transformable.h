#pragma once

#include "Transform.h"

class Transformable
{
protected:
	Transform transform;

	Transformable() = delete;
	Transformable(const TransformProps& props) : transform(props) { }
	Transformable(const Transformable&) = default;
	Transformable(Transformable&&) = delete;
	~Transformable() = default;
	Transformable& operator=(const Transformable&) = default;
	Transformable& operator=(Transformable&&) = delete;
	
public:
	Transform& getTransform() { return transform; }
	glm::mat4 getModelMatrix() const { return transform.getModelMatrix(); }
};
