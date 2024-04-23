#pragma once

#include <stdint.h>
#include "RenderPipeline/Camera/CameraBase/Camera.h"

class SceneCamera : public Camera
{
private:
	float rotationSpeed;
	float slowMovementSpeed;
	float fastMovementSpeed;
	float zoomSensitivity;

public:
	SceneCamera() = delete;
	SceneCamera(const glm::vec3& position, uint32_t width, uint32_t height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -1.0f, float right = 1.0f, float top = 1.0f,
		float bottom = -1.0f, float rotationSpeed = glm::radians(180.0f), float slowMovementSpeed = 5.0f, float fastMovementSpeed = 15.0f, float zoomSensitivity = glm::radians(5.0f));

	SceneCamera(const glm::vec3& position, const glm::vec3& rotation, uint32_t width, uint32_t height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -1.0f, float right = 1.0f, float top = 1.0f,
		float bottom = -1.0f, float rotationSpeed = 1.2f, float slowMovementSpeed = 0.18f, float fastMovementSpeed = 0.72f, float zoomSensitivity = 0.05f);

	SceneCamera(const glm::vec3& position, const glm::quat& rotation, uint32_t width, uint32_t height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -1.0f, float right = 1.0f, float top = 1.0f,
		float bottom = -1.0f, float rotationSpeed = 1.2f, float slowMovementSpeed = 0.18f, float fastMovementSpeed = 0.72f, float zoomSensitivity = 0.05f);

	SceneCamera(const Transform& transform, uint32_t width, uint32_t height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 100.0f, float left = -1.0f, float right = 1.0f, float top = 1.0f,
		float bottom = -1.0f, float rotationSpeed = 1.2f, float slowMovementSpeed = 0.18f, float fastMovementSpeed = 0.72f, float zoomSensitivity = 0.05f);

	void update();

	float getRotationSpeed() const { return rotationSpeed; }
	float getMovementSpeed() const { return slowMovementSpeed; }
	float getSpeedMultiplier() const { return fastMovementSpeed; }

	void setRotationSpeed(float rotationSpeed) { this->rotationSpeed = rotationSpeed; }
	void setMovementSpeed(float slowMovementSpeed) { this->slowMovementSpeed = slowMovementSpeed; }
	void setSpeedMultiplier(float fastMovementSpeed) { this->fastMovementSpeed = fastMovementSpeed; }
};