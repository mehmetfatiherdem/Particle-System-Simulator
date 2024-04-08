#pragma once

#include "LightBase/PositionalLight.h"

class PointLight : public PositionalLight
{
protected:
	void lightSourceUpdated() const override final;
public:
	PointLight() = delete;
	PointLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& position,
		LightDistance distance = LightDistance::AD_100) : PositionalLight(lightTracker, color, position, distance) { }

	PointLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& position, float constant,
		float linear, float quadratic) : PositionalLight(lightTracker, color, position, constant, linear, quadratic) { }

	PointLight(const PointLight&) = default;
	PointLight(PointLight&&) = delete;
	~PointLight() = default;

	PointLight& operator=(const PointLight&) = delete;
	PointLight& operator=(PointLight&&) = delete;
};
