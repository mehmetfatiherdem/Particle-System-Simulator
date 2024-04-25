#pragma once

#include <stdint.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "LightSource.h"
#include "RenderPipeline/Light/Data/LightDistance.h"

class PositionalLight : public LightSource
{
protected:
	static constexpr glm::vec2 attenuationTable[12] = {
		glm::vec2{0.7f, 1.8f},
		glm::vec2{0.35f, 0.44f},
		glm::vec2{0.22f, 0.20f},
		glm::vec2{0.14f, 0.07f},
		glm::vec2{0.09f, 0.032f},
		glm::vec2{0.07f, 0.017f},
		glm::vec2{0.045f, 0.0075f},
		glm::vec2{0.027f, 0.0028f},
		glm::vec2{0.022f, 0.0019f},
		glm::vec2{0.014f, 0.0007f},
		glm::vec2{0.007f, 0.0002f},
		glm::vec2{0.0014f, 0.000007f},
	};

	constexpr glm::vec2 getAttenuation(LightDistance distance) { return attenuationTable[static_cast<uint32_t>(distance)]; }

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;

	PositionalLight() = delete;
	PositionalLight(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color, LightDistance distance = LightDistance::AD_100);
	PositionalLight(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color, float constant, float linear, float quadratic);

public:

	glm::vec3 getAttenuation() const { return glm::vec3{constantAttenuation, linearAttenuation, quadraticAttenuation}; }
	float getConstantAttenuation() const { return constantAttenuation; }
	float getLinearAttenuation() const { return linearAttenuation; }
	float getQuadraticAttenuation() const { return quadraticAttenuation; }
	glm::vec3 getPosition() const { return transform.getPosition(); }

	void setAttenuation(LightDistance distance);
	void setAttenuation(float constant, float linear, float quadratic);
	void setAttenuation(const glm::vec3& attenuation);
	void setConstantAttenuation(float constantAttenuation);
	void setLinearAttenuation(float linearAttenuation);
	void setQuadraticAttenuation(float quadraticAttenuation);
	void setPosition(const glm::vec3& position);
};