#pragma once

#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class SizeBySpeed : Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method;
	float minSpeed;
	float maxSpeed;
	CubicBezierCurve<float> minBezier;
	CubicBezierCurve<float> maxBezier;

public:
	SizeBySpeed() = delete;

	SizeBySpeed(float minSpeed, float maxSpeed, const CubicBezierCurve<float>& bezier) :
		Component(4), method(ComponentMethod::Curve), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(bezier), maxBezier(bezier) { }
	
	SizeBySpeed(float minSpeed, float maxSpeed, const CubicBezierCurve<float>& minBezier, const CubicBezierCurve<float>& maxBezier) :
		Component(4), method(ComponentMethod::Random_Between_Two_Curves), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~SizeBySpeed() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

	virtual ComponentType getType() const override { return ComponentType::Size_By_Speed; }
};