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

	static constexpr glm::vec2 getAttenuation(LightDistance distance) { return attenuationTable[static_cast<uint32_t>(distance)]; }

	float constant;
	float linear;
	float quadratic;

	PositionalLight() = delete;
	PositionalLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& position,
		LightDistance distance = LightDistance::AD_100) : LightSource(lightTracker, TransformProps{position}, color),
		constant(1.0f), linear(getAttenuation(distance).x), quadratic(getAttenuation(distance).y) { }

	PositionalLight(const LightTracker& lightTracker, const Color3& color, const glm::vec3& position, float constant,
		float linear, float quadratic) : LightSource(lightTracker, TransformProps{position}, color),
		constant(constant), linear(linear), quadratic(quadratic) { }

	PositionalLight(const PositionalLight&) = default;
	PositionalLight(PositionalLight&&) = delete;
	~PositionalLight() = default;

	PositionalLight& operator=(const PositionalLight&) = delete;
	PositionalLight& operator=(PositionalLight&&) = delete;

public:

	glm::vec3 getAttenuation() const { return glm::vec3{constant, linear, quadratic}; }
	float getConstantAttenuation() const { return constant; }
	float getLinearAttenuation() const { return linear; }
	float getQuadraticAttenuation() const { return quadratic; }
	glm::vec3 getPosition() const { return transform.getPosition(); }

	void setAttenuation(LightDistance distance);
	void setAttenuation(float constant, float linear, float quadratic);
	void setAttenuation(const glm::vec3& attenuation);
	void setConstantAttenuation(float constant);
	void setLinearAttenuation(float linear);
	void setQuadraticAttenuation(float quadratic);
	void setPosition(const glm::vec3& position);

	void rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle);
	void rotateAround(const Transform& transform, const glm::vec3& axis, float angle);
};