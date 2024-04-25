#pragma once

#include "RenderPipeline/Transform/Transform.h"

struct TransformProps
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	TransformProps() :
		position(glm::vec3{0.0, 0.0, 0.0}),
		rotation(glm::quat{1.0, 0.0, 0.0, 0.0}),
		scale(glm::vec3{1.0, 1.0, 1.0})
	{}

	TransformProps(const glm::vec3& position) :
		position(position),
		rotation(glm::quat{1.0f, 0.0f, 0.0f, 0.0f}),
		scale(glm::vec3{1.0f, 1.0f, 1.0f})
	{}

	TransformProps(const glm::quat& rotation) :
		position(glm::vec3{0.0f, 0.0f, 0.0f}),
		rotation(rotation),
		scale(glm::vec3{1.0f, 1.0f, 1.0f})
	{}

	TransformProps(const glm::vec3& position, const glm::vec3& rotation) :
		position(position),
		rotation(rotation),
		scale(glm::vec3{1.0f, 1.0f, 1.0f})
	{}

	TransformProps(const glm::vec3& position, const glm::quat& rotation) :
		position(position),
		rotation(rotation),
		scale(glm::vec3{1.0f, 1.0f, 1.0f})
	{}

	TransformProps(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
		position(position),
		rotation(rotation),
		scale(scale)
	{}

	TransformProps(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) :
		position(position),
		rotation(rotation),
		scale(scale)
	{}

	TransformProps(const Transform& transform) :
		position(transform.getPosition()),
		rotation(transform.getRotation()),
		scale(transform.getScale())
	{}

	TransformProps(const TransformProps&) = delete;
	TransformProps(TransformProps&&) = delete;
	~TransformProps() = default;

	TransformProps& operator=(const TransformProps&) = delete;
	TransformProps& operator=(TransformProps&&) = delete;
};