#include "LightManagement/LightTracker.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& direction) : LightSource(lightTracker, color)
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
