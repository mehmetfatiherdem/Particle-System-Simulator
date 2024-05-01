#pragma once

#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class SizeOverLifetime : public Component
{
private:
	ComponentMethod method;
	float minSize;
	float maxSize;
	CubicBezierCurve<float> minBezier;
	CubicBezierCurve<float> maxBezier;

public:
	SizeOverLifetime() = delete;
	SizeOverLifetime(float minConstant, float maxConstant) :
		Component(0), method(ComponentMethod::RandomBetweenTwoConstants), minSize(minConstant), maxSize(maxConstant) {}
	SizeOverLifetime(const CubicBezierCurve<float>& bezier) :
		Component(0), method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier) { }
	SizeOverLifetime(const CubicBezierCurve<float>& minBezier, const CubicBezierCurve<float>& maxBezier) :
		Component(0), method(ComponentMethod::RandomBetweenTwoCurves), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~SizeOverLifetime() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;
};