#pragma once

#include "RenderPipeline/Camera/Data/CameraType.h"
#include "RenderPipeline/Transform/Transformable.h"

class Camera : public Transformable
{
protected:
	CameraType cameraType;
	unsigned int width;
	unsigned int height;
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
	Camera(const glm::vec3& position, unsigned int width, unsigned int height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f, float bottom = -4.5f);

	Camera(const glm::vec3& position, const glm::vec3& rotation, unsigned int width, unsigned int height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f, float bottom = -4.5f);

	Camera(const glm::vec3& position, const glm::quat& rotation, unsigned int width, unsigned int height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f, float bottom = -4.5f);

	Camera(const Transform& transform, unsigned int width, unsigned int height, CameraType cameraType = CameraType::Perspective, float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f, float bottom = -4.5f);

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewProjectionMatrix() const;

	unsigned int getWidth() const { return width; }
	unsigned int getHeight() const { return height; }
	float getAspectRatio() const { return aspectRatio; }
	float getFov() const { return fov; }
	float getNear() const { return near; }
	float getFar() const { return far; }
	float getLeft() const { return left; }
	float getRight() const { return right; }
	float getTop() const { return top; }
	float getBottom() const { return bottom; }
	CameraType getCameraType() const { return cameraType; }

	void setWidth(unsigned int width);
	void setHeight(unsigned int height);
	void setAspectRatio(unsigned int width, unsigned int height);
	void setFov(float fov) { this->fov = fov; }
	void setNear(float near) { this->near = near; }
	void setFar(float far) { this->far = far; }
	void setLeft(float left) { this->left = left; }
	void setRight(float right) { this->right = right; }
	void setTop(float top) { this->top = top; }
	void setBottom(float bottom) { this->bottom = bottom; }
	void setCameraType(CameraType cameraType) { this->cameraType = cameraType; }
};