#pragma once

#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class SizeBySpeed : Component
{
private:
	ComponentMethod method;
	float minSpeed;
	float maxSpeed;
	CubicBezierCurve<float> minBezier;
	CubicBezierCurve<float> maxBezier;

public:
	SizeBySpeed() = delete;

	SizeBySpeed(float minSpeed, float maxSpeed, const CubicBezierCurve<float>& bezier) :
		Component(1), method(ComponentMethod::Curve), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(bezier), maxBezier(bezier) { }
	
	SizeBySpeed(float minSpeed, float maxSpeed, const CubicBezierCurve<float>& minBezier, const CubicBezierCurve<float>& maxBezier) :
		Component(1), method(ComponentMethod::RandomBetweenTwoCurves), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~SizeBySpeed() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;
};