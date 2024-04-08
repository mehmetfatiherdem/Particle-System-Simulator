#include "RenderPipeline/Light/LightManagement/LightTracker.h"
#include "LightSource.h"

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