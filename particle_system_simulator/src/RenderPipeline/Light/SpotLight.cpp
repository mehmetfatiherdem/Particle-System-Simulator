#include "LightManagement/LightTracker.h"
#include "SpotLight.h"

SpotLight::SpotLight(const LightTracker& lightTracker, const glm::vec3& position, const glm::vec3& direction, const Color3& color, float innerCutOffAngle, float outerCutOffAngle,
	LightDistance distance) : PositionalLight(lightTracker, position, color, distance), innerCutOffAngle(innerCutOffAngle), outerCutOffAngle(outerCutOffAngle) 
{
	transform.setForwardVector(direction);
}

SpotLight::SpotLight(const LightTracker& lightTracker, const glm::vec3& position, const glm::vec3& direction, const Color3& color, float innerCutOffAngle, float outerCutOffAngle,
	float constant, float linear, float quadratic) : PositionalLight(lightTracker, position, color, constant, linear, quadratic), innerCutOffAngle(innerCutOffAngle),
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