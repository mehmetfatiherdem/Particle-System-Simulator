#pragma once

#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/BezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class SizeOverLifetime : public Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method = ComponentMethod::Random_Between_Two_Constants;
	float minSize = 0.0f;
	float maxSize = 0.0f;
	BezierCurve<float> minBezier = Bezier::createLinear();
	BezierCurve<float> maxBezier = Bezier::createLinear();

protected:
	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

public:
	SizeOverLifetime() : Component(1) { }

	SizeOverLifetime(float minConstant, float maxConstant) :
		Component(1), method(ComponentMethod::Random_Between_Two_Constants), minSize(minConstant), maxSize(maxConstant) {}

	SizeOverLifetime(const BezierCurve<float>& bezier) :
		Component(1), method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier) { }

	SizeOverLifetime(const BezierCurve<float>& minBezier, const BezierCurve<float>& maxBezier) :
		Component(1), method(ComponentMethod::Random_Between_Two_Curves), minBezier(minBezier), maxBezier(maxBezier) { }

	SizeOverLifetime(ComponentMethod method, float minConstant, float maxConstant, const BezierCurve<float>& minBezier,
		const BezierCurve<float>& maxBezier) : Component(1), method(method), minSize(minConstant),
		maxSize(maxConstant), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~SizeOverLifetime() override = default;

	virtual ComponentType getType() const override { return ComponentType::Size_Over_Lifetime; }

	virtual void serialize(Serializer& serializer, const std::string& objectName = "") const override;
};