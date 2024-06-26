#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"
#include "Material.h"
#include "RenderPipeline/Application.h"
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "MaterialGLSL.h"

Texture* Material::lastDiffuseMap = nullptr;
Texture* Material::lastSpecularMap = nullptr;

Material::Material(Texture* diffuseMap, Texture* specularMap, const Color4& color, float shininess) : diffuseMap(diffuseMap),
	specularMap(specularMap), color(color), shininess(shininess) { }

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

void Material::useMaterial(const Shader& shader) const
{
	MaterialGLSL material
	{
		color.ambient,
		0.0f,
		color.diffuse,
		0.0f,
		color.specular,
		color.alpha,
		shininess,
		diffuseMap ? true : false,
		specularMap ? true : false,
		0.0f,
	};

	Application::getInstance().getScene().getShaderManager().updateMaterial(material);

	if (diffuseMap && (diffuseMap != lastDiffuseMap))
	{
		diffuseMap->useTexture(0);
	}
	if (specularMap && (specularMap != lastSpecularMap))
	{
		specularMap->useTexture(1);
	}
}