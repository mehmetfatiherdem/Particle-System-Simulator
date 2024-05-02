#pragma once

#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class ForceOverLifetime : public Component
{
private:
	ComponentMethod method;
	glm::vec3 minForce;
	glm::vec3 maxForce;
	CubicBezierCurve<glm::vec3> minBezier;
	CubicBezierCurve<glm::vec3> maxBezier;

public:
	ForceOverLifetime() = delete;

	ForceOverLifetime(const glm::vec3& constantForce) : 
		Component(2), method(ComponentMethod::Constant), minForce(constantForce), maxForce(constantForce) { }
	ForceOverLifetime(const glm::vec3& minForce, const glm::vec3& maxForce) :
		Component(2), method(ComponentMethod::RandomBetweenTwoConstants), minForce(minForce), maxForce(maxForce) { }
	ForceOverLifetime(const CubicBezierCurve<glm::vec3>& bezier) :
		Component(2), method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier) { }
	ForceOverLifetime(const CubicBezierCurve<glm::vec3>& minBezier, const CubicBezierCurve<glm::vec3>& maxBezier) :
		Component(2), method(ComponentMethod::RandomBetweenTwoCurves), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~ForceOverLifetime() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;
};