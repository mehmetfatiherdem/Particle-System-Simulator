#include "PositionalLight.h"

PositionalLight::PositionalLight(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color, LightDistance distance) :
	LightSource(lightTracker, position, color), constantAttenuation(1.0f), linearAttenuation(getAttenuation(distance).x), quadraticAttenuation(getAttenuation(distance).y) { }

PositionalLight::PositionalLight(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color, float constant, float linear, float quadratic) :
	LightSource(lightTracker, position, color), constantAttenuation(constant), linearAttenuation(linear), quadraticAttenuation(quadratic) { }

void PositionalLight::setAttenuation(LightDistance distance)
{
	constantAttenuation = 1.0f;
	auto attenuation = getAttenuation(distance);
	linearAttenuation = attenuation.x;
	quadraticAttenuation = attenuation.y;
	lightSourceUpdated();
}

void PositionalLight::setAttenuation(float constant, float linear, float quadratic)
{
	constantAttenuation = constant;
	linearAttenuation = linear;
	quadraticAttenuation = quadratic;
	lightSourceUpdated();
}

void PositionalLight::setAttenuation(const glm::vec3& attenuation)
{
	constantAttenuation = attenuation.x;
	linearAttenuation = attenuation.y;
	quadraticAttenuation = attenuation.z;
	lightSourceUpdated();
}

void PositionalLight::setConstantAttenuation(float constantAttenuation)
{
	this->constantAttenuation = constantAttenuation;
	lightSourceUpdated();
}

void PositionalLight::setLinearAttenuation(float linearAttenuation)
{
	this->linearAttenuation = linearAttenuation;
	lightSourceUpdated();
}

void PositionalLight::setQuadraticAttenuation(float quadraticAttenuation)
{
	this->quadraticAttenuation = quadraticAttenuation;
	lightSourceUpdated();
}

void PositionalLight::setPosition(const glm::vec3& position)
{
	this->transform.setPosition(position);
}