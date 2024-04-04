#include "Input Management/Input.h"
#include "Input Management/Data/KeyCode.h"
#include "Input Management/Data/MouseButton.h"
#include "SceneCamera.h"
#include <iostream>

SceneCamera::SceneCamera(const glm::vec3& position, unsigned int width, unsigned int height, CameraType cameraType, float fov, float near, float far,
	float left, float right, float top, float bottom, float rotationSpeed, float slowMovementSpeed, float fastMovementSpeed) :
	Camera(position, width, height, cameraType, fov, near, far, left, right, top, bottom), rotationSpeed(rotationSpeed), slowMovementSpeed(slowMovementSpeed),
	fastMovementSpeed(fastMovementSpeed) { }

SceneCamera::SceneCamera(const glm::vec3& position, const glm::vec3& rotation, unsigned int width, unsigned int height, CameraType cameraType, float fov,
	float near, float far, float left, float right, float top, float bottom, float rotationSpeed, float slowMovementSpeed, float fastMovementSpeed) :
	Camera(position, rotation, width, height, cameraType, fov, near, far, left, right, top, bottom), rotationSpeed(rotationSpeed),
	slowMovementSpeed(slowMovementSpeed), fastMovementSpeed(fastMovementSpeed) { }

SceneCamera::SceneCamera(const glm::vec3& position, const glm::quat& rotation, unsigned int width, unsigned int height, CameraType cameraType, float fov,
	float near, float far, float left, float right, float top, float bottom, float rotationSpeed, float slowMovementSpeed, float fastMovementSpeed) :
	Camera(position, rotation, width, height, cameraType, fov, near, far, left, right, top, bottom), rotationSpeed(rotationSpeed),
	slowMovementSpeed(slowMovementSpeed), fastMovementSpeed(fastMovementSpeed) { }

SceneCamera::SceneCamera(const Transform& transform, unsigned int width, unsigned int height, CameraType cameraType, float fov, float near, float far,
	float left, float right, float top, float bottom, float rotationSpeed, float slowMovementSpeed, float fastMovementSpeed) :
	Camera(transform, width, height, cameraType, fov, near, far, left, right, top, bottom), rotationSpeed(rotationSpeed), slowMovementSpeed(slowMovementSpeed),
	fastMovementSpeed(fastMovementSpeed) { }

void SceneCamera::update()
{
	if(!Input::getMouseButton(MouseButton::MOUSE_RIGHT))
		return;

	glm::vec2 mouseDelta = Input::getMouseDelta();
	float yRotation = -rotationSpeed * (mouseDelta.x / width);
	float xRotation = -rotationSpeed * (mouseDelta.y / height);

	transform.rotateAround(transform.WORLD_UP, yRotation);
	transform.rotateAround(transform.getRightVector(), xRotation);

	std::cout << transform.getEulerRotation().z << "\n";

	glm::vec3 move{0.0f, 0.0f, 0.0f};

	if(Input::getKey(KeyCode::KEY_W))
	{
		move -= transform.getForwardVector();
	}
	else if(Input::getKey(KeyCode::KEY_S))
	{
		move += transform.getForwardVector();
	}

	if(Input::getKey(KeyCode::KEY_A))
	{
		move -= transform.getRightVector();
	}
	else if(Input::getKey(KeyCode::KEY_D))
	{
		move += transform.getRightVector();
	}

	if(Input::getKey(KeyCode::KEY_Q))
	{
		move -= transform.getUpVector();
	}
	else if(Input::getKey(KeyCode::KEY_E))
	{
		move += transform.getUpVector();
	}

	float movementSpeed = Input::getKey(KeyCode::SHIFT) ? fastMovementSpeed : slowMovementSpeed;

	transform.translate(movementSpeed * move);
}