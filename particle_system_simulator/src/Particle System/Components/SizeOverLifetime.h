#pragma once

#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class SizeOverLifetime : public Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method;
	float minSize;
	float maxSize;
	CubicBezierCurve<float> minBezier;
	CubicBezierCurve<float> maxBezier;

public:
	SizeOverLifetime() = delete;
	SizeOverLifetime(float minConstant, float maxConstant) :
		Component(1), method(ComponentMethod::Random_Between_Two_Constants), minSize(minConstant), maxSize(maxConstant) {}

	SizeOverLifetime(const CubicBezierCurve<float>& bezier) :
		Component(1), method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier) { }

	SizeOverLifetime(const CubicBezierCurve<float>& minBezier, const CubicBezierCurve<float>& maxBezier) :
		Component(1), method(ComponentMethod::Random_Between_Two_Curves), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~SizeOverLifetime() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

	virtual ComponentType getType() const override { return ComponentType::Size_Over_Lifetime; }
};