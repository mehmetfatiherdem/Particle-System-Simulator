#pragma once

#include "LightBase/PositionalLight.h"

class SpotLight : public PositionalLight
{
private:
	float innerCutOffAngle;
	float outerCutOffAngle;
protected:
	void lightSourceUpdated() const override final;
public:
	SpotLight() = delete;
	SpotLight(const LightTracker& lightTracker, const glm::vec3& position, const glm::vec3& direction, const Color3& color, float innerCutOffAngle, float outerCutOffAngle,
		LightDistance distance = LightDistance::AD_100);
	SpotLight(const LightTracker& lightTracker, const glm::vec3& position, const glm::vec3& direction, const Color3& color, float innerCutOffAngle, float outerCutOffAngle,
		float constant, float linear, float quadratic);

	float getInnerCutOffAngle() const { return innerCutOffAngle; }
	float getOuterCutOffAngle() const { return outerCutOffAngle; }
	float getInnerCutOff() const { return glm::cos(innerCutOffAngle); }
	float getOuterCutOff() const { return glm::cos(outerCutOffAngle); }
	float getEpsilon() const { return getInnerCutOff() - getOuterCutOff(); }
	glm::vec3 getDirection() const { return transform.getForwardVector(); }

	void setInnerCutOffAngle(float angle);
	void setOuterCutOffAngle(float angle);
	void setCutOffAngles(float inner, float outer);
	void setDirection(const glm::vec3& direction);
};

