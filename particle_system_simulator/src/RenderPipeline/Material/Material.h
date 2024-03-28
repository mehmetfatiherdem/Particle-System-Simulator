#pragma once

#include "RenderPipeline/Color/Color4.h"

class Shader;
class Texture;

class Material
{
private:
	Color4 color;
	Texture* diffuseMap;
	Texture* specularMap;
	float ambientStrength;
	float shininess;

public:
	Material() = delete;
	Material(Texture* diffuseMap, Texture* specularMap, const Color4& color, float ambientStrength, float shininess);

	void useMaterial(const Shader& shader) const;

	Color4 getColor() const { return color; }
	glm::vec4 getAmbientColor() const { return color.ambient; }
	glm::vec4 getDiffuseColor() const { return color.diffuse; }
	glm::vec4 getSpecularColor() const { return color.specular; }
	const Texture* getDiffuseMap() const { return diffuseMap; }
	const Texture* getSpecularMap() const { return specularMap; }
	float getAmbientStrength() const { return ambientStrength; }
	float getShininess() const { return shininess; }

	void setColor(const Color4& color) { this->color = color; }
	void setColor(const glm::vec4& color) { this->color.setColor(color); }
	void setColor(const glm::vec3& color) { this->color.setColor(color); }
	void setAmbientColor(const glm::vec4& ambient) { this->color.ambient = ambient; }
	void setAmbientColor(const glm::vec3& ambient) { this->color.ambient = glm::vec4{ambient, 1.0f}; }
	void setDiffuseColor(const glm::vec4& diffuse) { this->color.diffuse = diffuse; }
	void setDiffuseColor(const glm::vec3& diffuse) { this->color.diffuse = glm::vec4{diffuse, 1.0f}; }
	void setSpecularColor(const glm::vec4& specular) { this->color.specular = specular; }
	void setSpecularColor(const glm::vec3& specular) { this->color.specular = glm::vec4{specular, 1.0f}; }
	void setDiffuseMap(Texture* diffuseMap) { this->diffuseMap = diffuseMap; }
	void setSpecularMap(Texture* specularMap) { this->specularMap = specularMap; }
	void setAmbientStrength(float ambientStrength) { this->ambientStrength = ambientStrength; }
	void setShininess(float shininess) { this->shininess = shininess; }
};
