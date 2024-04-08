#include "LightManagement/LightTracker.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& direction) :
	LightSource(lightTracker, TransformProps{}, color)
{
	transform.setForwardVector(direction);
}

void DirectionalLight::lightSourceUpdated() const
{
	lightTracker.updateLight(this);
}

void DirectionalLight::setDirection(const glm::vec3& direction)
{
	this->transform.setForwardVector(direction);
	lightSourceUpdated();
}

void DirectionalLight::rotate(const glm::vec3& rotation)
{
	this->transform.rotate(rotation);
	lightSourceUpdated();
}

void DirectionalLight::rotate(const glm::quat& rotation)
{
	this->transform.rotate(rotation);
	lightSourceUpdated();
}

void DirectionalLight::rotateAround(const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(axis, angle);
	lightSourceUpdated();
}

void DirectionalLight::rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(point, axis, angle);
	lightSourceUpdated();
}

void DirectionalLight::rotateAround(const Transform& transform, const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(transform, axis, angle);
	lightSourceUpdated();
}

void DirectionalLight::lookAt(const Transform& transform)
{
	this->transform.lookAt(transform);
	lightSourceUpdated();
}

void DirectionalLight::lookAt(const glm::vec3& point)
{
	this->transform.lookAt(point);
	lightSourceUpdated();
}

void DirectionalLight::setRightVector(const glm::vec3& right)
{
	this->transform.setRightVector(right);
	lightSourceUpdated();
}

void DirectionalLight::setUpVector(const glm::vec3& up)
{
	this->transform.setUpVector(up);
	lightSourceUpdated();
}

void DirectionalLight::setForwardVector(const glm::vec3& forward)
{
	this->transform.setForwardVector(forward);
	lightSourceUpdated();
}

void DirectionalLight::setRotation(const glm::quat& rotation)
{
	this->transform.setRotation(rotation);
	lightSourceUpdated();
}

void DirectionalLight::setEulerRotation(const glm::vec3& rotation)
{
	this->transform.setEulerRotation(rotation);
	lightSourceUpdated();
}
