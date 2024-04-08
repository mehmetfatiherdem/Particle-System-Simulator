#include "LightManagement/LightTracker.h"
#include "SpotLight.h"

SpotLight::SpotLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& position,
	const glm::vec3& direction, float innerCutOffAngle, float outerCutOffAngle, LightDistance distance) :
	PositionalLight(lightTracker, color, position, distance), innerCutOffAngle(innerCutOffAngle),
	outerCutOffAngle(outerCutOffAngle)
{
	transform.setForwardVector(direction);
}

SpotLight::SpotLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& position,
	const glm::vec3& direction, float innerCutOffAngle, float outerCutOffAngle, float constant, float linear, float quadratic) :
	PositionalLight(lightTracker, color, position, constant, linear, quadratic), innerCutOffAngle(innerCutOffAngle),
	outerCutOffAngle(outerCutOffAngle)

{
	transform.setForwardVector(direction);
}

void SpotLight::lightSourceUpdated() const
{
	lightTracker.updateLight(this);
}

void SpotLight::setInnerCutOffAngle(float angle)
{
	this->innerCutOffAngle = angle;
	lightSourceUpdated();
}

void SpotLight::setOuterCutOffAngle(float angle)
{
	this->outerCutOffAngle = angle;
	lightSourceUpdated();
}

void SpotLight::setCutOffAngles(float inner, float outer)
{
	this->innerCutOffAngle = inner;
	this->outerCutOffAngle = outer;
	lightSourceUpdated();
}

void SpotLight::setDirection(const glm::vec3& direction)
{
	this->transform.setForwardVector(direction);
	lightSourceUpdated();
}

void SpotLight::rotate(const glm::vec3& rotation)
{
	this->transform.rotate(rotation);
	lightSourceUpdated();
}

void SpotLight::rotate(const glm::quat& rotation)
{
	this->transform.rotate(rotation);
	lightSourceUpdated();
}

void SpotLight::rotateAround(const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(axis, angle);
	lightSourceUpdated();
}

void SpotLight::rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(point, axis, angle);
	lightSourceUpdated();
}

void SpotLight::rotateAround(const Transform& transform, const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(transform, axis, angle);
	lightSourceUpdated();
}

void SpotLight::lookAt(const Transform& transform)
{
	this->transform.lookAt(transform);
	lightSourceUpdated();
}

void SpotLight::lookAt(const glm::vec3& point)
{
	this->transform.lookAt(point);
	lightSourceUpdated();
}

void SpotLight::setRightVector(const glm::vec3& right)
{
	this->transform.setRightVector(right);
	lightSourceUpdated();
}

void SpotLight::setUpVector(const glm::vec3& up)
{
	this->transform.setUpVector(up);
	lightSourceUpdated();
}

void SpotLight::setForwardVector(const glm::vec3& forward)
{
	this->transform.setForwardVector(forward);
	lightSourceUpdated();
}

void SpotLight::setRotation(const glm::quat& rotation)
{
	this->transform.setRotation(rotation);
	lightSourceUpdated();
}

void SpotLight::setEulerRotation(const glm::vec3& rotation)
{
	this->transform.setEulerRotation(rotation);
	lightSourceUpdated();
}