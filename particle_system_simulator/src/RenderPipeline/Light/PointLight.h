#pragma once

#include "LightBase/PositionalLight.h"

class PointLight : public PositionalLight
{
protected:
	void lightSourceUpdated() const override final;
public:
	PointLight() = delete;
	PointLight(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color, LightDistance distance = LightDistance::AD_100);
	PointLight(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color, float constant, float linear, float quadratic);

	void rotate(const glm::vec3& rotation) override { this->transform.rotate(rotation); }
	void rotate(const glm::quat& rotation) override final { this->transform.rotate(rotation); }
	void rotateAround(const glm::vec3& axis, float angle) override final { this->transform.rotateAround(axis, angle); }
	void lookAt(const glm::vec3& position) override final { this->transform.lookAt(position); }
	void lookAt(const Transform& transform) override final { this->transform.lookAt(transform); }
	void setRightVector(const glm::vec3& right) override final { this->transform.setRightVector(right); }
	void setUpVector(const glm::vec3& up) override final { this->transform.setUpVector(up); }
	void setForwardvector(const glm::vec3& forward) override final { this->transform.setForwardVector(forward); }
	void setRotation(const glm::quat& rotation) override final { this->transform.setRotation(rotation); }
	void setEulerRotation(const glm::vec3& rotation) override final { this->transform.setEulerRotation(rotation); }
};
