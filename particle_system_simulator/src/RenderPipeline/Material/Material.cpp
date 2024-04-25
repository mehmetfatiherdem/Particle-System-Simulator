#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"
#include "Material.h"

Material::Material(Texture* diffuseMap, Texture* specularMap, const Color4& color, float ambientStrength, float shininess) : diffuseMap(diffuseMap),
specularMap(specularMap), color(color), ambientStrength(ambientStrength), shininess(shininess)
{
	if (diffuseMap)
	{
		diffuseMap->setTextureUnit(0);
	}
	if (specularMap)
	{
		specularMap->setTextureUnit(1);
	}
}

Material& Material::defaultMaterial()
{
	static Material defaultMaterial(nullptr, nullptr, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}, 1.0f, 32.0f);
	return defaultMaterial;
}

void Material::useMaterial(const Shader& shader) const
{
	shader.setVector("material.color.ambient", color.ambient);
	shader.setVector("material.color.diffuse", color.diffuse);
	shader.setVector("material.color.specular", color.specular);
	shader.setFloat("material.ambientStrength", ambientStrength);
	shader.setFloat("material.shininess", shininess);

	shader.setBool("material.useDiffuseMap", diffuseMap);
	shader.setBool("material.useSpecularMap", specularMap);

	if (diffuseMap)
	{
		diffuseMap->useTexture();
		shader.setInt("material.diffuseMap", 0);
	}
	if (specularMap)
	{
		specularMap->useTexture();
		shader.setInt("material.specularMap", 1);
	}
}