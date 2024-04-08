#pragma once

#include "LightBase/LightSource.h"

class DirectionalLight : public LightSource
{
protected:
	void lightSourceUpdated() const override final;
public:
	DirectionalLight() = delete;
	DirectionalLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& direction);
	DirectionalLight(const DirectionalLight&) = default;
	DirectionalLight(DirectionalLight&&) = delete;
	~DirectionalLight() = default;

	DirectionalLight& operator=(const DirectionalLight&) = delete;
	DirectionalLight& operator=(DirectionalLight&&) = delete;

	glm::vec3 getDirection() const { return this->transform.getForwardVector(); }
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
