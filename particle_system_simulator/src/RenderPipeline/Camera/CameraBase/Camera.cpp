#include "Persistence/Serializer.h"
#include "Persistence/Deserializer.h"
#include "Persistence/Serialization Utils/SerializationUtils.h"
#include "RenderPipeline/Application.h"
#include "Camera.h"

Camera::Camera(const TransformProps& transform, uint32_t width, uint32_t height, CameraType cameraType, float fov, float near, float far, float left,
	float right, float top, float bottom) : Transformable(transform), width(width), height(height), aspectRatio((float)width / height), cameraType(cameraType),
	fov(fov), near(near), far(far), left(left), right(right), top(top), bottom(bottom) { }

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

void Camera::reset()
{
	transform.reset();
	fov = glm::radians(45.0f);
	left = -8.0f;
	right = 8.0f;
	top = 4.5f;
	bottom = -4.5f;
	near = 0.1f;
	far = 1000.0f;
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

void Camera::serialize(Serializer& serializer, const std::string& objectName) const
{
	serializer.startObject(objectName);

	serializer["Fov"].real(fov);
	serializer["Top"].real(top);
	serializer["Bottom"].real(bottom);
	serializer["Left"].real(left);
	serializer["Right"].real(right);
	serializer["Near"].real(near);
	serializer["Far"].real(far);

	serializer.startObject("Transform");
	persistence::utils::serializeVector(serializer, transform.getPosition(), "Position");
	persistence::utils::serializeQuaternion(serializer, transform.getRotation(), "Rotation");
	persistence::utils::serializeVector(serializer, transform.getScale(), "Scale");
	serializer.endObject();

	serializer.endObject();
}

void Camera::deserialize(const Deserializer& deserializer)
{
	fov = deserializer["Fov"].getReal();
	top = deserializer["Top"].getReal();
	bottom = deserializer["Bottom"].getReal();
	left = deserializer["Left"].getReal();
	right = deserializer["Right"].getReal();
	near = deserializer["Near"].getReal();
	far = deserializer["Far"].getReal();

	glm::vec3 position, scale;
	glm::quat rotation;

	persistence::utils::deserializeVector(deserializer["Transform"]["Position"], &position);
	persistence::utils::deserializeQuaternion(deserializer["Transform"]["Rotation"], &rotation);
	persistence::utils::deserializeVector(deserializer["Transform"]["Scale"], &scale);

	transform.setPosition(position);
	transform.setRotation(rotation);
	transform.setScale(scale);
}