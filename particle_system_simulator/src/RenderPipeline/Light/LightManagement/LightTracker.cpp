#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "GeneralUtility/ArrayUtility.h"
#include "LightTracker.h"

LightTracker::LightTracker(const GlobalShaderManager& shaderManager) : shaderManager(shaderManager), dirLights(), pointLights(), spotLights(), noOfDirLights(0),
	noOfPointLights(0), noOfSpotLights(0) { }

bool LightTracker::trackLight(const DirectionalLight* lightSource)
{
	if(noOfDirLights == MAX_DIRECTIONAL_LIGHTS)
		return false;

	dirLights[noOfDirLights++] = lightSource;
	shaderManager.addDirectionalLight(*lightSource, noOfDirLights);
}

bool LightTracker::trackLight(const PointLight* lightSource)
{
	if(noOfPointLights == MAX_POINT_LIGHTS)
		return false;

	pointLights[noOfPointLights++] = lightSource;
	shaderManager.addPointLight(*lightSource, noOfPointLights);
}

bool LightTracker::trackLight(const SpotLight* lightSource)
{
	if(noOfSpotLights == MAX_SPOT_LIGHTS)
		return false;

	spotLights[noOfSpotLights++] = lightSource;
	shaderManager.addSpotLight(*lightSource, noOfSpotLights);
}

void LightTracker::updateLight(const DirectionalLight* lightSource) const
{
	uint index = utility::array::findIndex(dirLights, lightSource, noOfDirLights);
	shaderManager.updateDirectionalLight(*lightSource, index);
}

void LightTracker::updateLight(const PointLight* lightSource) const
{
	uint index = utility::array::findIndex(pointLights, lightSource, noOfPointLights);
	shaderManager.updatePointLight(*lightSource, index);
}

void LightTracker::updateLight(const SpotLight* lightSource) const
{
	uint index = utility::array::findIndex(spotLights, lightSource, noOfSpotLights);
	shaderManager.updateSpotLight(*lightSource, index);
}

void LightTracker::untrackLight(const DirectionalLight* lightSource)
{
	uint index = utility::array::findIndex(dirLights, lightSource, noOfDirLights--);

	const DirectionalLight* replacementLight = nullptr;

	if(index != noOfDirLights)
	{
		replacementLight = dirLights[noOfDirLights];
		dirLights[index] = replacementLight;
	}

	shaderManager.removeDirectionalLight(replacementLight, index, noOfDirLights);
}

void LightTracker::untrackLight(const PointLight* lightSource)
{
	uint index = utility::array::findIndex(pointLights, lightSource, noOfPointLights--);

	const PointLight* replacementLight = nullptr;

	if(index != noOfPointLights)
	{
		replacementLight = pointLights[noOfPointLights];
		pointLights[index] = replacementLight;
	}

	shaderManager.removePointLight(replacementLight, index, noOfPointLights);
}

void LightTracker::untrackLight(const SpotLight* lightSource)
{
	uint index = utility::array::findIndex(spotLights, lightSource, noOfSpotLights--);

	const SpotLight* replacementLight = nullptr;

	if(index != noOfSpotLights)
	{
		replacementLight = spotLights[noOfSpotLights];
		spotLights[index] = replacementLight;
	}

	shaderManager.removeSpotLight(replacementLight, index, noOfSpotLights);
}
