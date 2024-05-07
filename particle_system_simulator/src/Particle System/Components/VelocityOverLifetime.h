#pragma once

#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class VelocityOverLifetime : public Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method;
	glm::vec3 minVelocity;
	glm::vec3 maxVelocity;
	CubicBezierCurve<glm::vec3> minBezier;
	CubicBezierCurve<glm::vec3> maxBezier;

public:
	VelocityOverLifetime() = delete;

	VelocityOverLifetime(const glm::vec3& constantVelocity) :
		Component(0), method(ComponentMethod::Constant), minVelocity(constantVelocity), maxVelocity(constantVelocity)
	{}
	VelocityOverLifetime(const glm::vec3& minVelocity, const glm::vec3& maxVelocity) :
		Component(0), method(ComponentMethod::Random_Between_Two_Constants), minVelocity(minVelocity), maxVelocity(maxVelocity)
	{}
	VelocityOverLifetime(const CubicBezierCurve<glm::vec3>& bezier) :
		Component(0), method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier)
	{}
	VelocityOverLifetime(const CubicBezierCurve<glm::vec3>& minBezier, const CubicBezierCurve<glm::vec3>& maxBezier) :
		Component(0), method(ComponentMethod::Random_Between_Two_Curves), minBezier(minBezier), maxBezier(maxBezier)
	{}

	virtual ~VelocityOverLifetime() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

	virtual ComponentType getType() const override { return ComponentType::Velocity_Over_Lifetime; }
};