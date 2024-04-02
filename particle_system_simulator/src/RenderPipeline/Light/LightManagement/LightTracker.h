#pragma once

#include "RenderPipeline/Light/Data/LightConstants.h"
#include "GeneralUtility/Aliases.h"

class LightSource;
class DirectionalLight;
class PointLight;
class SpotLight;
class GlobalShaderManager;

class LightTracker
{
private:
	friend class Scene;

	LightTracker(const GlobalShaderManager& shaderManager);

	const GlobalShaderManager& shaderManager;
	
	const DirectionalLight* dirLights[MAX_DIRECTIONAL_LIGHTS];
	const PointLight* pointLights[MAX_POINT_LIGHTS];
	const SpotLight* spotLights[MAX_SPOT_LIGHTS];

	uint noOfDirLights;
	uint noOfPointLights;
	uint noOfSpotLights;

	bool trackLight(const DirectionalLight* lightSource);
	bool trackLight(const PointLight* lightSource);
	bool trackLight(const SpotLight* lightSource);

	void untrackLight(const DirectionalLight* lightSource);
	void untrackLight(const PointLight* lightSource);
	void untrackLight(const SpotLight* lightSource);

	bool canAddDirectionalLight() const { return noOfDirLights != MAX_DIRECTIONAL_LIGHTS; }
	bool canAddPointLight() const { return noOfPointLights != MAX_POINT_LIGHTS; }
	bool canAddSpotLight() const { return noOfSpotLights != MAX_SPOT_LIGHTS; }

public:
	LightTracker() = delete;
	LightTracker(const LightTracker& lightTracker) = delete;
	LightTracker(LightTracker&& lightTracker) = delete;

	LightTracker& operator=(const LightTracker& lightTracker) = delete;
	LightTracker& operator=(LightTracker&& lightTracker) = delete;

	void updateLight(const DirectionalLight* lightSource) const;
	void updateLight(const PointLight* lightSource) const;
	void updateLight(const SpotLight* lightSource) const;
};
