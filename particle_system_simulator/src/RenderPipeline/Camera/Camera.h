#pragma once

#include "RenderPipeline/Transform/Transformable.h"

enum class CameraType : bool
{
	Orthographic = 0,
	Perspective,
};

class Camera : public Transformable
{
private:
	CameraType cameraType;
	float aspectRatio;
	float fov;
	float near;
	float far;
	float left;
	float right;
	float top;
	float bottom;

public:
	Camera() = delete;
	Camera(const glm::vec3& position, float aspectRatio, CameraType cameraType = CameraType::Perspective, float fov = glm::radians(60.0f), float near = 0.1f,
		float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f, float bottom = -4.5f);

	Camera(const glm::vec3& position, const glm::vec3& rotation, float aspectRatio, CameraType cameraType = CameraType::Perspective, float fov = glm::radians(60.0f),
		float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f, float bottom = -4.5f);

	Camera(const glm::vec3& position, const glm::quat& rotation, float aspectRatio, CameraType cameraType = CameraType::Perspective, float fov = glm::radians(60.0f),
		float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f, float bottom = -4.5f);

	Camera(const Transform& transform, float aspectRatio, CameraType cameraType = CameraType::Perspective, float fov = glm::radians(60.0f), float near = 0.1f,
		float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f, float bottom = -4.5f);

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewProjectionMatrix() const;

	float getAspectRatio() const { return aspectRatio; }
	float getFov() const { return fov; }
	float getNear() const { return near; }
	float getFar() const { return far; }
	float getLeft() const { return left; }
	float getRight() const { return right; }
	float getTop() const { return top; }
	float getBottom() const { return bottom; }
	CameraType getCameraType() const { return cameraType; }

	void setAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }
	void setFov(float fov) { this->fov = fov; }
	void setNear(float near) { this->near = near; }
	void setFar(float far) { this->far = far; }
	void setLeft(float left) { this->left = left; }
	void setRight(float right) { this->right = right; }
	void setTop(float top) { this->top = top; }
	void setBottom(float bottom) { this->bottom = bottom; }
	void setCameraType(CameraType cameraType) { this->cameraType = cameraType; }
};