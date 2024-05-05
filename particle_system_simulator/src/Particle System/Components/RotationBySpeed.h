#pragma once

#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class RotationBySpeed : Component
{
private:
	ComponentMethod method;
	float minSpeed;
	float maxSpeed;
	CubicBezierCurve<glm::vec3> minBezier;
	CubicBezierCurve<glm::vec3> maxBezier;

public:
	RotationBySpeed() = delete;

	RotationBySpeed(float minSpeed, float maxSpeed, const CubicBezierCurve<glm::vec3>& bezier) :
		Component(5), method(ComponentMethod::Curve), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(bezier), maxBezier(bezier) { }

	RotationBySpeed(float minSpeed, float maxSpeed, const CubicBezierCurve<glm::vec3>& minBezier, const CubicBezierCurve<glm::vec3>& maxBezier) :
		Component(5), method(ComponentMethod::RandomBetweenTwoCurves), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~RotationBySpeed() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;
};