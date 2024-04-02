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
	return glm::inverse(transform.getModelMatrix());
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