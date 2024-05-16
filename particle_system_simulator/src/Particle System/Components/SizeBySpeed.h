#pragma once

#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/BezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class SizeBySpeed : public Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method = ComponentMethod::Curve;
	float minSpeed = 0.0f;
	float maxSpeed = 1.0f;
	BezierCurve<float> minBezier = Bezier::createLinear();
	BezierCurve<float> maxBezier = Bezier::createLinear();

protected:
	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

public:
	SizeBySpeed() : Component(4) { }

	SizeBySpeed(float minSpeed, float maxSpeed, const BezierCurve<float>& bezier) :
		Component(4), method(ComponentMethod::Curve), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(bezier), maxBezier(bezier) { }
	
	SizeBySpeed(float minSpeed, float maxSpeed, const BezierCurve<float>& minBezier, const BezierCurve<float>& maxBezier) :
		Component(4), method(ComponentMethod::Random_Between_Two_Curves), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(minBezier), maxBezier(maxBezier) { }

	SizeBySpeed(ComponentMethod method, float minSpeed, float maxSpeed, const BezierCurve<float>& minBezier, const BezierCurve<float>& maxBezier) : Component(4), method(method), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~SizeBySpeed() override = default;

	virtual ComponentType getType() const override { return ComponentType::Size_By_Speed; }

	virtual void serialize(Serializer& serializer, const std::string& objectName = "") const override;
};