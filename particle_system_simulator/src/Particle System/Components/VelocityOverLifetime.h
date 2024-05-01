#pragma once

#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class VelocityOverLifetime : public Component
{
private:
	ComponentMethod method;
	glm::vec3 minVelocity;
	glm::vec3 maxVelocity;
	CubicBezierCurve<glm::vec3> minBezier;
	CubicBezierCurve<glm::vec3> maxBezier;

public:
	VelocityOverLifetime() = delete;

	VelocityOverLifetime(const glm::vec3& constantVelocity) :
		method(ComponentMethod::Constant), minVelocity(constantVelocity), maxVelocity(constantVelocity)
	{}
	VelocityOverLifetime(const glm::vec3& minVelocity, const glm::vec3& maxVelocity) :
		method(ComponentMethod::RandomBetweenTwoConstants), minVelocity(minVelocity), maxVelocity(maxVelocity)
	{}
	VelocityOverLifetime(const CubicBezierCurve<glm::vec3>& bezier) :
		method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier)
	{}
	VelocityOverLifetime(const CubicBezierCurve<glm::vec3>& minBezier, const CubicBezierCurve<glm::vec3>& maxBezier) :
		method(ComponentMethod::RandomBetweenTwoCurves), minBezier(minBezier), maxBezier(maxBezier)
	{}

	virtual ~VelocityOverLifetime() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;
};