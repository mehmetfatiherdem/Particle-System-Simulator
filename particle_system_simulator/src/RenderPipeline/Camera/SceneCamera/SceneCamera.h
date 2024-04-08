#pragma once

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
	SceneCamera(const CameraProps& props) : Camera(props) { }
	SceneCamera(const SceneCamera&) = default;
	SceneCamera(SceneCamera&&) = delete;
	~SceneCamera() = default;

	SceneCamera& operator=(const SceneCamera&) = default;
	SceneCamera& operator=(SceneCamera&&) = delete;

	void update();

	float getRotationSpeed() const { return rotationSpeed; }
	float getMovementSpeed() const { return slowMovementSpeed; }
	float getSpeedMultiplier() const { return fastMovementSpeed; }

	void setRotationSpeed(float rotationSpeed) { this->rotationSpeed = rotationSpeed; }
	void setMovementSpeed(float slowMovementSpeed) { this->slowMovementSpeed = slowMovementSpeed; }
	void setSpeedMultiplier(float fastMovementSpeed) { this->fastMovementSpeed = fastMovementSpeed; }
};