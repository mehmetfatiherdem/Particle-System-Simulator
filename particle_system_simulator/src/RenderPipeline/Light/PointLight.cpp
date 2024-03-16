#include "LightManagement/LightTracker.h"
#include "PointLight.h"

PointLight::PointLight(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color, LightDistance distance) :
	PositionalLight(lightTracker, position, color, distance) { }

PointLight::PointLight(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color, float constant, float linear, float quadratic) :
	PositionalLight(lightTracker, position, color, constant, linear, quadratic) { }

void PointLight::lightSourceUpdated() const
{
	lightTracker.updateLight(this);
}
