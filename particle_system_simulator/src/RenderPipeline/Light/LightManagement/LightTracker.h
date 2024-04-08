#pragma once

#include "RenderPipeline/Light/Data/LightConstants.h"


class LightSource;
class DirectionalLight;
class PointLight;
class SpotLight;
class GlobalShaderManager;

class LightTracker
{
private:
	friend class Scene;

	const GlobalShaderManager& shaderManager;
	
	const DirectionalLight* dirLights[MAX_DIRECTIONAL_LIGHTS];
	const PointLight* pointLights[MAX_POINT_LIGHTS];
	const SpotLight* spotLights[MAX_SPOT_LIGHTS];

	uint32_t noOfDirLights;
	uint32_t noOfPointLights;
	uint32_t noOfSpotLights;

	bool trackLight(const DirectionalLight* lightSource);
	bool trackLight(const PointLight* lightSource);
	bool trackLight(const SpotLight* lightSource);

	void untrackLight(const DirectionalLight* lightSource);
	void untrackLight(const PointLight* lightSource);
	void untrackLight(const SpotLight* lightSource);

	bool canAddDirectionalLight() const { return noOfDirLights != MAX_DIRECTIONAL_LIGHTS; }
	bool canAddPointLight() const { return noOfPointLights != MAX_POINT_LIGHTS; }
	bool canAddSpotLight() const { return noOfSpotLights != MAX_SPOT_LIGHTS; }

	LightTracker(const GlobalShaderManager& shaderManager) : shaderManager(shaderManager),
		dirLights(), pointLights(), spotLights(),
		noOfDirLights(0), noOfPointLights(0), noOfSpotLights(0) { }
public:
	LightTracker() = delete;
	LightTracker(const LightTracker& lightTracker) = delete;
	LightTracker(LightTracker&& lightTracker) = delete;
	~LightTracker() = default;

	LightTracker& operator=(const LightTracker& lightTracker) = delete;
	LightTracker& operator=(LightTracker&& lightTracker) = delete;

	void updateLight(const DirectionalLight* lightSource) const;
	void updateLight(const PointLight* lightSource) const;
	void updateLight(const SpotLight* lightSource) const;
};
