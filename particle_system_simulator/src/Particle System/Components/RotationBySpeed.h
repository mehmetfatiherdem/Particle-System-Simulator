#pragma once

#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/BezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class RotationBySpeed : public Component
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
	RotationBySpeed() : Component(5) { }

	RotationBySpeed(float minSpeed, float maxSpeed, const BezierCurve<float>& bezier) :
		Component(5), method(ComponentMethod::Curve), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(bezier), maxBezier(bezier) { }

	RotationBySpeed(float minSpeed, float maxSpeed, const BezierCurve<float>& minBezier, const BezierCurve<float>& maxBezier) :
		Component(5), method(ComponentMethod::Random_Between_Two_Curves), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~RotationBySpeed() override = default;

	virtual ComponentType getType() const override { return ComponentType::Rotation_By_Speed; }
};