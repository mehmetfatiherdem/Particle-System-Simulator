#include "RenderPipeline/Light/DirectionalLight.h"
#include "RenderPipeline/Light/PointLight.h"
#include "RenderPipeline/Light/SpotLight.h"
#include "RenderPipeline/Constants/PipelineConstants.h"
#include "GLSLObjectUtils.h"

using namespace constants::light;

unsigned int sizeOfLightsUBO()
{
	return (MAX_DIRECTIONAL_LIGHTS * sizeof(DirectionalLightGLSL)) +
		(MAX_POINT_LIGHTS * sizeof(PointLightGLSL)) +
		(MAX_SPOT_LIGHTS * sizeof(SpotLightGLSL)) + 12;
}

unsigned int calculateNumberDataOffset(LightSourceType type)
{
	return sizeOfLightsUBO() - (static_cast<unsigned int>(type) << 2);
}

unsigned int calculateLightDataOffset(LightSourceType type, unsigned int index)
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

DirectionalLightToBytes getGLSLRepresentation(const DirectionalLight& light)
{
	Color3 color = light.getColor();

	return DirectionalLightToBytes {
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

PointLightToBytes getGLSLRepresentation(const PointLight& light)
{
	Color3 color = light.getColor();
	glm::vec3 attenuation = light.getAttenuation();

	return PointLightToBytes {
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

SpotLightToBytes getGLSLRepresentation(const SpotLight& light)
{
	Color3 color = light.getColor();
	glm::vec3 attenuation = light.getAttenuation();

	return SpotLightToBytes {
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