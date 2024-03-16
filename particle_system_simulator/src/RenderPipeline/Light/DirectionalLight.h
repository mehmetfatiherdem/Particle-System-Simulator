#pragma once

#include "LightBase/LightSource.h"

class DirectionalLight : public LightSource
{
protected:
	void lightSourceUpdated() const override final;
public:
	DirectionalLight() = delete;
	DirectionalLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& direction);

	glm::vec3 getDirection() const { return transform.getForwardVector(); }
	void setDirection(const glm::vec3& direction);

	void translate(const glm::vec3& displacement) override final { this->transform.translate(displacement); }
	void setPosition(const glm::vec3& position) override final { this->transform.setPosition(position); }
};
