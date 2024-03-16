#include "RenderPipeline/Light/LightManagement/LightTracker.h"
#include "LightSource.h"

LightSource::LightSource(const LightTracker& lightTracker, const Color3& color) : lightTracker(lightTracker), transform(), color(color) { }

LightSource::LightSource(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color) : lightTracker(lightTracker), transform(position), color(color) { }

void LightSource::translate(const glm::vec3& displacement)
{
	this->transform.translate(displacement);
	lightSourceUpdated();
}

void LightSource::rotate(const glm::vec3& rotation)
{
	this->transform.rotate(rotation);
	lightSourceUpdated();
}

void LightSource::rotate(const glm::quat& rotation)
{
	this->transform.rotate(rotation);
	lightSourceUpdated();
}

void LightSource::rotateAround(const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(axis, angle);
	lightSourceUpdated();
}

void LightSource::rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(point, axis, angle);
	lightSourceUpdated();
}

void LightSource::rotateAround(const Transform& transform, const glm::vec3& axis, float angle)
{
	this->transform.rotateAround(transform, axis, angle);
	lightSourceUpdated();
}

void LightSource::lookAt(const glm::vec3& position)
{
	this->transform.lookAt(position);
	lightSourceUpdated();
}

void LightSource::lookAt(const Transform& transform)
{
	this->transform.lookAt(transform);
	lightSourceUpdated();
}

void LightSource::setRightVector(const glm::vec3& right)
{
	this->transform.setRightVector(right);
	lightSourceUpdated();
}

void LightSource::setUpVector(const glm::vec3& up)
{
	this->transform.setUpVector(up);
	lightSourceUpdated();
}

void LightSource::setForwardvector(const glm::vec3& forward)
{
	this->transform.setForwardVector(forward);
	lightSourceUpdated();
}

void LightSource::setPosition(const glm::vec3& position)
{
	this->transform.setPosition(position);
	lightSourceUpdated();
}

void LightSource::setRotation(const glm::quat& rotation)
{
	this->transform.setRotation(rotation);
	lightSourceUpdated();
}

void LightSource::setEulerRotation(const glm::vec3& rotation)
{
	this->transform.setEulerRotation(rotation);
	lightSourceUpdated();
}

void LightSource::setColor(const Color3& color)
{
	this->color = color;
	lightSourceUpdated();
}

void LightSource::setColor(const glm::vec3& color)
{
	this->color.setColor(color);
	lightSourceUpdated();
}

void LightSource::setAmbientColor(const glm::vec3& ambient)
{
	this->color.ambient = ambient;
	lightSourceUpdated();
}

void LightSource::setDiffuseColor(const glm::vec3& diffuse)
{
	this->color.diffuse = diffuse;
	lightSourceUpdated();
}

void LightSource::setSpecularColor(const glm::vec3& specular)
{
	this->color.specular = specular;
	lightSourceUpdated();
}