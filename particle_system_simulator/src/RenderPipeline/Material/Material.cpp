#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"
#include "Material.h"
#include "RenderPipeline/Application.h"
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "MaterialGLSL.h"

Texture* Material::lastDiffuseMap = nullptr;
Texture* Material::lastSpecularMap = nullptr;

Material::Material(Texture* diffuseMap, Texture* specularMap, const Color4& color, float shininess) : diffuseMap(diffuseMap),
specularMap(specularMap), color(color), shininess(shininess)
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

bool Material::operator==(const Material& other) const
{
	return color == other.color &&
		diffuseMap == other.diffuseMap &&
		specularMap == other.specularMap &&
		shininess == other.shininess;
}

Material Material::defaultMaterial()
{
	return Material(nullptr, nullptr, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}, 32.0f);
}

void Material::useMaterial(const Shader& shader, bool shaderUpdated) const
{
	MaterialGLSL material
	{
		color.ambient,
		color.diffuse,
		color.specular,
		shininess,
		diffuseMap ? 1 : 0,
		specularMap ? 1 : 0,
		0.0f,
	};

	Application::getInstance().getScene().getShaderManager().updateMaterial(material);


	//TODO: maybe move the texture check to the Texture class
	if (diffuseMap && (shaderUpdated || diffuseMap != lastDiffuseMap))
	{
		shader.setInt("diffuseMap", 0);
		diffuseMap->useTexture();
	}
	if (specularMap && (shaderUpdated || specularMap != lastSpecularMap))
	{
		shader.setInt("specularMap", 1);
		specularMap->useTexture();
	}
}