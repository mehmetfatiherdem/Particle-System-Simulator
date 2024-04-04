#include "RenderPipeline/Application.h"
#include "Camera.h"

Camera::Camera(const glm::vec3& position, unsigned int width, unsigned int height, CameraType cameraType, float fov, float near, float far, float left,
	float right, float top, float bottom) : Transformable(position), width(width), height(height), aspectRatio((float)width / height), cameraType(cameraType),
	fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, unsigned int width, unsigned int height, CameraType cameraType, float fov, float near,
	float far, float left, float right, float top, float bottom) : Transformable(position, rotation), width(width), height(height),
	aspectRatio((float)width / height), cameraType(cameraType), fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

Camera::Camera(const glm::vec3& position, const glm::quat& rotation, unsigned int width, unsigned int height, CameraType cameraType, float fov, float near,
	float far, float left, float right, float top, float bottom) : Transformable(position, rotation), width(width), height(height),
	aspectRatio((float)width / height), cameraType(cameraType), fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

Camera::Camera(const Transform& transform, unsigned int width, unsigned int height, CameraType cameraType, float fov, float near, float far, float left,
	float right, float top, float bottom) : Transformable(transform), width(width), height(height), aspectRatio((float)width / height),
	cameraType(cameraType), fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

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

void Camera::setWidth(unsigned int width)
{
	this->width = width;
	this->aspectRatio = (float)width / height;
}

void Camera::setHeight(unsigned int height)
{
	this->height = height;
	this->aspectRatio = (float)width / height;
}

void Camera::setAspectRatio(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->aspectRatio = (float)width / height;
}