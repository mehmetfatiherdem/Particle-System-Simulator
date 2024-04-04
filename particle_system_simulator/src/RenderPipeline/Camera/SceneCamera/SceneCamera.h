#pragma once

#include "RenderPipeline/Camera/CameraBase/Camera.h"

class SceneCamera : public Camera
{
private:
	float rotationSpeed;
	float slowMovementSpeed;
	float fastMovementSpeed;

public:
	SceneCamera() = delete;
	SceneCamera(const glm::vec3& position, unsigned int width, unsigned int height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f,
		float bottom = -4.5f, float rotationSpeed = 1.2f, float slowMovementSpeed = 0.18f, float fastMovementSpeed = 0.72f);

	SceneCamera(const glm::vec3& position, const glm::vec3& rotation, unsigned int width, unsigned int height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f,
		float bottom = -4.5f, float rotationSpeed = 1.2f, float slowMovementSpeed = 0.18f, float fastMovementSpeed = 0.72f);

	SceneCamera(const glm::vec3& position, const glm::quat& rotation, unsigned int width, unsigned int height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f,
		float bottom = -4.5f, float rotationSpeed = 1.2f, float slowMovementSpeed = 0.18f, float fastMovementSpeed = 0.72f);

	SceneCamera(const Transform& transform, unsigned int width, unsigned int height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -8.0f, float right = 8.0f, float top = 4.5f,
		float bottom = -4.5f, float rotationSpeed = 1.2f, float slowMovementSpeed = 0.18f, float fastMovementSpeed = 0.72f);

	void update();

	float getRotationSpeed() const { return rotationSpeed; }
	float getMovementSpeed() const { return slowMovementSpeed; }
	float getSpeedMultiplier() const { return fastMovementSpeed; }

	void setRotationSpeed(float rotationSpeed) { this->rotationSpeed = rotationSpeed; }
	void setMovementSpeed(float slowMovementSpeed) { this->slowMovementSpeed = slowMovementSpeed; }
	void setSpeedMultiplier(float fastMovementSpeed) { this->fastMovementSpeed = fastMovementSpeed; }
};