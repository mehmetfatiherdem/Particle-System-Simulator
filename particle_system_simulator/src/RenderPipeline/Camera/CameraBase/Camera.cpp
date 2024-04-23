#include "RenderPipeline/Application.h"
#include "Camera.h"

Camera::Camera(const glm::vec3& position, uint32_t width, uint32_t height, CameraType cameraType, float fov, float near, float far, float left,
	float right, float top, float bottom) : Transformable(position), width(width), height(height), aspectRatio((float)width / height), cameraType(cameraType),
	fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, uint32_t width, uint32_t height, CameraType cameraType, float fov, float near,
	float far, float left, float right, float top, float bottom) : Transformable(position, rotation), width(width), height(height),
	aspectRatio((float)width / height), cameraType(cameraType), fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

Camera::Camera(const glm::vec3& position, const glm::quat& rotation, uint32_t width, uint32_t height, CameraType cameraType, float fov, float near,
	float far, float left, float right, float top, float bottom) : Transformable(position, rotation), width(width), height(height),
	aspectRatio((float)width / height), cameraType(cameraType), fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

Camera::Camera(const Transform& transform, uint32_t width, uint32_t height, CameraType cameraType, float fov, float near, float far, float left,
	float right, float top, float bottom) : Transformable(transform), width(width), height(height), aspectRatio((float)width / height),
	cameraType(cameraType), fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

glm::mat4 Camera::getViewMatrix() const
{
	return glm::inverse(transform.getModelMatrix());
}

glm::mat4 Camera::getProjectionMatrix(float useFov) const
{
	switch(cameraType)
	{
	case CameraType::Orthographic:
		return glm::ortho(left, right, bottom, top, near, far);
	case CameraType::Perspective:
		return glm::perspective((useFov < 0 ? fov : useFov), aspectRatio, near, far);
	}
}

glm::mat4 Camera::getViewProjectionMatrix(float useFov) const
{
	return getProjectionMatrix(useFov) * getViewMatrix();
}

void Camera::setWidth(uint32_t width)
{
	this->width = width;
	this->aspectRatio = (float)width / height;
}

void Camera::setHeight(uint32_t height)
{
	this->height = height;
	this->aspectRatio = (float)width / height;
}

void Camera::setAspectRatio(uint32_t width, uint32_t height)
{
	this->width = width;
	this->height = height;
	this->aspectRatio = (float)width / height;
}