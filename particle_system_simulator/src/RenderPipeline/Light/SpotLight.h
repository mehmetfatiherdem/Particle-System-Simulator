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
	SpotLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& position, const glm::vec3& direction,
		float innerCutOffAngle, float outerCutOffAngle, LightDistance distance = LightDistance::AD_100);
	SpotLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& position, const glm::vec3& direction,
		float innerCutOffAngle, float outerCutOffAngle, float constant, float linear, float quadratic);
	SpotLight(const SpotLight&) = default;
	SpotLight(SpotLight&&) = delete;
	~SpotLight() = default;

	SpotLight& operator=(const SpotLight&) = delete;
	SpotLight& operator=(SpotLight&&) = delete;

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

	void rotate(const glm::vec3& rotation);
	void rotate(const glm::quat& rotation);
	void rotateAround(const glm::vec3& axis, float angle);
	void rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle);
	void rotateAround(const Transform& transform, const glm::vec3& axis, float angle);

	void lookAt(const Transform& transform);
	void lookAt(const glm::vec3& point);

	void setRightVector(const glm::vec3& right);
	void setUpVector(const glm::vec3& up);
	void setForwardVector(const glm::vec3& forward);

	void setRotation(const glm::quat& rotation);
	void setEulerRotation(const glm::vec3& rotation);
};

