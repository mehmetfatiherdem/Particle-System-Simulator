#pragma once

#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class RotationBySpeed : public Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method = ComponentMethod::Curve;
	float minSpeed = 0.0f;
	float maxSpeed = 1.0f;
	CubicBezierCurve<float> minBezier = CubicBezierCurve<float>(0.0f, 1.0f, 2.0f, 3.0f);
	CubicBezierCurve<float> maxBezier = CubicBezierCurve<float>(0.0f, 1.0f, 2.0f, 3.0f);

public:
	RotationBySpeed() : Component(5) { }

	RotationBySpeed(float minSpeed, float maxSpeed, const CubicBezierCurve<float>& bezier) :
		Component(5), method(ComponentMethod::Curve), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(bezier), maxBezier(bezier) { }

	RotationBySpeed(float minSpeed, float maxSpeed, const CubicBezierCurve<float>& minBezier, const CubicBezierCurve<float>& maxBezier) :
		Component(5), method(ComponentMethod::Random_Between_Two_Curves), minSpeed(minSpeed), maxSpeed(maxSpeed), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~RotationBySpeed() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

	virtual ComponentType getType() const override { return ComponentType::Rotation_By_Speed; }
};