#include "Camera.h"

Camera::Camera(const glm::vec3& position, float aspectRatio, CameraType cameraType, float fov, float near, float far, float left, float right,
	float top, float bottom) : Transformable(position), aspectRatio(aspectRatio), cameraType(cameraType), fov(fov), near(near), far(far), left(left),
	right(right), top(top), bottom(bottom) { }

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, float aspectRatio, CameraType cameraType, float fov, float near, float far, float left, float right,
	float top, float bottom) : Transformable(position, rotation), aspectRatio(aspectRatio), cameraType(cameraType), fov(fov), near(near), far(far), left(left),
	right(right), top(top), bottom(bottom) { }

Camera::Camera(const glm::vec3& position, const glm::quat& rotation, float aspectRatio, CameraType cameraType, float fov, float near, float far, float left, float right,
	float top, float bottom) : Transformable(position, rotation), aspectRatio(aspectRatio), cameraType(cameraType), fov(fov), near(near), far(far), left(left),
	right(right), top(top), bottom(bottom) { }

Camera::Camera(const Transform& transform, float aspectRatio, CameraType cameraType, float fov, float near, float far, float left, float right, float top, float bottom) :
	Transformable(transform), aspectRatio(aspectRatio), cameraType(cameraType), fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

glm::mat4 Camera::getViewMatrix() const
{
	/*glm::vec3 position = transform.getPosition();
	glm::quat rotation = transform.getRotation();
	glm::vec3 forward = transform.getForwardVector();
	glm::vec3 up = transform.getUpVector();
	glm::vec3 right = transform.getRightVector();

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix[0][0] = right.x;
	viewMatrix[1][0] = right.y;
	viewMatrix[2][0] = right.z;
	viewMatrix[0][1] = up.x;
	viewMatrix[1][1] = up.y;
	viewMatrix[2][1] = up.z;
	viewMatrix[0][2] = -forward.x;
	viewMatrix[1][2] = -forward.y;
	viewMatrix[2][2] = -forward.z;
	viewMatrix[3][0] = -glm::dot(right, position);
	viewMatrix[3][1] = -glm::dot(up, position);
	viewMatrix[3][2] = glm::dot(forward, position);

	return viewMatrix;*/

	//The above code reverses the z axis, we can still find a workaround and compare these two methods' efficiencies
	return glm::inverse(getModelMatrix());
}

glm::mat4 Camera::getProjectionMatrix() const
{
	switch(cameraType)
	{
	case CameraType::Orthographic:
		return glm::ortho(left, right, bottom, top, near, far);
	case CameraType::Perspective:
		return glm::perspective(fov, aspectRatio, near, far);
	}
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
	return getProjectionMatrix() * getViewMatrix();
}