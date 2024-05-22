#pragma once

#include "RenderPipeline/Color/Color4.h"

class Shader;
class Texture;
class ParticleSystemEditor;

class Material
{
private:
	friend class ParticleSystemEditor;

	Color4 color;
	Texture* diffuseMap;
	Texture* specularMap;
	float shininess;

	static Texture* lastDiffuseMap;
	static Texture* lastSpecularMap;

public:
	Material() = delete;
	Material(Texture* diffuseMap, Texture* specularMap, const Color4& color, float shininess);

	bool operator==(const Material& other) const;

	static Material defaultMaterial();

	void useMaterial(const Shader& shader) const;

	Color4 getColor() const { return color; }
	glm::vec3 getAmbientColor() const { return color.ambient; }
	glm::vec3 getDiffuseColor() const { return color.diffuse; }
	glm::vec3 getSpecularColor() const { return color.specular; }
	float getAlpha() const { return color.alpha; }
	const Texture* getDiffuseMap() const { return diffuseMap; }
	const Texture* getSpecularMap() const { return specularMap; }
	float getShininess() const { return shininess; }

	void setColor(const Color4& color) { this->color = color; }
	void setColor(const glm::vec4& color) { this->color.setColor(color); }
	void setColor(const glm::vec3& color) { this->color.setColor(color); }
	void setAmbientColor(const glm::vec3& ambient) { this->color.ambient = ambient; }
	void setDiffuseColor(const glm::vec3& diffuse) { this->color.diffuse = diffuse; }
	void setSpecularColor(const glm::vec3& specular) { this->color.specular = specular; }
	void setAlpha(float alpha) { this->color.alpha = alpha; }
	void setDiffuseMap(Texture* diffuseMap) { this->diffuseMap = diffuseMap; }
	void setSpecularMap(Texture* specularMap) { this->specularMap = specularMap; }
	void setShininess(float shininess) { this->shininess = shininess; }
};
