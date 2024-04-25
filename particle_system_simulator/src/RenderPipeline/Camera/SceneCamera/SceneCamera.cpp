#include "Input Management/Input.h"
#include "Time Management/Time.h"
#include "Input Management/Data/KeyCode.h"
#include "Input Management/Data/MouseButton.h"
#include "GeneralUtility/MathConstants.h"
#include "SceneCamera.h"

SceneCamera::SceneCamera(const TransformProps& transform, uint32_t width, uint32_t height, CameraType cameraType, float fov, float near, float far,
	float left, float right, float top, float bottom, float rotationSpeed, float slowMovementSpeed, float fastMovementSpeed, float zoomSensitivity) :
	Camera(transform, width, height, cameraType, fov, near, far, left, right, top, bottom), rotationSpeed(rotationSpeed), slowMovementSpeed(slowMovementSpeed),
	fastMovementSpeed(fastMovementSpeed), zoomSensitivity(zoomSensitivity) { }

void SceneCamera::update()
{
	if(Input::getKeyDown(KeyCode::KEY_Z))
	{
		cameraType = static_cast<CameraType>(!static_cast<bool>(cameraType));
	}

	fov += -150 * Input::getScroll() * zoomSensitivity * Time::deltaTime();

	if(fov <= 0)
	{
		fov = std::numeric_limits<float>::epsilon();
	}
	else if(fov >= PI)
	{
		fov = PI - std::numeric_limits<float>::epsilon();
	}

	if(!Input::getMouseButton(MouseButton::MOUSE_RIGHT))
		return;

	glm::vec2 rot = -150 * rotationSpeed * Input::getMouseDelta() * Time::deltaTime();

	transform.rotateAround(transform.WORLD_UP, rot.x);
	transform.rotateAround(transform.getRightVector(), rot.y);

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

	transform.translate(movementSpeed * move * Time::deltaTime());
}