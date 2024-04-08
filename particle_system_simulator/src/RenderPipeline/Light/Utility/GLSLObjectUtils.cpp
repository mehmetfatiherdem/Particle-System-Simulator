#include "RenderPipeline/Light/DirectionalLight.h"
#include "RenderPipeline/Light/PointLight.h"
#include "RenderPipeline/Light/SpotLight.h"
#include "RenderPipeline/Light/Data/LightConstants.h"
#include "GLSLObjectUtils.h"

uint32_t sizeOfLightsUBO()
{
	return (MAX_DIRECTIONAL_LIGHTS * sizeof(DirectionalLightGLSL)) +
		(MAX_POINT_LIGHTS * sizeof(PointLightGLSL)) +
		(MAX_SPOT_LIGHTS * sizeof(SpotLightGLSL)) + 12;
}

uint32_t calculateNumberDataOffset(LightSourceType type)
{
	return sizeOfLightsUBO() - (static_cast<uint32_t>(type) << 2);
}

uint32_t calculateLightDataOffset(LightSourceType type, uint32_t index)
{
	switch(type)
	{
	case LightSourceType::DirectionalLight:
		return (MAX_SPOT_LIGHTS * sizeof(SpotLightGLSL)) +
			(MAX_POINT_LIGHTS * sizeof(PointLightGLSL)) +
			(index * sizeof(DirectionalLightGLSL));

	case LightSourceType::PointLight:
		return (MAX_SPOT_LIGHTS * sizeof(SpotLightGLSL)) +
			(index * sizeof(PointLightGLSL));

	case LightSourceType::SpotLight:
		return (index * sizeof(SpotLightGLSL));

	default: return 0;
	}
}

DirectionalLightGLSL getGLSLRepresentation(const DirectionalLight& light)
{
	Color3 color = light.getColor();

	return DirectionalLightGLSL {
		color.ambient,
		0.0f,
		color.diffuse,
		0.0f,
		color.specular,
		0.0f,
		light.getDirection(),
		0.0f
	};
}

PointLightGLSL getGLSLRepresentation(const PointLight& light)
{
	Color3 color = light.getColor();
	glm::vec3 attenuation = light.getAttenuation();

	return PointLightGLSL {
		color.ambient,
		0.0f,
		color.diffuse,
		0.0f,
		color.specular,
		0.0f,
		light.getPosition(),
		0.0f,
		attenuation.x,
		attenuation.y,
		attenuation.z,
		0.0f
	};
}

SpotLightGLSL getGLSLRepresentation(const SpotLight& light)
{
	Color3 color = light.getColor();
	glm::vec3 attenuation = light.getAttenuation();

	return SpotLightGLSL {
		color.ambient,
		0.0f,
		color.diffuse,
		0.0f,
		color.specular,
		0.0f,
		light.getPosition(),
		0.0f,
		light.getDirection(),
		0.0f,
		attenuation.x,
		attenuation.y,
		attenuation.z,
		0.0f,
		light.getInnerCutOff(),
		light.getOuterCutOff(),
		light.getEpsilon(),
		0.0f
	};
}