#include "PositionalLight.h"

void PositionalLight::setAttenuation(LightDistance distance)
{
	constant = 1.0f;
	auto attenuation = getAttenuation(distance);
	linear = attenuation.x;
	quadratic = attenuation.y;
	lightSourceUpdated();
}

void PositionalLight::setAttenuation(float constant, float linear, float quadratic)
{
	constant = constant;
	linear = linear;
	quadratic = quadratic;
	lightSourceUpdated();
}

void PositionalLight::setAttenuation(const glm::vec3& attenuation)
{
	constant = attenuation.x;
	linear = attenuation.y;
	quadratic = attenuation.z;
	lightSourceUpdated();
}

void PositionalLight::setConstantAttenuation(float constant)
{
	this->constant = constant;
	lightSourceUpdated();
}

void PositionalLight::setLinearAttenuation(float linear)
{
	this->linear = linear;
	lightSourceUpdated();
}

void PositionalLight::setQuadraticAttenuation(float quadratic)
{
	this->quadratic = quadratic;
	lightSourceUpdated();
}

void PositionalLight::rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(point, axis, angle);
	lightSourceUpdated();
}

void PositionalLight::rotateAround(const Transform& transform, const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(transform, axis, angle);
	lightSourceUpdated();
}

void PositionalLight::setPosition(const glm::vec3& position)
{
	this->transform.setPosition(position);
	lightSourceUpdated();
}