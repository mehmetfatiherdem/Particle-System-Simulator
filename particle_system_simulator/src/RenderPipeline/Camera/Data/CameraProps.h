#pragma once

#include "RenderPipeline/Transform/Data/TransformProps.h"
#include "CameraType.h"

struct CameraProps
{
	const TransformProps& transform;
	CameraType cameraType;
	uint32_t width;
	uint32_t height;
	float fov;
	float near;
	float far;
	float left;
	float right;
	float top;
	float bottom;

	CameraProps() = delete;

	CameraProps(const TransformProps& transform, uint32_t width, uint32_t height, float fov = glm::radians(45.0f),
		float near = 0.1f, float far = 100.0f) :
		transform(transform),
		width(width),
		height(height),
		fov(fov),
		near(near),
		far(far),
		cameraType(CameraType::Perspective),
		left(-8.0f),
		right(8.0f),
		top(4.5f),
		bottom(-4.5f) { }

	CameraProps(const TransformProps& transform, uint32_t width, uint32_t height, float near = 0.1f, float far = 100.0f,
		float left = -8.0f, float right = 8.0f, float top = 4.5f, float bottom = -4.5f) :
		transform(transform),
		width(width),
		height(height),
		near(near),
		far(far),
		left(left),
		right(right),
		top(top),
		bottom(bottom),
		cameraType(CameraType::Orthographic),
		fov(glm::radians(45.0f)) { }

	CameraProps(const TransformProps& transform, uint32_t width, uint32_t height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f,
		float top = 4.5f, float bottom = -4.5f) :
		transform(transform),
		width(width),
		height(height),
		cameraType(cameraType),
		fov(fov),
		near(near),
		far(far),
		left(left),
		right(right),
		top(top),
		bottom(bottom) { }

	CameraProps(const CameraProps&) = delete;
	CameraProps(CameraProps&&) = delete;
	~CameraProps() = default;

	CameraProps& operator=(const CameraProps&) = delete;
	CameraProps& operator=(CameraProps&&) = delete;
};